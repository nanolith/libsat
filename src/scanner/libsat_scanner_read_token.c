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
static int peek_character(libsat_scanner* scanner);
static int scan_true_or_variable(
    libsat_scanner_token* details, libsat_scanner* scanner);
static int scan_false_or_variable(
    libsat_scanner_token* details, libsat_scanner* scanner);
static int scan_variable(
    libsat_scanner_token* details, libsat_scanner* scanner);
static int scan_math_block(
    libsat_scanner_token* details, libsat_scanner* scanner);
static int scan_negation(
    libsat_scanner_token* details, libsat_scanner* scanner);

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
    int retval;
    int ch = skip_whitespace(scanner);
    start_details(details, scanner);

    switch (ch)
    {
        case 0:
            retval =
                end_details(details, scanner, LIBSAT_SCANNER_TOKEN_TYPE_EOF);
            goto done;

        case ';':
            retval =
                end_details(
                    details, scanner, LIBSAT_SCANNER_TOKEN_TYPE_SEMICOLON);
            goto consume_input;

        case '(':
            retval =
                end_details(
                    details, scanner, LIBSAT_SCANNER_TOKEN_TYPE_OPEN_PAREN);
            goto consume_input;

        case 't':
            retval = scan_true_or_variable(details, scanner);
            goto done;

        case 'f':
            retval = scan_false_or_variable(details, scanner);
            goto done;

        case ')':
            retval =
                end_details(
                    details, scanner, LIBSAT_SCANNER_TOKEN_TYPE_CLOSE_PAREN);
            goto consume_input;

        case 0xE2:
            retval = scan_math_block(details, scanner);
            goto done;

        case 0xC2:
            retval = scan_negation(details, scanner);
            goto done;

        default:
            if (isalpha(ch) || '_' == ch)
            {
                retval = scan_variable(details, scanner);
            }
            else
            {
                retval =
                    end_details(
                        details, scanner, LIBSAT_SCANNER_TOKEN_TYPE_BAD_INPUT);
            }
            goto done;
    }

consume_input:
    next_character(scanner);

done:
    return retval;
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

    return (unsigned char)*(scanner->input);
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

/**
 * \brief Peek the next character in the stream.
 *
 * \param scanner       The scanner instance for this operation.
 *
 * \returns the next character.
 */
static int peek_character(libsat_scanner* scanner)
{
    return (unsigned char)*(scanner->input + 1);
}

/**
 * \brief Scan the true literal, or fall back to scanning a variable.
 *
 * \param details       The token details for this operation.
 * \param scanner       The scanner for this operation.
 *
 * \returns the scanned token.
 */
static int scan_true_or_variable(
    libsat_scanner_token* details, libsat_scanner* scanner)
{
    int peek = peek_character(scanner);

    if ('r' != peek)
    {
        return scan_variable(details, scanner);
    }

    next_character(scanner);
    peek = peek_character(scanner);

    if ('u' != peek)
    {
        return scan_variable(details, scanner);
    }

    next_character(scanner);
    peek = peek_character(scanner);

    if ('e' != peek)
    {
        return scan_variable(details, scanner);
    }

    next_character(scanner);
    peek = peek_character(scanner);

    if (isalnum(peek))
    {
        return scan_variable(details, scanner);
    }
    else
    {
        peek =
            end_details(
                details, scanner, LIBSAT_SCANNER_TOKEN_TYPE_LITERAL_TRUE);

        next_character(scanner);

        return peek;
    }
}

/**
 * \brief Scan the false literal, or fall back to scanning a variable.
 *
 * \param details       The token details for this operation.
 * \param scanner       The scanner for this operation.
 *
 * \returns the scanned token.
 */
static int scan_false_or_variable(
    libsat_scanner_token* details, libsat_scanner* scanner)
{
    int peek = peek_character(scanner);

    if ('a' != peek)
    {
        return scan_variable(details, scanner);
    }

    next_character(scanner);
    peek = peek_character(scanner);

    if ('l' != peek)
    {
        return scan_variable(details, scanner);
    }

    next_character(scanner);
    peek = peek_character(scanner);

    if ('s' != peek)
    {
        return scan_variable(details, scanner);
    }

    next_character(scanner);
    peek = peek_character(scanner);

    if ('e' != peek)
    {
        return scan_variable(details, scanner);
    }

    next_character(scanner);
    peek = peek_character(scanner);

    if (isalnum(peek))
    {
        return scan_variable(details, scanner);
    }
    else
    {
        peek =
            end_details(
                details, scanner, LIBSAT_SCANNER_TOKEN_TYPE_LITERAL_FALSE);

        next_character(scanner);

        return peek;
    }
}

/**
 * \brief Scan a variable.
 *
 * \param details       The token details for this operation.
 * \param scanner       The scanner for this operation.
 *
 * \returns the scanned token.
 */
static int scan_variable(
    libsat_scanner_token* details, libsat_scanner* scanner)
{
    int peek;

    peek = peek_character(scanner);

    while (isalnum(peek) || '_' == peek)
    {
        next_character(scanner);
        peek = peek_character(scanner);
    }

    peek =
        end_details(
            details, scanner, LIBSAT_SCANNER_TOKEN_TYPE_VARIABLE);

    next_character(scanner);

    return peek;
}

/**
 * \brief Scan a glyph in the math block.
 *
 * \param details       The token details for this operation.
 * \param scanner       The scanner for this operation.
 *
 * \returns the scanned token.
 */
static int scan_math_block(
    libsat_scanner_token* details, libsat_scanner* scanner)
{
    /* cache position in case of failure. */
    const char* input = scanner->input;
    size_t index = scanner->index;
    size_t line = scanner->line;
    size_t col = scanner->col;

    int peek = peek_character(scanner);
    if (0x86 == peek)
    {
        next_character(scanner);
        peek = peek_character(scanner);

        /* is this an implication? */
        if (0x92 == peek)
        {
            next_character(scanner);
            peek =
                end_details(
                    details, scanner, LIBSAT_SCANNER_TOKEN_TYPE_IMPLICATION);

            next_character(scanner);

            return peek;
        }
        else if (0x94 == peek)
        {
            next_character(scanner);
            peek =
                end_details(
                    details, scanner, LIBSAT_SCANNER_TOKEN_TYPE_BICONDITIONAL);

            next_character(scanner);

            return peek;
        }
        else
        {
            goto unexpected_glyph;
        }
    }
    else if (0x88 == peek)
    {
        next_character(scanner);
        peek = peek_character(scanner);

        /* is this a conjunction? */
        if (0xA7 == peek)
        {
            next_character(scanner);
            peek =
                end_details(
                    details, scanner, LIBSAT_SCANNER_TOKEN_TYPE_CONJUNCTION);

            next_character(scanner);

            return peek;
        }
        /* is this a disjunction? */
        else if (0xA8 == peek)
        {
            next_character(scanner);
            peek =
                end_details(
                    details, scanner, LIBSAT_SCANNER_TOKEN_TYPE_DISJUNCTION);

            next_character(scanner);

            return peek;
        }
        else
        {
            goto unexpected_glyph;
        }
    }
    else if (0x8A == peek)
    {
        next_character(scanner);
        peek = peek_character(scanner);

        /* is this an exclusive disjunction? */
        if (0xBB == peek)
        {
            next_character(scanner);
            peek =
                end_details(
                    details, scanner,
                    LIBSAT_SCANNER_TOKEN_TYPE_EXCLUSIVE_DISJUNCTION);

            next_character(scanner);

            return peek;
        }
        else
        {
            goto unexpected_glyph;
        }
    }
    else
    {
        goto unexpected_glyph;
    }

unexpected_glyph:
    /* reset scanner. */
    scanner->input = input;
    scanner->index = index;
    scanner->line = line;
    scanner->col = col;

    return end_details(details, scanner, LIBSAT_SCANNER_TOKEN_TYPE_BAD_INPUT);
}

/**
 * \brief Scan a negation glyph.
 *
 * \param details       The token details for this operation.
 * \param scanner       The scanner for this operation.
 *
 * \returns the scanned token.
 */
static int scan_negation(
    libsat_scanner_token* details, libsat_scanner* scanner)
{
    /* cache position in case of failure. */
    const char* input = scanner->input;
    size_t index = scanner->index;
    size_t line = scanner->line;
    size_t col = scanner->col;

    int peek = peek_character(scanner);
    if (0xAC == peek)
    {
        next_character(scanner);
        peek =
            end_details(details, scanner, LIBSAT_SCANNER_TOKEN_TYPE_NEGATION);

        next_character(scanner);

        return peek;
    }
    else
    {
        /* reset scanner. */
        scanner->input = input;
        scanner->index = index;
        scanner->line = line;
        scanner->col = col;

        return
            end_details(details, scanner, LIBSAT_SCANNER_TOKEN_TYPE_BAD_INPUT);
    }
}
