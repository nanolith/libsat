/**
 * \file parser/parser_internal.h
 *
 * \brief Internal details for the parser.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#pragma once

#include <libsat/parser.h>
#include <stdbool.h>

/* C++ compatibility. */
# ifdef   __cplusplus
extern "C" {
# endif /*__cplusplus*/

enum LIBSAT_SYM(libsat_operator_priority)
{
    LIBSAT_OPERATOR_PRIORITY_PARENTHESES                                = 0,
    LIBSAT_OPERATOR_PRIORITY_NEGATION                                   = 1,
    LIBSAT_OPERATOR_PRIORITY_CONJUNCTION                                = 2,
    LIBSAT_OPERATOR_PRIORITY_EXCLUSIVE_DISJUNCITON                      = 3,
    LIBSAT_OPERATOR_PRIORITY_DISJUNCITON                                = 4,
    LIBSAT_OPERATOR_PRIORITY_IMPLICATION                                = 5,
    LIBSAT_OPERATOR_PRIORITY_BICONDITIONAL                              = 6,
};

/******************************************************************************/
/* Start of private constructors.                                             */
/******************************************************************************/

/**
 * \brief Create an AST node from a variable name.
 *
 * \param node          Pointer to receive the node on success.
 * \param context       The \ref libsat_context for this operation.
 * \param var_name      Name of the variable.
 * \param mode          Variable lookup mode.
 *                      @see \ref libsat_context_variable_get_flags.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
LIBSAT_SYM(libsat_ast_node_create_from_variable)(
    LIBSAT_SYM(libsat_ast_node)** node, LIBSAT_SYM(libsat_context)* context,
    const char* var_name, int mode);

/**
 * \brief Create an AST node from a boolean literal.
 *
 * \param node          Pointer to receive the node on success.
 * \param context       The \ref libsat_context for this operation.
 * \param value         The boolean value.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
LIBSAT_SYM(libsat_ast_node_create_from_boolean_literal)(
    LIBSAT_SYM(libsat_ast_node)** node, LIBSAT_SYM(libsat_context)* context,
    bool value);

/**
 * \brief Create an AST node from as a negation of the given node.
 *
 * \param node          Pointer to receive the node on success.
 * \param context       The \ref libsat_context for this operation.
 * \param child         The child node that is negated. Note that this node
 *                      takes ownership of this child node on success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
LIBSAT_SYM(libsat_ast_node_create_as_negation)(
    LIBSAT_SYM(libsat_ast_node)** node, LIBSAT_SYM(libsat_context)* context,
    LIBSAT_SYM(libsat_ast_node)* child);

/**
 * \brief Create an AST node from as a conjunction of two nodes.
 *
 * \param node          Pointer to receive the node on success.
 * \param context       The \ref libsat_context for this operation.
 * \param lhs           The left-hand side of the conjunction. Note that this
 *                      node takes ownership of this left-hand-side node on
 *                      success.
 * \param rhs           The right-hand side of the conjunction. Note that this
 *                      node takes ownership of this right-hand-side node on
 *                      success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
LIBSAT_SYM(libsat_ast_node_create_as_conjunction)(
    LIBSAT_SYM(libsat_ast_node)** node, LIBSAT_SYM(libsat_context)* context,
    LIBSAT_SYM(libsat_ast_node)* lhs, LIBSAT_SYM(libsat_ast_node)* rhs);

/**
 * \brief Create an AST node from as a disjunction of two nodes.
 *
 * \param node          Pointer to receive the node on success.
 * \param context       The \ref libsat_context for this operation.
 * \param lhs           The left-hand side of the disjunction. Note that this
 *                      node takes ownership of this left-hand-side node on
 *                      success.
 * \param rhs           The right-hand side of the disjunction. Note that this
 *                      node takes ownership of this right-hand-side node on
 *                      success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
LIBSAT_SYM(libsat_ast_node_create_as_disjunction)(
    LIBSAT_SYM(libsat_ast_node)** node, LIBSAT_SYM(libsat_context)* context,
    LIBSAT_SYM(libsat_ast_node)* lhs, LIBSAT_SYM(libsat_ast_node)* rhs);

/**
 * \brief Create an AST node from as an exclusive disjunction of two nodes.
 *
 * \param node          Pointer to receive the node on success.
 * \param context       The \ref libsat_context for this operation.
 * \param lhs           The left-hand side of the disjunction. Note that this
 *                      node takes ownership of this left-hand-side node on
 *                      success.
 * \param rhs           The right-hand side of the disjunction. Note that this
 *                      node takes ownership of this right-hand-side node on
 *                      success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
LIBSAT_SYM(libsat_ast_node_create_as_exclusive_disjunction)(
    LIBSAT_SYM(libsat_ast_node)** node, LIBSAT_SYM(libsat_context)* context,
    LIBSAT_SYM(libsat_ast_node)* lhs, LIBSAT_SYM(libsat_ast_node)* rhs);

/**
 * \brief Create an AST node from as an implication of two nodes.
 *
 * \param node          Pointer to receive the node on success.
 * \param context       The \ref libsat_context for this operation.
 * \param lhs           The left-hand side of the implication. Note that this
 *                      node takes ownership of this left-hand-side node on
 *                      success.
 * \param rhs           The right-hand side of the implication. Note that this
 *                      node takes ownership of this right-hand-side node on
 *                      success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
LIBSAT_SYM(libsat_ast_node_create_as_implication)(
    LIBSAT_SYM(libsat_ast_node)** node, LIBSAT_SYM(libsat_context)* context,
    LIBSAT_SYM(libsat_ast_node)* lhs, LIBSAT_SYM(libsat_ast_node)* rhs);

/**
 * \brief Create an AST node from as a biconditional of two nodes.
 *
 * \param node          Pointer to receive the node on success.
 * \param context       The \ref libsat_context for this operation.
 * \param lhs           The left-hand side of the biconditional. Note that this
 *                      node takes ownership of this left-hand-side node on
 *                      success.
 * \param rhs           The right-hand side of the biconditional. Note that this
 *                      node takes ownership of this right-hand-side node on
 *                      success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
LIBSAT_SYM(libsat_ast_node_create_as_biconditional)(
    LIBSAT_SYM(libsat_ast_node)** node, LIBSAT_SYM(libsat_context)* context,
    LIBSAT_SYM(libsat_ast_node)* lhs, LIBSAT_SYM(libsat_ast_node)* rhs);

/**
 * \brief Create an AST node from as an assignment of the left-hand-side
 * variable to the right-hand-side expression.
 *
 * \param node          Pointer to receive the node on success.
 * \param context       The \ref libsat_context for this operation.
 * \param lhs           The left-hand side of the assignment. This must be a
 *                      variable. Note that this node takes ownership of this
 *                      left-hand-side node on success.
 * \param rhs           The right-hand side of the assignment. Note that this
 *                      node takes ownership of this right-hand-side node on
 *                      success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
LIBSAT_SYM(libsat_ast_node_create_as_assignment)(
    LIBSAT_SYM(libsat_ast_node)** node, LIBSAT_SYM(libsat_context)* context,
    LIBSAT_SYM(libsat_ast_node)* lhs, LIBSAT_SYM(libsat_ast_node)* rhs);

/**
 * \brief Create an AST node from as a statement wrapping  the given node.
 *
 * \param node          Pointer to receive the node on success.
 * \param context       The \ref libsat_context for this operation.
 * \param child         The child node that is wrapped. Note that this node
 *                      takes ownership of this child node on success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
LIBSAT_SYM(libsat_ast_node_create_as_statement)(
    LIBSAT_SYM(libsat_ast_node)** node, LIBSAT_SYM(libsat_context)* context,
    LIBSAT_SYM(libsat_ast_node)* child);

/**
 * \brief Create an AST node as an empty list.
 *
 * \param node          Pointer to receive the node on success.
 * \param context       The \ref libsat_context for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
LIBSAT_SYM(libsat_ast_node_create_as_list)(
    LIBSAT_SYM(libsat_ast_node)** node, LIBSAT_SYM(libsat_context)* context);

/**
 * \brief Release a \ref libsat_ast_node resource.
 *
 * \param r             The resource to release.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
LIBSAT_SYM(libsat_ast_node_resource_release)(
    RCPR_SYM(resource)* r);

/******************************************************************************/
/* Start of private methods.                                                  */
/******************************************************************************/

/**
 * \brief Push an AST node onto the list.
 *
 * \param list_node     The list node for this operation.
 * \param child         The child node to push onto the list. Note that the list
 *                      node takes ownership of this child node on success. This
 *                      node must be a statement.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
LIBSAT_SYM(libsat_ast_list_node_push)(
    LIBSAT_SYM(libsat_ast_node)* list_node, LIBSAT_SYM(libsat_ast_node)* child);

/******************************************************************************/
/* Start of private exports.                                                  */
/******************************************************************************/
#define __INTERNAL_LIBSAT_IMPORT_parser_internal_sym(sym) \
    LIBSAT_BEGIN_EXPORT \
    static inline status FN_DECL_MUST_CHECK \
    sym ## libsat_ast_node_create_from_variable( \
        LIBSAT_SYM(libsat_ast_node)** w, LIBSAT_SYM(libsat_context)* x, \
        const char* y, int z) { \
            return \
                LIBSAT_SYM(libsat_ast_node_create_from_variable)(w,x,y,z); } \
    static inline status FN_DECL_MUST_CHECK \
    sym ## libsat_ast_node_create_from_boolean_literal( \
        LIBSAT_SYM(libsat_ast_node)** x, LIBSAT_SYM(libsat_context)* y, \
        bool z) { \
            return \
                LIBSAT_SYM(libsat_ast_node_create_from_boolean_literal)( \
                    x,y,z); } \
    static inline status FN_DECL_MUST_CHECK \
    sym ## libsat_ast_node_create_as_negation( \
        LIBSAT_SYM(libsat_ast_node)** x, LIBSAT_SYM(libsat_context)* y, \
        LIBSAT_SYM(libsat_ast_node)* z) { \
            return LIBSAT_SYM(libsat_ast_node_create_as_negation)(x,y,z); } \
    static inline status FN_DECL_MUST_CHECK \
    sym ## libsat_ast_node_create_as_conjunction( \
        LIBSAT_SYM(libsat_ast_node)** w, LIBSAT_SYM(libsat_context)* x, \
        LIBSAT_SYM(libsat_ast_node)* y, LIBSAT_SYM(libsat_ast_node)* z) { \
            return \
                LIBSAT_SYM(libsat_ast_node_create_as_conjunction)(w,x,y,z); } \
    static inline status FN_DECL_MUST_CHECK \
    sym ## libsat_ast_node_create_as_disjunction( \
        LIBSAT_SYM(libsat_ast_node)** w, LIBSAT_SYM(libsat_context)* x, \
        LIBSAT_SYM(libsat_ast_node)* y, LIBSAT_SYM(libsat_ast_node)* z) { \
            return \
                LIBSAT_SYM(libsat_ast_node_create_as_disjunction)(w,x,y,z); } \
    static inline status FN_DECL_MUST_CHECK \
    sym ## libsat_ast_node_create_as_exclusive_disjunction( \
        LIBSAT_SYM(libsat_ast_node)** w, LIBSAT_SYM(libsat_context)* x, \
        LIBSAT_SYM(libsat_ast_node)* y, LIBSAT_SYM(libsat_ast_node)* z) { \
            return \
                LIBSAT_SYM(libsat_ast_node_create_as_exclusive_disjunction)( \
                    w,x,y,z); } \
    static inline status FN_DECL_MUST_CHECK \
    sym ## libsat_ast_node_create_as_implication( \
        LIBSAT_SYM(libsat_ast_node)** w, LIBSAT_SYM(libsat_context)* x, \
        LIBSAT_SYM(libsat_ast_node)* y, LIBSAT_SYM(libsat_ast_node)* z) { \
            return \
                LIBSAT_SYM(libsat_ast_node_create_as_implication)(w,x,y,z); } \
    static inline status FN_DECL_MUST_CHECK \
    sym ## libsat_ast_node_create_as_biconditional( \
        LIBSAT_SYM(libsat_ast_node)** w, LIBSAT_SYM(libsat_context)* x, \
        LIBSAT_SYM(libsat_ast_node)* y, LIBSAT_SYM(libsat_ast_node)* z) { \
            return \
                LIBSAT_SYM(libsat_ast_node_create_as_biconditional)( \
                    w,x,y,z); } \
    static inline status FN_DECL_MUST_CHECK \
    sym ## libsat_ast_node_create_as_assignment( \
        LIBSAT_SYM(libsat_ast_node)** w, LIBSAT_SYM(libsat_context)* x, \
        LIBSAT_SYM(libsat_ast_node)* y, LIBSAT_SYM(libsat_ast_node)* z) { \
            return \
                LIBSAT_SYM(libsat_ast_node_create_as_assignment)( \
                    w,x,y,z); } \
    static inline status FN_DECL_MUST_CHECK \
    sym ## libsat_ast_node_create_as_statement( \
        LIBSAT_SYM(libsat_ast_node)** x, LIBSAT_SYM(libsat_context)* y, \
        LIBSAT_SYM(libsat_ast_node)* z) { \
            return LIBSAT_SYM(libsat_ast_node_create_as_statement)(x,y,z); } \
    static inline status FN_DECL_MUST_CHECK \
    sym ## libsat_ast_node_create_as_list( \
        LIBSAT_SYM(libsat_ast_node)** x, LIBSAT_SYM(libsat_context)* y) { \
            return LIBSAT_SYM(libsat_ast_node_create_as_list)(x,y); } \
    static inline status FN_DECL_MUST_CHECK \
    sym ## libsat_ast_node_resource_release( \
        RCPR_SYM(resource)* x) { \
            return LIBSAT_SYM(libsat_ast_node_resource_release)(x); } \
    static inline status FN_DECL_MUST_CHECK \
    sym ## libsat_ast_list_node_push( \
        LIBSAT_SYM(libsat_ast_node)* x, LIBSAT_SYM(libsat_ast_node)* y) { \
            return LIBSAT_SYM(libsat_ast_list_node_push)(x,y); } \
    LIBSAT_END_EXPORT \
    REQUIRE_SEMICOLON_HERE
#define LIBSAT_IMPORT_parser_internal_as(sym) \
    __INTERNAL_LIBSAT_IMPORT_parser_internal_sym(sym ## _)
#define LIBSAT_IMPORT_parser_internal \
    __INTERNAL_LIBSAT_IMPORT_parser_internal_sym()

/* C++ compatibility. */
# ifdef   __cplusplus
}
# endif /*__cplusplus*/
