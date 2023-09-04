#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_IDENTIFIERLIST_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_IDENTIFIERLIST_H_

#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class identifierList {
 public:
    const char *types[1] = {"name"};

    identifierList() {}

    static IR::IndexedVector<IR::Declaration_ID> gen(size_t len);
};

}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_IDENTIFIERLIST_H_ */
