#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_ASSIGNMENTORMETHODCALLSTATEMENT_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_ASSIGNMENTORMETHODCALLSTATEMENT_H_

#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class assignmentOrMethodCallStatement {
 public:
    assignmentOrMethodCallStatement() {}

    static IR::Statement *gen_assign();
    static IR::Statement *gen(bool is_in_func = false);
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_ASSIGNMENTORMETHODCALLSTATEMENT_H_ */
