#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_EXPRESSION_BIT_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_EXPRESSION_BIT_H_

#include "backends/p4tools/modules/smith/common/expression.h"
#include "backends/p4tools/modules/smith/common/expression_boolean.h"
#include "backends/p4tools/modules/smith/common/expression_struct.h"
#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class expression_bit {
    friend class expression_boolean;
    friend class expression_struct;
    friend class expression_int;
    friend class expression;

 public:
    expression_bit() {}

 protected:
    static IR::Expression *construct(const IR::Type_Bits *tb);
    static IR::Expression *construct_unary_expr(const IR::Type_Bits *tb);
    static IR::Expression *construct_binary_expr(const IR::Type_Bits *tb);
    static IR::Expression *construct_ternary_expr(const IR::Type_Bits *tb);
    static IR::Expression *createSaturationOperand(const IR::Type_Bits *tb);
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_EXPRESSION_BIT_H_ */
