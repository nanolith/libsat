/**
 * \file base/libsat_context_resource_handle.c
 *
 * \brief Get the resource handle for a given \ref libsat_context instance.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <libsat/libsat.h>

#include "libsat_base_internal.h"

LIBSAT_IMPORT_base_internal;

/**
 * \brief Given a \ref libsat_context instance, return the resource handle for
 * this instance.
 *
 * \param context       The \ref libsat_context instance from which the resource
 *                      handle is returned.
 *
 * \returns the resource handle for this context instance.
 */
RCPR_SYM(resource)*
LIBSAT_SYM(libsat_context_resource_handle)(
    LIBSAT_SYM(libsat_context)* context)
{
    return &context->hdr;
}
