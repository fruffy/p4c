/*
Copyright 2024-present New York University.

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

#include <gtest/gtest.h>

#include "ir/ir.h"
#include "ir/node.h"
#include "ir/visitor.h"

static inline bool isSemanticallyLess(const IR::Node &a, const IR::Node &b) {
    return a.isSemanticallyLess(b);
}

#define EXECUTE_FUNCTION_FOR_P4C_NODE(BASE_TYPE, a, b, function)                                   \
    EXPECT_TRUE(function(*(a), *(b)));                                                             \
    EXPECT_TRUE(function(*(a), *static_cast<const BASE_TYPE *>(b)));                               \
    EXPECT_TRUE(function(*(a), *static_cast<const IR::Node *>(b)));                                \
    EXPECT_TRUE(function(*static_cast<const BASE_TYPE *>(a), *(b)));                               \
    EXPECT_TRUE(function(*static_cast<const BASE_TYPE *>(a), *static_cast<const BASE_TYPE *>(b))); \
    EXPECT_TRUE(function(*static_cast<const BASE_TYPE *>(a), *static_cast<const IR::Node *>(b)));  \
    EXPECT_TRUE(function(*static_cast<const IR::Node *>(a), *(b)));                                \
    EXPECT_TRUE(function(*static_cast<const IR::Node *>(a), *static_cast<const BASE_TYPE *>(b)));  \
    EXPECT_TRUE(function(*static_cast<const IR::Node *>(a), *static_cast<const IR::Node *>(b)));

#define CHECK_LESS_FOR_P4C_NODE(BASE_TYPE, a, b) \
    EXECUTE_FUNCTION_FOR_P4C_NODE(BASE_TYPE, a, b, isSemanticallyLess)
#define CHECK_LESS_FOR_P4C_TYPE(a, b) CHECK_LESS_FOR_P4C_NODE(IR::Type, a, b)
#define CHECK_LESS_FOR_P4C_EXPRESSION(a, b) CHECK_LESS_FOR_P4C_NODE(IR::Expression, a, b)
#define CHECK_LESS_FOR_P4C_NODE_VECTOR(a, b) CHECK_LESS_FOR_P4C_NODE(IR::Vector<IR::Node>, a, b)
#define CHECK_LESS_FOR_P4C_DECLARATION(a, b) CHECK_LESS_FOR_P4C_NODE(IR::Declaration, a, b)

#define CHECK_GREATER_FOR_P4C_TYPE(a, b) CHECK_LESS_FOR_P4C_TYPE(b, a)
#define CHECK_GREATER_FOR_P4C_EXPRESSION(a, b) CHECK_LESS_FOR_P4C_EXPRESSION(b, a)
#define CHECK_GREATER_FOR_P4C_NODE_VECTOR(a, b) CHECK_LESS_FOR_P4C_NODE_VECTOR(b, a)
#define CHECK_GREATER_FOR_P4C_DECLARATION(a, b) CHECK_LESS_FOR_P4C_DECLARATION(b, a)

bool checkEqualityWithLess(const IR::Node &a, const IR::Node &b) {
    return !(isSemanticallyLess(a, b) || isSemanticallyLess(b, a));
}

#define CHECK_EQUALITY_WITH_LESS(BASE_TYPE, a, b) \
    EXECUTE_FUNCTION_FOR_P4C_NODE(BASE_TYPE, a, b, checkEqualityWithLess)
#define CHECK_EQUALITY_FOR_P4C_TYPE(a, b) CHECK_EQUALITY_WITH_LESS(IR::Type, a, b)
#define CHECK_EQUALITY_FOR_P4C_EXPRESSION(a, b) CHECK_EQUALITY_WITH_LESS(IR::Expression, a, b)
#define CHECK_EQUALITY_FOR_P4C_NODE_VECTOR(a, b) \
    CHECK_EQUALITY_WITH_LESS(IR::Vector<IR::Node>, a, b)
#define CHECK_EQUALITY_FOR_P4C_DECLARATION(a, b) CHECK_EQUALITY_WITH_LESS(IR::Declaration, a, b)

TEST(OperatorLess, Types) {
    const auto *a = IR::Type_Bits::get(16, false);
    const auto *b = IR::Type_Bits::get(32, false);
    const auto *c = IR::Type_Bits::get(16, false);

    CHECK_LESS_FOR_P4C_TYPE(a, b);
    CHECK_GREATER_FOR_P4C_TYPE(b, a);
    CHECK_EQUALITY_FOR_P4C_TYPE(a, c);

    const auto *d = IR::Type_Boolean::get();
    CHECK_LESS_FOR_P4C_TYPE(d, a);
    CHECK_GREATER_FOR_P4C_TYPE(a, d);

    const auto *e = IR::Type_InfInt::get();
    CHECK_LESS_FOR_P4C_TYPE(a, e);
    CHECK_GREATER_FOR_P4C_TYPE(e, a);

    const auto *f = new IR::Type_Name("f");
    const auto *f2 = new IR::Type_Name("f");
    CHECK_LESS_FOR_P4C_TYPE(a, f);
    CHECK_GREATER_FOR_P4C_TYPE(f, a);
    CHECK_EQUALITY_FOR_P4C_TYPE(f, f2)

    const auto *g = IR::Type_String::get();
    CHECK_LESS_FOR_P4C_TYPE(a, g);
    CHECK_GREATER_FOR_P4C_TYPE(g, a);
}

TEST(OperatorLess, Constants) {
    // Check unsigned constants.
    {
        const auto *a = new IR::Constant(IR::Type_Bits::get(16, false), 5);
        const auto *b = new IR::Constant(IR::Type_Bits::get(16, false), 10);
        const auto *c = new IR::Constant(IR::Type_Bits::get(16, false), 5);

        CHECK_LESS_FOR_P4C_EXPRESSION(a, b);
        CHECK_GREATER_FOR_P4C_EXPRESSION(b, a);
        CHECK_EQUALITY_FOR_P4C_EXPRESSION(a, c);
    }
    // Check signed constants.
    {
        const auto *a = new IR::Constant(IR::Type_Bits::get(16, true), -1);
        const auto *b = new IR::Constant(IR::Type_Bits::get(16, true), 0);
        const auto *c = new IR::Constant(IR::Type_Bits::get(16, true), -1);

        CHECK_LESS_FOR_P4C_EXPRESSION(a, b);
        CHECK_EQUALITY_FOR_P4C_EXPRESSION(a, c);
    }
    // Check Strings
    {
        const auto *a = new IR::StringLiteral(IR::Type_String::get(), "a");
        const auto *b = new IR::StringLiteral(IR::Type_String::get(), "b");
        const auto *a2 = new IR::StringLiteral(IR::Type_String::get(), "a");

        CHECK_LESS_FOR_P4C_EXPRESSION(a, b);
        CHECK_GREATER_FOR_P4C_EXPRESSION(b, a);
        CHECK_EQUALITY_FOR_P4C_EXPRESSION(a, a2);
    }
    // Check booleans
    {
        const auto *a = new IR::BoolLiteral(false);
        const auto *b = new IR::BoolLiteral(true);
        const auto *c = new IR::BoolLiteral(false);

        CHECK_LESS_FOR_P4C_EXPRESSION(a, b);
        CHECK_GREATER_FOR_P4C_EXPRESSION(b, a);
        CHECK_EQUALITY_FOR_P4C_EXPRESSION(a, c);
    }
    // Check infinite precision integers.
    {
        const auto *a = new IR::Constant(IR::Type_InfInt::get(), 5);
        const auto *b = new IR::Constant(IR::Type_InfInt::get(), 10);
        const auto *c = new IR::Constant(IR::Type_InfInt::get(), 5);

        CHECK_LESS_FOR_P4C_EXPRESSION(a, b);
        CHECK_GREATER_FOR_P4C_EXPRESSION(b, a);
        CHECK_EQUALITY_FOR_P4C_EXPRESSION(a, c);
    }
}

TEST(OperatorLess, MixedConstants) {
    const auto *t = IR::Type_Bits::get(16, false);
    const auto *a = new IR::StringLiteral(IR::Type_String::get(), "a");
    const auto *b = new IR::Constant(t, 0);
    const auto *c = new IR::Constant(t, 10);
    const auto *d = new IR::BoolLiteral(false);

    CHECK_LESS_FOR_P4C_EXPRESSION(b, a);
    CHECK_LESS_FOR_P4C_EXPRESSION(c, a);
    CHECK_GREATER_FOR_P4C_EXPRESSION(a, b);
    CHECK_GREATER_FOR_P4C_EXPRESSION(a, c);

    CHECK_LESS_FOR_P4C_EXPRESSION(d, a);
    CHECK_GREATER_FOR_P4C_EXPRESSION(a, d);
    CHECK_LESS_FOR_P4C_EXPRESSION(b, d);
    CHECK_GREATER_FOR_P4C_EXPRESSION(d, b);
}

TEST(OperatorLess, ConstantVectors) {
    // Two empty vectors are equal.
    const auto *e1 = new IR::IndexedVector<IR::Node>();
    const auto *e2 = new IR::IndexedVector<IR::Node>();
    CHECK_EQUALITY_FOR_P4C_NODE_VECTOR(e1, e2);

    const auto *t = IR::Type_Bits::get(16, false);
    const auto *a = new IR::Constant(t, 5);
    const auto *b = new IR::Constant(t, 10);
    const auto *c = new IR::Constant(t, 5);

    auto *p1 = new IR::IndexedVector<IR::Node>(a);
    auto *p2 = new IR::IndexedVector<IR::Node>(b);
    auto *p3 = new IR::IndexedVector<IR::Node>(c);

    CHECK_LESS_FOR_P4C_NODE_VECTOR(p1, p2);
    CHECK_GREATER_FOR_P4C_NODE_VECTOR(p2, p1);
    CHECK_EQUALITY_FOR_P4C_NODE_VECTOR(p1, p3);

    // Check that we correctly compare uneven vectors.
    p2->push_back(b);
    CHECK_LESS_FOR_P4C_NODE_VECTOR(p1, p2);
    CHECK_GREATER_FOR_P4C_NODE_VECTOR(p2, p1);
    p1->push_back(b);
    p1->push_back(b);
    CHECK_LESS_FOR_P4C_NODE_VECTOR(p1, p2);
    CHECK_GREATER_FOR_P4C_NODE_VECTOR(p2, p1);

    // Check that we correctly compare vectors with different nodes.
    const auto *p4 = new IR::IndexedVector<IR::Node>(new IR::BoolLiteral(false));
    const auto *p5 = new IR::IndexedVector<IR::Node>(new IR::Constant(t, 0));
    const auto *p6 =
        new IR::IndexedVector<IR::Node>(new IR::StringLiteral(IR::Type_String::get(), "a"));

    CHECK_LESS_FOR_P4C_NODE_VECTOR(p5, p4);
    CHECK_GREATER_FOR_P4C_NODE_VECTOR(p4, p5);
    CHECK_LESS_FOR_P4C_NODE_VECTOR(p4, p6);
    CHECK_GREATER_FOR_P4C_NODE_VECTOR(p6, p4);
    CHECK_LESS_FOR_P4C_NODE_VECTOR(p5, p6);
    CHECK_GREATER_FOR_P4C_NODE_VECTOR(p6, p5);
}

TEST(OperatorLess, UnaryExpressions) {
    const auto *t = IR::Type_Bits::get(16, false);
    const auto *a = new IR::Constant(t, 5);
    const auto *b = new IR::Constant(t, 10);
    const auto *c = new IR::Constant(t, 5);

    const auto *p1 = new IR::LNot(a);
    const auto *p2 = new IR::LNot(b);
    const auto *p3 = new IR::LNot(c);

    CHECK_LESS_FOR_P4C_EXPRESSION(p1, p2);
    CHECK_GREATER_FOR_P4C_EXPRESSION(p2, p1);
    CHECK_EQUALITY_FOR_P4C_EXPRESSION(p1, p3);
}

TEST(OperatorLess, BinaryExpressions) {
    const auto *t = IR::Type_Bits::get(16, false);
    const auto *a = new IR::Constant(t, 5);
    const auto *b = new IR::Constant(t, 10);
    const auto *c = new IR::Constant(t, 5);

    const auto *p1 = new IR::Add(a, b);
    const auto *p2 = new IR::Add(b, a);
    const auto *p3 = new IR::Add(c, b);

    CHECK_LESS_FOR_P4C_EXPRESSION(p1, p2);
    CHECK_GREATER_FOR_P4C_EXPRESSION(p2, p1);
    CHECK_EQUALITY_FOR_P4C_EXPRESSION(p1, p3);
}

TEST(OperatorLess, TernaryExpressions) {
    const auto *t = IR::Type_Bits::get(16, false);
    const auto *a = new IR::Constant(t, 5);
    const auto *b = new IR::Constant(t, 10);
    const auto *c = new IR::Constant(t, 5);

    auto *trueCond = new IR::BoolLiteral(true);
    auto *falseCond = new IR::BoolLiteral(false);

    const auto *p1 = new IR::Mux(trueCond, a, c);
    const auto *p2 = new IR::Mux(trueCond, b, c);
    const auto *p3 = new IR::Mux(trueCond, c, a);

    CHECK_LESS_FOR_P4C_EXPRESSION(p1, p2);
    CHECK_GREATER_FOR_P4C_EXPRESSION(p2, p1);
    CHECK_EQUALITY_FOR_P4C_EXPRESSION(p1, p3);

    const auto *p4 = new IR::Mux(falseCond, a, c);

    CHECK_LESS_FOR_P4C_EXPRESSION(p4, p1);
    CHECK_LESS_FOR_P4C_EXPRESSION(p4, p2);
    CHECK_GREATER_FOR_P4C_EXPRESSION(p1, p4);
    CHECK_GREATER_FOR_P4C_EXPRESSION(p2, p4);
}

TEST(OperatorLess, PathExpressions) {
    const auto *a = new IR::PathExpression("a");
    const auto *b = new IR::PathExpression("b");
    const auto *a2 = new IR::PathExpression("a");

    CHECK_LESS_FOR_P4C_EXPRESSION(a, b);
    CHECK_GREATER_FOR_P4C_EXPRESSION(b, a);
    CHECK_EQUALITY_FOR_P4C_EXPRESSION(a, a2);

    const auto *c = new IR::PathExpression(new IR::Path("c"));
    auto *d = new IR::PathExpression(new IR::Path("d"));
    const auto *c2 = new IR::PathExpression(new IR::Path("c"));

    CHECK_LESS_FOR_P4C_EXPRESSION(c, d);
    CHECK_GREATER_FOR_P4C_EXPRESSION(d, c);
    CHECK_EQUALITY_FOR_P4C_EXPRESSION(c, c2);
}

TEST(OperatorLess, Members) {
    const auto *a = new IR::Member(new IR::PathExpression("a"), "b");
    const auto *b = new IR::Member(new IR::PathExpression("a"), "c");
    const auto *a2 = new IR::Member(new IR::PathExpression("a"), "b");

    CHECK_LESS_FOR_P4C_EXPRESSION(a, b);
    CHECK_GREATER_FOR_P4C_EXPRESSION(b, a);
    CHECK_EQUALITY_FOR_P4C_EXPRESSION(a, a2);

    const auto *c = new IR::Member(new IR::PathExpression("b"), "a");
    CHECK_LESS_FOR_P4C_EXPRESSION(a, c);
    CHECK_GREATER_FOR_P4C_EXPRESSION(c, a);
}

TEST(OperatorLess, Declarations) {
    const auto *t = IR::Type_Bits::get(16, false);
    {
        const auto *a = new IR::Declaration_Variable("a", t);
        const auto *b = new IR::Declaration_Variable("b", t);
        const auto *a2 = new IR::Declaration_Variable("a", t);

        CHECK_LESS_FOR_P4C_DECLARATION(a, b);
        CHECK_GREATER_FOR_P4C_DECLARATION(b, a);
        CHECK_EQUALITY_FOR_P4C_DECLARATION(a, a2);
    }

    {
        const auto *a = new IR::Declaration_Constant("a", t, new IR::Constant(t, 0));
        const auto *b = new IR::Declaration_Constant("b", t, new IR::Constant(t, 0));
        const auto *a2 = new IR::Declaration_Constant("a", t, new IR::Constant(t, 0));
        CHECK_LESS_FOR_P4C_DECLARATION(a, b);
        CHECK_GREATER_FOR_P4C_DECLARATION(b, a);
        CHECK_EQUALITY_FOR_P4C_DECLARATION(a, a2);

        const auto *a3 = new IR::Declaration_Constant("a", t, new IR::Constant(t, 1));
        CHECK_LESS_FOR_P4C_DECLARATION(a, a3);
        CHECK_GREATER_FOR_P4C_DECLARATION(a3, a);
    }
}
