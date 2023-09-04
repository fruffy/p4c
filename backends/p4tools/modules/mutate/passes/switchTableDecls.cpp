#include "backends/p4tools/modules/mutate/passes/switchTableDecls.h"

#include <algorithm>
#include <vector>

#include "backends/p4tools/modules/mutate/util/config.h"
#include "backends/p4tools/modules/mutate/util/util.h"
#include "ir/ir.h"

namespace P4Tools::P4Mutate {

const IR::Node *SwitchTableDeclsPass::postorder(IR::P4Control *p) {
    auto rndInt = MutateUtils::getRandPct();

    if (rndInt < Config::SWITCH_TABLE_DECLS_PCT) {
        auto *tables = new std::vector<const IR::P4Table *>();

        for (auto it = p->controlLocals.end(); it != p->controlLocals.begin(); --it) {
            const auto *table = (*it)->to<IR::P4Table>();
            if (table == nullptr) {
                continue;
            }
            tables->push_back(table);

            p->controlLocals.erase(it);
        }

        MutateUtils::shuffle(tables);
        p->controlLocals.append(*tables);
    }

    return p;
}

const IR::Node *SwitchTableDeclsPass::postorder(IR::Key *k) {
    if (MutateUtils::getRandPct() < Config::SHUFFLE_KEYS) {
        MutateUtils::shuffle(&k->keyElements);
    }
    return k;
}

const IR::P4Program *SwitchTableDeclsPass::switchTableDecls(const IR::P4Program *prog,
                                                            bool print_p4) {
    auto *switchTables = new SwitchTableDeclsPass();
    prog = prog->apply(*switchTables);
    if (print_p4) {
        std::cout << "After Switch table decls pass:\n";
        MutateUtils::printP4(prog);
    }
    cstring outFileName = switchTables->internalName + ".p4";
    MutateUtils::emitP4(prog, outFileName, true);

    return prog;
}

}  // namespace P4Tools::P4Mutate
