#include "backends/p4tools/modules/mutate/passes/reorderExpressionTerms.h"

#include "backends/p4tools/modules/mutate/util/util.h"
#include "ir/ir.h"

namespace P4Tools::P4Mutate {

const IR::Node *ReorderExpressionTermsPass::postorder(IR::Operation_Binary *expr) {
    const auto *tmp = expr->right;
    expr->right = expr->left;
    expr->left = tmp;
    return expr;
}

const IR::P4Program *ReorderExpressionTermsPass::reorderExpressionTerms(const IR::P4Program *prog,
                                                                        bool print_p4) {
    auto *reorderExpressionTerms = new ReorderExpressionTermsPass();
    prog = prog->apply(*reorderExpressionTerms);
    if (print_p4) {
        std::cout << "After Reorder Expressions:\n";
        MutateUtils::printP4(prog);
    }
    cstring outFileName = reorderExpressionTerms->internalName + ".p4";
    MutateUtils::emitP4(prog, outFileName, true);
    return prog;
}

}  // namespace P4Tools::P4Mutate
