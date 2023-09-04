#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_STRUCTTYPEDECLARATION_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_STRUCTTYPEDECLARATION_H_

#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class structTypeDeclaration {
 public:
    structTypeDeclaration() {}

    static IR::Type_Struct *gen();
    static IR::Type_Struct *gen_Headers();

 private:
    static IR::StructField *pick_field();
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_STRUCTTYPEDECLARATION_H_ */
