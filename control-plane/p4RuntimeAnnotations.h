/*
Copyright 2018-present Barefoot Networks, Inc.

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

#ifndef CONTROL_PLANE_P4RUNTIMEANNOTATIONS_H_
#define CONTROL_PLANE_P4RUNTIMEANNOTATIONS_H_

#include "frontends/p4/parseAnnotations.h"
#include "ir/ir.h"

namespace P4 {

namespace ControlPlaneAPI {

/// Parses P4Runtime-specific annotations.
class ParseP4RuntimeAnnotations : public ParseAnnotations {
 public:
    ParseP4RuntimeAnnotations()
        : ParseAnnotations(
              "P4Runtime", false,
              {
                  PARSE("controller_header"_cs, StringLiteral),
                  PARSE_EMPTY("hidden"_cs),
                  PARSE("id"_cs, Constant),
                  PARSE("brief"_cs, StringLiteral),
                  PARSE("description"_cs, StringLiteral),
                  PARSE_KV_LIST("platform_property"_cs),
                  // These annotations are architecture-specific in theory, but
                  // given that they are "reserved" by the P4Runtime
                  // specification, I don't really have any qualms about adding
                  // them here. I don't think it is possible to just run a
                  // different ParseAnnotations pass in the constructor of the
                  // architecture-specific P4RuntimeArchHandlerIface
                  // implementation, since ParseAnnotations modifies the
                  // program. I don't really like the possible alternatives
                  // either: 1) modify the P4RuntimeArchHandlerIface interface
                  // so that each implementation can provide a custom
                  // ParseAnnotations instance, or 2) run a ParseAnnotations
                  // pass "locally" (in this case on action profile instances
                  // since these annotations are for them).
                  PARSE("max_group_size"_cs, Constant),
                  PARSE("selector_size_semantics"_cs, StringLiteral),
                  PARSE("max_member_weight"_cs, Constant),
                  {"p4runtime_translation"_cs, &ParseAnnotations::parseP4rtTranslationAnnotation},
              }) {}
};

}  // namespace ControlPlaneAPI

}  // namespace P4

#endif  // CONTROL_PLANE_P4RUNTIMEANNOTATIONS_H_
