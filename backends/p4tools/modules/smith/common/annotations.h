#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_ANNOTATIONS_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_ANNOTATIONS_H_

#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

class Annotations {
 public:
    Annotations() {}

    static IR::Annotations *gen();
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_ANNOTATIONS_H_ */
