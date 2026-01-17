/**
 * \file base/libsat_base_internal.h
 *
 * \brief Internals for the base libsat library.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#pragma once

#include <libsat/function_decl.h>
#include <rcpr/allocator.h>
#include <rcpr/compare.h>
#include <rcpr/resource.h>
#include <rcpr/resource/protected.h>

/* C++ compatibility. */
# ifdef   __cplusplus
extern "C" {
# endif /*__cplusplus*/

/**
 * \brief Entry in the intern table.
 */
typedef struct LIBSAT_SYM(intern_entry) LIBSAT_SYM(intern_entry);
struct LIBSAT_SYM(intern_entry)
{
    RCPR_SYM(resource) hdr;
    RCPR_SYM(allocator)* alloc;
    int ref_count;
    char* string;
    size_t string_index;
};

/******************************************************************************/
/* Start of constructors.                                                     */
/******************************************************************************/

/**
 * \brief Create an \ref intern_entry.
 *
 * \param entry         Pointer to the \ref intern_entry pointer to set to this
 *                      instance on success.
 * \param alloc         The allocator to use for this operation.
 * \param str           The string value for this entry.
 * \param index         The interned string index for this entry.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
LIBSAT_SYM(intern_entry_create)(
    LIBSAT_SYM(intern_entry)** entry, RCPR_SYM(allocator)* alloc,
    const char* str, size_t index);

/**
 * \brief Release an \ref intern_entry resource.
 *
 * \param r             The resource to release.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
LIBSAT_SYM(intern_entry_resource_release)(
    RCPR_SYM(resource)* r);

/******************************************************************************/
/* Start of private methods.                                                  */
/******************************************************************************/

/**
 * \brief Compare two opaque \ref intern_entry values for sorting in an
 * intern-to-string tree mapping.
 *
 * \param context       Unused.
 * \param lhs           The left-hand side of the comparison.
 * \param rhs           The right-hand side of the comparison.
 *
 * \returns an integer value representing the comparison result.
 *      - RCPR_COMPARE_LT if \p lhs &lt; \p rhs.
 *      - RCPR_COMPARE_EQ if \p lhs == \p rhs.
 *      - RCPR_COMPARE_GT if \p lhs &gt; \p rhs.
 */
RCPR_SYM(rcpr_comparison_result)
LIBSAT_SYM(intern_to_string_tree_compare)(
    void* context, const void* lhs, const void* rhs);

/**
 * \brief Given an \ref intern_entry resource handle, return its string_index
 * key value.
 *
 * \param context       Unused.
 * \param r             The resource handle of an \ref intern_entry.
 *
 * \returns the string_index key for this resource.
 */
const void*
LIBSAT_SYM(intern_to_string_tree_key)(
    void* context, const RCPR_SYM(resource)* r);

/**
 * \brief Compare two opaque \ref intern_entry values for sorting in an
 * string-to-intern tree mapping.
 *
 * \param context       Unused.
 * \param lhs           The left-hand side of the comparison.
 * \param rhs           The right-hand side of the comparison.
 *
 * \returns an integer value representing the comparison result.
 *      - RCPR_COMPARE_LT if \p lhs &lt; \p rhs.
 *      - RCPR_COMPARE_EQ if \p lhs == \p rhs.
 *      - RCPR_COMPARE_GT if \p lhs &gt; \p rhs.
 */
RCPR_SYM(rcpr_comparison_result)
LIBSAT_SYM(string_to_intern_tree_compare)(
    void* context, const void* lhs, const void* rhs);

/**
 * \brief Given an \ref intern_entry resource handle, return its string
 * key value.
 *
 * \param context       Unused.
 * \param r             The resource handle of an \ref intern_entry.
 *
 * \returns the string key for this resource.
 */
const void*
LIBSAT_SYM(string_to_intern_tree_key)(
    void* context, const RCPR_SYM(resource)* r);

/******************************************************************************/
/* Start of private exports.                                                  */
/******************************************************************************/
#define __INTERNAL_LIBSAT_IMPORT_base_internal_sym(sym) \
    LIBSAT_BEGIN_EXPORT \
    typedef LIBSAT_SYM(intern_entry) sym ## intern_entry; \
    static inline status FN_DECL_MUST_CHECK \
    sym ## intern_entry_create( \
        LIBSAT_SYM(intern_entry)** w, RCPR_SYM(allocator)* x, const char* y, \
        size_t z) { \
            return LIBSAT_SYM(intern_entry_create)(w,x,y,z); }  \
    static inline status FN_DECL_MUST_CHECK \
    sym ## intern_entry_resource_release( \
        RCPR_SYM(resource)* x) { \
            return LIBSAT_SYM(intern_entry_resource_release)(x); }  \
    LIBSAT_END_EXPORT \
    REQUIRE_SEMICOLON_HERE
#define LIBSAT_IMPORT_base_internal_as(sym) \
    __INTERNAL_LIBSAT_IMPORT_base_internal_sym(sym ## _)
#define LIBSAT_IMPORT_base_internal \
    __INTERNAL_LIBSAT_IMPORT_base_internal_sym()

/* C++ compatibility. */
# ifdef   __cplusplus
}
# endif /*__cplusplus*/
