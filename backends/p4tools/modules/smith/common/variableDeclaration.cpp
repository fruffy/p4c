#include "backends/p4tools/modules/smith/common/variableDeclaration.h"

#include "backends/p4tools/modules/smith/common/baseType.h"
#include "backends/p4tools/modules/smith/common/expression.h"
#include "backends/p4tools/modules/smith/common/headerStackType.h"
#include "backends/p4tools/modules/smith/common/scope.h"
#include "backends/p4tools/modules/smith/common/typeRef.h"
#include "backends/p4tools/modules/smith/util/util.h"

namespace P4Tools {

namespace P4Smith {

IR::Declaration_Variable *variableDeclaration::gen() {
    cstring name = randStr(6);

    typeref_probs type_percent = {
        PCT.VARIABLEDECLARATION_BASETYPE_BIT,    PCT.VARIABLEDECLARATION_BASETYPE_SIGNED_BIT,
        PCT.VARIABLEDECLARATION_BASETYPE_VARBIT, PCT.VARIABLEDECLARATION_BASETYPE_INT,
        PCT.VARIABLEDECLARATION_BASETYPE_ERROR,  PCT.VARIABLEDECLARATION_BASETYPE_BOOL,
        PCT.VARIABLEDECLARATION_BASETYPE_STRING, PCT.VARIABLEDECLARATION_DERIVED_ENUM,
        PCT.VARIABLEDECLARATION_DERIVED_HEADER,  PCT.VARIABLEDECLARATION_DERIVED_HEADER_STACK,
        PCT.VARIABLEDECLARATION_DERIVED_STRUCT,  PCT.VARIABLEDECLARATION_DERIVED_HEADER_UNION,
        PCT.VARIABLEDECLARATION_DERIVED_TUPLE,   PCT.VARIABLEDECLARATION_TYPE_VOID,
        PCT.VARIABLEDECLARATION_TYPE_MATCH_KIND,
    };

    IR::Type *tp = typeRef::pick_rnd_type(type_percent);

    IR::Declaration_Variable *ret = nullptr;

    if (tp->is<IR::Type_Bits>() || tp->is<IR::Type_InfInt>() || tp->is<IR::Type_Boolean>() ||
        tp->is<IR::Type_Name>()) {
        auto expr = expression::gen_expr(tp);
        ret = new IR::Declaration_Variable(name, tp, expr);
    } else if (tp->is<IR::Type_Stack>()) {
        // header stacks do !have an initializer yet
        ret = new IR::Declaration_Variable(name, tp);
    } else {
        BUG("Type %s !supported!", tp->node_type_name());
    }

    P4Scope::add_to_scope(ret);

    return ret;
}
}  // namespace P4Smith

}  // namespace P4Tools
