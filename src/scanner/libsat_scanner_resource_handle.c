/**
 * \file scanner/libsat_scanner_resource_handle.c
 *
 * \brief Get the resource handle for a given \ref libsat_scanner instance.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <libsat/scanner.h>

#include "scanner_internal.h"

/**
 * \brief Given a \ref libsat_scanner instance, return the resource handle for
 * this instance.
 *
 * \param scanner      The \ref libsat_scanner instance from which the resource
 *                      handle is returned.
 *
 * \returns the resource handle for this scanner instance.
 */
RCPR_SYM(resource)*
LIBSAT_SYM(libsat_scanner_resource_handle)(
    LIBSAT_SYM(libsat_scanner)* scanner)
{
    return &scanner->hdr;
}
