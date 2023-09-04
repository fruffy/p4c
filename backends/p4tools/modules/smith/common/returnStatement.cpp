#include "backends/p4tools/modules/smith/common/returnStatement.h"

namespace P4Tools {

namespace P4Smith {

IR::ReturnStatement *returnStatement::gen_ret_stat(const IR::Type *tp) {
    IR::Expression *expr = nullptr;

    // Type_Void is also empty
    if (tp && !tp->to<IR::Type_Void>()) {
        expr = expression::gen_expr(tp);
    }
    return new IR::ReturnStatement(expr);
}
}  // namespace P4Smith

}  // namespace P4Tools
