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
#include <libsat/libsat_fwd.h>
#include <rcpr/allocator.h>
#include <rcpr/resource.h>
#include <rcpr/resource/protected.h>

/* C++ compatibility. */
# ifdef   __cplusplus
extern "C" {
# endif /*__cplusplus*/

/**
 * \brief Token details.
 */
typedef struct LIBSAT_SYM(libsat_scanner_token)
LIBSAT_SYM(libsat_scanner_token);
struct LIBSAT_SYM(libsat_scanner_token)
{
    int type;
    size_t begin_index;
    size_t end_index;
    size_t begin_line;
    size_t begin_col;
    size_t end_line;
    size_t end_col;
    union
    {
        int istring;
        uint64_t u64;
        int64_t i64;
        bool boolean;
    } value;
};

/**
 * \brief A scanner capable of generating tokens based on input strings for
 * creating propositional logic statements.
 */
typedef struct LIBSAT_SYM(libsat_scanner) LIBSAT_SYM(libsat_scanner);
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

/* C++ compatibility. */
# ifdef   __cplusplus
}
# endif /*__cplusplus*/
