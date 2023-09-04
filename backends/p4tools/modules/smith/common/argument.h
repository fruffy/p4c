#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_ARGUMENT_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_ARGUMENT_H_

#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class argument {
 public:
    const char *types[2] = {"expression", "DONTCARE"};

    argument() {}
    static IR::Expression *gen_input_arg(const IR::Parameter *param);
    static bool check_input_arg(const IR::Parameter *param);
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_ARGUMENT_H_ */
