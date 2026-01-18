/**
 * \file base/intern_to_string_tree_key.c
 *
 * \brief Get the string index mapping for an intern to string tree.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include "libsat_base_internal.h"

LIBSAT_IMPORT_base_internal;

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
    void* context, const RCPR_SYM(resource)* r)
{
    (void)context;
    intern_entry* entry = (intern_entry*)r;

    return &entry->string_index;
}
