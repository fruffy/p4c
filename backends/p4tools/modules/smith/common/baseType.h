#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_BASETYPE_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_BASETYPE_H_

#include <vector>

#include "backends/p4tools/modules/smith/util/util.h"
#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class baseType {
 public:
    const char *types[10] = {
        "BOOL",
        "ERROR",  // not
        "INT",
        "BIT",
        "BIT <INTEGER>",
        "INT <INTEGER>",
        "VARBIT <INTEGER>",        // not
        "BIT < (expression) >",    // not
        "INT < (expression) >",    // not
        "VARBIT < (expression) >"  // not
    };

    baseType() {}
    static constexpr int bit_widths[6] = {4, 8, 16, 32, 64, 128};
    // static constexpr int bit_widths[8] = {1, 2, 4, 8, 16, 32, 64, 128};

    static IR::Type *pick_rnd_base_type(const std::vector<int64_t> &type);

    static IR::Type_Boolean *gen_bool_type() { return new IR::Type_Boolean(); }
    static IR::Type_InfInt *gen_int_type() { return new IR::Type_InfInt(); }
    static IR::Type_Bits *gen_bit_type(bool isSigned);

    static IR::BoolLiteral *gen_bool_literal();
    static IR::Constant *gen_int_literal(size_t bit_width = INTEGER_WIDTH);
    static IR::Constant *gen_bit_literal(const IR::Type *tb);
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_BASETYPE_H_ */
