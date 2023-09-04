#include "backends/p4tools/modules/emi/emi.h"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <random>
#include <sstream>
#include <vector>

#include "backends/p4tools/common/core/solver.h"
#include "backends/p4tools/common/core/z3_solver.h"
#include "backends/p4tools/common/lib/util.h"
#include "frontends/common/parser_options.h"
#include "lib/cstring.h"
#include "lib/error.h"

#include "backends/p4tools/modules/testgen/core/program_info.h"
#include "backends/p4tools/modules/testgen/core/symbolic_executor/depth_first.h"
#include "backends/p4tools/modules/testgen/core/symbolic_executor/greedy_stmt_cov.h"
#include "backends/p4tools/modules/testgen/core/symbolic_executor/max_stmt_cov.h"
#include "backends/p4tools/modules/testgen/core/symbolic_executor/path_selection.h"
#include "backends/p4tools/modules/testgen/core/symbolic_executor/random_backtrack.h"
#include "backends/p4tools/modules/testgen/core/symbolic_executor/selected_branches.h"
#include "backends/p4tools/modules/testgen/core/symbolic_executor/symbolic_executor.h"
#include "backends/p4tools/modules/testgen/core/target.h"
#include "backends/p4tools/modules/testgen/lib/logging.h"
#include "backends/p4tools/modules/testgen/options.h"
#include "backends/p4tools/modules/testgen/register.h"

namespace P4Tools::Emi {

P4Testgen::SymbolicExecutor *pickExecutionEngine(const P4Testgen::TestgenOptions &testgenOptions,
                                                 const P4Testgen::ProgramInfo *programInfo,
                                                 AbstractSolver &solver) {
    const auto &pathSelectionPolicy = testgenOptions.pathSelectionPolicy;
    if (pathSelectionPolicy == P4Testgen::PathSelectionPolicy::GreedyStmtCoverage) {
        return new P4Testgen::GreedyStmtSelection(solver, *programInfo);
    }
    if (pathSelectionPolicy == P4Testgen::PathSelectionPolicy::RandomBacktrack) {
        return new P4Testgen::RandomBacktrack(solver, *programInfo);
    }
    if (pathSelectionPolicy == P4Testgen::PathSelectionPolicy::RandomMaxStmtCoverage) {
        return new P4Testgen::RandomMaxStmtCoverage(solver, *programInfo,
                                                    testgenOptions.saddlePoint);
    }
    if (!testgenOptions.selectedBranches.empty()) {
        std::string selectedBranchesStr = testgenOptions.selectedBranches;
        return new P4Testgen::SelectedBranches(solver, *programInfo, selectedBranchesStr);
    }
    return new P4Testgen::DepthFirstSearch(solver, *programInfo);
}

const std::vector<P4Testgen::FinalState> *collectTestSamples(
    const P4Testgen::TestgenOptions &testgenOptions, const P4Testgen::ProgramInfo *programInfo,
    P4Testgen::SymbolicExecutor &symbex) {
    // Get the filename of the input file and remove the extension
    // This assumes that inputFile is not null.
    auto const inputFile = P4CContext::get().options().file;
    auto testPath = std::filesystem::path(inputFile.c_str()).stem();
    // Create the directory, if the directory string is valid and if it does not exist.
    cstring testDirStr = testgenOptions.outputDir;
    if (!testDirStr.isNullOrEmpty()) {
        auto testDir = std::filesystem::path(testDirStr.c_str());
        std::filesystem::create_directories(testDir);
        testPath = testDir / testPath;
    }
    // Define how to handle the final state for each test. This is target defined.
    // We delegate execution to the symbolic executor.
    // Each test back end has a different run function.
    auto *testBackend = P4Testgen::TestgenTarget::getTestBackend(*programInfo, symbex, testPath);
    auto *finalStates = new std::vector<P4Testgen::FinalState>;
    auto callBack = [finalStates, testBackend](const P4Testgen::FinalState &finalState) {
        finalStates->emplace_back(finalState);
        return testBackend->run(std::forward<decltype(finalState)>(finalState));
    };

    // Run the symbolic executor with given exploration strategy.
    symbex.run(callBack);
    return finalStates;
}

void Emi::registerTarget() { P4Testgen::registerCompilerTargets(); }

int Emi::mainImpl(const IR::P4Program *program) {
    // Register all available Testgen targets.
    // These are discovered by CMAKE, which fills out the register.h.in file.
    P4Testgen::registerTestgenTargets();

    const auto *programInfo = P4Testgen::TestgenTarget::initProgram(program);
    if (programInfo == nullptr) {
        ::error("Program not supported by target device and architecture.");
        return EXIT_FAILURE;
    }
    if (::errorCount() > 0) {
        ::error("Emi: Encountered errors during preprocessing. Exiting");
        return EXIT_FAILURE;
    }

    // Print basic information for each test.
    P4Testgen::enableInformationLogging();

    // Get the options and the seed.
    const auto &testgenOptions = P4Testgen::TestgenOptions::get();
    auto seed = Utils::getCurrentSeed();
    if (seed) {
        printFeature("test_info", 4, "============ Program seed %1% =============\n", *seed);
    }

    // Need to declare the solver here to ensure its lifetime.
    Z3Solver solver;
    auto symbex = P4Testgen::RandomBacktrack(solver, *programInfo);

    // TODO: Process output.
    collectTestSamples(testgenOptions, programInfo, symbex);

    return ::errorCount() == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

}  // namespace P4Tools::Emi
