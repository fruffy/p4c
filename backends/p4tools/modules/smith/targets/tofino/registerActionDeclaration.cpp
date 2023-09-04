#include "backends/p4tools/modules/smith/targets/tofino/registerActionDeclaration.h"

#include "backends/p4tools/modules/smith/common/baseType.h"
#include "backends/p4tools/modules/smith/common/parameter.h"
#include "backends/p4tools/modules/smith/common/scope.h"
#include "backends/p4tools/modules/smith/common/typeRef.h"
#include "backends/p4tools/modules/smith/tofino/register.h"
#include "ir/ir.h"

namespace P4Tools::P4Smith::Tofino {

IR::Declaration_Instance *RegisterActionDeclaration::genDeclInstance(
    IR::IndexedVector<IR::Declaration> *parserLocals) {
    auto *param_reg_decl = P4Register::genDeclInstance();
    parserLocals->push_back(param_reg_decl);
    // Get the <T, I> vector from the register
    const IR::Vector<IR::Type> *param_reg_type =
        (dynamic_cast<const IR::Type_Specialized *>(param_reg_decl->type))->arguments;
    const cstring action_name = randStr(5);
    auto *arguments = new IR::Vector<IR::Argument>();
    IR::BlockStatement *blk = nullptr;

    // Create the type for the RegisterAction.
    // Reference: p4c/p4include/tofino1_base.p4
    auto *baseName = new IR::Type_Name("RegisterAction");
    auto *typeArgs = new IR::Vector<IR::Type>();

    // RegisterAction: T, type of the value stored in the
    // input register.
    const auto *regActionT = param_reg_type->at(0);
    typeArgs->push_back(regActionT);

    // Register: I, type of the index into the register.
    const auto *regActionI = param_reg_type->at(1);
    typeArgs->push_back(regActionI);

    // Register: U, return type of the action.

    typeref_probs type_percent = {
        PCT.FUNCTIONDECLARATION_BASETYPE_BIT,    PCT.FUNCTIONDECLARATION_BASETYPE_SIGNED_BIT,
        PCT.FUNCTIONDECLARATION_BASETYPE_VARBIT, PCT.FUNCTIONDECLARATION_BASETYPE_INT,
        PCT.FUNCTIONDECLARATION_BASETYPE_ERROR,  PCT.FUNCTIONDECLARATION_BASETYPE_BOOL,
        PCT.FUNCTIONDECLARATION_BASETYPE_STRING, PCT.FUNCTIONDECLARATION_DERIVED_ENUM,
        PCT.FUNCTIONDECLARATION_DERIVED_HEADER,  PCT.FUNCTIONDECLARATION_DERIVED_HEADER_STACK,
        PCT.FUNCTIONDECLARATION_DERIVED_STRUCT,  PCT.FUNCTIONDECLARATION_DERIVED_HEADER_UNION,
        PCT.FUNCTIONDECLARATION_DERIVED_TUPLE,   PCT.FUNCTIONDECLARATION_TYPE_VOID,
        PCT.FUNCTIONDECLARATION_TYPE_MATCH_KIND,
    };
    const auto *regActionU = typeRef::pick_rnd_type(type_percent);
    typeArgs->push_back(regActionU);

    auto *type = new IR::Type_Specialized(baseName, typeArgs);

    const auto *reg_param = new IR::PathExpression(new IR::Path(param_reg_decl->name));
    arguments->push_back(new IR::Argument(reg_param));

    auto *ret = new IR::Declaration_Instance(action_name, type, arguments);

    // Start the scope for apply function.
    P4Scope::start_local_scope();

    // Params : inout T value, out U rv,
    // based on the register.
    IR::IndexedVector<IR::Parameter> function_params;

    function_params.push_back(
        new IR::Parameter(cstring(randStr(5)), IR::Direction::InOut, regActionT));
    function_params.push_back(
        new IR::Parameter(cstring(randStr(5)), IR::Direction::Out, regActionU));

    auto *param_list = new IR::ParameterList(function_params);

    // Add params to the local scope.
    for (const auto *param : param_list->parameters) {
        P4Scope::add_to_scope(param);
    }
    blk = blockStatement::gen(true);

    // End the scope for the apply function.
    P4Scope::end_local_scope();

    cstring function_name = "apply";
    auto *type_function = new IR::Type_Method(new IR::Type_Void(), param_list, function_name);

    auto *apply_func = new IR::Function(function_name, type_function, blk);
    IR::IndexedVector<IR::StatOrDecl> blockstmt_body;
    blockstmt_body.push_back(apply_func);
    // Push the apply function inside the registerAction.
    ret->initializer = new IR::BlockStatement(blockstmt_body);

    P4Scope::add_to_scope(ret);
    return ret;
}

}  // namespace P4Tools::P4Smith::Tofino
