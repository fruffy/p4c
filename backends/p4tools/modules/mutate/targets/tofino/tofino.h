#ifndef BACKENDS_P4TOOLS_MODULES_MUTATE_TARGETS_TOFINO_TOFINO_H_
#define BACKENDS_P4TOOLS_MODULES_MUTATE_TARGETS_TOFINO_TOFINO_H_

#include <optional>
#include <string>

#include "backends/p4tools/common/compiler/compiler_target.h"
#include "backends/p4tools/common/compiler/midend.h"
#include "frontends/common/options.h"
#include "lib/compile_context.h"

namespace P4Tools::P4Mutate::Tofino {

class AbstractTofinoCompilerTarget : public CompilerTarget {
 private:
    static const AbstractTofinoCompilerTarget &get();

 protected:
    std::optional<const IR::P4Program *> runCompilerImpl(
        const IR::P4Program *program) const override;

    explicit AbstractTofinoCompilerTarget(std::string deviceName, std::string archName);
};

class Tofino_TnaCompilerTarget : public AbstractTofinoCompilerTarget {
 public:
    static void make();

 private:
    Tofino_TnaCompilerTarget();
};

class JBay_T2naCompilerTarget : public AbstractTofinoCompilerTarget {
 public:
    /// Registers this target.
    static void make();

 private:
    JBay_T2naCompilerTarget();
};

}  // namespace P4Tools::P4Mutate::Tofino

#endif /* BACKENDS_P4TOOLS_MODULES_MUTATE_TARGETS_TOFINO_TOFINO_H_ */
