#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_KEYELEMENTLIST_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_KEYELEMENTLIST_H_

#include "backends/p4tools/modules/smith/common/keyElement.h"
#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class keyElementList {
 public:
    keyElementList() {}

    ~keyElementList() {}

    static IR::Key *gen(size_t len);
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_KEYELEMENTLIST_H_ */
