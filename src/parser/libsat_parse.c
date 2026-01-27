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
#include <string.h>

#include "parser_internal.h"

LIBSAT_IMPORT_base;
LIBSAT_IMPORT_parser;
LIBSAT_IMPORT_parser_internal;
LIBSAT_IMPORT_scanner;
RCPR_IMPORT_resource;

/* forward decls. */
typedef struct parser_context
{
    int token;
    libsat_scanner_token details;
    libsat_scanner* scanner;
    libsat_context* context;
    const char* input;
} parser_context;

static status create_variable(libsat_ast_node** node, parser_context* context);
static status parse_statement_from_variable(
    libsat_ast_node** node, parser_context* context);
static status parse_expression_from_variable(
    libsat_ast_node** node, parser_context* context);
static status parse_statement_from_negation(
    libsat_ast_node** node, parser_context* context);
static status parse_expression_from_negation(
    libsat_ast_node** node, parser_context* context);

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
    parser_context parser;
    libsat_ast_node* tmp;

    /* set up parser context. */
    memset(&parser, 0, sizeof(parser));
    parser.context = context;
    parser.input = input;

    /* create a scanner for this input string. */
    retval = libsat_scanner_create(&parser.scanner, parser.context, input);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* read the first token. */
    token = libsat_scanner_read_token(&parser.details, parser.scanner);

    switch (token)
    {
        case LIBSAT_SCANNER_TOKEN_TYPE_EOF:
            retval = ERROR_LIBSAT_PARSER_EMPTY_INPUT;
            break;

        case LIBSAT_SCANNER_TOKEN_TYPE_VARIABLE:
            retval = parse_statement_from_variable(&tmp, &parser);
            break;

        case LIBSAT_SCANNER_TOKEN_TYPE_NEGATION:
            retval = parse_statement_from_negation(&tmp, &parser);
            break;

        default:
            retval = ERROR_LIBSAT_PARSER_UNEXPECTED_TOKEN;
            break;
    }

    /* if the parse failed, clean up. */
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_scanner;
    }

    /* parse success. */
    *node = tmp;
    retval = STATUS_SUCCESS;
    goto cleanup_scanner;

cleanup_scanner:
    release_retval =
        resource_release(libsat_scanner_resource_handle(parser.scanner));
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

done:
    return retval;
}

/**
 * \brief Create a variable from the scanned variable.
 *
 * \param node              Pointer to the node pointer to hold this node on
 *                          success.
 * \param context           The parser context for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status create_variable(libsat_ast_node** node, parser_context* context)
{
    status retval;
    char var_name[1024];

    /* calculate the total length of the ASCII-Z string. */
    size_t string_size =
        ((context->details.end_index+1) - context->details.begin_index) + 1;

    /* if this string size is larger than the variable name, then this is an
      * error. */
    if (string_size > sizeof(var_name))
    {
        retval = ERROR_LIBSAT_PARSER_VARIABLE_NAME_TOO_LARGE;
        goto done;
    }

    /* create an ASCII-Z variable name. */
    memcpy(
        var_name, context->input + context->details.begin_index,
        string_size - 1);
    var_name[string_size] = 0;

    /* create the AST node instance based on the parsed variable. */
    retval =
        libsat_ast_node_create_from_variable(
            node, context->context, var_name, LIBSAT_VARIABLE_GET_DEFAULT);
    goto done;

done:
    return retval;
}

/**
 * \brief Parse a statement starting with a variable.
 *
 * \param context           The parser context for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status parse_statement_from_variable(
    libsat_ast_node** node, parser_context* context)
{
    status retval, release_retval;
    libsat_ast_node* expr;
    libsat_ast_node* stmt;

    /* parse an expression from this variable. */
    retval = parse_expression_from_variable(&expr, context);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* create a statement from this expression. */
    retval = libsat_ast_node_create_as_statement(&stmt, context->context, expr);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_expr;
    }

    /* success. */
    *node = stmt;
    retval = STATUS_SUCCESS;
    goto done;

cleanup_expr:
    release_retval = resource_release(&expr->hdr);
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

done:
    return retval;
}

/**
 * \brief Parse an expression starting with a variable.
 *
 * \param context           The parser context for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status parse_expression_from_variable(
    libsat_ast_node** node, parser_context* context)
{
    status retval, release_retval;
    libsat_ast_node* tmp;
    int next_token;

    /* shift this variable. */
    retval = create_variable(&tmp, context);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* read the next token from the scanner. */
    next_token = libsat_scanner_read_token(&context->details, context->scanner);

    switch (next_token)
    {
        case LIBSAT_SCANNER_TOKEN_TYPE_EOF:
            /* the variable ends this scan. */
            *node = tmp;
            retval = STATUS_SUCCESS;
            goto done;

        default:
            retval = ERROR_LIBSAT_PARSER_UNEXPECTED_TOKEN;
            goto cleanup_tmp;
    }

cleanup_tmp:
    release_retval = resource_release(&tmp->hdr);
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

done:
    return retval;
}

/**
 * \brief Parse a statement starting with a negation.
 *
 * \param context           The parser context for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status parse_statement_from_negation(
    libsat_ast_node** node, parser_context* context)
{
    status retval, release_retval;
    libsat_ast_node* expr;
    libsat_ast_node* stmt;

    /* parse an expression from this negation. */
    retval = parse_expression_from_negation(&expr, context);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* create a statement from this expression. */
    retval = libsat_ast_node_create_as_statement(&stmt, context->context, expr);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_expr;
    }

    /* success. */
    *node = stmt;
    retval = STATUS_SUCCESS;
    goto done;

cleanup_expr:
    release_retval = resource_release(&expr->hdr);
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

done:
    return retval;
}

/**
 * \brief Parse an expression starting with a negation.
 *
 * \param context           The parser context for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status parse_expression_from_negation(
    libsat_ast_node** node, parser_context* context)
{
    status retval, release_retval;
    libsat_ast_node* tmp;
    libsat_ast_node* subexpr;
    int next_token;

    /* read the next token from the scanner. */
    next_token = libsat_scanner_read_token(&context->details, context->scanner);

    switch (next_token)
    {
        case LIBSAT_SCANNER_TOKEN_TYPE_EOF:
            retval = ERROR_LIBSAT_PARSER_INCOMPLETE_EXPRESSION;
            break;

        case LIBSAT_SCANNER_TOKEN_TYPE_VARIABLE:
            retval = parse_expression_from_variable(&subexpr, context);
            break;

        default:
            retval = ERROR_LIBSAT_PARSER_UNEXPECTED_TOKEN;
            break;
    }

    /* decode response. */
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* wrap this in a negation. */
    retval =
        libsat_ast_node_create_as_negation(&tmp, context->context, subexpr);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_subexpr;
    }

    /* success. */
    *node = tmp;
    retval = STATUS_SUCCESS;
    goto done;

cleanup_subexpr:
    release_retval = resource_release(&subexpr->hdr);
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

done:
    return retval;
}
