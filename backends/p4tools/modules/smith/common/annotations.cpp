#include "backends/p4tools/modules/smith/common/annotations.h"

#include "backends/p4tools/modules/smith/util/util.h"

namespace P4Tools {

namespace P4Smith {

IR::Annotations *Annotations::gen() {
    Util::SourceInfo si;
    IR::Vector<IR::Annotation> annotations;
    IR::Vector<IR::Expression> exprs;
    cstring name = IR::Annotation::nameAnnotation;
    auto str_literal = new IR::StringLiteral(randStr(6));

    exprs.push_back(str_literal);

    auto annotation = new IR::Annotation(si, name, exprs, false);
    annotations.push_back(annotation);

    return new IR::Annotations(annotations);
}
}  // namespace P4Smith

}  // namespace P4Tools
