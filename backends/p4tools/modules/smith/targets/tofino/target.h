#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_TARGETS_TOFINO_TARGET_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_TARGETS_TOFINO_TARGET_H_

#include <stdint.h>

#include <optional>
#include <string>

#include "backends/p4tools/modules/smith/core/target.h"
#include "bf-p4c/ir/gress.h"
#include "ir/ir.h"

namespace P4Tools::P4Smith::Tofino {

class AbstractTofinoSmithTarget : public SmithTarget {
 protected:
    explicit AbstractTofinoSmithTarget(std::string deviceName, std::string archName);
};

class Tofino_TnaSmithTarget : public AbstractTofinoSmithTarget {
 public:
    /// Registers this target.
    static void make();

    //  const ArchSpec* getArchSpecImpl() const override;

 protected:
 private:
    Tofino_TnaSmithTarget();

    //  static const ArchSpec archSpec;
};

class JBay_T2naSmithTarget : public AbstractTofinoSmithTarget {
 public:
    /// Registers this target.
    static void make();
    //  const ArchSpec* getArchSpecImpl() const override;

 private:
    JBay_T2naSmithTarget();

    //  static const ArchSpec archSpec;
};

}  // namespace P4Tools::P4Smith::Tofino

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_TARGETS_TOFINO_TARGET_H_ */
