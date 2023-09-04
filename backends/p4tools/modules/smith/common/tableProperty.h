#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_TABLEPROPERTY_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_TABLEPROPERTY_H_

#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class tableProperty {
 public:
    tableProperty() {}

    // Tao: note that we always tag the property as !const
    static IR::Property *gen_keys();
    static IR::Property *gen_act_lists();
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_TABLEPROPERTY_H_ */
