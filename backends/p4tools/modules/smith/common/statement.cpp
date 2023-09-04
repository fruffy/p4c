#include "backends/p4tools/modules/smith/common/statement.h"

#include <vector>

#include "backends/p4tools/modules/smith/common/assignmentOrMethodCallStatement.h"
#include "backends/p4tools/modules/smith/common/blockStatement.h"
#include "backends/p4tools/modules/smith/common/conditionalStatement.h"
#include "backends/p4tools/modules/smith/common/exitStatement.h"
#include "backends/p4tools/modules/smith/common/returnStatement.h"
#include "backends/p4tools/modules/smith/common/switchStatement.h"

namespace P4Tools {

namespace P4Smith {

IR::Statement *statement::gen_rnd(bool is_in_func = false) {
    // functions can!have exit statements so set their probability to zero
    int64_t pct_exit = PCT.STATEMENT_EXIT;
    if (is_in_func) {
        pct_exit = 0;
    }
    std::vector<int64_t> percent = {PCT.STATEMENT_SWITCH,
                                    PCT.STATEMENT_ASSIGNMENTORMETHODCALL,
                                    PCT.STATEMENT_IF,
                                    PCT.STATEMENT_RETURN,
                                    pct_exit,
                                    PCT.STATEMENT_BLOCK};
    IR::Statement *stmt = nullptr;
    bool use_default_stmt = false;

    switch (randInd(percent)) {
        case 0: {
            stmt = switchStatement::gen();
            if (!stmt) {
                use_default_stmt = true;
            }
            break;
        }
        case 1: {
            use_default_stmt = true;
            break;
        }
        case 2: {
            stmt = conditionalStatement::gen_if_stat(is_in_func);
            break;
        }
        case 3: {
            stmt = returnStatement::gen_ret_stat(P4Scope::prop.ret_type);
            break;
        }
        case 4: {
            stmt = exitStatement::gen();
            break;
        }
        case 5: {
            stmt = blockStatement::gen(is_in_func);
            break;
        }
    }
    if (use_default_stmt) {
        stmt = assignmentOrMethodCallStatement::gen(is_in_func);
    }
    return stmt;
}
}  // namespace P4Smith

}  // namespace P4Tools
