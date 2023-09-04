#ifndef BACKENDS_P4TOOLS_MODULES_MUTATE_PASSES_INSERTDEADCODE_H_
#define BACKENDS_P4TOOLS_MODULES_MUTATE_PASSES_INSERTDEADCODE_H_

#include "frontends/common/resolveReferences/referenceMap.h"
#include "frontends/p4/typeMap.h"
#include "ir/ir.h"

namespace P4Tools::P4Mutate {

class InsertDeadCodePass : public Transform {
 public:
    InsertDeadCodePass() {
        visitDagOnce = true;
        setName("InsertDeadCode");
    }

    const IR::Node *postorder(IR::Statement *stmt) override;
    const IR::Node *postorder(IR::P4Program *prog) override;
    const IR::Node *postorder(IR::BlockStatement *blk) override;

    /// This function applys the InsertDeadCode pass to @param program,
    /// and @returns the resulting program.
    /// The pass insert dead code snippets throughout statements.
    /// Currently, we do the following :-
    /// 1. We insert if blocks, i.e, we convert stmt to if (true) {stmt}.
    /// 2. Insert empty 0 length headers.
    /// 3. Insert unreachable code blocks, i.e, we insert if (flase) {stmts}.
    /// If @param print_p4 is set, then it also pretty prints the p4 to stdout.
    static const IR::P4Program *insertDeadCode(const IR::P4Program *program, bool print_p4 = false);

 private:
    /// This function generates a predicate that evaluates to @param expectedVal,
    /// with the given depth of operations.
    IR::Expression *genExpression(bool expectedVal, int depth);

    /// This function @returns either a boolLiteral corresponsing to @param expectedVal,
    /// or randomly generates an int and @returns a cast.
    static IR::Expression *genConstant(bool expectedVal);

    /// This function @returns an IR::LNot expression of !expectedVal.
    IR::LNot *genNegation(bool expectedVal, int depth);

    /// This function @returns an IR::LAnd expression that evals to expectedVal.
    IR::LAnd *genAnd(bool expectedVal, int depth);

    /// This function @returns an IR::LOr expression that evals to expectedVal.
    IR::LOr *genOr(bool expectedVal, int depth);
};

}  // namespace P4Tools::P4Mutate

#endif /* BACKENDS_P4TOOLS_MODULES_MUTATE_PASSES_INSERTDEADCODE_H_ */
