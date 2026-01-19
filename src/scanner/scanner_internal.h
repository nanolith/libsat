/**
 * \file scanner/scanner_internal.h
 *
 * \brief Internals for the libsat scanner library.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#pragma once

#include <libsat/libsat.h>
#include <libsat/function_decl.h>
#include <rcpr/allocator.h>
#include <rcpr/resource.h>
#include <rcpr/resource/protected.h>

/* C++ compatibility. */
# ifdef   __cplusplus
extern "C" {
# endif /*__cplusplus*/

/**
 * \brief libsat_scanner implementation.
 */
struct LIBSAT_SYM(libsat_scanner)
{
    RCPR_SYM(resource) hdr;
    RCPR_SYM(allocator)* alloc;
    LIBSAT_SYM(libsat_context)* context;
    const char* input;
    size_t index;
    size_t line;
    size_t col;
};

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

/******************************************************************************/
/* Start of public exports.                                                   */
/******************************************************************************/
#define __INTERNAL_LIBSAT_IMPORT_scanner_internal_sym(sym) \
    LIBSAT_BEGIN_EXPORT \
    static inline status FN_DECL_MUST_CHECK \
    sym ## libsat_scanner_resource_release( \
        RCPR_SYM(resource)* x) { \
            return LIBSAT_SYM(libsat_scanner_resource_release)(x); } \
    LIBSAT_END_EXPORT \
    REQUIRE_SEMICOLON_HERE
#define LIBSAT_IMPORT_scanner_internal_as(sym) \
    __INTERNAL_LIBSAT_IMPORT_scanner_internal_sym(sym ## _)
#define LIBSAT_IMPORT_scanner_internal \
    __INTERNAL_LIBSAT_IMPORT_scanner_internal_sym()

/* C++ compatibility. */
# ifdef   __cplusplus
}
# endif /*__cplusplus*/
