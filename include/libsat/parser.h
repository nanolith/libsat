/**
 * \file libsat/parser.h
 *
 * \brief Parser for libsat.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#pragma once

#include <libsat/function_decl.h>

/* C++ compatibility. */
# ifdef   __cplusplus
extern "C" {
# endif /*__cplusplus*/

/**
 * \brief AST node types.
 */
enum LIBSAT_SYM(libsat_parser_ast_node_type)
{
    /** \brief A variable node. */
    LIBSAT_PARSER_AST_NODE_TYPE_VARIABLE,

    /** \brief A negation node. */
    LIBSAT_PARSER_AST_NODE_TYPE_NEGATION,

    /** \brief A conjunction node. */
    LIBSAT_PARSER_AST_NODE_TYPE_CONJUNCTION,

    /** \brief A disjunction node. */
    LIBSAT_PARSER_AST_NODE_TYPE_DISJUNCTION,

    /** \brief An exclusive disjunction node. */
    LIBSAT_PARSER_AST_NODE_TYPE_EXCLUSIVE_DISJUNCTION,

    /** \brief An implication node. */
    LIBSAT_PARSER_AST_NODE_TYPE_IMPLICATION,

    /** \brief A biconditional node. */
    LIBSAT_PARSER_AST_NODE_TYPE_BICONDITIONAL,
};

/**
 * \brief A parser capable of generating an AST based on input strings for
 * creating propositional logic statements.
 */
typedef struct LIBSAT_SYM(libsat_parser) LIBSAT_SYM(libsat_parser);

/* C++ compatibility. */
# ifdef   __cplusplus
}
# endif /*__cplusplus*/
