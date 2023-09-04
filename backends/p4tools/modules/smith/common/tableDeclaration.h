#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_TABLEDECLARATION_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_TABLEDECLARATION_H_

#include "backends/p4tools/modules/smith/common/tablePropertyList.h"
#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class tableDeclaration {
 public:
    tableDeclaration() {}

    ~tableDeclaration() {}

    static IR::P4Table *gen();
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_TABLEDECLARATION_H_ */
