#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_CONTROLDECLARATION_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_CONTROLDECLARATION_H_

#include "backends/p4tools/modules/smith/common/actionDeclaration.h"
#include "backends/p4tools/modules/smith/common/blockStatement.h"
#include "backends/p4tools/modules/smith/common/constantDeclaration.h"
#include "backends/p4tools/modules/smith/common/expression.h"
#include "backends/p4tools/modules/smith/common/scope.h"
#include "backends/p4tools/modules/smith/common/tableDeclaration.h"
#include "backends/p4tools/modules/smith/common/variableDeclaration.h"
#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class controlDeclaration {
 public:
    controlDeclaration() {}

    ~controlDeclaration() {}

    static IR::IndexedVector<IR::Declaration> gen_local_decls();
    static IR::P4Control *gen();

    static IR::Declaration_Instance *genDeclInstance();
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_CONTROLDECLARATION_H_ */
