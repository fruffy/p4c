#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_ENUMDECLARATION_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_ENUMDECLARATION_H_

#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class enumDeclaration {
 public:
    enumDeclaration() {}
    ~enumDeclaration() {}

    static IR::Type *gen(int type);
};

}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_ENUMDECLARATION_H_ */
