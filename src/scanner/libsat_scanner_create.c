/**
 * \file scanner/libsat_scanner_create.c
 *
 * \brief Create a \ref libsat_scanner instance.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <rcpr/vtable.h>
#include <string.h>

#include "scanner_internal.h"
#include "../base/libsat_base_internal.h"

LIBSAT_IMPORT_scanner;
LIBSAT_IMPORT_scanner_internal;
RCPR_IMPORT_allocator;
RCPR_IMPORT_resource;

/* the vtable entry for the libsat_scanner instance. */
RCPR_VTABLE
resource_vtable libsat_scanner_vtable = {
    &libsat_scanner_resource_release };

/**
 * \brief Create a scanner instance.
 *
 * \param scanner       Pointer to the scanner pointer to be set to this created
 *                      scanner instance on success.
 * \param context       The context for this operation.
 * \param input         The input string to scan.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
LIBSAT_SYM(libsat_scanner_create)(
    LIBSAT_SYM(libsat_scanner)** scanner, LIBSAT_SYM(libsat_context)* context,
    const char* input)
{
    status retval;
    libsat_scanner* tmp;

    /* allocate memory for this instance. */
    retval = allocator_allocate(context->alloc, (void**)&tmp, sizeof(*tmp));
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* clear memory. */
    memset(tmp, 0, sizeof(*tmp));

    /* initialize resource. */
    resource_init(&tmp->hdr, &libsat_scanner_vtable);

    /* initialize scanner. */
    tmp->alloc = context->alloc;
    tmp->context = context;
    tmp->input = input;
    tmp->line = tmp->col = 1;

    /* success. */
    *scanner = tmp;
    retval = STATUS_SUCCESS;
    goto done;

done:
    return retval;
}
