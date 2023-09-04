#include "backends/p4tools/modules/mutate/passes/insertDeadCode.h"

#include <algorithm>

#include "backends/p4tools/modules/mutate/util/config.h"
#include "backends/p4tools/modules/mutate/util/util.h"
#include "backends/p4tools/modules/smith/common/blockStatement.h"
#include "backends/p4tools/modules/smith/util/wordlist.h"
#include "ir/ir.h"

namespace P4Tools::P4Mutate {

const IR::Node *InsertDeadCodePass::postorder(IR::P4Program *prog) {
    auto rndInt = MutateUtils::getRandPct();
    if (rndInt < Config::ZERO_LENGTH_HEADERS_PCT) {
        auto numHeaders = MutateUtils::getRandInt(Config::MAX_ZERO_LENGTH_HEADERS);

        for (size_t i = 0; i < numHeaders; i++) {
            cstring name = P4Tools::P4Smith::Wordlist::getFromWordlist();
            auto *header = new IR::Type_Header(name);

            // Insert 0-length headers at a random pos in
            // the program.
            auto rndPos = MutateUtils::getRandInt(prog->objects.size() - 1);
            prog->objects.insert(prog->objects.begin() + rndPos, header);
        }
    }

    return prog;
}

const IR::Node *InsertDeadCodePass::postorder(IR::BlockStatement *blk) {
    auto rndInt = MutateUtils::getRandPct();
    if (rndInt < Config::DEAD_IF_BLOCK_PCT) {
        auto *expr = genExpression(false, Config::BOOL_DEADCODE_EXPRESSION_MAX_DEPTH);
        auto *bodyStmt = P4Tools::P4Smith::blockStatement::gen();
        const auto *ifStmt = new IR::IfStatement(expr, bodyStmt, nullptr);
        blk->components.push_back(ifStmt);
    }
    return blk;
}

const IR::Node *InsertDeadCodePass::postorder(IR::Statement *stmt) {
    if (stmt->is<IR::BlockStatement>()) {
        return stmt;
    }

    auto rndInt = MutateUtils::getRandPct();
    if (rndInt < Config::IF_TRUE_GUARD_PCT) {
        auto *expression = genExpression(true, Config::BOOL_DEADCODE_EXPRESSION_MAX_DEPTH);
        IR::IndexedVector<IR::StatOrDecl> bodyVec;
        bodyVec.push_back(stmt);
        auto *bodystmt = new IR::BlockStatement(bodyVec);
        const auto *ifStmt = new IR::IfStatement(expression, bodystmt, nullptr);
        return ifStmt;
    }
    return stmt;
}

const IR::P4Program *InsertDeadCodePass::insertDeadCode(const IR::P4Program *prog, bool print_p4) {
    auto *insertDeadCode = new InsertDeadCodePass();
    prog = prog->apply(*insertDeadCode);
    if (print_p4) {
        std::cout << "After InsertDeadCode:\n";
        MutateUtils::printP4(prog);
    }
    cstring outFileName = insertDeadCode->internalName + ".p4";
    MutateUtils::emitP4(prog, outFileName, true);

    return prog;
}

IR::Expression *InsertDeadCodePass::genConstant(bool expectedVal) {
    auto rndInt = MutateUtils::getRandPct();
    if (rndInt < Config::GEN_CONST_PCT) {
        IR::Constant *cnst = nullptr;
        // P4 only allows you to cast either a 1 or a 0 to a bool.
        cnst = new IR::Constant(static_cast<int>(expectedVal));
        return new IR::Cast(new IR::Type_Boolean(), cnst);
    }
    return new IR::BoolLiteral(expectedVal);
}

IR::LNot *InsertDeadCodePass::genNegation(bool expectedVal, int depth) {
    auto rndInt = MutateUtils::getRandPct();

    auto *expr = depth <= 0 || rndInt < Config::FLIP_LITERAL_PCT
                     ? genConstant(!expectedVal)
                     : genExpression(!expectedVal, depth - 1);
    return new IR::LNot(expr);
}

IR::LAnd *InsertDeadCodePass::genAnd(bool expectedVal, int depth) {
    auto *tautology = depth <= 0 ? genConstant(true) : genExpression(true, depth - 1);
    auto *fallacy = depth <= 0 ? genConstant(false) : genExpression(false, depth - 1);

    if (expectedVal) {
        return new IR::LAnd(tautology, tautology);
    }

    auto rndInt = MutateUtils::getRandInt(2);
    IR::Expression *left = nullptr;
    IR::Expression *right = nullptr;

    switch (rndInt) {
        case 0:
            left = tautology;
            right = fallacy;
            break;
        case 1:
            left = fallacy;
            right = tautology;
            break;
        case 2:
            left = fallacy;
            right = fallacy;
            break;
        default:
            BUG("Reached unreachable expression");
    }

    return new IR::LAnd(left, right);
}

IR::LOr *InsertDeadCodePass::genOr(bool expectedVal, int depth) {
    auto *tautology = depth <= 0 ? genConstant(true) : genExpression(true, depth - 1);
    auto *fallacy = depth <= 0 ? genConstant(false) : genExpression(false, depth - 1);

    if (!expectedVal) {
        return new IR::LOr(fallacy, fallacy);
    }

    auto rndInt = MutateUtils::getRandInt(2);
    IR::Expression *left = nullptr;
    IR::Expression *right = nullptr;

    switch (rndInt) {
        case 0:
            left = tautology;
            right = fallacy;
            break;
        case 1:
            left = fallacy;
            right = tautology;
            break;
        case 2:
            left = tautology;
            right = tautology;
            break;
        default:
            BUG("Reached unreachable expression");
    }

    return new IR::LOr(left, right);
}

IR::Expression *InsertDeadCodePass::genExpression(bool expectedVal, int depth) {
    auto rndInt = MutateUtils::getRandInt(3);
    switch (rndInt) {
        case 0:
            return genConstant(expectedVal);
            break;
        case 1:
            return genNegation(expectedVal, depth - 1);
            break;
        case 2:
            return genAnd(expectedVal, depth - 1);
            break;
        case 3:
            return genOr(expectedVal, depth - 1);
            break;
        default:
            BUG("Reached unreachable expression");
    }
}

}  // namespace P4Tools::P4Mutate
