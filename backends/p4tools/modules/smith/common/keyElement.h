#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_KEYELEMENT_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_KEYELEMENT_H_

#include "backends/p4tools/modules/smith/common/annotations.h"
#include "backends/p4tools/modules/smith/common/scope.h"
#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class keyElement {
 public:
    keyElement() {}

    ~keyElement() {}

    static IR::KeyElement *gen(cstring match_kind);
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_KEYELEMENT_H_ */
