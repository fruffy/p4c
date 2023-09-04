#include "backends/p4tools/modules/smith/common/tableDeclaration.h"

#include "backends/p4tools/modules/smith/common/scope.h"
#include "backends/p4tools/modules/smith/util/util.h"

namespace P4Tools {

namespace P4Smith {

IR::P4Table *tableDeclaration::gen() {
    IR::TableProperties *tb_properties = tablePropertyList::gen();
    cstring name = randStr(6);
    auto ret = new IR::P4Table(name, tb_properties);
    P4Scope::add_to_scope(ret);
    P4Scope::callable_tables.emplace(ret);
    return ret;
}
}  // namespace P4Smith

}  // namespace P4Tools
