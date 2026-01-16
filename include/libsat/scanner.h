/**
 * \file libsat/scanner.h
 *
 * \brief Scanner for libsat.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#pragma once

#include <libsat/function_decl.h>
#include <rcpr/resource.h>
#include <rcpr/resource/protected.h>

/* C++ compatibility. */
# ifdef   __cplusplus
extern "C" {
# endif /*__cplusplus*/

/**
 * \brief A scanner capable of generating tokens based on input strings for
 * creating propositional logic statements.
 */
typedef struct LIBSAT_SYM(libsat_scanner) LIBSAT_SYM(libsat_scanner);
struct LIBSAT_SYM(libsat_scanner)
{
    RCPR_SYM(resource) hdr;
    const char* input;
    size_t index;
};

/* C++ compatibility. */
# ifdef   __cplusplus
}
# endif /*__cplusplus*/
