#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_TYPEDEFDECLARATION_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_TYPEDEFDECLARATION_H_

#include "backends/p4tools/modules/smith/common/baseType.h"
#include "backends/p4tools/modules/smith/common/headerTypeDeclaration.h"
#include "backends/p4tools/modules/smith/common/headerUnionDeclaration.h"
#include "backends/p4tools/modules/smith/common/scope.h"
#include "backends/p4tools/modules/smith/common/structTypeDeclaration.h"
#include "backends/p4tools/modules/smith/common/typeRef.h"
#include "backends/p4tools/modules/smith/util/util.h"
#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class typedefDeclaration {
 public:
    const char *types[2] = {"typedef", "type"};

    typedefDeclaration() {}
    ~typedefDeclaration() {}

    static IR::Type *gen();
    static void gen_base_t();
    static IR::Type_Typedef *gen_typedef();
    static IR::Type_Newtype *gen_newtype();
};

}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_TYPEDEFDECLARATION_H_ */
