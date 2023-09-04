#ifndef BACKENDS_P4TOOLS_MODULES_MUTATE_OPTIONS_H_
#define BACKENDS_P4TOOLS_MODULES_MUTATE_OPTIONS_H_

#include "backends/p4tools/common/options.h"

namespace P4Tools {

class MutateOptions : public AbstractP4cToolOptions {
 public:
    static MutateOptions &get();

    const char *getIncludePath() override;

    /// The p4 compiler binary.
    const char *compiler = nullptr;

    /// The output filename for the mutated program.
    const char *outputMutatedFileName = "mutated.p4";

    /// The output filename for the unmutated program, but after the frontend/midend passes.
    const char *outputUnmutatedFileName = "unmutated.p4";

    /// The output folder name which stores the p4 prog after each pass.
    const char *outputFolder = "mutations";

    ///
    const char *parserOutputFile = "parser.out";

    ///
    bool emi = false;

 private:
    MutateOptions();
};

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_MUTATE_OPTIONS_H_ */
