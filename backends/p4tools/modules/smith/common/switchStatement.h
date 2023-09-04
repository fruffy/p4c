#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_SWITCHSTATEMENT_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_SWITCHSTATEMENT_H_

#include "backends/p4tools/modules/smith/common/blockStatement.h"
#include "backends/p4tools/modules/smith/common/expression.h"
#include "backends/p4tools/modules/smith/common/scope.h"
#include "backends/p4tools/modules/smith/util/util.h"
#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class switchStatement {
 public:
    switchStatement() {}

    ~switchStatement() {}

    static IR::SwitchStatement *gen();
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_SWITCHSTATEMENT_H_ */
