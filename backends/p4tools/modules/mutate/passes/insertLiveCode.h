#ifndef BACKENDS_P4TOOLS_MODULES_MUTATE_PASSES_INSERTLIVECODE_H_
#define BACKENDS_P4TOOLS_MODULES_MUTATE_PASSES_INSERTLIVECODE_H_

#include <map>
#include <set>
#include <utility>
#include <vector>

#include "backends/p4tools/modules/mutate/util/config.h"
#include "frontends/common/resolveReferences/referenceMap.h"
#include "frontends/p4/typeMap.h"
#include "ir/ir.h"

namespace P4Tools::P4Mutate {

class InsertLiveCodePass : public Transform {
    P4::ReferenceMap *refMap;

    P4::TypeMap *typeMap;

    /// This stores the lvals that have been assigned a constant value in a blockStatement, and
    /// hence can be modified. Note: This is unique for each blockStatement, and hence, is reset
    /// each time we encounter a new blockStatement.
    std::map<const IR::Expression *const, std::pair<const IR::Constant *, const IR::Type *>>
        *modifiableDecls{};

    /// This will be the elements of our playground - for now these are just all lvals of type_bits
    /// that get assigned something in a blockStatemnent. Note: This is unique for each
    /// blockStatement, and hence, is reset each time we encounter a new blockStatement.
    std::set<const IR::Expression *> *scope{};

 public:
    InsertLiveCodePass(P4::ReferenceMap *refMap, P4::TypeMap *typeMap)
        : refMap(refMap), typeMap(typeMap) {
        visitDagOnce = true;
        setName("InsertLiveCode");
    }

    const IR::Node *postorder(IR::BlockStatement *blk) override;

    /// This function applys the InsertLiveCode pass to @param program,
    /// and @returns the resulting program.
    /// The pass inserts live code snippets throughout statements.
    /// Currently, we do the following :-
    /// 1. We insert if blocks, i.e, we convert stmt to if (true) {stmt}.
    /// 2. Insert empty 0 length headers.
    /// If @param print_p4 is set, then it also pretty prints the p4 to stdout.
    static const IR::P4Program *insertLiveCode(const IR::P4Program *program, bool print_p4 = false);

 private:
    /// This function generates expressions of @param type, with the max expression depth = @param
    /// depth. Note: For now, we just cast all exressions ot @param type, TODO: this needs be
    /// changed.
    const IR::Expression *genExpression(const IR::Type *type,
                                        int depth = Config::MAX_LIVECODE_EXPRESSION_DEPTH);

    /// This function generates add expressions of @param type, with the max expression depth =
    /// @param depth.
    const IR::Add *genAddExpression(const IR::Type *type, int depth);

    /// This function generates sub expressions of @param type, with the max expression depth =
    /// @param depth.
    const IR::Sub *genSubExpression(const IR::Type *type, int depth);

    /// This function generates div expressions of @param type, with the max expression depth =
    /// @param depth.
    const IR::Div *genDivExpression(const IR::Type *type, int depth);

    /// This function generates mul expressions of @param type, with the max expression depth =
    /// @param depth.
    const IR::Mul *genMulExpression(const IR::Type *type, int depth);

    /// This function picks a random lval from the current scope.
    const IR::Expression *pickRandomLval(const IR::Type *type);

    // const IR::Expression* generateRvals(const IR::Expression* lval);
    IR::IndexedVector<IR::StatOrDecl> generateEffectFreeExpressions();
};

}  // namespace P4Tools::P4Mutate

#endif /* BACKENDS_P4TOOLS_MODULES_MUTATE_PASSES_INSERTLIVECODE_H_ */
