#ifndef BACKENDS_P4TOOLS_MODULES_MUTATE_PASSES_REORDERSTATEMENTS_H_
#define BACKENDS_P4TOOLS_MODULES_MUTATE_PASSES_REORDERSTATEMENTS_H_

#include "ir/ir.h"

namespace P4Tools::P4Mutate {

class ReorderStatementsPass : public Transform {
 public:
    ReorderStatementsPass() {
        visitDagOnce = true;
        setName("ReorderStatements");
    }

    const IR::Node *postorder(IR::BlockStatement *blk) override;

    /// This function applys the ReorderStatements pass to @param program,
    /// and @returns the resulting program. The pass looks at IR::BlockStatements,
    /// and randomly shuffles the statements inside it.
    /// If @param print_p4 is set, then it also pretty prints the p4 to stdout.
    static const IR::P4Program *reorderStatements(const IR::P4Program *program,
                                                  bool print_p4 = false);
};

}  // namespace P4Tools::P4Mutate

#endif /* BACKENDS_P4TOOLS_MODULES_MUTATE_PASSES_REORDERSTATEMENTS_H_ */
