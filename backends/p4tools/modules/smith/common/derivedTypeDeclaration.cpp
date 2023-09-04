#include "backends/p4tools/modules/smith/common/derivedTypeDeclaration.h"

#include "backends/p4tools/modules/smith/common/headerTypeDeclaration.h"

namespace P4Tools {

namespace P4Smith {

IR::Type *derivedTypeDeclaration::gen() { return headerTypeDeclaration::gen(); }

}  // namespace P4Smith

}  // namespace P4Tools
