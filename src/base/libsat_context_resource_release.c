/**
 * \file base/libsat_context_resource_release.c
 *
 * \brief Release a \ref libsat_context resource.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <libsat/libsat.h>

#include "libsat_base_internal.h"

LIBSAT_IMPORT_base;
LIBSAT_IMPORT_base_internal;
RCPR_IMPORT_allocator;
RCPR_IMPORT_rbtree;
RCPR_IMPORT_resource;

/**
 * \brief Release a \ref libsat_context resource.
 *
 * \param r             The resource to release.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
LIBSAT_SYM(libsat_context_resource_release)(
    RCPR_SYM(resource)* r)
{
    status retval = STATUS_SUCCESS;
    status release_retval;
    libsat_context* ctx = (libsat_context*)r;

    /* cache allocator. */
    allocator* alloc = ctx->alloc;

    /* release string_to_intern tree if set. */
    if (NULL != ctx->string_to_intern)
    {
        release_retval =
            resource_release(rbtree_resource_handle(ctx->string_to_intern));
        if (STATUS_SUCCESS != release_retval)
        {
            retval = release_retval;
        }
    }

    /* release intern_to_string tree if set. */
    if (NULL != ctx->intern_to_string)
    {
        release_retval =
            resource_release(rbtree_resource_handle(ctx->intern_to_string));
        if (STATUS_SUCCESS != release_retval)
        {
            retval = release_retval;
        }
    }

    /* reclaim structure. */
    release_retval = allocator_reclaim(alloc, ctx);
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

    /* return decoded status code. */
    return retval;
}
