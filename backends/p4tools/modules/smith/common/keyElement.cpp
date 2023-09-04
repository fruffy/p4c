#include "backends/p4tools/modules/smith/common/keyElement.h"

#include "backends/p4tools/modules/smith/common/expression.h"
namespace P4Tools {

namespace P4Smith {

IR::KeyElement *keyElement::gen(cstring match_kind) {
    auto match = new IR::PathExpression(match_kind);
    auto annotations = Annotations::gen();
    auto bit_type = P4Scope::pick_declared_bit_type(false);

    // Ideally this should have a fallback option
    if (!bit_type) {
        printf("Could not find key lval for key matches\n");
        return nullptr;
    }
    // this expression can!be an infinite precision integer
    P4Scope::req.require_scalar = true;
    auto expr = expression::gen_expr(bit_type);
    P4Scope::req.require_scalar = false;
    auto key = new IR::KeyElement(annotations, expr, match);

    return key;
}
}  // namespace P4Smith

}  // namespace P4Tools
