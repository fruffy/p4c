#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_EXPRESSION_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_EXPRESSION_H_

#include "backends/p4tools/modules/smith/common/scope.h"
#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

#define MAX_DEPTH 3

class expression {
 public:
    expression() {}

    static IR::MethodCallExpression *gen_functioncall(cstring method_name,
                                                      IR::ParameterList params);
    static IR::Expression *gen_expr(const IR::Type *tp);

    static IR::MethodCallExpression *pick_function(
        IR::IndexedVector<IR::Declaration> viable_functions, const IR::Type **ret_type);
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_EXPRESSION_H_ */
