/**
 * \file base/intern_entry_create.c
 *
 * \brief Create an \ref intern_entry.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <rcpr/string.h>
#include <rcpr/vtable.h>
#include <string.h>

#include "libsat_base_internal.h"

LIBSAT_IMPORT_base_internal;
RCPR_IMPORT_allocator;
RCPR_IMPORT_resource;
RCPR_IMPORT_string_as(rcpr);

/* the vtable entry for the intern_entry instance. */
RCPR_VTABLE
resource_vtable intern_entry_vtable = {
    &intern_entry_resource_release };

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
    const char* str, size_t index)
{
    status retval, release_retval;
    intern_entry* tmp;

    /* attempt to allocate memory for this intern_entry. */
    retval = allocator_allocate(alloc, (void**)&tmp, sizeof(intern_entry));
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* initialize the entry. */
    memset(tmp, 0, sizeof(*tmp));
    tmp->alloc = alloc;
    tmp->ref_count = 1;
    tmp->string_index = index;

    /* initialize the resource. */
    resource_init(&tmp->hdr, &intern_entry_vtable);

    /* copy the string. */
    retval = rcpr_strdup(&tmp->string, alloc, str);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_tmp;
    }

    /* success. */
    *entry = tmp;
    retval = STATUS_SUCCESS;
    goto done;

cleanup_tmp:
    release_retval = resource_release(&tmp->hdr);
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

done:
    return retval;
}
