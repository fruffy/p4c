#include "backends/p4tools/modules/smith/common/controlDeclaration.h"

namespace P4Tools {

namespace P4Smith {

IR::IndexedVector<IR::Declaration> controlDeclaration::gen_local_decls() {
    IR::IndexedVector<IR::Declaration> local_decls;

    auto vars = getRndInt(DECL.MIN_VAR, DECL.MAX_VAR);
    auto decls = getRndInt(DECL.MIN_INSTANCE, DECL.MAX_INSTANCE);
    auto actions = getRndInt(DECL.MIN_ACTION, DECL.MAX_ACTION);
    auto tables = getRndInt(DECL.MIN_TABLE, DECL.MAX_TABLE);

    // variableDeclarations
    for (int i = 0; i <= vars; i++) {
        auto var_decl = variableDeclaration::gen();
        local_decls.push_back(var_decl);
    }

    // declaration_instance
    for (int i = 0; i <= decls; i++) {
        auto decl_ins = genDeclInstance();

        if (decl_ins == nullptr) {
            continue;
        }
        local_decls.push_back(decl_ins);
    }

    // actionDeclarations
    for (int i = 0; i <= actions; i++) {
        auto act_decl = actionDeclaration::gen();
        local_decls.push_back(act_decl);
    }

    for (int i = 0; i <= tables; i++) {
        auto tab_decl = tableDeclaration::gen();
        local_decls.push_back(tab_decl);
    }
    return local_decls;
    // instantiations
}

IR::P4Control *controlDeclaration::gen() {
    // start of new scope
    P4Scope::start_local_scope();
    cstring name = randStr(7);
    IR::ParameterList *params = parameterList::gen();
    IR::Type_Control *type_ctrl = new IR::Type_Control(name, params);

    IR::IndexedVector<IR::Declaration> local_decls = controlDeclaration::gen_local_decls();
    // apply body
    auto apply_block = blockStatement::gen();

    // end of scope
    P4Scope::end_local_scope();

    // add to the whole scope
    IR::P4Control *p4ctrl = new IR::P4Control(name, type_ctrl, local_decls, apply_block);
    P4Scope::add_to_scope(p4ctrl);

    return p4ctrl;
}

IR::Declaration_Instance *controlDeclaration::genDeclInstance() {
    auto p4_ctrls = P4Scope::get_decls<IR::P4Control>();
    size_t size = p4_ctrls.size();

    if (size == 0) {
        // FIXME: Figure out a better way to handle this nullptr
        return nullptr;
    }
    IR::Vector<IR::Argument> *args = new IR::Vector<IR::Argument>();
    const IR::P4Control *p4ctrl = p4_ctrls.at(getRndInt(0, size - 1));
    IR::Type *tp = new IR::Type_Name(p4ctrl->name);
    auto decl = new IR::Declaration_Instance(cstring(randStr(6)), tp, args);
    P4Scope::add_to_scope(decl);
    return decl;
}
}  // namespace P4Smith

}  // namespace P4Tools
