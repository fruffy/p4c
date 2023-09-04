#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_VARIABLEDECLARATION_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_VARIABLEDECLARATION_H_

#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class variableDeclaration {
 public:
    variableDeclaration() {}

    ~variableDeclaration() {}

    static IR::Declaration_Variable *gen();
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_VARIABLEDECLARATION_H_ */
