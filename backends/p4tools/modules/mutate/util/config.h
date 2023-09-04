#ifndef BACKENDS_P4TOOLS_MODULES_MUTATE_UTIL_CONFIG_H_
#define BACKENDS_P4TOOLS_MODULES_MUTATE_UTIL_CONFIG_H_

#include <limits>

class Config {
 public:
    constexpr static const int DEAD_IF_BLOCK_PCT = 80;
    constexpr static const int IF_TRUE_GUARD_PCT = 40;
    constexpr static const int ZERO_LENGTH_HEADERS_PCT = 80;
    constexpr static const int MAX_ZERO_LENGTH_HEADERS = 10;
    constexpr static const int REORDER_STMTS_PCT = 70;
    constexpr static const int REORDER_PARSER_STATES_PCT = 70;
    constexpr static const int INSERT_LINKED_PARSER_STATES_PCT = 50;
    constexpr static const int INSERT_ORPHAN_PARSER_STATES_PCT = 100;
    constexpr static const int MAX_PARSER_STATES_LENGTH = 5;
    constexpr static const int MAX_ORPHAN_PARSER_STATES_LENGTH = 10;
    constexpr static const int SWITCH_TABLE_DECLS_PCT = 90;
    constexpr static const int BOOL_DEADCODE_EXPRESSION_MAX_DEPTH = 30;
    constexpr static const int MAX_LIVECODE_EXPRESSION_DEPTH = 3;
    constexpr static const int FLIP_LITERAL_PCT = 40;
    constexpr static const int GEN_CONST_PCT = 20;
    constexpr static const int MAX_CONST_INT_VAL = std::numeric_limits<int>::max();
    constexpr static const int SHUFFLE_KEYS = 60;
};

#endif /* BACKENDS_P4TOOLS_MODULES_MUTATE_UTIL_CONFIG_H_ */
