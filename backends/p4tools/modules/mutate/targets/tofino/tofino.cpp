#include "backends/p4tools/modules/mutate/targets/tofino/tofino.h"

#include <optional>
#include <string>

#include "backends/p4tools/common/compiler/configuration.h"
#include "backends/p4tools/common/compiler/context.h"
#include "backends/p4tools/common/core/target.h"
#include "bf-p4c/bf-p4c-options.h"
#include "frontends/common/parseInput.h"

namespace P4Tools::P4Mutate::Tofino {

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
const AbstractTofinoCompilerTarget &AbstractTofinoCompilerTarget::get() {
    return Target::get<AbstractTofinoCompilerTarget>("compiler");
}

std::optional<const IR::P4Program *> AbstractTofinoCompilerTarget::runCompilerImpl(
    const IR::P4Program *program) const {
    const auto &self = get();
    program = self.runFrontend(program);
    if (program == nullptr) {
        return std::nullopt;
    }

    return program;
}

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

}  // namespace P4Tools::P4Mutate::Tofino
