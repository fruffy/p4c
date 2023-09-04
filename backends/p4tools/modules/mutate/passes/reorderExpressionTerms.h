#ifndef BACKENDS_P4TOOLS_MODULES_MUTATE_PASSES_REORDEREXPRESSIONTERMS_H_
#define BACKENDS_P4TOOLS_MODULES_MUTATE_PASSES_REORDEREXPRESSIONTERMS_H_

#include "ir/ir.h"

namespace P4Tools::P4Mutate {

class ReorderExpressionTermsPass : public Transform {
 public:
    ReorderExpressionTermsPass() {
        visitDagOnce = true;
        setName("ReorderExpressionTerms");
    }

    const IR::Node *postorder(IR::Operation_Binary *expr) override;

    /// This function applies the ReorderExpressionTerms pass to @param program,
    /// and @returns the resulting program.
    /// The pass swaps the terms of a Operation_Binary expression.
    /// If @param print_p4 is set, then it also pretty prints the p4 to stdout.
    static const IR::P4Program *reorderExpressionTerms(const IR::P4Program *program,
                                                       bool print_p4 = false);

    // To be Implemented -----------
    // const IR::Node *postorder(IR::Neg *expr) override;
    // const IR::Node *postorder(IR::Cmpl *expr) override;
    // const IR::Node *postorder(IR::Mul *expr) override;
    // const IR::Node *postorder(IR::Div *expr) override;
    // const IR::Node *postorder(IR::Add *expr) override;
    // const IR::Node *postorder(IR::AddSat *expr) override;
    // const IR::Node *postorder(IR::Sub *expr) override;
    // const IR::Node *postorder(IR::SubSat *expr) override;
    // const IR::Node *postorder(IR::BAnd *expr) override;
    // const IR::Node *postorder(IR::BOr *expr) override;
    // const IR::Node *postorder(IR::BXor *expr) override;
    // const IR::Node *postorder(IR::Mod *expr) override;
    // const IR::Node *postorder(IR::Shl *expr) override;
    // const IR::Node *postorder(IR::Shr *expr) override;
    // const IR::Node *postorder(IR::SelectExpression *expr) override;
    // const IR::Node *postorder(const IR::Range *r);
    // const IR::Node *postorder(const IR::Cast *c);
    // const IR::Node *postorder(const IR::Concat *c);
    // const IR::Node *postorder(const IR::Slice *s);
    // const IR::Node *postorder(const IR::Mux *m);
    // const IR::Node *postorder(const IR::Member *m);
    // const IR::Node *postorder(IR::Member *p) override;
    // const IR::Node *postorder(const IR::SerEnumMember *m);
    // const IR::Node *postorder(const IR::DefaultExpression *de);
    // const IR::Node *postorder(const IR::ListExpression *le);
    // const IR::Node *postorder(const IR::TypeNameExpression *tn);
    // const IR::Node *postorder(const IR::NamedExpression *ne);
    // const IR::Node *postorder(const IR::StructExpression *sie);
    // const IR::Node *postorder(const IR::ConstructorCallExpression *cc);
    // const IR::Node *postorder(const IR::MethodCallExpression *mce);
    // const IR::Node *postorder(const IR::StructExpression *s);
};

}  // namespace P4Tools::P4Mutate

#endif /* BACKENDS_P4TOOLS_MODULES_MUTATE_PASSES_REORDEREXPRESSIONTERMS_H_ */
