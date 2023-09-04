#include "backends/p4tools/modules/smith/common/tableProperty.h"

#include "backends/p4tools/modules/smith/common/actionList.h"
#include "backends/p4tools/modules/smith/common/keyElementList.h"

namespace P4Tools {

namespace P4Smith {

IR::Property *tableProperty::gen_keys() {
    cstring name = IR::TableProperties::keyPropertyName;
    auto keys = keyElementList::gen(getRndInt(0, 3));

    // isConstant --> false
    return new IR::Property(name, keys, false);
}

IR::Property *tableProperty::gen_act_lists() {
    cstring name = IR::TableProperties::actionsPropertyName;
    auto acts = actionList::gen(getRndInt(0, 3));

    return new IR::Property(name, acts, false);
}
}  // namespace P4Smith

}  // namespace P4Tools
