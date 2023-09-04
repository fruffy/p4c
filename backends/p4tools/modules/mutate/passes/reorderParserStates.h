#ifndef BACKENDS_P4TOOLS_MODULES_MUTATE_PASSES_REORDERPARSERSTATES_H_
#define BACKENDS_P4TOOLS_MODULES_MUTATE_PASSES_REORDERPARSERSTATES_H_

#include "ir/ir.h"

namespace P4Tools::P4Mutate {

class ReorderParserStatesPass : public Transform {
 public:
    ReorderParserStatesPass() {
        visitDagOnce = true;
        setName("ReorderParserStates");
    }

    const IR::Node *postorder(IR::P4Parser *p) override;

    /// This function applys the ReorderParserStates pass to @param program,
    /// and @returns the resulting program.
    /// This pass randomly shuffles the states inside a P4Parser.
    /// If @param print_p4 is set, then it also pretty prints the p4 to stdout.
    static const IR::P4Program *reorderParserStates(const IR::P4Program *program,
                                                    bool print_p4 = false);
};

}  // namespace P4Tools::P4Mutate

#endif /* BACKENDS_P4TOOLS_MODULES_MUTATE_PASSES_REORDERPARSERSTATES_H_ */
