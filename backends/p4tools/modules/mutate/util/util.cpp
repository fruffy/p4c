#include "backends/p4tools/modules/mutate/util/util.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>

#include "backends/p4tools/modules/mutate/options.h"
#include "frontends/p4/toP4/toP4.h"
#include "ir/ir.h"
#include "lib/cstring.h"
#include "lib/log.h"

namespace P4Tools::P4Mutate {

void MutateUtils::emitP4(const IR::P4Program *prog, cstring fileName, bool emitToOutputFolder) {
    auto outFileName = fileName;
    cstring outFolderName = MutateOptions::get().outputFolder;

    if (emitToOutputFolder) {
        outFileName = outFolderName + std::filesystem::path::preferred_separator + fileName;
    }
    auto *outStream = new std::ofstream(outFileName);
    auto *printer = new P4::ToP4(outStream, false);
    prog->apply(*printer);
}

void MutateUtils::printP4(const IR::P4Program *prog) {
    auto *printer = new P4::ToP4(&std::cout, false);
    prog->apply(*printer);
}

static std::pair<int, cstring> exec(cstring cmd) {
    constexpr int bufSize = 1000;
    constexpr int chunkSize = 128;

    std::array<char, bufSize> buffer{};
    std::string output;
    auto *pipe = popen(cmd, "r");  // get rid of shared_ptr

    if (pipe == nullptr) {
        throw std::runtime_error("popen() failed!");
    }

    while (feof(pipe) == 0) {
        if (fgets(buffer.data(), chunkSize, pipe) != nullptr) {
            output += buffer.data();
        }
    }

    auto errCode = pclose(pipe);
    return {errCode, output};
}

ExitInfo MutateUtils::getExitInfo(cstring name) {
    ExitInfo exitInfo;

    cstring command = P4Tools::MutateOptions::get().compiler;
    command += " --Wdisable ";
    command += name;
    // Apparently popen doesn't like stderr hence redirecting stderr to
    // stdout
    command += " 2>&1";
    auto result = exec(command);
    exitInfo.exitCode = WEXITSTATUS(result.first);
    exitInfo.errMsg = result.second;
    exitInfo.isCompilerBug = (exitInfo.errMsg.find("Compiler Bug") == nullptr);

    return exitInfo;
}

int MutateUtils::initializeOutputDir(cstring fileName) {
    auto mutateDirPath = std::filesystem::path(fileName.c_str());
    if (std::filesystem::exists(mutateDirPath)) {
        if (std::filesystem::is_directory(mutateDirPath)) {
            // Directory already exists, try to use it.
            if (!std::filesystem::is_empty(mutateDirPath)) {
                std::cerr << "Mutate directory: " << std::filesystem::absolute(mutateDirPath)
                          << " exists but is not empty; aborting.";
                return EXIT_FAILURE;
            }
            LOG2("Using existing directory: " << mutateDirPath << '\n');
            return EXIT_SUCCESS;
        }
        std::cerr << "Non-directory: " << std::filesystem::absolute(mutateDirPath)
                  << " already exists, either remove it, or provide another output dir using "
                     "--outputDir.";
        return EXIT_FAILURE;
    }

    if (!std::filesystem::create_directory(mutateDirPath)) {
        std::cerr << "Failed to create outputDir; aborting.";
        return EXIT_FAILURE;
    }
    LOG2("Created mutate dir: " << mutateDirPath << '\n');
    return EXIT_SUCCESS;
}

uint64_t MutateUtils::getRandPct() { return getRandInt(100); }

}  // namespace P4Tools::P4Mutate
