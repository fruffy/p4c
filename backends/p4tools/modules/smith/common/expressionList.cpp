#include "backends/p4tools/modules/smith/common/expressionList.h"

#include "backends/p4tools/modules/smith/common/expression.h"
#include "backends/p4tools/modules/smith/util/util.h"

namespace P4Tools {

namespace P4Smith {

IR::ListExpression *expressionList::gen(IR::Vector<IR::Type> types, bool only_lval) {
    IR::Vector<IR::Expression> components;
    for (auto tb : types) {
        IR::Expression *expr;
        if (only_lval) {
            cstring lval_name = P4Scope::pick_lval(tb);
            expr = new IR::PathExpression(lval_name);
        } else {
            expr = expression::gen_expr(tb);
        }
        components.push_back(expr);
    }
    return new IR::ListExpression(components);
}

}  // namespace P4Smith

}  // namespace P4Tools
