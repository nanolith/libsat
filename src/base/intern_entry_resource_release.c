/**
 * \file base/intern_entry_resource_release.c
 *
 * \brief Release an \ref intern_entry resource.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include "libsat_base_internal.h"

LIBSAT_IMPORT_base_internal;
RCPR_IMPORT_allocator;
RCPR_IMPORT_resource;

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
    RCPR_SYM(resource)* r)
{
    status retval = STATUS_SUCCESS;
    status release_retval;
    intern_entry* entry = (intern_entry*)r;

    /* cache allocator. */
    allocator* alloc = entry->alloc;

    /* are there more references? */
    if (entry->ref_count > 1)
    {
        /* decrement the reference count; we no longer share ownership. */
        entry->ref_count -= 1;

        /* success. */
        goto done;
    }

    /* reclaim string if set. */
    if (NULL != entry->string)
    {
        release_retval = allocator_reclaim(alloc, entry->string);
        if (STATUS_SUCCESS != release_retval)
        {
            retval = release_retval;
        }
    }

    /* reclaim structure. */
    release_retval = allocator_reclaim(alloc, entry);
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

done:
    /* return decoded status code. */
    return retval;
}
