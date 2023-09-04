#ifndef BACKENDS_P4TOOLS_MODULES_MUTATE_PASSES_SWITCHTABLEDECLS_H_
#define BACKENDS_P4TOOLS_MODULES_MUTATE_PASSES_SWITCHTABLEDECLS_H_

#include "ir/ir.h"

namespace P4Tools::P4Mutate {

class SwitchTableDeclsPass : public Transform {
 public:
    SwitchTableDeclsPass() {
        visitDagOnce = true;
        setName("SwitchTableDecls");
    }

    const IR::Node *postorder(IR::P4Control *p) override;
    const IR::Node *postorder(IR::Key *k) override;

    /// This function applys the SwitchTableDecls pass to @param program,
    /// and @returns the resulting program.
    /// This pass randomly shuffles the table declarations inside a P4Parser.
    /// If @param print_p4 is set, then it also pretty prints the p4 to stdout.
    static const IR::P4Program *switchTableDecls(const IR::P4Program *program,
                                                 bool print_p4 = false);
};

}  // namespace P4Tools::P4Mutate

#endif /* BACKENDS_P4TOOLS_MODULES_MUTATE_PASSES_SWITCHTABLEDECLS_H_ */
