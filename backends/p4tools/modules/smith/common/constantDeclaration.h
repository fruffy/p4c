#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_CONSTANTDECLARATION_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_CONSTANTDECLARATION_H_

#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class constantDeclaration {
 public:
    constantDeclaration() {}

    ~constantDeclaration() {}

    static IR::Declaration_Constant *gen();
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_CONSTANTDECLARATION_H_ */
