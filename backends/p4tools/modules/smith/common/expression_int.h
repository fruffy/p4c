#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_EXPRESSION_INT_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_EXPRESSION_INT_H_

#include "backends/p4tools/modules/smith/common/expression.h"
#include "backends/p4tools/modules/smith/common/expression_boolean.h"
#include "backends/p4tools/modules/smith/common/expression_struct.h"
#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class expression_int {
    friend class expression_boolean;
    friend class expression_struct;
    friend class expression_bit;
    friend class expression;

 public:
    expression_int() {}

 protected:
    static IR::Expression *construct();
    static IR::Expression *construct_unary_expr();
    static IR::Expression *construct_binary_expr();
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_EXPRESSION_INT_H_ */
