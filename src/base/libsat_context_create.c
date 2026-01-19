/**
 * \file base/libsat_context_create.c
 *
 * \brief Create a \ref libsat_context instance.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <libsat/libsat.h>
#include <rcpr/vtable.h>
#include <string.h>

#include "libsat_base_internal.h"

LIBSAT_IMPORT_base;
LIBSAT_IMPORT_base_internal;
RCPR_IMPORT_allocator;
RCPR_IMPORT_rbtree;
RCPR_IMPORT_resource;

/* the vtable entry for the libsat_context instance. */
RCPR_VTABLE
resource_vtable libsat_context_vtable = {
    &libsat_context_resource_release };

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
    LIBSAT_SYM(libsat_context)** context, RCPR_SYM(allocator)* alloc)
{
    status retval, release_retval;
    libsat_context* tmp;

    /* attempt to allocate memory for this libsat_context. */
    retval = allocator_allocate(alloc, (void**)&tmp, sizeof(*tmp));
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* initialize the context. */
    memset(tmp, 0, sizeof(*tmp));
    tmp->alloc = alloc;

    /* initialize the resource. */
    resource_init(&tmp->hdr, &libsat_context_vtable);

    /* create the string to intern tree. */
    retval =
        rbtree_create(
            &tmp->string_to_intern, alloc, &string_to_intern_tree_compare,
            &string_to_intern_tree_key, NULL);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_tmp;
    }

    /* create the intern to string tree. */
    retval =
        rbtree_create(
            &tmp->intern_to_string, alloc, &intern_to_string_tree_compare,
            &intern_to_string_tree_key, NULL);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_tmp;
    }

    /* success. */
    *context = tmp;
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
