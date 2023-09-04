#include "backends/p4tools/modules/mutate/passes/insertLiveCode.h"

#include <algorithm>
#include <map>
#include <utility>
#include <vector>

#include "backends/p4tools/modules/mutate/util/config.h"
#include "backends/p4tools/modules/mutate/util/util.h"
#include "frontends/common/constantFolding.h"
#include "frontends/common/resolveReferences/referenceMap.h"
#include "frontends/common/resolveReferences/resolveReferences.h"
#include "frontends/p4/typeChecking/typeChecker.h"
#include "frontends/p4/typeMap.h"
#include "ir/ir.h"

namespace P4Tools::P4Mutate {

const IR::Node *InsertLiveCodePass::postorder(IR::BlockStatement *blk) {
    auto stmts = blk->components;
    /// Reset both of these for each block statement.
    modifiableDecls = new std::map<const IR::Expression *const,
                                   std::pair<const IR::Constant *, const IR::Type *>>();
    scope = new std::set<const IR::Expression *>();
    for (const auto *stmt : stmts) {
        if (stmt->is<IR::AssignmentStatement>()) {
            const auto *assignment = stmt->to<IR::AssignmentStatement>();
            if (assignment->left->type->is<IR::Type_Bits>() ||
                assignment->right->type->is<IR::Type_Bits>()) {
                scope->insert(assignment->left);
            }

            if (assignment->right->is<IR::Constant>()) {
                modifiableDecls->emplace(
                    assignment->left,
                    std::make_pair(assignment->right->to<IR::Constant>(), assignment->right->type));
            }
        }
    }
    blk->components.append(generateEffectFreeExpressions());
    return blk;
}

IR::IndexedVector<IR::StatOrDecl> InsertLiveCodePass::generateEffectFreeExpressions() {
    IR::IndexedVector<IR::StatOrDecl> result;

    for (auto entry : *modifiableDecls) {
        result.push_back(
            new IR::AssignmentStatement(entry.first, genExpression(entry.second.second)));
    }

    // Reset the values to their original values.
    for (auto entry : *modifiableDecls) {
        result.push_back(new IR::AssignmentStatement(entry.first, entry.second.first));
    }
    return result;
}

const IR::Expression *InsertLiveCodePass::genExpression(const IR::Type *type, int depth) {
    const IR::Expression *result = nullptr;
    if (depth <= 0) {
        result = pickRandomLval(type);
        return result;
    }

    int randInt = MutateUtils::getRandInt(4);
    switch (randInt) {
        case 0:
            result = pickRandomLval(type);
            break;
        case 1:
            result = genAddExpression(type, depth - 1);
            break;
        case 2:
            result = genSubExpression(type, depth - 1);
            break;
        case 3:
            result = genDivExpression(type, depth - 1);
            break;
        case 4:
            result = genMulExpression(type, depth - 1);
            break;

        default:
            BUG("Reached an unreachable expression.");
    }
    if (result->type == type) {
        return result;
    }
    return new IR::Cast(type, result);
}

const IR::Expression *InsertLiveCodePass::pickRandomLval(const IR::Type *type) {
    const auto *expr = *MutateUtils::pickRandom(scope->begin(), scope->end());
    if (expr->type == type) {
        return expr;
    }
    return new IR::Cast(type, expr);
}

const IR::Add *InsertLiveCodePass::genAddExpression(const IR::Type *type, int depth) {
    const auto *left = depth <= 0 ? pickRandomLval(type) : genExpression(type, depth - 1);
    const auto *right = depth <= 0 ? pickRandomLval(type) : genExpression(type, depth - 1);

    auto *addExpr = new IR::Add(left, right);
    BUG_CHECK(left->type == right->type,
              "Left hand expression type must equal right hand expression type.");
    addExpr->type = left->type;
    return addExpr;
}

const IR::Sub *InsertLiveCodePass::genSubExpression(const IR::Type *type, int depth) {
    const auto *left = depth <= 0 ? pickRandomLval(type) : genExpression(type, depth - 1);
    const auto *right = depth <= 0 ? pickRandomLval(type) : genExpression(type, depth - 1);

    auto *subExpr = new IR::Sub(left, right);
    BUG_CHECK(left->type == right->type,
              "Left hand expression type must equal right hand expression type.");
    subExpr->type = left->type;

    return subExpr;
}

const IR::Div *InsertLiveCodePass::genDivExpression(const IR::Type *type, int depth) {
    const auto *left = depth <= 0 ? pickRandomLval(type) : genExpression(type, depth - 1);
    const auto *right = depth <= 0 ? pickRandomLval(type) : genExpression(type, depth - 1);

    auto *divExpr = new IR::Div(left, right);
    BUG_CHECK(left->type == right->type,
              "Left hand expression type must equal right hand expression type.");
    divExpr->type = left->type;

    return divExpr;
}

const IR::Mul *InsertLiveCodePass::genMulExpression(const IR::Type *type, int depth) {
    const auto *left = depth <= 0 ? pickRandomLval(type) : genExpression(type, depth - 1);
    const auto *right = depth <= 0 ? pickRandomLval(type) : genExpression(type, depth - 1);

    auto *mulExpr = new IR::Mul(left, right);
    BUG_CHECK(left->type == right->type,
              "Left hand expression type must equal right hand expression type.");
    mulExpr->type = left->type;

    return mulExpr;
}

const IR::P4Program *InsertLiveCodePass::insertLiveCode(const IR::P4Program *prog, bool print_p4) {
    P4::ReferenceMap refMap;
    P4::TypeMap typeMap;

    PassManager passes(
        {new P4::TypeChecking(&refMap, &typeMap, true), new InsertLiveCodePass(&refMap, &typeMap)});

    prog = prog->apply(passes);
    if (print_p4) {
        std::cout << "After Reorder Statements:\n";
        MutateUtils::printP4(prog);
    }
    return prog;
}

}  // namespace P4Tools::P4Mutate
