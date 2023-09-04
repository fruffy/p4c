#include "backends/p4tools/modules/mutate/mutate.h"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <random>
#include <sstream>
#include <vector>

#include "backends/p4tools/modules/mutate/options.h"
#include "backends/p4tools/modules/mutate/passes/insertDeadCode.h"
#include "backends/p4tools/modules/mutate/passes/insertLiveCode.h"
#include "backends/p4tools/modules/mutate/passes/insertParserStates.h"
#include "backends/p4tools/modules/mutate/passes/reorderExpressionTerms.h"
#include "backends/p4tools/modules/mutate/passes/reorderParserStates.h"
#include "backends/p4tools/modules/mutate/passes/reorderStatements.h"
#include "backends/p4tools/modules/mutate/passes/switchTableDecls.h"
#include "backends/p4tools/modules/mutate/register.h"
#include "backends/p4tools/modules/mutate/util/util.h"
#include "frontends/p4/toP4/toP4.h"
#include "ir/ir.h"

namespace P4Tools::P4Mutate {

void Mutate::registerTarget() { registerCompilerTargets(); }

std::vector<Mutate::StringMap *> *Mutate::parseParserFile(const char *fileName) {
    /*
     * the returned vector is of this structure
     * [ [ "gress1": [state1, state2 ...],  "gress2": [state1, state2 ...], ...], // packet 1
     *   [ "gress1": [state1, state2 ...],  "gress2": [state1, state2 ...]  ...], // packet 2
     *   ...
     *  ]
     */

    std::filesystem::path path(fileName);
    if (!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path) ||
        std::filesystem::is_empty(path)) {
        std::cerr << "Parser file does not exist, or is empty" << '\n';
        return nullptr;
    }

    std::ifstream parserFile(path);

    std::string line;
    auto *result = new std::vector<StringMap *>();
    auto *currentPacketVector = new StringMap();
    result->emplace_back(currentPacketVector);

    bool insidePacket = false;
    while (std::getline(parserFile, line)) {
        std::istringstream stream(line);
        auto inpStr = stream.str();
        if (inpStr == "---") {
            insidePacket = !insidePacket;
            // This packet ended, move on to the next packet.
            if (!insidePacket) {
                currentPacketVector = new StringMap();
                result->emplace_back(currentPacketVector);
                std::cout << "Packet ends\n";

            } else {
                std::cout << "New packet begins:\n";
            }

            continue;
        }

        if (insidePacket) {
            // Just parsing states right now.
            (*currentPacketVector)[stream.str()].emplace_back(inpStr);
            std::cout << "State: " << inpStr << ", Gress: " << stream.str() << '\n';
        }
    }

    return result;
}

const IR::P4Program *Mutate::mutate(const IR::P4Program *program) {
    /// Switching these off right now
    // program = reorderExpressionTerms(program, false);
    // program = reorderStatements(program, false);
    ///////////////////////
    program = InsertDeadCodePass::insertDeadCode(program, false);
    program = InsertLiveCodePass::insertLiveCode(program, false);
    program = InsertParserStatesPass::insertParserStates(program, false);
    program = ReorderParserStatesPass::reorderParserStates(program, false);
    program = SwitchTableDeclsPass::switchTableDecls(program, false);
    return program;
}

int Mutate::mainImpl(const IR::P4Program *program) {
    auto seed = MutateOptions::get().seed;
    const auto *outMutatedFile = MutateOptions::get().outputMutatedFileName;
    const auto *outUnmutatedFile = MutateOptions::get().outputUnmutatedFileName;
    const auto *outputFolder = MutateOptions::get().outputFolder;
    const auto *parserOutputFile = MutateOptions::get().parserOutputFile;
    const auto emi = MutateOptions::get().emi;

    if (MutateOptions::get().compiler == nullptr) {
        std::cerr << "Please provide the compiler binary using --compilerBin." << '\n';
        return EXIT_FAILURE;
    }

    if (seed == std::nullopt) {
        // No seed provided, so we generate our own.
        std::random_device r;
        seed = r();
    }
    // Initialize the global seed for randomness.
    MutateUtils::setRandomSeed(*seed);
    std::cout << "============ Program seed " << *seed << " =============" << '\n';

    std::cout << "Emitting input program (after frontend passes) to: " << outUnmutatedFile << '\n';
    MutateUtils::emitP4(program, outUnmutatedFile);

    // Initialize the mutations directory.
    int errCode = MutateUtils::initializeOutputDir(outputFolder);
    if (errCode == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    // Check if we need EMI
    if (emi) {
        parseParserFile(parserOutputFile);
        std::exit(1);
    }
    std::cout << "=====================================Begin "
                 "mutate========================================= \n";
    program = mutate(program);

    std::cout << "Emitting mutated program to: " << outMutatedFile << '\n';
    MutateUtils::emitP4(program, outMutatedFile);

    return EXIT_SUCCESS;
}

}  // namespace P4Tools::P4Mutate
