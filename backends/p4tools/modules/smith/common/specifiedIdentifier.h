#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_SPECIFIEDIDENTIFIER_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_SPECIFIEDIDENTIFIER_H_

#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class specifiedIdentifier {
 public:
    specifiedIdentifier() {}

    // TODO: Tao: better expr generation
    static IR::IndexedVector<IR::SerEnumMember> gen(size_t len);
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_SPECIFIEDIDENTIFIER_H_ */
