#include "backends/p4tools/modules/smith/common/structTypeDeclaration.h"

#include <vector>

#include "backends/p4tools/modules/smith/common/annotations.h"
#include "backends/p4tools/modules/smith/common/baseType.h"
#include "backends/p4tools/modules/smith/common/headerStackType.h"
#include "backends/p4tools/modules/smith/common/scope.h"
#include "backends/p4tools/modules/smith/common/structFieldList.h"
#include "backends/p4tools/modules/smith/common/typeRef.h"
#include "backends/p4tools/modules/smith/util/util.h"

namespace P4Tools {

namespace P4Smith {

IR::Type_Struct *structTypeDeclaration::gen() {
    cstring name = randStr(6);

    IR::IndexedVector<IR::StructField> fields;
    typeref_probs type_percent = {
        PCT.STRUCTTYPEDECLARATION_BASETYPE_BIT,    PCT.STRUCTTYPEDECLARATION_BASETYPE_SIGNED_BIT,
        PCT.STRUCTTYPEDECLARATION_BASETYPE_VARBIT, PCT.STRUCTTYPEDECLARATION_BASETYPE_INT,
        PCT.STRUCTTYPEDECLARATION_BASETYPE_ERROR,  PCT.STRUCTTYPEDECLARATION_BASETYPE_BOOL,
        PCT.STRUCTTYPEDECLARATION_BASETYPE_STRING, PCT.STRUCTTYPEDECLARATION_DERIVED_ENUM,
        PCT.STRUCTTYPEDECLARATION_DERIVED_HEADER,  PCT.STRUCTTYPEDECLARATION_DERIVED_HEADER_STACK,
        PCT.STRUCTTYPEDECLARATION_DERIVED_STRUCT,  PCT.STRUCTTYPEDECLARATION_DERIVED_HEADER_UNION,
        PCT.STRUCTTYPEDECLARATION_DERIVED_TUPLE,   PCT.STRUCTTYPEDECLARATION_TYPE_VOID,
        PCT.STRUCTTYPEDECLARATION_TYPE_MATCH_KIND,
    };
    auto l_types = P4Scope::get_decls<IR::Type_Header>();
    if (l_types.size() == 0) {
        return nullptr;
    }
    size_t len = getRndInt(1, 5);

    for (size_t i = 0; i < len; i++) {
        IR::Type *field_tp = typeRef::pick_rnd_type(type_percent);
        cstring field_name = randStr(4);
        if (field_tp->to<IR::Type_Stack>()) {
            // Right now there is now way to initialize a header stack
            // So we have to add the entire structure to the banned expressions
            P4Scope::not_initialized_structs.insert(name);
        }
        IR::StructField *sf = new IR::StructField(field_name, field_tp);
        fields.push_back(sf);
    }

    auto ret = new IR::Type_Struct(name, fields);

    P4Scope::add_to_scope(ret);

    return ret;
}

IR::Type_Struct *structTypeDeclaration::gen_Headers() {
    IR::IndexedVector<IR::StructField> fields;

    // Tao: hard code for ethernet_t eth_hdr;
    auto eth_sf = new IR::StructField(ETH_HDR, new IR::Type_Name(ETH_HEADER_T));
    fields.push_back(eth_sf);

    size_t len = getRndInt(1, 5);
    // we can only generate very specific types for headers
    // header, header stack, header union
    std::vector<int64_t> percent = {PCT.STRUCTTYPEDECLARATION_HEADERS_HEADER,
                                    PCT.STRUCTTYPEDECLARATION_HEADERS_STACK};
    for (size_t i = 0; i < len; i++) {
        cstring field_name = randStr(4);
        IR::Type *tp = nullptr;
        switch (randInd(percent)) {
            case 0: {
                // TODO: We have to assume that this works
                auto l_types = P4Scope::get_decls<IR::Type_Header>();
                if (l_types.size() == 0) {
                    BUG("structTypeDeclaration: No available header for Headers!");
                }
                auto candidate_type = l_types.at(getRndInt(0, l_types.size() - 1));
                tp = new IR::Type_Name(candidate_type->name.name);
                break;
            }
            case 1: {
                tp = headerStackType::gen();
                // Right now there is now way to initialize a header stack
                // So we have to add the entire structure to the banned expressions
                P4Scope::not_initialized_structs.insert("Headers");
            }
        }
        fields.push_back(new IR::StructField(field_name, tp));
    }
    auto ret = new IR::Type_Struct("Headers", fields);

    P4Scope::add_to_scope(ret);

    return ret;
}

}  // namespace P4Smith

}  // namespace P4Tools
