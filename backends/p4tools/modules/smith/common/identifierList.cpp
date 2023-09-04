#include "backends/p4tools/modules/smith/common/identifierList.h"

#include <set>

#include "backends/p4tools/modules/smith/util/util.h"

namespace P4Tools {

namespace P4Smith {

IR::IndexedVector<IR::Declaration_ID> identifierList::gen(size_t len) {
    IR::IndexedVector<IR::Declaration_ID> decl_ids;
    std::set<cstring> decl_ids_name;

    for (size_t i = 0; i < len; i++) {
        cstring name = randStr(2);
        IR::Declaration_ID *decl_id = new IR::Declaration_ID(name);

        if (decl_ids_name.find(name) != decl_ids_name.end()) {
            delete name;
            delete decl_id;
            continue;
        }

        decl_ids.push_back(decl_id);
    }

    return decl_ids;
}

}  // namespace P4Smith

}  // namespace P4Tools
