/**
 * \file scanner/scanner_internal.h
 *
 * \brief Internals for the libsat scanner library.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#pragma once

/* C++ compatibility. */
# ifdef   __cplusplus
extern "C" {
# endif /*__cplusplus*/

/******************************************************************************/
/* Start of constructors.                                                     */
/******************************************************************************/

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
    RCPR_SYM(resource)* r);

/* C++ compatibility. */
# ifdef   __cplusplus
}
# endif /*__cplusplus*/
