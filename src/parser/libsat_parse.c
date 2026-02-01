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

static bool token_is_binary_operator(int token);
static bool next_operation_binds_tighter(parser_context* context, int token);
static status parse_expression(
    libsat_ast_node** node, parser_context* context, int left_operator);
static status parse_operation(
    libsat_ast_node** node, parser_context* context, libsat_ast_node* lhs);
static status create_variable(libsat_ast_node** node, parser_context* context);
static status parse_statement_from_variable(
    libsat_ast_node** node, parser_context* context);
static status parse_expression_from_variable(
    libsat_ast_node** node, parser_context* context, int left_operator);
static status parse_statement_from_negation(
    libsat_ast_node** node, parser_context* context);
static status parse_expression_from_negation(
    libsat_ast_node** node, parser_context* context);
static status parse_expression_from_conjunction(
    libsat_ast_node** node, parser_context* context, libsat_ast_node* lhs);
static status parse_expression_from_disjunction(
    libsat_ast_node** node, parser_context* context, libsat_ast_node* lhs);

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
 * \brief Returns true if the token is a binary operator.
 *
 * \param token             The token to check.
 *
 * \returns true if this is a binary operator and false otherwise.
 */
static bool token_is_binary_operator(int token)
{
    switch (token)
    {
        case LIBSAT_SCANNER_TOKEN_TYPE_CONJUNCTION:
        case LIBSAT_SCANNER_TOKEN_TYPE_EXCLUSIVE_DISJUNCTION:
        case LIBSAT_SCANNER_TOKEN_TYPE_DISJUNCTION:
        case LIBSAT_SCANNER_TOKEN_TYPE_IMPLICATION:
        case LIBSAT_SCANNER_TOKEN_TYPE_BICONDITIONAL:
            return true;

        default:
            return false;
    }
}

/**
 * \brief Peek at the next token; return true if the next token is an operator
 * and it has higher priority than the given token.
 *
 * \param context           The parser context for this predicate.
 * \param token             The current (left-hand) token to check against.
 *
 * \returns true if the next token is an operator with tighter binding.
 */
static bool next_operation_binds_tighter(parser_context* context, int token)
{
    libsat_scanner_token details;

    int right_token = libsat_scanner_peek_token(&details, context->scanner);

    return
        token_is_binary_operator(right_token)
     && !should_combine_left(token, right_token);
}

/**
 * \brief Parse an expression.
 *
 * \param node              Pointer to the node pointer to hold this expression
 *                          node on success.
 * \param context           The parser context for this operation.
 * \param left_operator     The left-hand operator for lookahead.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status parse_expression(
    libsat_ast_node** node, parser_context* context, int left_operator)
{
    status retval;
    libsat_ast_node* tmp;
    int next_token;

    /* read the next token from the scanner. */
    next_token = libsat_scanner_read_token(&context->details, context->scanner);

    switch (next_token)
    {
        case LIBSAT_SCANNER_TOKEN_TYPE_EOF:
            retval = ERROR_LIBSAT_PARSER_INCOMPLETE_EXPRESSION;
            break;

        case LIBSAT_SCANNER_TOKEN_TYPE_VARIABLE:
            retval =
                parse_expression_from_variable(&tmp, context, left_operator);
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

    /* success. */
    *node = tmp;
    retval = STATUS_SUCCESS;
    goto done;

done:
    return retval;
}

/**
 * \brief Parse an optional operation involving the left-hand side.
 *
 * \param node              Pointer to the node pointer to hold this expression
 *                          node on success.
 * \param context           The parser context for this operation.
 * \param lhs               The left-hand side of the operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status parse_operation(
    libsat_ast_node** node, parser_context* context, libsat_ast_node* lhs)
{
    status retval;
    int next_token;

    /* read the next token from the scanner. */
    next_token = libsat_scanner_read_token(&context->details, context->scanner);

    switch (next_token)
    {
        case LIBSAT_SCANNER_TOKEN_TYPE_EOF:
            /* the left-hand side expression ends this scan. */
            *node = lhs;
            retval = STATUS_SUCCESS;
            break;

        case LIBSAT_SCANNER_TOKEN_TYPE_CONJUNCTION:
            /* create a conjunction expression. */
            retval = parse_expression_from_conjunction(node, context, lhs);
            break;

        case LIBSAT_SCANNER_TOKEN_TYPE_DISJUNCTION:
            /* create a disjunction expression. */
            retval = parse_expression_from_disjunction(node, context, lhs);
            break;

        default:
            retval = ERROR_LIBSAT_PARSER_UNEXPECTED_TOKEN;
            break;
    }

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
    var_name[string_size - 1] = 0;

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
    retval =
        parse_expression_from_variable(
            &expr, context, LIBSAT_SCANNER_TOKEN_TYPE_NOP);
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
 * \param node              Pointer to the node pointer to store the parsed node
 *                          on success.
 * \param context           The parser context for this operation.
 * \param left_operator     The left-hand-side operator for lookahead.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status parse_expression_from_variable(
    libsat_ast_node** node, parser_context* context, int left_operator)
{
    status retval, release_retval;
    libsat_ast_node* tmp;

    /* shift this variable. */
    retval = create_variable(&tmp, context);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* is the next operator tighter binding than the previous one? */
    if (next_operation_binds_tighter(context, left_operator))
    {
        /* fold this variable into the next operation. */
        retval = parse_operation(node, context, tmp);
        if (STATUS_SUCCESS != retval)
        {
            goto cleanup_tmp;
        }
    }
    else
    {
        *node = tmp;
    }

    /* success. */
    goto done;

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

    /* read the next expression. */
    retval = parse_expression(&subexpr, context, 0);
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

/**
 * \brief Attempt to parse an expression from a conjunction operator and a
 * left-hand-side expression.
 *
 * \param node          Pointer to the node pointer to receive this expression
 *                      on success.
 * \param context       The context for this operation.
 * \param lhs           The left-hand-side expression for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status parse_expression_from_conjunction(
    libsat_ast_node** node, parser_context* context, libsat_ast_node* lhs)
{
    status retval, release_retval;
    libsat_ast_node* tmp;
    libsat_ast_node* rhs;

    /* parse the next expression. */
    retval =
        parse_expression(&rhs, context, LIBSAT_SCANNER_TOKEN_TYPE_CONJUNCTION);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* fold this right-hand-side into the more tightly binding operation. */
    if (
        next_operation_binds_tighter(
            context, LIBSAT_SCANNER_TOKEN_TYPE_CONJUNCTION))
    {
        /* fold this next expression into this operation. */
        retval = parse_operation(&rhs, context, rhs);
        if (STATUS_SUCCESS != retval)
        {
            goto cleanup_rhs;
        }
    }

    /* create the conjunction. */
    retval =
        libsat_ast_node_create_as_conjunction(
            &tmp, context->context, lhs, rhs);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_rhs;
    }

    /* fold this conjunction into the next operation. */
    retval = parse_operation(node, context, tmp);
    if (STATUS_SUCCESS != retval)
    {
        /* the caller maintains ownership of lhs. */
        tmp->value.binary.lhs = NULL;
        goto cleanup_tmp;
    }

    /* success. */
    goto done;

cleanup_tmp:
    release_retval = resource_release(&tmp->hdr);
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

cleanup_rhs:
    release_retval = resource_release(&rhs->hdr);
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

done:
    return retval;
}

/**
 * \brief Attempt to parse an expression from a disjunction operator and a
 * left-hand-side expression.
 *
 * \param node          Pointer to the node pointer to receive this expression
 *                      on success.
 * \param context       The context for this operation.
 * \param lhs           The left-hand-side expression for this operation.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status parse_expression_from_disjunction(
    libsat_ast_node** node, parser_context* context, libsat_ast_node* lhs)
{
    status retval, release_retval;
    libsat_ast_node* tmp;
    libsat_ast_node* rhs;

    /* parse the next expression. */
    retval =
        parse_expression(&rhs, context, LIBSAT_SCANNER_TOKEN_TYPE_DISJUNCTION);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* fold this right-hand-side into the more tightly binding operation. */
    if (
        next_operation_binds_tighter(
            context, LIBSAT_SCANNER_TOKEN_TYPE_DISJUNCTION))
    {
        /* fold this next expression into this operation. */
        retval = parse_operation(&rhs, context, rhs);
        if (STATUS_SUCCESS != retval)
        {
            goto cleanup_rhs;
        }
    }

    /* create the conjunction. */
    retval =
        libsat_ast_node_create_as_disjunction(
            &tmp, context->context, lhs, rhs);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_rhs;
    }

    /* fold this conjunction into the next operation. */
    retval = parse_operation(node, context, tmp);
    if (STATUS_SUCCESS != retval)
    {
        /* the caller maintains ownership of lhs. */
        tmp->value.binary.lhs = NULL;
        goto cleanup_tmp;
    }

    /* success. */
    goto done;

cleanup_tmp:
    release_retval = resource_release(&tmp->hdr);
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

cleanup_rhs:
    release_retval = resource_release(&rhs->hdr);
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

done:
    return retval;
}
