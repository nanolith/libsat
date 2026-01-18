/**
 * \file base/string_to_intern_tree_compare.c
 *
 * \brief Compare two strings for a string-to-index tree.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <string.h>

#include "libsat_base_internal.h"

LIBSAT_IMPORT_base_internal;

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
    void* context, const void* lhs, const void* rhs)
{
    (void)context;
    const char* l = (const char*)lhs;
    const char* r = (const char*)rhs;

    int result = strcmp(l, r);

    if (result < 0)
    {
        return RCPR_COMPARE_LT;
    }
    else if (result > 0)
    {
        return RCPR_COMPARE_GT;
    }
    else
    {
        return RCPR_COMPARE_EQ;
    }
}
