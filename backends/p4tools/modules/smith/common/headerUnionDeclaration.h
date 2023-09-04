#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_HEADERUNIONDECLARATION_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_HEADERUNIONDECLARATION_H_

#include "backends/p4tools/modules/smith/common/scope.h"
#include "backends/p4tools/modules/smith/common/structFieldList.h"
#include "backends/p4tools/modules/smith/util/util.h"
#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class headerUnionDeclaration {
 public:
    headerUnionDeclaration() {}

    static IR::Type_HeaderUnion *gen();
};

}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_HEADERUNIONDECLARATION_H_ */
