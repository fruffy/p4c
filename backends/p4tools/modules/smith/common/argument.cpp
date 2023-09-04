#include "backends/p4tools/modules/smith/common/argument.h"

#include "backends/p4tools/modules/smith/common/expression.h"
#include "backends/p4tools/modules/smith/common/scope.h"

namespace P4Tools {

namespace P4Smith {

IR::Expression *argument::gen_input_arg(const IR::Parameter *param) {
    if (param->direction == IR::Direction::In) {
        // this can be any value
        return expression::gen_expr(param->type);
    } else if (param->direction == IR::Direction::None) {
        // such args can only be compile-time constants
        P4Scope::req.compile_time_known = true;
        auto expr = expression::gen_expr(param->type);
        P4Scope::req.compile_time_known = false;
        return expr;
    } else {
        // for inout and out the value must be writeable
        return P4Scope::pick_lval_or_slice(param->type);
    }
}

bool argument::check_input_arg(const IR::Parameter *param) {
    if (param->direction == IR::Direction::In) {
        return P4Scope::check_lval(param->type, false);
    } else {
        return P4Scope::check_lval(param->type, true);
    }
}

}  // namespace P4Smith

}  // namespace P4Tools
