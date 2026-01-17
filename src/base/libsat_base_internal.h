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
#include <rcpr/compare.h>

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
LIBSAT_SYM(libsat_intern_tree_compare)(
    void* context, const void* lhs, const void* rhs);

/* C++ compatibility. */
# ifdef   __cplusplus
}
# endif /*__cplusplus*/
