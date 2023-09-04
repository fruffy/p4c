#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_FUNCTIONDECLARATION_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_FUNCTIONDECLARATION_H_

#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class functionDeclaration {
 public:
    functionDeclaration() {}

    ~functionDeclaration() {}

    static IR::Function *gen();
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_FUNCTIONDECLARATION_H_ */
