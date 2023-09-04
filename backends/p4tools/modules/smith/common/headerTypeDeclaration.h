#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_HEADERTYPEDECLARATION_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_HEADERTYPEDECLARATION_H_

#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class headerTypeDeclaration {
 public:
    headerTypeDeclaration() {}

    static IR::Type_Header *gen();
    static IR::Type_Header *gen_eth();
};

}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_HEADERTYPEDECLARATION_H_ */
