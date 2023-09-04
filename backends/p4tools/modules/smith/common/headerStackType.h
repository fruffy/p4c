#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_HEADERSTACKTYPE_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_HEADERSTACKTYPE_H_

#include "backends/p4tools/modules/smith/common/expression.h"
#include "backends/p4tools/modules/smith/common/scope.h"
#include "ir/ir.h"

#define MAX_HEADER_STACK_SIZE 10

namespace P4Tools {

namespace P4Smith {

class headerStackType {
 public:
    headerStackType() {}

    ~headerStackType() {}

    static IR::Type *gen();
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_HEADERSTACKTYPE_H_ */
