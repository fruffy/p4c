#include "backends/p4tools/modules/smith/common/assignmentOrMethodCallStatement.h"

#include <set>
#include <vector>

#include "backends/p4tools/modules/smith/common/argument.h"
#include "backends/p4tools/modules/smith/common/blockStatement.h"
#include "backends/p4tools/modules/smith/common/expression.h"
#include "backends/p4tools/modules/smith/common/scope.h"
#include "backends/p4tools/modules/smith/util/util.h"

namespace P4Tools {

namespace P4Smith {

static void removeLval(IR::Expression *left, IR::Type *type) {
    cstring lvalStr = nullptr;
    if (const auto *path = left->to<IR::PathExpression>()) {
        lvalStr = path->path->name.name;
    } else if (const auto *mem = left->to<IR::Member>()) {
        lvalStr = mem->member.name;
    } else if (const auto *slice = left->to<IR::Slice>()) {
        lvalStr = slice->e0->to<IR::PathExpression>()->path->name.name;
    } else if (const auto *arrIdx = left->to<IR::ArrayIndex>()) {
        lvalStr = arrIdx->left->to<IR::PathExpression>()->path->name.name;
    }

    P4Scope::delete_lval(type, lvalStr);
}

IR::Statement *assignmentOrMethodCallStatement::gen_assign() {
    IR::AssignmentStatement *assignstat = nullptr;
    IR::Expression *left = nullptr;
    IR::Expression *right = nullptr;

    std::vector<int64_t> percent = {PCT.ASSIGNMENTORMETHODCALLSTATEMENT_ASSIGN_BIT,
                                    PCT.ASSIGNMENTORMETHODCALLSTATEMENT_ASSIGN_STRUCTLIKE};

    switch (randInd(percent)) {
        case 0: {
            IR::Type_Bits *bitType = P4Scope::pick_declared_bit_type(true);
            // Ideally this should have a fallback option
            if (bitType == nullptr) {
                LOG3("Could not find writable bit lval for assignment!\n");
                // TODO: Find a more meaningful assignment statement
                return nullptr;
            }
            left = P4Scope::pick_lval_or_slice(bitType);
            if (P4Scope::constraints.single_stage_actions) {
                removeLval(left, bitType);
            }
            right = expression::gen_expr(bitType);
            return new IR::AssignmentStatement(left, right);
        }
        case 1:
            // TODO: Compound types
            break;
    }

    return assignstat;
}

IR::Statement *gen_methodcall_expression(IR::PathExpression *method_name,
                                         IR::ParameterList params) {
    auto *args = new IR::Vector<IR::Argument>();
    IR::IndexedVector<IR::StatOrDecl> decls;

    // all this boilerplate should be somewhere else...
    P4Scope::start_local_scope();

    for (const auto *par : params) {
        IR::Argument *arg;
        // TODO: Fix the direction none issue here.
        if (!argument::check_input_arg(par) && par->direction != IR::Direction::None) {
            auto name = cstring(randStr(6));
            auto expr = expression::gen_expr(par->type);
            // all this boilerplate should be somewhere else...
            auto decl = new IR::Declaration_Variable(name, par->type, expr);
            P4Scope::add_to_scope(decl);
            decls.push_back(decl);
        }
        arg = new IR::Argument(argument::gen_input_arg(par));
        args->push_back(arg);
    }
    auto *mce = new IR::MethodCallExpression(method_name, args);
    auto *mcs = new IR::MethodCallStatement(mce);
    P4Scope::end_local_scope();
    if (decls.empty()) {
        return mcs;
    } else {
        auto blkStmt = new IR::BlockStatement(decls);
        blkStmt->push_back(mcs);
        return blkStmt;
    }
}

IR::Statement *gen_methodcall(bool is_in_func) {
    IR::MethodCallExpression *mce = nullptr;

    // functions cannot call actions or tables so set their chance to zero
    int16_t tmp_action_pct = PCT.ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_ACTION;
    int16_t tmp_tbl_pct = PCT.ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_TABLE;
    int16_t tmp_ctrl_pct = PCT.ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_CTRL;
    if (is_in_func) {
        PCT.ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_ACTION = 0;
        PCT.ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_TABLE = 0;
    }
    if (P4Scope::prop.in_action) {
        PCT.ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_CTRL = 0;
    }
    std::vector<int64_t> percent = {PCT.ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_ACTION,
                                    PCT.ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_FUNCTION,
                                    PCT.ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_TABLE,
                                    PCT.ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_CTRL,
                                    PCT.ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_BUILT_IN};

    switch (randInd(percent)) {
        case 0: {
            auto actions = P4Scope::get_decls<IR::P4Action>();
            if (actions.empty()) {
                break;
            }
            size_t idx = getRndInt(0, actions.size() - 1);
            auto p4Fun = actions.at(idx);
            auto params = p4Fun->getParameters()->parameters;
            auto methodName = new IR::PathExpression(p4Fun->name);
            return gen_methodcall_expression(methodName, params);
        }
        case 1: {
            auto funcs = P4Scope::get_decls<IR::Function>();
            if (funcs.size() == 0) {
                break;
            }
            size_t idx = getRndInt(0, funcs.size() - 1);
            auto p4Fun = funcs.at(idx);
            auto params = p4Fun->getParameters()->parameters;
            auto *methodName = new IR::PathExpression(p4Fun->name);
            return gen_methodcall_expression(methodName, params);
        }
        case 2: {
            auto *tbl_set = P4Scope::get_callable_tables();
            if (tbl_set->empty()) {
                break;
            }
            auto idx = getRndInt(0, tbl_set->size() - 1);
            auto tblIter = std::begin(*tbl_set);
            std::advance(tblIter, idx);
            const IR::P4Table *tbl = *tblIter;
            auto *mem = new IR::Member(new IR::PathExpression(tbl->name), "apply");
            mce = new IR::MethodCallExpression(mem);
            tbl_set->erase(tblIter);
            break;
        }
        case 3: {
            auto decls = P4Scope::get_decls<IR::Declaration_Instance>();
            if (decls.empty()) {
                break;
            }
            auto idx = getRndInt(0, decls.size() - 1);
            auto decl_iter = std::begin(decls);
            std::advance(decl_iter, idx);
            const IR::Declaration_Instance *decl_instance = *decl_iter;
            // avoid member here
            std::stringstream tmp_method_str;
            tmp_method_str << decl_instance->name << ".apply";
            cstring tmpMethodCstr(tmp_method_str.str());
            auto *methodName = new IR::PathExpression(tmpMethodCstr);
            const auto *typeName = decl_instance->type->to<IR::Type_Name>();

            const auto *resolvedType = P4Scope::get_type_by_name(typeName->path->name);
            if (resolvedType == nullptr) {
                BUG("Type Name %s not found", typeName->path->name);
            }
            if (const auto *ctrl = resolvedType->to<IR::P4Control>()) {
                auto params = ctrl->getApplyParameters()->parameters;
                return gen_methodcall_expression(methodName, params);
            }
            BUG("Declaration Instance type %s not yet supported",
                decl_instance->type->node_type_name());
        }
        case 4: {
            auto hdrs = P4Scope::get_decls<IR::Type_Header>();
            if (hdrs.empty()) {
                break;
            }
            std::set<cstring> hdrLvals;
            for (const auto *hdr : hdrs) {
                auto availableLvals = P4Scope::get_candidate_lvals(hdr, true);
                hdrLvals.insert(availableLvals.begin(), availableLvals.end());
            }
            if (hdrLvals.empty()) {
                break;
            }
            auto idx = getRndInt(0, hdrLvals.size() - 1);
            auto hdrLvalIter = std::begin(hdrLvals);
            std::advance(hdrLvalIter, idx);
            cstring hdrLval = *hdrLvalIter;
            cstring call;
            if (getRndInt(0, 1) != 0) {
                call = "setValid";
            } else {
                call = "setInvalid";
            }
            auto *mem = new IR::Member(new IR::PathExpression(hdrLval), call);
            mce = new IR::MethodCallExpression(mem);
            break;
        }
    }
    // restore previous probabilities
    PCT.ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_ACTION = tmp_action_pct;
    PCT.ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_TABLE = tmp_tbl_pct;
    PCT.ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_CTRL = tmp_ctrl_pct;
    if (mce != nullptr) {
        return new IR::MethodCallStatement(mce);
    }
    // unable to return a methodcall, return an assignment instead
    return assignmentOrMethodCallStatement::gen_assign();
}

IR::Statement *assignmentOrMethodCallStatement::gen(bool is_in_func) {
    std::vector<int64_t> percent = {PCT.ASSIGNMENTORMETHODCALLSTATEMENT_ASSIGN,
                                    PCT.ASSIGNMENTORMETHODCALLSTATEMENT_METHOD_CALL};
    auto val = randInd(percent);
    if (val == 0) {
        return assignmentOrMethodCallStatement::gen_assign();
    }
    return gen_methodcall(is_in_func);
}
}  // namespace P4Smith

}  // namespace P4Tools
