#ifndef BACKENDS_P4TOOLS_MODULES_MUTATE_TARGETS_TOFINO_REGISTER_H_
#define BACKENDS_P4TOOLS_MODULES_MUTATE_TARGETS_TOFINO_REGISTER_H_

#include "backends/p4tools/common/p4ctool.h"
#include "backends/p4tools/modules/mutate/mutate.h"
#include "backends/p4tools/modules/mutate/targets/tofino/tofino.h"

namespace P4Tools::P4Mutate {

inline void tofinoRegisterCompilerTarget() {
    Tofino::Tofino_TnaCompilerTarget::make();
    Tofino::JBay_T2naCompilerTarget::make();
}

}  // namespace P4Tools::P4Mutate

#endif /* BACKENDS_P4TOOLS_MODULES_MUTATE_TARGETS_TOFINO_REGISTER_H_ */
