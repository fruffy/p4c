#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_ACTIONLIST_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_ACTIONLIST_H_

#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class actionList {
 public:
    actionList() {}

    ~actionList() {}

    static IR::ActionList *gen(size_t len);
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_ACTIONLIST_H_ */
