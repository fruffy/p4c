#include "backends/p4tools/modules/mutate/passes/insertParserStates.h"

#include <algorithm>

#include "backends/p4tools/modules/mutate/util/config.h"
#include "backends/p4tools/modules/mutate/util/util.h"
#include "backends/p4tools/modules/smith/util/wordlist.h"
#include "ir/ir.h"
#include "ir/node.h"
#include "lib/cstring.h"

namespace P4Tools::P4Mutate {

const IR::ParserState *InsertParserStatesPass::pickRandomState(
    IR::IndexedVector<IR::ParserState> &vec) {
    const IR::ParserState *pick = nullptr;

    do {
        pick = *MutateUtils::pickRandom(vec.begin(), vec.end());
    } while (pick->name.name == "accept" || pick->name.name == "reject");

    return pick;
}

const IR::ParserState *InsertParserStatesPass::createAndInsertState(
    IR::IndexedVector<IR::ParserState> &vec, const IR::ParserState *initState) {
    const auto *finalDest = initState->selectExpression;

    const auto *newName = P4Tools::P4Smith::Wordlist::getFromWordlist();
    const auto *newPath = new IR::PathExpression(newName);

    const auto *newState = new IR::ParserState(newName, finalDest);
    // Push our newly created state
    vec.push_back(newState);

    // Need to create a copy of the init state because ParserStates are consts inside a parser.
    const auto *newInitState = new IR::ParserState(initState->name, initState->components, newPath);

    // Replace the old initState to point to the new state we created.
    // For some reason, parser->state.erase does not work.
    vec.removeByName(initState->name.name);
    vec.push_back(newInitState);
    return newState;
}

bool InsertParserStatesPass::existsParserState(IR::IndexedVector<IR::ParserState> states,
                                               cstring name) {
    return std::any_of(states.begin(), states.end(),
                       [name](const IR::ParserState *state) { return state->name.name == name; });
}

cstring InsertParserStatesPass::getRandomName(const IR::IndexedVector<IR::ParserState> &states) {
    cstring randomName = nullptr;
    // pick a random name until you find a unique parser state
    do {
        randomName = P4Tools::P4Smith::Wordlist::getFromWordlist();
    } while (existsParserState(states, randomName));

    return randomName;
}

const IR::Node *InsertParserStatesPass::postorder(IR::P4Parser *parser) {
    if (MutateUtils::getRandPct() <= Config::INSERT_LINKED_PARSER_STATES_PCT) {
        // The init state after which we add the states.
        const auto *initState = pickRandomState(parser->states);

        const int chainLen = MutateUtils::getRandInt(Config::MAX_PARSER_STATES_LENGTH);

        for (int i = 0; i < chainLen; i++) {
            initState = createAndInsertState(parser->states, initState);
        }
    }

    if (MutateUtils::getRandPct() <= Config::INSERT_ORPHAN_PARSER_STATES_PCT) {
        const int orphanStatesLen =
            MutateUtils::getRandInt(Config::MAX_ORPHAN_PARSER_STATES_LENGTH);

        for (int i = 0; i < orphanStatesLen; i++) {
            cstring randomName = getRandomName(parser->states);
            // We don't need to care about the uniquness of the destination because this state
            // is orphaned, but this state must exist for the program to compile.
            cstring transitionName = pickRandomState(parser->states)->name.name;
            auto *newState =
                new IR::ParserState(randomName, new IR::PathExpression(transitionName));
            parser->states.push_back(newState);
        }
    }

    return parser;
}

const IR::P4Program *InsertParserStatesPass::insertParserStates(const IR::P4Program *prog,
                                                                bool printP4) {
    auto *insertParserStates = new InsertParserStatesPass();
    prog = prog->apply(*insertParserStates);

    if (printP4) {
        std::cout << "After Insert Parser states:\n";
        MutateUtils::printP4(prog);
    }
    cstring outFileName = insertParserStates->internalName + ".p4";
    MutateUtils::emitP4(prog, outFileName, true);

    return prog;
}

}  // namespace P4Tools::P4Mutate
