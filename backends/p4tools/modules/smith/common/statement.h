#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_STATEMENT_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_STATEMENT_H_

#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class statement {
 public:
    const char *types[8] = {"assignmentOrMethodCallStatement",
                            "directApplication",
                            "conditionalStatement",
                            "emptyStatement",
                            "blockStatement",
                            "exitStatement",
                            "returnStatement",
                            "switchStatement"};

    statement() {}
    ~statement() {}

    static IR::Statement *gen_rnd(bool if_in_func);
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_STATEMENT_H_ */
