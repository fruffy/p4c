#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_CORE_TARGET_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_CORE_TARGET_H_

#include <stddef.h>
#include <stdint.h>

#include <map>
#include <optional>
#include <string>
#include <vector>

#include "ir/ir.h"
#include "lib/cstring.h"
#include "lib/exceptions.h"

// #include "backends/p4tools/common/core/solver.h"
#include "backends/p4tools/common/core/target.h"
// #include "backends/p4tools/modules/smith/core/program_info.h"
// #include "backends/p4tools/modules/smith/core/small_step/cmd_stepper.h"
// #include "backends/p4tools/modules/smith/core/small_step/expr_stepper.h"
// #include "backends/p4tools/modules/smith/core/symbolic_executor.h"
// #include "backends/p4tools/modules/smith/lib/execution_state.h"
// #include "backends/p4tools/modules/smith/lib/namespace_context.h"
// #include "backends/p4tools/modules/smith/lib/test_backend.h"

namespace P4Tools {

namespace P4Smith {

/// Specifies a canonical representation of the target pipeline as documented in P4 code.
// class ArchSpec {
//  public:
//     /// An ArchMember represents a construct in the pipe. It has a name and parameters.
//     struct ArchMember {
//         cstring blockName;
//         std::vector<cstring> blockParams;
//     };

//  private:
//     /// The ArchSpec has a name that typically corresponds with the name of the package for main.
//     cstring packageName;

//     /// The ordered list of members in the architecture specification.
//     std::vector<ArchMember> archVector;

//     /// Keeps track of the block indices in the architecture specification.
//     /// This is useful to lookup the index for a particular block label.
//     std::map<cstring, size_t> blockIndices;

//  public:
//     explicit ArchSpec(cstring packageName, const std::vector<ArchMember>& archVectorInput);

//     /// @returns the index that corresponds to the given name in this architecture specification.
//     /// A bug is thrown if the index does not exist.
//     size_t getBlockIndex(cstring blockName) const;

//     /// @returns the architecture member that corresponds to the given index in this architecture
//     /// specification. A bug is thrown if the index does not exist.
//     const ArchMember* getArchMember(size_t blockIndex) const;

//     /// @returns name of the parameter for the given block and parameter index in this
//     architecture
//     /// specification. A bug is thrown if the indices are out of range.
//     cstring getParamName(size_t blockIndex, size_t paramIndex) const;

//     /// @returns name of the parameter for the given block label and parameter index in this
//     /// architecture specification. A bug is thrown if the index is out of range or the block
//     label
//     /// does not exist.
//     cstring getParamName(cstring blockName, size_t paramIndex) const;

//     /// @returns the size of the architecture specification vector.
//     size_t getArchVectorSize() const;

//     /// @returns the label of the architecture specification.
//     cstring getPackageName() const;
// };

class SmithTarget : public P4Tools::Target {
 public:
    /// @returns the singleton instance for the current target.
    static const SmithTarget &get();

 protected:
    explicit SmithTarget(std::string deviceName, std::string archName);

 private:
};

}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_CORE_TARGET_H_ */
