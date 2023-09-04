#ifndef BACKENDS_P4TOOLS_MODULES_EMI_EMI_H_
#define BACKENDS_P4TOOLS_MODULES_EMI_EMI_H_

#include <vector>

#include "backends/p4tools/common/options.h"
#include "backends/p4tools/common/p4ctool.h"
#include "ir/ir.h"

#include "backends/p4tools/modules/testgen/options.h"

namespace P4Tools::Emi {

class Emi : public AbstractP4cTool<P4Testgen::TestgenOptions> {
 public:
    virtual ~Emi() = default;

 protected:
    void registerTarget() override;
    int mainImpl(const IR::P4Program *program) override;
};

}  // namespace P4Tools::Emi

#endif /* BACKENDS_P4TOOLS_MODULES_EMI_EMI_H_ */
