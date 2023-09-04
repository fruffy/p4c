#ifndef BACKENDS_P4TOOLS_MODULES_MUTATE_MUTATE_H_
#define BACKENDS_P4TOOLS_MODULES_MUTATE_MUTATE_H_

#include <vector>

#include "backends/p4tools/common/options.h"
#include "backends/p4tools/common/p4ctool.h"
#include "backends/p4tools/modules/mutate/options.h"
#include "ir/ir.h"

namespace P4Tools::P4Mutate {

class Mutate : public AbstractP4cTool<MutateOptions> {
 public:
    virtual ~Mutate() = default;

    using StringMap = std::map<std::string, std::vector<std::string>>;

 protected:
    void registerTarget() override;
    int mainImpl(const IR::P4Program *program) override;
    /// Takes in a @param program and @returns a p4program after applying mutation passes to it.
    /// Note : See passes/ for all available passes.
    static const IR::P4Program *mutate(const IR::P4Program *program);
    ///
    static std::vector<StringMap *> *parseParserFile(const char *fileName);
};

}  // namespace P4Tools::P4Mutate

#endif /* BACKENDS_P4TOOLS_MODULES_MUTATE_MUTATE_H_ */
