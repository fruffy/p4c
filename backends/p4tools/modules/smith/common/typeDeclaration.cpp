#include "backends/p4tools/modules/smith/common/typeDeclaration.h"

#include <vector>

#include "backends/p4tools/modules/smith/common/enumDeclaration.h"
#include "backends/p4tools/modules/smith/common/headerStackType.h"
#include "backends/p4tools/modules/smith/common/headerTypeDeclaration.h"
#include "backends/p4tools/modules/smith/common/headerUnionDeclaration.h"
#include "backends/p4tools/modules/smith/common/structTypeDeclaration.h"
#include "backends/p4tools/modules/smith/util/util.h"

namespace P4Tools {

namespace P4Smith {

IR::Type_Declaration *typeDeclaration::gen() {
    std::vector<int64_t> percent = {PCT.TYPEDECLARATION_HEADER, PCT.TYPEDECLARATION_STRUCT,
                                    PCT.TYPEDECLARATION_UNION};
    IR::Type_Declaration *decl = nullptr;
    bool use_default_decl = false;
    switch (randInd(percent)) {
        case 0: {
            use_default_decl = true;
            break;
        }
        case 1: {
            decl = structTypeDeclaration::gen();
            break;
        }
        case 2: {
            // header unions are disabled for now, need to fix assignments
            auto hdrs = P4Scope::get_decls<IR::Type_Header>();
            // we can only generate a union if we have at least two headers
            if (hdrs.size() > 1) {
                decl = headerUnionDeclaration::gen();
                if (!decl) {
                    use_default_decl = true;
                }
            } else {
                use_default_decl = true;
            }
            break;
        }
    }
    if (use_default_decl) decl = headerTypeDeclaration::gen();

    return decl;
}

}  // namespace P4Smith

}  // namespace P4Tools
