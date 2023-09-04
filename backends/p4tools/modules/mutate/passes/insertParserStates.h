#ifndef BACKENDS_P4TOOLS_MODULES_MUTATE_PASSES_INSERTPARSERSTATES_H_
#define BACKENDS_P4TOOLS_MODULES_MUTATE_PASSES_INSERTPARSERSTATES_H_

#include "ir/ir.h"

namespace P4Tools::P4Mutate {

class InsertParserStatesPass : public Transform {
 public:
    InsertParserStatesPass() {
        visitDagOnce = true;
        setName("InsertParserStates");
    }

    const IR::Node *postorder(IR::P4Parser *p) override;

    /// This function applys the InsertParserStates pass to @param program,
    /// and @returns the resulting program. The pass adds empty transient
    /// to parsers.
    /// If @param print_p4 is set, then it also pretty prints the p4 to stdout.
    static const IR::P4Program *insertParserStates(const IR::P4Program *program,
                                                   bool print_p4 = false);

 private:
    /// @returns a random state from the given @param:states vec.
    /// This tries to find a state that is not accept or reject.
    /// We assume that there is atleast one state in the given parser except
    // accept and reject.
    static const IR::ParserState *pickRandomState(IR::IndexedVector<IR::ParserState> &vec);

    /// Adds a new parserState after @param init_state
    /// to the states vector @param vec. Also
    /// modifies init_state to add this new state between init_state and
    /// init_state->selectExpression. Modifies @param vec in-place.
    /// @returns this newly created parserState.
    static const IR::ParserState *createAndInsertState(IR::IndexedVector<IR::ParserState> &vec,
                                                       const IR::ParserState *init_state);

    /// @returns True if a state with the give @param name, exists in the
    /// @param states vector.
    bool existsParserState(IR::IndexedVector<IR::ParserState> states, cstring name);

    /// @returns a unique random parser state name with respect to the given @param states vector.
    cstring getRandomName(const IR::IndexedVector<IR::ParserState> &states);
};

}  // namespace P4Tools::P4Mutate

#endif /* BACKENDS_P4TOOLS_MODULES_MUTATE_PASSES_INSERTPARSERSTATES_H_ */
