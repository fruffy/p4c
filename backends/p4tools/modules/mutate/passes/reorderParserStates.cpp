#include "backends/p4tools/modules/mutate/passes/reorderParserStates.h"

#include <algorithm>

#include "backends/p4tools/modules/mutate/util/config.h"
#include "backends/p4tools/modules/mutate/util/util.h"
#include "ir/indexed_vector.h"
#include "ir/ir.h"

namespace P4Tools::P4Mutate {

const IR::Node *ReorderParserStatesPass::postorder(IR::P4Parser *p) {
    auto rndInt = MutateUtils::getRandPct();

    if (rndInt < Config::REORDER_PARSER_STATES_PCT) {
        // Note: shuffle modifies inp in-place
        MutateUtils::shuffle(&p->states);
    }

    return p;
}

const IR::P4Program *ReorderParserStatesPass::reorderParserStates(const IR::P4Program *prog,
                                                                  bool print_p4) {
    auto *reorderParserStates = new ReorderParserStatesPass();
    prog = prog->apply(*reorderParserStates);
    if (print_p4) {
        std::cout << "After Reorder Parser states:\n";
        MutateUtils::printP4(prog);
    }
    cstring outFileName = reorderParserStates->internalName + ".p4";
    MutateUtils::emitP4(prog, outFileName, true);

    return prog;
}

}  // namespace P4Tools::P4Mutate
