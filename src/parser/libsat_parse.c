/**
 * \file parser/libsat_parse.c
 *
 * \brief Parse an input stream.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <libsat/libsat.h>
#include <libsat/parser.h>
#include <libsat/scanner.h>
#include <libsat/status.h>

LIBSAT_IMPORT_base;
LIBSAT_IMPORT_parser;
LIBSAT_IMPORT_scanner;
RCPR_IMPORT_resource;

/**
 * \brief Parse an input string.
 *
 * \param node          The AST node created on success.
 * \param context       The context for this operation.
 * \param input         The input string to parse.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
LIBSAT_SYM(libsat_parse)(
    LIBSAT_SYM(libsat_ast_node)** node, LIBSAT_SYM(libsat_context)* context,
    const char* input)
{
    status retval, release_retval;
    int token;
    libsat_scanner* scanner;
    libsat_scanner_token details;
    libsat_ast_node* tmp = NULL;

    /* create a scanner for this input string. */
    retval = libsat_scanner_create(&scanner, context, input);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* begin parse loop. */
    do
    {
        /* read the next token. */
        token = libsat_scanner_read_token(&details, scanner);

        switch (token)
        {
            case LIBSAT_SCANNER_TOKEN_TYPE_EOF:
                if (NULL == tmp)
                {
                    retval = ERROR_LIBSAT_PARSER_EMPTY_INPUT;
                    goto cleanup_scanner;
                }
                else
                {
                    /* TODO - replace with completion logic. */
                    retval = ERROR_LIBSAT_PARSER_UNEXPECTED_TOKEN;
                    goto cleanup_scanner;
                }

            default:
                retval = ERROR_LIBSAT_PARSER_UNEXPECTED_TOKEN;
                goto cleanup_scanner;
        }

    } while (token != LIBSAT_SCANNER_TOKEN_TYPE_EOF);

    /* TODO - refactor to remove this. */
    (void)node;
    retval = -1;

cleanup_scanner:
    release_retval = resource_release(libsat_scanner_resource_handle(scanner));
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

done:
    return retval;
}
