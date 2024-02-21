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

#include "lib/hvec_map.h"

#include <gtest/gtest.h>

namespace Test {

TEST(hvec_map, map_equal) {
    hvec_map<unsigned, unsigned> a;
    hvec_map<unsigned, unsigned> b;

    EXPECT_TRUE(a == b);

    a[1] = 111;
    a[2] = 222;
    a[3] = 333;
    a[4] = 444;

    b[1] = 111;
    b[2] = 222;
    b[3] = 333;
    b[4] = 444;

    EXPECT_TRUE(a == b);

    a.erase(2);
    b.erase(2);

    EXPECT_TRUE(a == b);

    a.clear();
    b.clear();

    EXPECT_TRUE(a == b);
}

TEST(hvec_map, map_not_equal) {
    hvec_map<unsigned, unsigned> a;
    hvec_map<unsigned, unsigned> b;

    EXPECT_TRUE(a == b);

    a[1] = 111;
    a[2] = 222;
    a[3] = 333;
    a[4] = 444;

    b[4] = 444;
    b[3] = 333;
    b[2] = 222;
    b[1] = 111;

    EXPECT_TRUE(a != b);

    a.clear();
    b.clear();

    EXPECT_TRUE(a == b);

    a[1] = 111;
    a[2] = 222;

    b[1] = 111;
    b[2] = 222;
    b[3] = 333;

    EXPECT_TRUE(a != b);

    a.clear();
    b.clear();

    EXPECT_TRUE(a == b);

    a[1] = 111;
    a[2] = 222;
    a[3] = 333;
    a[4] = 444;

    b[4] = 111;
    b[3] = 222;
    b[2] = 333;
    b[1] = 444;

    EXPECT_TRUE(a != b);

    a.clear();
    b.clear();

    EXPECT_TRUE(a == b);

    a[1] = 111;
    a[2] = 222;
    a[3] = 333;
    a[4] = 444;

    b[1] = 111;
    b[2] = 111;
    b[3] = 111;
    b[4] = 111;

    EXPECT_TRUE(a != b);
}

TEST(hvec_map, insert_emplace_erase) {
    hvec_map<unsigned, unsigned> om;
    std::map<unsigned, unsigned> sm;

    typename hvec_map<unsigned, unsigned>::const_iterator it = om.end();
    for (auto v : {0, 1, 2, 3, 4, 5, 6, 7, 8}) {
        sm.emplace(v, 2 * v);
        std::pair<unsigned, unsigned> pair{v, 2 * v};
        if (v % 2 == 0) {
            if ((v / 2) % 2 == 0) {
                it = om.insert(pair).first;
            } else {
                it = om.emplace(v, pair.second).first;
            }
        } else {
            if ((v / 2) % 2 == 0) {
                it = om.insert(std::move(pair)).first;
            } else {
                it = om.emplace(std::move(v), v * 2).first;
            }
        }
    }

    EXPECT_TRUE(std::equal(om.begin(), om.end(), sm.begin(), sm.end()));

    it = std::next(om.begin(), 2);
    om.erase(it);
    sm.erase(std::next(sm.begin(), 2));

    EXPECT_TRUE(om.size() == sm.size());
    EXPECT_TRUE(std::equal(om.begin(), om.end(), sm.begin(), sm.end()));
}

TEST(hvec_map, string_map) {
    hvec_map<std::string, std::string> m, m1;

    for (int i = 1; i <= 100; ++i) {
        m[std::to_string(i)] = "test";
        m[std::to_string(i)] += std::to_string(i);
    }
    m1 = m;

    hvec_map<std::string, std::string> m2(m);

    EXPECT_EQ(m1.size(), 100);
    for (int i = 1; i <= 100; i += 2) m1.erase(std::to_string(i));
    EXPECT_EQ(m1.size(), 50);
    for (int i = 102; i <= 200; i += 2) m1[std::to_string(i)] = "foobar";
    EXPECT_EQ(m1.size(), 100);

    int idx = 2;
    for (auto &el : m1) {
        EXPECT_TRUE(el.first == std::to_string(idx));
        if (idx <= 100)
            EXPECT_TRUE(el.second.c_str() + 4 == std::to_string(idx));
        else
            EXPECT_EQ(el.second, "foobar");
        idx += 2;
    }
}

}  // namespace Test
