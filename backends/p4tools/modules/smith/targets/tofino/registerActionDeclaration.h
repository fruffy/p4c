#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_TARGETS_TOFINO_REGISTERACTIONDECLARATION_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_TARGETS_TOFINO_REGISTERACTIONDECLARATION_H_

#include "backends/p4tools/modules/smith/common/blockStatement.h"
#include "backends/p4tools/modules/smith/common/parameterList.h"
#include "ir/ir.h"

namespace P4Tools::P4Smith::Tofino {

class RegisterActionDeclaration {
 public:
    RegisterActionDeclaration() = default;

    ~RegisterActionDeclaration() = default;

    static IR::Declaration_Instance *genDeclInstance(
        IR::IndexedVector<IR::Declaration> *parserLocals);
};

}  // namespace P4Tools::P4Smith::Tofino

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_TARGETS_TOFINO_REGISTERACTIONDECLARATION_H_ */
