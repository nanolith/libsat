/**
 * \file libsat/parser.h
 *
 * \brief Parser for libsat.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#pragma once

#include <libsat/function_decl.h>

/* C++ compatibility. */
# ifdef   __cplusplus
extern "C" {
# endif /*__cplusplus*/

/**
 * \brief A parser capable of generating an AST based on input strings for
 * creating propositional logic statements.
 */
typedef struct LIBSAT_SYM(libsat_parser) LIBSAT_SYM(libsat_parser);

/* C++ compatibility. */
# ifdef   __cplusplus
}
# endif /*__cplusplus*/
