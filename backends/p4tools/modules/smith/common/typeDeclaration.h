#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_TYPEDECLARATION_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_TYPEDECLARATION_H_

#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class typeDeclaration {
 public:
    const char *types[5] = {"derivedTypeDeclaration", "typedefDeclaration", "parserTypeDeclaration",
                            "controlTypeDeclaration", "packageTypeDeclaration"};

    typeDeclaration() {}

    static IR::Type_Declaration *gen();
};

}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_TYPEDECLARATION_H_ */
