#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_EXPRESSIONLIST_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_EXPRESSIONLIST_H_

#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class expressionList {
 public:
    const char *types[1] = {"expression"};

    expressionList() = default;
    static IR::ListExpression *gen(IR::Vector<IR::Type> types, bool only_lval = false);
};

}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_EXPRESSIONLIST_H_ */
