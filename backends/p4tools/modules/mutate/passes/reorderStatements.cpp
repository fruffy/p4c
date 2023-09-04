#include "backends/p4tools/modules/mutate/passes/reorderStatements.h"

#include <algorithm>

#include "backends/p4tools/modules/mutate/util/config.h"
#include "backends/p4tools/modules/mutate/util/util.h"
#include "ir/ir.h"

namespace P4Tools::P4Mutate {

const IR::Node *ReorderStatementsPass::postorder(IR::BlockStatement *blk) {
    auto rndInt = MutateUtils::getRandPct();

    if (rndInt < Config::REORDER_STMTS_PCT) {
        // Note: shuffle modifies inp in-place.
        MutateUtils::shuffle(&blk->components);
    }

    return blk;
}

const IR::P4Program *ReorderStatementsPass::reorderStatements(const IR::P4Program *prog,
                                                              bool print_p4) {
    auto *reorderStatements = new ReorderStatementsPass();
    prog = prog->apply(*reorderStatements);
    if (print_p4) {
        std::cout << "After Reorder Statements:\n";
        MutateUtils::printP4(prog);
    }
    cstring outFileName = reorderStatements->internalName + ".p4";
    MutateUtils::emitP4(prog, outFileName, true);
    return prog;
}

}  // namespace P4Tools::P4Mutate
