#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_PARAMETER_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_PARAMETER_H_

#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class parameter {
 public:
    parameter() {}
    ~parameter() {}

    static IR::Parameter *gen(bool if_none_dir = false);
    static IR::Parameter *gen_param(IR::Direction dir, cstring p_name, cstring t_name);
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_PARAMETER_H_ */
