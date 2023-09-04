#include "backends/p4tools/modules/smith/common/specifiedIdentifierList.h"

#include <set>

#include "backends/p4tools/modules/smith/common/baseType.h"
#include "backends/p4tools/modules/smith/util/util.h"

namespace P4Tools {

namespace P4Smith {

IR::IndexedVector<IR::SerEnumMember> specifiedIdentifierList::gen(size_t len) {
    IR::IndexedVector<IR::SerEnumMember> members;
    std::set<cstring> members_name;

    for (size_t i = 0; i < len; i++) {
        cstring name = randStr(2);
        IR::Expression *ex = baseType::gen_int_literal();

        if (members_name.find(name) != members_name.end()) {
            delete ex;
            continue;
        }

        auto member = new IR::SerEnumMember(name, ex);

        members.push_back(member);
    }

    return members;
}

}  // namespace P4Smith

}  // namespace P4Tools
