#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_SPECIFIEDIDENTIFIERLIST_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_SPECIFIEDIDENTIFIERLIST_H_

#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class specifiedIdentifierList {
 public:
    const char *types[1] = {"specifiedIdentifier"};

    specifiedIdentifierList() {}

    // TODO: Tao: better expr generation
    static IR::IndexedVector<IR::SerEnumMember> gen(size_t len);
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_SPECIFIEDIDENTIFIERLIST_H_ */
