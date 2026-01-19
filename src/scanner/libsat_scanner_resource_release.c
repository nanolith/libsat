/**
 * \file scanner/libsat_scanner_resource_release.c
 *
 * \brief Release the resources associated with a scanner.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include "scanner_internal.h"

LIBSAT_IMPORT_base;
LIBSAT_IMPORT_scanner;
RCPR_IMPORT_allocator;

/**
 * \brief Release a \ref libsat_scanner resource.
 *
 * \param r             The resource to release.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
LIBSAT_SYM(libsat_scanner_resource_release)(
    RCPR_SYM(resource)* r)
{
    status retval = STATUS_SUCCESS, release_retval;
    libsat_scanner* scanner = (libsat_scanner*)r;

    /* cache allocator. */
    allocator* alloc = scanner->alloc;

    /* reclaim memory. */
    release_retval = allocator_reclaim(alloc, scanner);
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

    /* return decoded status. */
    return retval;
}
