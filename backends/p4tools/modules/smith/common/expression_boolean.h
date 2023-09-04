#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_EXPRESSION_BOOLEAN_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_EXPRESSION_BOOLEAN_H_

#include "backends/p4tools/modules/smith/common/expression.h"
#include "backends/p4tools/modules/smith/common/expression_bit.h"
#include "backends/p4tools/modules/smith/common/expression_struct.h"
#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class expression_boolean {
    friend class expression_bit;
    friend class expression_struct;
    friend class expression_int;
    friend class expression;

 public:
    expression_boolean() {}

 protected:
    static IR::Expression *construct();
    static IR::Expression *construct_cmp_expr();
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_EXPRESSION_BOOLEAN_H_ */
