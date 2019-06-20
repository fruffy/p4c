/*
Copyright 2013-present Barefoot Networks, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "codeGen.h"
#include "ebpfObject.h"
#include "ebpfType.h"
#include "frontends/p4/enumInstance.h"
#include "frontends/p4/methodInstance.h"
#include "frontends/common/resolveReferences/referenceMap.h"

namespace EBPF {

void initByteArray(CodeBuilder* builder, const IR::Constant *expression) {
    if (!expression->is<IR::Constant>())
        ::error("%1%: Unexpected type", expression);
    unsigned bytes = (expression->type->width_bits() + 7) / 8;
    cstring input = expression->toString();
    builder->append("{");
    if (input.startsWith("0x")) {
        if (strlen(input) % 2 != 0)
          input = input.replace("0x", "0");
        else
          input = input.replace("0x", "");
        const char *pos = input.c_str();
        unsigned char val;
        for (unsigned count = 0; count < bytes; count++) {
          sscanf(pos, "%2hhx", &val);
          builder->appendFormat("0x%02x,", val);
          pos += 2;
        }
    }
    else {
        unsigned value = expression->asUint64();
        unsigned char *int_p=(unsigned char*)&value;
        unsigned width  = (expression->type->width_bits() + 7) / 8;
        for(int i = width - 1; i >= 0; i--)
            builder->appendFormat("0x%02x,", int_p[i]);
    }
    builder->append("}");
}

void CodeGenInspector::substitute(const IR::Parameter* p, const IR::Parameter* with)
{ substitution.emplace(p, with); }

bool CodeGenInspector::preorder(const IR::Constant* expression) {
    unsigned width = (expression->type->width_bits() + 7) / 8;
    builder->appendFormat("((u8 [%d])", width);
    initByteArray(builder, expression);
    builder->append(")");
    return true;
}

bool CodeGenInspector::preorder(const IR::StringLiteral* expression) {
    builder->appendFormat("\"%s\"", expression->toString());
    return true;
}

bool CodeGenInspector::preorder(const IR::Declaration_Variable* decl) {
    auto type = EBPFTypeFactory::instance->create(decl->type);
    type->declare(builder, decl->name.name, false);
    if (decl->initializer != nullptr) {
        builder->append(" = ");
        visit(decl->initializer);
    }
    builder->endOfStatement();
    return false;
}

/* for example:
 * &(u32) {ntohl(ntohl(*(u32 *)headers.ipv4->dstAddr) + }
 */
bool CodeGenInspector::preorder(const IR::Operation_Binary* b) {
    widthCheck(b);
    auto ebpftype = EBPFTypeFactory::instance->create(b->type);
    builder->append("(");
    if(auto st = ebpftype->to<EBPFScalarType>()) {

        /* get the address of the result */
        builder->append("&(");
        st->emit(builder);
        builder->append(")");
        /* convert result back to network byte order */
        builder->append("{");
        st->byteOperator(builder);
        builder->append("(");
        /* convert left to host-order integer */
        st->byteOperator(builder);
        builder->append("(*(");
        st->emit(builder);
        builder->append(" *)");
        visit(b->left);
        builder->append(")");
        /* the operator */
        builder->spc();
        builder->append(b->getStringOp());
        builder->spc();
        /* convert right to host-order integer */
        st->byteOperator(builder);
        builder->append("(*(");
        st->emit(builder);
        builder->append(" *)");
        visit(b->right);
        builder->append(")");;
        /* done */
        builder->append(")");
        builder->append("}");
    } else {
        visit(b->left);
        builder->spc();
        builder->append(b->getStringOp());
        builder->spc();
        visit(b->right);
    }
    builder->append(")");
    return false;
}


bool CodeGenInspector::comparison(const IR::Operation_Relation* b) {
    unsigned memcmp_size = (b->right->type->width_bits() + 7) / 8;
    builder->append("(memcmp(");
    visit(b->left);
    builder->append(", ");
    visit(b->right);
    builder->appendFormat(", %d) ", memcmp_size);
    builder->append(b->getStringOp());
    builder->append(" 0)");

    return false;
}

bool CodeGenInspector::preorder(const IR::Mux* b) {
    widthCheck(b);
    builder->append("(");
    visit(b->e0);
    builder->append(" ? ");
    visit(b->e1);
    builder->append(" : ");
    visit(b->e2);
    builder->append(")");
    return false;
}

bool CodeGenInspector::preorder(const IR::Operation_Unary* u) {
    widthCheck(u);
    builder->append("(");
    builder->append(u->getStringOp());
    visit(u->expr);
    builder->append(")");
    return false;
}

bool CodeGenInspector::preorder(const IR::ArrayIndex* a) {
    builder->append("(");
    visit(a->left);
    builder->append("[");
    if (auto ct = a->right->to<IR::Constant>())
        builder->appendFormat("%lu]", ct->asUint64());
    else
        ::error("%1%: Unexpected type", a->right);
    builder->append(")");
    return false;
}

bool CodeGenInspector::preorder(const IR::Cast* c) {
    widthCheck(c);
    builder->append("(");
    builder->append("(");
    auto et = EBPFTypeFactory::instance->create(c->destType);
    et->emit(builder);
    builder->append(")");
    builder->append(")");
    visit(c->expr);
    return false;
}

bool CodeGenInspector::preorder(const IR::Member* expression) {
    auto ei = P4::EnumInstance::resolve(expression, typeMap);
    if (ei == nullptr) {
        visit(expression->expr);
        auto type = expression->expr->type;
        if (type->is<IR::Type_Struct>() || type->is<IR::Type_HeaderUnion>())
            builder->append(".");
        else
            builder->append(".");
    }
    builder->append(expression->member);
    return false;
}

bool CodeGenInspector::preorder(const IR::PathExpression* expression) {
    visit(expression->path);
    return false;
}

bool CodeGenInspector::preorder(const IR::Path* p) {
    if (p->absolute)
        ::error("%1%: Unexpected absolute path", p);
    builder->append(p->name);
    return false;
}

bool CodeGenInspector::preorder(const IR::BoolLiteral* b) {
    builder->append(b->toString());
    return false;
}

bool CodeGenInspector::preorder(const IR::ListExpression* expression) {
    bool first = true;
    for (auto e : expression->components) {
        if (!first)
            builder->append(", ");
        first = false;
        visit(e);
    }
    return false;
}

bool CodeGenInspector::preorder(const IR::MethodCallExpression* expression) {
    auto mi = P4::MethodInstance::resolve(expression, refMap, typeMap);
    auto bim = mi->to<P4::BuiltInMethod>();
    if (bim != nullptr) {
        builder->emitIndent();
        auto type = typeMap->getType(bim->appliedTo);
        auto ht = type->to<IR::Type_Header>();
        if (bim->name == IR::Type_Header::isValid) {
            builder->appendFormat("%s_valid", ht->name.name);
            return false;
        } else if (bim->name == IR::Type_Header::setValid) {
            builder->appendFormat("%s_valid = true", ht->name.name);
            return false;
        } else if (bim->name == IR::Type_Header::setInvalid) {
            builder->appendFormat("%s_valid = false", ht->name.name);
            return false;
        }
    }

    visit(expression->method);
    builder->append("(");
    bool first = true;
    for (auto p : *mi->substitution.getParametersInArgumentOrder()) {
        if (!first)
            builder->append(", ");
        first = false;

        if (p->direction == IR::Direction::Out ||
            p->direction == IR::Direction::InOut)
            builder->append("&");
        auto arg = mi->substitution.lookup(p);
        visit(arg);
    }
    builder->append(")");
    return false;
}

/////////////////////////////////////////

bool CodeGenInspector::preorder(const IR::Type_Typedef* type) {
    auto et = EBPFTypeFactory::instance->create(type->type);
    builder->append("typedef ");
    et->emit(builder);
    builder->spc();
    builder->append(type->name);
    builder->endOfStatement();
    return false;
}

bool CodeGenInspector::preorder(const IR::Type_Enum* type) {
    builder->append("enum ");
    builder->append(type->name);
    builder->spc();
    builder->blockStart();
    for (auto e : *type->getDeclarations()) {
        builder->emitIndent();
        builder->append(e->getName().name);
        builder->appendLine(",");
    }
    builder->blockEnd(true);
    return false;
}

bool CodeGenInspector::preorder(const IR::AssignmentStatement* a) {
    auto ltype = typeMap->getType(a->left);
    auto rtype = typeMap->getType(a->left);
    auto lebpfType = EBPFTypeFactory::instance->create(ltype);
    auto rebpfType = EBPFTypeFactory::instance->create(rtype);
    EBPFScalarType* scalar = nullptr;

    if (a->right->is<IR::MethodCallExpression>()) {
        auto mc = a->right->to<IR::MethodCallExpression>();
        auto mi = P4::MethodInstance::resolve(mc, refMap, typeMap);
        visit(mc->method);
        bool first = true;
        builder->append("(");
        for (auto p : *mi->substitution.getParametersInArgumentOrder()) {
            if (first) {
                builder->append("");
                visit(a->left);
                first = false;
            }
            builder->append(", ");

            if (p->direction == IR::Direction::Out ||
                p->direction == IR::Direction::InOut)
                builder->append("&");
            auto arg = mi->substitution.lookup(p);
            visit(arg);
        }
        builder->append(")");
        builder->endOfStatement();
    } else if(lebpfType->is<EBPFScalarType>() || rebpfType->is<EBPFScalarType>()){
        scalar = lebpfType->to<EBPFScalarType>();
        builder->append("memcpy(");
        visit(a->left);
        builder->append(", ");
        visit(a->right);
        builder->appendFormat(", %d)", scalar->bytesRequired());
        builder->endOfStatement();
    } else  {
        visit(a->left);
        builder->append(" = ");
        visit(a->right);
        builder->endOfStatement();
    }
    return false;
}

bool CodeGenInspector::preorder(const IR::BlockStatement* s) {
    builder->blockStart();
    bool first = true;
    for (auto a : s->components) {
        if (!first) {
            builder->newline();
            builder->emitIndent();
        }
        first = false;
        visit(a);
    }
    if (!s->components.empty())
        builder->newline();
    builder->blockEnd(false);
    return false;
}

// This is correct only after inlining
bool CodeGenInspector::preorder(const IR::ExitStatement*) {
    builder->append("return");
    builder->endOfStatement();
    return false;
}

bool CodeGenInspector::preorder(const IR::ReturnStatement*) {
    builder->append("return");
    builder->endOfStatement();
    return false;
}

bool CodeGenInspector::preorder(const IR::EmptyStatement*) {
    builder->endOfStatement();
    return false;
}

bool CodeGenInspector::preorder(const IR::IfStatement* s) {
    builder->append("if( ");
    visit(s->condition);
    builder->append(") ");
    if (!s->ifTrue->is<IR::BlockStatement>()) {
        builder->increaseIndent();
        builder->newline();
        builder->emitIndent();
    }
    visit(s->ifTrue);
    if (!s->ifTrue->is<IR::BlockStatement>())
        builder->decreaseIndent();
    if (s->ifFalse != nullptr) {
        builder->newline();
        builder->emitIndent();
        builder->append("else ");
        builder->increaseIndent();
        builder->newline();
        builder->emitIndent();
        visit(s->ifFalse);
        builder->decreaseIndent();
    }
    return false;
}

bool CodeGenInspector::preorder(const IR::MethodCallStatement* s) {
    visit(s->methodCall);
    builder->endOfStatement();
    return false;
}

void CodeGenInspector::widthCheck(const IR::Node* node) const {
    // This is a temporary solution.
    // Rather than generate incorrect results, we reject programs that
    // do not perform arithmetic on machine-supported widths.
    // In the future we will support a wider range of widths
    CHECK_NULL(node);
    auto type = typeMap->getType(node, true);
    auto tb = type->to<IR::Type_Bits>();
    if (tb == nullptr) return;
    if (tb->size % 8 == 0 && EBPFScalarType::generatesScalar(tb->size))
        return;

    if (tb->size <= 64)
        // This is a bug which we can probably fix
        BUG("%1%: Computations on %2% bits not yet supported", node, tb->size);
    // We could argue that this may not be supported ever
    ::error("%1%: Computations on %2% bits not supported", node, tb->size);
}

}  // namespace EBPF
