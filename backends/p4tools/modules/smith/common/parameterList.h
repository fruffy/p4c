#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_PARAMETERLIST_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_PARAMETERLIST_H_

#include "backends/p4tools/modules/smith/common/expression.h"
#include "backends/p4tools/modules/smith/common/parameter.h"
#include "backends/p4tools/modules/smith/common/scope.h"
#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class parameterList {
 public:
    const char *types[1] = {"nonEmptyParameterList"};

    parameterList() {}

    static IR::ParameterList *gen();
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_PARAMETERLIST_H_ */
