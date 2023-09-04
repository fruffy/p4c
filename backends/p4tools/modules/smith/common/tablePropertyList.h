#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_TABLEPROPERTYLIST_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_TABLEPROPERTYLIST_H_

#include "backends/p4tools/modules/smith/common/tableProperty.h"
#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class tablePropertyList {
 public:
    const char *types[1] = {"tableProperty"};

    tablePropertyList() {}

    static IR::TableProperties *gen();
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_TABLEPROPERTYLIST_H_ */
