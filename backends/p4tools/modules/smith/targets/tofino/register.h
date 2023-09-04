#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_TARGETS_TOFINO_REGISTER_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_TARGETS_TOFINO_REGISTER_H_

#include "backends/p4tools/common/p4ctool.h"
#include "backends/p4tools/modules/smith/smith.h"
#include "backends/p4tools/modules/smith/targets/tofino/target.h"
#include "backends/p4tools/modules/smith/targets/tofino/tofino.h"

namespace P4Tools::P4Smith::Tofino {

inline void tofinoRegisterCompilerTarget() {
    Tofino_TnaCompilerTarget::make();
    JBay_T2naCompilerTarget::make();
}

inline void tofinoRegisterSmithTarget() {
    Tofino_TnaSmithTarget::make();
    JBay_T2naSmithTarget::make();
}

}  // namespace P4Tools::P4Smith::Tofino

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_TARGETS_TOFINO_REGISTER_H_ */
