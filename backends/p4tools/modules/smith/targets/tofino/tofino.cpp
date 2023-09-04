#include "backends/p4tools/modules/smith/targets/tofino/tofino.h"

#include <string>

// The type checker needs to be included for rewrite_egress_intrinsic_metadata_header
#include "bf-p4c/midend/type_checker.h"  // IWYU pragma: keep
// Normal includes
#include "backends/p4tools/common/compiler/configuration.h"
#include "backends/p4tools/common/compiler/context.h"
#include "backends/p4tools/common/core/target.h"
#include "bf-p4c/bf-p4c-options.h"
#include "bf-p4c/common/parse_annotations.h"
#include "bf-p4c/device.h"
#include "bf-p4c/lib/error_type.h"
#include "bf-p4c/midend/rewrite_egress_intrinsic_metadata_header.h"
#include "lib/error.h"
// #include "backends/p4tools/modules/smith/targets/tofino/rename_keys.h"

namespace P4Tools::P4Smith::Tofino {

/// Inherits most functionality from BFNContext, but takes compiler configuration from
/// CompileContext.
class TofinoContext : public BFNContext, public CompileContext<BFN_Options> {
 public:
    BFN_Options &options() override { return BFNContext::options(); }

 protected:
    const CompilerConfiguration &getConfigImpl() override {
        return CompileContext::getConfigImpl();
    }
};

AbstractTofinoCompilerTarget::AbstractTofinoCompilerTarget(std::string deviceName,
                                                           std::string archName)
    : CompilerTarget(deviceName, archName) {}

/* =============================================================================================
 *  Tofino_TnaCompilerTarget implementation
 * ============================================================================================= */

Tofino_TnaCompilerTarget::Tofino_TnaCompilerTarget()
    : AbstractTofinoCompilerTarget("tofino", "tna") {}

void Tofino_TnaCompilerTarget::make() {
    static Tofino_TnaCompilerTarget *instance = nullptr;
    if (instance == nullptr) {
        instance = new Tofino_TnaCompilerTarget();
    }
}

/* =============================================================================================
 *  JBay_T2naCompilerTarget implementation
 * ============================================================================================= */

JBay_T2naCompilerTarget::JBay_T2naCompilerTarget()
    : AbstractTofinoCompilerTarget("tofino2", "t2na") {}

void JBay_T2naCompilerTarget::make() {
    static JBay_T2naCompilerTarget *instance = nullptr;
    if (instance == nullptr) {
        instance = new JBay_T2naCompilerTarget();
    }
}

}  // namespace P4Tools::P4Smith::Tofino
