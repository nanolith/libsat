/**
 * \file libsat/libsat.h
 *
 * \brief Main header for libsat.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#pragma once

#include <libsat/scanner.h>
#include <rcpr/allocator.h>
#include <rcpr/rbtree.h>
#include <rcpr/resource.h>
#include <rcpr/resource/protected.h>
#include <stdbool.h>

/* C++ compatibility. */
# ifdef   __cplusplus
extern "C" {
# endif /*__cplusplus*/

/**
 * \brief Context for the libsat instance.
 */
typedef struct LIBSAT_SYM(libsat_context) LIBSAT_SYM(libsat_context);
struct LIBSAT_SYM(libsat_context)
{
    RCPR_SYM(resource) hdr;
    RCPR_SYM(allocator)* alloc;
    RCPR_SYM(rbtree)* string_to_intern;
    RCPR_SYM(rbtree)* intern_to_string;
    size_t variable_count;
};

/**
 * \brief Flags to pass to libsat_context_variable_get.
 */
enum LIBSAT_SYM(libsat_context_variable_get_flags)
{
    /** \brief By default, variables are created if they don't exist, and are
     * returned if they already do. */
    LIBSAT_VARIABLE_GET_DEFAULT =                                   0x0000,

    /** \brief This variable is being created; check that it doesn't already
     * exist. */
    LIBSAT_VARIABLE_GET_CREATE =                                    0x0001,

    /** \brief This variable is being referenced; it MUST already exist. */
    LIBSAT_VARIABLE_GET_REF =                                       0x0002,

    /** \brief The created variable is unnamed. Must include CREATE. */
    LIBSAT_VARIABLE_GET_UNIQUE =                                    0x0004,
};

/******************************************************************************/
/* Start of model checking properties.                                        */
/******************************************************************************/

/**
 * \brief Valid context property.
 *
 * \param context       The context instance to be verified.
 *
 * \returns true if the context instance is valid.
 */
bool
LIBSAT_SYM(prop_context_valid)(
    const LIBSAT_SYM(libsat_context)* context);

/******************************************************************************/
/* Start of constructors.                                                     */
/******************************************************************************/

/**
 * \brief Create a context instance.
 *
 * \param context       Pointer to the context pointer to be set to this created
 *                      context instance on success.
 * \param alloc         The allocator to use for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
LIBSAT_SYM(libsat_context_create)(
    LIBSAT_SYM(libsat_context)** context, RCPR_SYM(allocator)* alloc);

/******************************************************************************/
/* Start of public methods.                                                   */
/******************************************************************************/

/**
 * \brief Get or create a variable by name.
 *
 * \param var_id        Pointer to the variable id to be set on success.
 * \param context       The context for this operation.
 * \param var_name      The name of the variable.
 * \param flags         The flags to use for this get operation.
 *                      \see libsat_context_variable_get_flags.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
LIBSAT_SYM(libsat_context_variable_get)(
    size_t* var_id, LIBSAT_SYM(libsat_context)* context, const char* var_name,
    int flags);

/**
 * \brief Given a \ref libsat_context instance, return the resource handle for
 * this instance.
 *
 * \param context       The \ref libsat_context instance from which the resource
 *                      handle is returned.
 *
 * \returns the resource handle for this context instance.
 */
RCPR_SYM(resource)*
LIBSAT_SYM(libsat_context_resource_handle)(
    LIBSAT_SYM(libsat_context)* context);

/******************************************************************************/
/* Start of public exports.                                                   */
/******************************************************************************/
#define __INTERNAL_LIBSAT_IMPORT_base_sym(sym) \
    LIBSAT_BEGIN_EXPORT \
    typedef LIBSAT_SYM(libsat_context) sym ## libsat_context; \
    static inline status FN_DECL_MUST_CHECK sym ## libsat_context_create( \
        LIBSAT_SYM(libsat_context)** x, RCPR_SYM(allocator)* y) { \
            return LIBSAT_SYM(libsat_context_create)(x,y); } \
    static inline status FN_DECL_MUST_CHECK \
    sym ## libsat_context_variable_get( \
        size_t* w, LIBSAT_SYM(libsat_context)* x, const char* y, bool z) { \
            return LIBSAT_SYM(libsat_context_variable_get)(w,x,y,z); } \
    LIBSAT_END_EXPORT \
    REQUIRE_SEMICOLON_HERE
#define LIBSAT_IMPORT_base_as(sym) \
    __INTERNAL_LIBSAT_IMPORT_base_sym(sym ## _)
#define LIBSAT_IMPORT_base \
    __INTERNAL_LIBSAT_IMPORT_base_sym()

/* C++ compatibility. */
# ifdef   __cplusplus
}
# endif /*__cplusplus*/
