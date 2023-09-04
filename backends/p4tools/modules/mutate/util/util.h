#ifndef BACKENDS_P4TOOLS_MODULES_MUTATE_UTIL_UTIL_H_
#define BACKENDS_P4TOOLS_MODULES_MUTATE_UTIL_UTIL_H_

#include <cstdint>

#include "backends/p4tools/common/lib/util.h"
#include "ir/ir.h"

namespace P4Tools::P4Mutate {

struct ExitInfo {
 public:
    int exitCode;
    cstring errMsg;
    bool isCompilerBug;
    ExitInfo() : exitCode(0), errMsg(nullptr), isCompilerBug(false) {}
};

class MutateUtils : public Utils {
 public:
    /// Pretty prints the given IR @param prog as P4 to stdout.
    static void printP4(const IR::P4Program *prog);

    /// Pretty prints the given IR @param prog as P4 to the given @param filename. If @param
    /// emitToOutputFolder is set, then we emit the P4 inside the output directory that was provided
    /// to us through --outputDir.
    static void emitP4(const IR::P4Program *prog, cstring fileName,
                       bool emitToOutputFolder = false);

    /// Initialize the given output directory where the transient programs(after each pass) will be
    /// emitted.
    static int initializeOutputDir(cstring fileName);

    /// Tries running the compiler on the p4 program with @param name.
    /// @returns the exit code, the error mesage, and if we encountered a compiler bug from the run.
    static ExitInfo getExitInfo(cstring name);

    /// Return a random value between [0:100] inclusive.
    static uint64_t getRandPct();
};

}  // namespace P4Tools::P4Mutate

#endif /* BACKENDS_P4TOOLS_MODULES_MUTATE_UTIL_UTIL_H_ */
