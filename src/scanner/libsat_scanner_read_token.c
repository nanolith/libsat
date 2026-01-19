/**
 * \file scanner/libsat_scanner_read_token.c
 *
 * \brief Read a token from the scanner.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <string.h>
#include <ctype.h>

#include "scanner_internal.h"

LIBSAT_IMPORT_scanner;

/* forward decls. */
static int skip_whitespace(libsat_scanner* scanner);
static void start_details(
    libsat_scanner_token* details, libsat_scanner* scanner);
static int end_details(
    libsat_scanner_token* details, libsat_scanner* scanner, int token_type);
static void next_character(libsat_scanner* scanner);

/**
 * \brief Read a token from the scanner instance, populating the provided token
 * structure with additional details.
 *
 * \param details       Pointer to the token structure to receive additional
 *                      details.
 * \param scanner       The scanner instance from which this token is read.
 *
 * \returns a token from the scanner.
 *      - LIBSAT_SCANNER_TOKEN_TYPE_EOF on end of input.
 *      - LIBSAT_SCANNER_TOKEN_TYPE_BAD_INPUT if the scanner encounters bad
 *        input.
 *      - a value belonging to the \ref libsat_scanner_token_type enumeration.
 */
int FN_DECL_MUST_CHECK
LIBSAT_SYM(libsat_scanner_read_token)(
    LIBSAT_SYM(libsat_scanner_token)* details,
    LIBSAT_SYM(libsat_scanner)* scanner)
{
    int ch = skip_whitespace(scanner);
    start_details(details, scanner);

    switch (ch)
    {
        case 0:
            return end_details(details, scanner, LIBSAT_SCANNER_TOKEN_TYPE_EOF);

        default:
            return
                end_details(
                    details, scanner, LIBSAT_SCANNER_TOKEN_TYPE_BAD_INPUT);
    }
}

/**
 * \brief Update the details structure with the start of the token.
 *
 * \param details       Token details to update.
 * \param scanner       The scanner instance for this operation.
 */
static void start_details(
    libsat_scanner_token* details, libsat_scanner* scanner)
{
    memset(details, 0, sizeof(*details));
    details->begin_index = details->end_index = scanner->index;
    details->begin_line = details->end_line = scanner->line;
    details->begin_col = details->end_col = scanner->col;
}

/**
 * \brief Update the details structure with the end of the token.
 *
 * \param details       Token details to update.
 * \param scanner       The scanner instance for this operation.
 * \param token_type    The token type of these details.
 *
 * \returns the token type.
 */
static int end_details(
    libsat_scanner_token* details, libsat_scanner* scanner, int token_type)
{
    details->type = token_type;
    details->end_index = scanner->index;
    details->end_line = scanner->line;
    details->end_col = scanner->col;

    return token_type;
}

/**
 * \brief Skip whitespace to get the start of the next token.
 *
 * \param scanner       The scanner instance for this operation.
 *
 * \returns the start of the next token.
 */
static int skip_whitespace(libsat_scanner* scanner)
{
    while (*(scanner->input) != 0 && isspace(*(scanner->input)))
    {
        next_character(scanner);
    }

    return *(scanner->input);
}

/**
 * \brief Skip to the next character.
 *
 * \param scanner       The scanner instance for this operation.
 */
static void next_character(libsat_scanner* scanner)
{
    /* increment the column if this is not a multibyte character. */
    if (0 == (*(scanner->input) & 0x80))
        scanner->col += 1;

    if ('\n' == *(scanner->input))
    {
        scanner->col = 1;
        scanner->line += 1;
    }

    scanner->input += 1;
    scanner->index += 1;
}
