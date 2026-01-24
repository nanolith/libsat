/**
 * \file scanner/libsat_ast_node_resource_release.c
 *
 * \brief Release the resources associated with an AST node.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <libsat/status.h>

#include "parser_internal.h"

LIBSAT_IMPORT_parser;
RCPR_IMPORT_allocator;
RCPR_IMPORT_resource;

/* forward decls. */
static status release_variable(libsat_ast_node* node);
static status release_boolean_literal(libsat_ast_node* node);
static status release_unary(libsat_ast_node* node);
static status release_binary(libsat_ast_node* node);
static status release_list(libsat_ast_node* node);

/**
 * \brief Release a \ref libsat_ast_node resource.
 *
 * \param r             The resource to release.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
LIBSAT_SYM(libsat_ast_node_resource_release)(
    RCPR_SYM(resource)* r)
{
    status retval = STATUS_SUCCESS, release_retval;
    libsat_ast_node* node = (libsat_ast_node*)r;

    /* cache allocator. */
    allocator* alloc = node->alloc;

    /* decode node type. */
    switch (node->type)
    {
        case LIBSAT_PARSER_AST_NODE_TYPE_VARIABLE:
            release_retval = release_variable(node);
            break;
        case LIBSAT_PARSER_AST_NODE_TYPE_BOOLEAN_LITERAL:
            release_retval = release_boolean_literal(node);
            break;
        case LIBSAT_PARSER_AST_NODE_TYPE_NEGATION:
        case LIBSAT_PARSER_AST_NODE_TYPE_STATEMENT:
            release_retval = release_unary(node);
            break;
        case LIBSAT_PARSER_AST_NODE_TYPE_CONJUNCTION:
        case LIBSAT_PARSER_AST_NODE_TYPE_DISJUNCTION:
        case LIBSAT_PARSER_AST_NODE_TYPE_EXCLUSIVE_DISJUNCTION:
        case LIBSAT_PARSER_AST_NODE_TYPE_IMPLICATION:
        case LIBSAT_PARSER_AST_NODE_TYPE_BICONDITIONAL:
        case LIBSAT_PARSER_AST_NODE_TYPE_ASSIGNMENT:
            release_retval = release_binary(node);
            break;
        case LIBSAT_PARSER_AST_NODE_TYPE_STATEMENT_LIST:
            release_retval = release_list(node);
            break;
        default:
            release_retval = ERROR_LIBSAT_PARSER_UNSUPPORTED_AST_NODE_TYPE;
    }

    /* decode release return value. */
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

    /* reclaim memory. */
    release_retval = allocator_reclaim(alloc, node);
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

    /* return decoded result. */
    return retval;
}

/**
 * \brief Release variable node details.
 *
 * \param node          The node details to release.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status release_variable(libsat_ast_node* node)
{
    /* no details to release. */
    (void)node;

    return STATUS_SUCCESS;
}

/**
 * \brief Release boolean literal node details.
 *
 * \param node          The node details to release.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status release_boolean_literal(libsat_ast_node* node)
{
    /* no details to release. */
    (void)node;

    return STATUS_SUCCESS;
}

/**
 * \brief Release unary node details.
 *
 * \param node          The node details to release.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status release_unary(libsat_ast_node* node)
{
    return resource_release(&(node->value.unary->hdr));
}

/**
 * \brief Release binary node details.
 *
 * \param node          The node details to release.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status release_binary(libsat_ast_node* node)
{
    status retval = STATUS_SUCCESS, release_retval;

    /* release lhs. */
    release_retval = resource_release(&(node->value.binary.lhs->hdr));
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

    /* release rhs. */
    release_retval = resource_release(&(node->value.binary.rhs->hdr));
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

    /* return decoded status. */
    return retval;
}

/**
 * \brief Release expression list node details.
 *
 * \param node          The node details to release.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
static status release_list(libsat_ast_node* node)
{
    status retval = STATUS_SUCCESS, release_retval;
    libsat_ast_node* x = node->value.list.head;

    while (NULL != x)
    {
        libsat_ast_node* tmp = x->next;

        release_retval = resource_release(&x->hdr);
        if (STATUS_SUCCESS != release_retval)
        {
            retval = release_retval;
        }

        x = tmp;
    }

    /* return decoded status. */
    return retval;
}
