#include "backends/p4tools/modules/smith/common/constantDeclaration.h"

#include "backends/p4tools/modules/smith/common/baseType.h"
#include "backends/p4tools/modules/smith/common/expression.h"
#include "backends/p4tools/modules/smith/common/scope.h"
#include "backends/p4tools/modules/smith/common/typeRef.h"
#include "backends/p4tools/modules/smith/util/util.h"

namespace P4Tools {

namespace P4Smith {

IR::Declaration_Constant *constantDeclaration::gen() {
    cstring name = randStr(6);
    typeref_probs type_percent = {
        PCT.CONSTANTDECLARATION_BASETYPE_BIT,    PCT.CONSTANTDECLARATION_BASETYPE_SIGNED_BIT,
        PCT.CONSTANTDECLARATION_BASETYPE_VARBIT, PCT.CONSTANTDECLARATION_BASETYPE_INT,
        PCT.CONSTANTDECLARATION_BASETYPE_ERROR,  PCT.CONSTANTDECLARATION_BASETYPE_BOOL,
        PCT.CONSTANTDECLARATION_BASETYPE_STRING, PCT.CONSTANTDECLARATION_DERIVED_ENUM,
        PCT.CONSTANTDECLARATION_DERIVED_HEADER,  PCT.CONSTANTDECLARATION_DERIVED_HEADER_STACK,
        PCT.CONSTANTDECLARATION_DERIVED_STRUCT,  PCT.CONSTANTDECLARATION_DERIVED_HEADER_UNION,
        PCT.CONSTANTDECLARATION_DERIVED_TUPLE,   PCT.CONSTANTDECLARATION_TYPE_VOID,
        PCT.CONSTANTDECLARATION_TYPE_MATCH_KIND,
    };

    IR::Type *tp = typeRef::pick_rnd_type(type_percent);

    IR::Declaration_Constant *ret = nullptr;
    // constant declarations need to be compile-time known
    P4Scope::req.compile_time_known = true;

    if (tp->is<IR::Type_Bits>() || tp->is<IR::Type_InfInt>() || tp->is<IR::Type_Boolean>() ||
        tp->is<IR::Type_Name>()) {
        auto expr = expression::gen_expr(tp);
        ret = new IR::Declaration_Constant(name, tp, expr);
    } else {
        BUG("Type %s !supported!", tp->node_type_name());
    }
    P4Scope::req.compile_time_known = false;

    P4Scope::add_to_scope(ret);

    return ret;
}

}  // namespace P4Smith

}  // namespace P4Tools
