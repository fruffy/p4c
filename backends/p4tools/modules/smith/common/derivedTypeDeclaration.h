#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_DERIVEDTYPEDECLARATION_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_DERIVEDTYPEDECLARATION_H_

#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class derivedTypeDeclaration {
 public:
    const char *types[4] = {"headerTypeDeclaration", "headerUnionDeclaration",
                            "structTypeDeclaration", "enumDeclaration"};

    derivedTypeDeclaration() {}

    static IR::Type *gen();
};

}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_DERIVEDTYPEDECLARATION_H_ */
