#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_EXTERNDECLARATION_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_EXTERNDECLARATION_H_

#include "backends/p4tools/modules/smith/common/blockStatement.h"
#include "backends/p4tools/modules/smith/common/parameterList.h"
#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class externDeclaration {
 public:
    externDeclaration() {}

    ~externDeclaration() {}

    static IR::Method *gen();

 protected:
    static IR::Type *gen_return_type();
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_EXTERNDECLARATION_H_ */
