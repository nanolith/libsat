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

/* C++ compatibility. */
# ifdef   __cplusplus
extern "C" {
# endif /*__cplusplus*/

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
/* Start of private exports.                                                  */
/******************************************************************************/
#define __INTERNAL_LIBSAT_IMPORT_parser_internal_sym(sym) \
    LIBSAT_BEGIN_EXPORT \
    static inline status FN_DECL_MUST_CHECK \
    sym ## libsat_ast_node_create_from_variable( \
        LIBSAT_SYM(libsat_ast_node)** w, LIBSAT_SYM(libsat_context)* x \
        const char* y, int z) { \
            return \
                LIBSAT_SYM(libsat_ast_node_create_from_variable)(w,x,y,z); } \
    static inline status FN_DECL_MUST_CHECK \
    sym ## libsat_ast_node_resource_release( \
        RCPR_SYM(resource)* x) { \
            return LIBSAT_SYM(libsat_ast_node_resource_release)(x); } \
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
