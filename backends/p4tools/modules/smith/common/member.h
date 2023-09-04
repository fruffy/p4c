#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_MEMBER_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_MEMBER_H_

#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class member {
 public:
    const char *types[1] = {"name"};

    member() {}
};

}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_MEMBER_H_ */
