#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_RETURNSTATEMENT_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_RETURNSTATEMENT_H_

#include "backends/p4tools/modules/smith/common/expression.h"
#include "backends/p4tools/modules/smith/common/scope.h"
#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class returnStatement {
 public:
    returnStatement() {}

    ~returnStatement() {}

    static IR::ReturnStatement *gen_ret_stat(const IR::Type *tp = nullptr);
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_RETURNSTATEMENT_H_ */
