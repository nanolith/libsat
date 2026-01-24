/**
 * \file parser/parser_internal.h
 *
 * \brief Internal details for the parser.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#pragma once

#include <libsat/parser.h>

/* C++ compatibility. */
# ifdef   __cplusplus
extern "C" {
# endif /*__cplusplus*/

/******************************************************************************/
/* Start of private constructors.                                             */
/******************************************************************************/

/******************************************************************************/
/* Start of private exports.                                                  */
/******************************************************************************/
#define __INTERNAL_LIBSAT_IMPORT_parser_internal_sym(sym) \
    LIBSAT_BEGIN_EXPORT \
    LIBSAT_END_EXPORT \
    REQUIRE_SEMICOLON_HERE
#define LIBSAT_IMPORT_parser_internal_as(sym) \
    __INTERNAL_LIBSAT_IMPORT_parser_internal_sym(sym ## _)
#define LIBSAT_IMPORT_parser_internal \
    __INTERNAL_LIBSAT_IMPORT_parser_internal_sym()

/* C++ compatibility. */
# ifdef   __cplusplus
}
# endif /*__cplusplus*/
