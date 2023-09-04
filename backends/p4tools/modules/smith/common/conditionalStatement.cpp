#include "backends/p4tools/modules/smith/common/conditionalStatement.h"

#include "backends/p4tools/modules/smith/common/expression.h"
#include "backends/p4tools/modules/smith/common/statement.h"

namespace P4Tools {

namespace P4Smith {

IR::IfStatement *conditionalStatement::gen_if_stat(bool is_in_func) {
    IR::Expression *cond = nullptr;
    IR::Statement *if_true = nullptr, *if_false = nullptr;

    cond = expression::gen_expr(new IR::Type_Boolean());
    if (!cond) {
        BUG("cond in IfStatement should !be nullptr!");
    }
    if_true = statement::gen_rnd(is_in_func);
    if (!if_true) {
        // could !generate a statement
        // this happens when there is now way to generate an assignment
        if_true = new IR::EmptyStatement();
    }
    if_false = statement::gen_rnd(is_in_func);
    if (!if_false) {
        // could !generate a statement
        // this happens when there is now way to generate an assignment
        if_false = new IR::EmptyStatement();
    }
    return new IR::IfStatement(cond, if_true, if_false);
}
}  // namespace P4Smith

}  // namespace P4Tools
