#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_CONDITIONALSTATEMENT_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_CONDITIONALSTATEMENT_H_

#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class conditionalStatement {
 public:
    conditionalStatement() {}

    ~conditionalStatement() {}

    static IR::IfStatement *gen_if_stat(bool if_in_func = false);
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_CONDITIONALSTATEMENT_H_ */
