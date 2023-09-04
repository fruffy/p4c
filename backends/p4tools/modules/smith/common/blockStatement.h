#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_BLOCKSTATEMENT_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_BLOCKSTATEMENT_H_

#include "backends/p4tools/modules/smith/util/util.h"
#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class blockStatement {
 public:
    blockStatement() {}
    ~blockStatement() {}

    static IR::BlockStatement *gen(bool if_in_func = false);
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_BLOCKSTATEMENT_H_ */
