#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_EXITSTATEMENT_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_EXITSTATEMENT_H_

#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class exitStatement {
 public:
    exitStatement() {}
    ~exitStatement() {}

    static IR::ExitStatement *gen() { return new IR::ExitStatement(); }
};

}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_EXITSTATEMENT_H_ */
