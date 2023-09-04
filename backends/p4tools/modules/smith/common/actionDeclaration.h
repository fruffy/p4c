#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_ACTIONDECLARATION_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_ACTIONDECLARATION_H_

#include "backends/p4tools/modules/smith/common/blockStatement.h"
#include "backends/p4tools/modules/smith/common/parameterList.h"
#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class actionDeclaration {
 public:
    actionDeclaration() {}

    ~actionDeclaration() {}

    static IR::P4Action *gen();
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_ACTIONDECLARATION_H_ */
