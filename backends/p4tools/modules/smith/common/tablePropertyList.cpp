

#include "backends/p4tools/modules/smith/common/tablePropertyList.h"

namespace P4Tools {

namespace P4Smith {

IR::TableProperties *tablePropertyList::gen() {
    IR::IndexedVector<IR::Property> tab_properties;

    auto tab_property_gen = new tableProperty();

    tab_properties.push_back(tab_property_gen->gen_keys());
    tab_properties.push_back(tab_property_gen->gen_act_lists());

    return new IR::TableProperties(tab_properties);
}

}  // namespace P4Smith

}  // namespace P4Tools
