/**
 * \file libsat/libsat.h
 *
 * \brief Main header for libsat.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#pragma once

#include <libsat/scanner.h>
#include <rcpr/allocator.h>
#include <rcpr/rbtree.h>
#include <rcpr/resource.h>
#include <rcpr/resource/protected.h>

/* C++ compatibility. */
# ifdef   __cplusplus
extern "C" {
# endif /*__cplusplus*/

/**
 * \brief Context for the libsat instance.
 */
typedef struct LIBSAT_SYM(libsat_context) LIBSAT_SYM(libsat_context);
struct LIBSAT_SYM(libsat_context)
{
    RCPR_SYM(resource) hdr;
    RCPR_SYM(allocator)* alloc;
    RCPR_SYM(rbtree)* intern;
    size_t variable_count;
};

/******************************************************************************/
/* Start of model checking properties.                                        */
/******************************************************************************/

/**
 * \brief Valid context property.
 *
 * \param context       The context instance to be verified.
 *
 * \returns true if the context instance is valid.
 */
bool
LIBSAT_SYM(prop_context_valid)(
    const LIBSAT_SYM(libsat_context)* context);

/* C++ compatibility. */
# ifdef   __cplusplus
}
# endif /*__cplusplus*/
