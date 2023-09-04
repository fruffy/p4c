#include "backends/p4tools/modules/smith/core/target.h"

#include <cstddef>
#include <map>
#include <string>
#include <vector>

#include "backends/p4tools/modules/smith/targets/tofino/target.h"
#include "ir/ir.h"

namespace P4Tools::P4Smith::Tofino {

/* =============================================================================================
 *  AbstractTofinoSmithTarget implementation
 * ============================================================================================= */

AbstractTofinoSmithTarget::AbstractTofinoSmithTarget(std::string deviceName, std::string archName)
    : SmithTarget(deviceName, archName) {}

/* =============================================================================================
 *  Tofino_TnaSmithTarget implementation
 * ============================================================================================= */

Tofino_TnaSmithTarget::Tofino_TnaSmithTarget() : AbstractTofinoSmithTarget("tofino", "tna") {}

void Tofino_TnaSmithTarget::make() {
    static Tofino_TnaSmithTarget *instance = nullptr;
    if (instance == nullptr) {
        instance = new Tofino_TnaSmithTarget();
    }
}

// /* =============================================================================================
//  *  JBay_T2naSmithTarget implementation
//  * =============================================================================================
//  */

JBay_T2naSmithTarget::JBay_T2naSmithTarget() : AbstractTofinoSmithTarget("tofino2", "t2na") {}

void JBay_T2naSmithTarget::make() {
    static JBay_T2naSmithTarget *instance = nullptr;
    if (instance == nullptr) {
        instance = new JBay_T2naSmithTarget();
    }
}

}  // namespace P4Tools::P4Smith::Tofino
