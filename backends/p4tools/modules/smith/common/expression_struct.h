#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_EXPRESSION_STRUCT_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_EXPRESSION_STRUCT_H_

#include "backends/p4tools/modules/smith/common/expression.h"
#include "backends/p4tools/modules/smith/common/expression_bit.h"
#include "backends/p4tools/modules/smith/common/expression_boolean.h"
#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class expression_struct {
    friend class expression_bit;
    friend class expression_int;
    friend class expression_boolean;
    friend class expression;

 public:
    expression_struct() {}

 protected:
    static IR::Expression *construct(const IR::Type_Name *tn);
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_EXPRESSION_STRUCT_H_ */
