#include "backends/p4tools/modules/smith/common/enumDeclaration.h"

#include "backends/p4tools/modules/smith/common/baseType.h"
#include "backends/p4tools/modules/smith/common/identifierList.h"
#include "backends/p4tools/modules/smith/common/scope.h"
#include "backends/p4tools/modules/smith/common/specifiedIdentifierList.h"

namespace P4Tools {

namespace P4Smith {

static IR::Type_Enum *gen_enum(cstring name) {
    auto decl_ids = identifierList::gen(3);

    auto ret = new IR::Type_Enum(name, decl_ids);

    P4Scope::add_to_scope(ret);
    return ret;
}

static IR::Type_SerEnum *gen_ser_enum(cstring name) {
    auto members = specifiedIdentifierList::gen(3);
    IR::Type_Bits *tp = baseType::gen_bit_type(false);

    auto ret = new IR::Type_SerEnum(name, tp, members);

    P4Scope::add_to_scope(ret);
    return ret;
}

IR::Type *enumDeclaration::gen(int type) {
    cstring name = randStr(4);
    if (type == 0) {
        return gen_enum(name);
    } else {
        return gen_ser_enum(name);
    }
}

}  // namespace P4Smith

}  // namespace P4Tools
