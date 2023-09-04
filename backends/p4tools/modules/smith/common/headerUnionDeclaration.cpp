#include "backends/p4tools/modules/smith/common/headerUnionDeclaration.h"

#include <set>

namespace P4Tools {

namespace P4Smith {

IR::Type_HeaderUnion *headerUnionDeclaration::gen() {
    cstring name = randStr(6);

    IR::IndexedVector<IR::StructField> fields;
    auto l_types = P4Scope::get_decls<IR::Type_Header>();
    if (l_types.size() < 2) {
        BUG("Creating a header union assumes at least two headers!");
    }
    // !sure if this correct...
    size_t len = getRndInt(2, l_types.size() - 2);
    std::set<cstring> visited_headers;
    // we need to guarantee correct execution so try as long as we can
    // this is a bit dicey... do !like it
    int attempts = 0;
    while (true) {
        if (attempts >= 100) {
            BUG("We should !need this many attempts!");
        }
        attempts++;
        cstring field_name = randStr(4);
        auto hdr_tp = l_types.at(getRndInt(0, l_types.size() - 1));
        // check if we have already added this header
        if (visited_headers.find(hdr_tp->name) != visited_headers.end()) {
            continue;
        }
        auto tp_name = new IR::Type_Name(hdr_tp->name);
        IR::StructField *sf = new IR::StructField(field_name, tp_name);
        visited_headers.insert(hdr_tp->name);
        fields.push_back(sf);
        if (fields.size() == len) {
            break;
        }
    }

    auto ret = new IR::Type_HeaderUnion(name, fields);

    P4Scope::add_to_scope(ret);

    return ret;
}

}  // namespace P4Smith

}  // namespace P4Tools
