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
#include <libsat/libsat_fwd.h>
#include <rcpr/allocator.h>
#include <rcpr/resource.h>
#include <rcpr/resource/protected.h>

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
    LIBSAT_PARSER_AST_NODE_TYPE_VARIABLE = 1,

    /** \brief A literal node. */
    LIBSAT_PARSER_AST_NODE_TYPE_BOOLEAN_LITERAL,

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

    /** \brief An assignment node. */
    LIBSAT_PARSER_AST_NODE_TYPE_ASSIGNMENT,

    /** \brief A statement node. */
    LIBSAT_PARSER_AST_NODE_TYPE_STATEMENT,

    /** \brief A statement list. */
    LIBSAT_PARSER_AST_NODE_TYPE_STATEMENT_LIST,
};

/**
 * \brief An AST node from the parser.
 */
typedef struct LIBSAT_SYM(libsat_ast_node) LIBSAT_SYM(libsat_ast_node);
struct LIBSAT_SYM(libsat_ast_node)
{
    RCPR_SYM(resource) hdr;
    RCPR_SYM(allocator)* alloc;
    LIBSAT_SYM(libsat_ast_node)* next;
    int type;
    union {
        /** \brief variable index. */
        size_t variable_index;

        /** \brief boolean literal. */
        bool boolean_literal;

        /** \brief unary node. */
        LIBSAT_SYM(libsat_ast_node)* unary;

        /** \brief binary node. */
        struct
        {
            LIBSAT_SYM(libsat_ast_node)* lhs;
            LIBSAT_SYM(libsat_ast_node)* rhs;
        } binary;

        /** \brief list node. */
        struct
        {
            LIBSAT_SYM(libsat_ast_node)* head;
        } list;
    } value;
};

/******************************************************************************/
/* Start of public methods.                                                   */
/******************************************************************************/

/**
 * \brief Parse an input string.
 *
 * \param node          The AST node created on success.
 * \param context       The context for this operation.
 * \param input         The input string to parse.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
LIBSAT_SYM(libsat_parse)(
    LIBSAT_SYM(libsat_ast_node)** node, LIBSAT_SYM(libsat_context)* context,
    const char* input);

/**
 * \brief Get the resource associated with a \ref libsat_ast_node.
 *
 * \param node          The node for this operation.
 *
 * \returns the resource handle for this \ref libsat_ast_node.
 */
RCPR_SYM(resource)*
LIBSAT_SYM(libsat_ast_node_resource_handle)(
    LIBSAT_SYM(libsat_ast_node)* node);

/******************************************************************************/
/* Start of public exports.                                                   */
/******************************************************************************/
#define __INTERNAL_LIBSAT_IMPORT_parser_sym(sym) \
    LIBSAT_BEGIN_EXPORT \
    typedef LIBSAT_SYM(libsat_ast_node) sym ## libsat_ast_node; \
    static inline status FN_DECL_MUST_CHECK \
    sym ## libsat_parse( \
        LIBSAT_SYM(libsat_ast_node)** x, LIBSAT_SYM(libsat_context)* y, \
        const char* z) { \
            return LIBSAT_SYM(libsat_parse)(x,y,z); } \
    static inline RCPR_SYM(resource)* \
    sym ## libsat_ast_node_resource_handle( \
        LIBSAT_SYM(libsat_ast_node)* x) { \
            return LIBSAT_SYM(libsat_ast_node_resource_handle)(x); } \
    LIBSAT_END_EXPORT \
    REQUIRE_SEMICOLON_HERE
#define LIBSAT_IMPORT_parser_as(sym) \
    __INTERNAL_LIBSAT_IMPORT_parser_sym(sym ## _)
#define LIBSAT_IMPORT_parser \
    __INTERNAL_LIBSAT_IMPORT_parser_sym()

/* C++ compatibility. */
# ifdef   __cplusplus
}
# endif /*__cplusplus*/
