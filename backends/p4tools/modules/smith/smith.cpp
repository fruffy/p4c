#include "backends/p4tools/modules/smith/smith.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

#include "backends/p4tools/modules/smith/common/scope.h"
#include "backends/p4tools/modules/smith/options.h"
#include "backends/p4tools/modules/smith/register.h"
#include "backends/p4tools/modules/smith/version.h"
#include "backends/psa.h"
#include "backends/tna.h"
#include "backends/top.h"
#include "backends/v1model.h"
#include "frontends/common/parser_options.h"
#include "frontends/p4/toP4/toP4.h"
#include "ir/ir-generated.h"
#include "ir/ir.h"
#include "lib/crash.h"
#include "lib/cstring.h"
#include "lib/error.h"
#include "lib/gc.h"
#include "lib/nullstream.h"

namespace P4Tools {

namespace P4Smith {

void genP4Code(cstring output_file, cstring target) {
    std::ostream *ostream = openFile(output_file, false);
    P4Scope::start_local_scope();

    IR::P4Program *program = nullptr;
    if (target == "v1model") {
        V1Model::generate_includes(ostream);
        program = V1Model::gen();
    } else if (target == "tna") {
        Tofino::TNA::generate_includes(ostream);
        program = Tofino::TNA::gen();
    } else if (target == "top") {
        Top::generate_includes(ostream);
        program = Top::gen();
    } else if (target == "psa") {
        PSA::generate_includes(ostream);
        program = PSA::gen();
    } else {
        ::error("Architecture must be v1model, tna, or top");
        return;
    }
    // output to the file
    P4::ToP4 top4(ostream, false);
    program->apply(top4);
    ostream->flush();
    P4Scope::end_local_scope();
}
void Smith::registerTarget() { registerCompilerTargets(); }

int Smith::main(const std::vector<const char *> &args) {
    // Register supported compiler targets.
    registerTarget();
    // Process command-line options.
    P4Tools::SmithOptions::get().processArgs(args);
    return mainImpl(nullptr);
}

int Smith::mainImpl(const IR::P4Program * /*program */) {
    registerSmithTargets();

    // Establish the real compilation context.
    auto *compilerContext = P4Tools::CompilerTarget::makeContext();
    AutoCompileContext autoContext(compilerContext);

    auto &options = P4Tools::SmithOptions::get();
    // we only handle P4_16 right now
    options.compiler_version = SMITH_VERSION_STRING;

    // use a default name if no specific output name is provided
    if (options.output_file == nullptr) {
        options.output_file = "out.p4";
    }
    // use a default name if no specific output name is provided
    if (options.smith_arch == nullptr) {
        ::error("--smithArch is required. Possible options: v1model, tna, or top");
        // options.usage();
        exit(EXIT_FAILURE);
    }

    const auto *ostream = openFile(options.output_file, false);
    if (ostream == nullptr) {
        ::error("must have --output [file]");
        exit(EXIT_FAILURE);
    }
    uint32_t seed = 0;
    if (options.seed != std::nullopt) {
        std::cerr << "Using provided seed.\n";
        seed = *options.seed;
    } else {
        // no seed provided, we generate our own
        std::cerr << "Using generated seed.\n";
        std::random_device r;
        seed = r();
    }
    std::cerr << "Seed:" << seed << "\n";
    setSeed(seed);
    genP4Code(options.output_file, options.smith_arch);

    return EXIT_SUCCESS;
}

}  // namespace P4Smith
}  // namespace P4Tools
