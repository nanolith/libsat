/**
 * \file scanner/libsat_scanner_peek_token.c
 *
 * \brief Peek at the next token from the scanner.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <libsat/scanner.h>

#include "scanner_internal.h"

LIBSAT_IMPORT_scanner;

/* forward decls. */
static void copy_scanner_position(
    libsat_scanner* lhs, const libsat_scanner* rhs);

/**
 * \brief Peek a token from the scanner instance, populating the provided token
 * structure with additional details.
 *
 * \note This method preserves the previous state of the scanner, reads the next
 * token, then restores this previous state.
 *
 * \param details       Pointer to the token structure to receive additional
 *                      details.
 * \param scanner       The scanner instance from which to peek for the next
 *                      token.
 *
 * \returns a token from the scanner.
 *      - LIBSAT_SCANNER_TOKEN_TYPE_EOF on end of input.
 *      - LIBSAT_SCANNER_TOKEN_TYPE_BAD_INPUT if the scanner encounters bad
 *        input.
 *      - a value belonging to the \ref libsat_scanner_token_type enumeration.
 */
int FN_DECL_MUST_CHECK
LIBSAT_SYM(libsat_scanner_peek_token)(
    LIBSAT_SYM(libsat_scanner_token)* details,
    LIBSAT_SYM(libsat_scanner)* scanner)
{
    int token;
    libsat_scanner cache;

    /* cache the scanner position before read. */
    copy_scanner_position(&cache, scanner);

    token = libsat_scanner_read_token(details, scanner);

    /* restore the scanner position after read. */
    copy_scanner_position(scanner, &cache);

    return token;
}

/**
 * \brief Copy the scanner position from rhs into lhs.
 *
 * \param lhs           The left-hand-side that will receive the scanner
 *                      position from the right-hand-side.
 * \param rhs           The right-hand-side from which the scanner position is
 *                      copied.
 */
static void copy_scanner_position(
    libsat_scanner* lhs, const libsat_scanner* rhs)
{
    lhs->input = rhs->input;
    lhs->index = rhs->index;
    lhs->line = rhs->line;
    lhs->col = rhs->col;
}
