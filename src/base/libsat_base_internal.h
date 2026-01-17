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
    size_t string_size;
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
 * \brief Compare two opaque \ref intern_entry values.
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

/* C++ compatibility. */
# ifdef   __cplusplus
}
# endif /*__cplusplus*/
