/**
 * \file scanner/libsat_ast_list_node_push.c
 *
 * \brief Push a statement node onto a statement list.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <libsat/status.h>

#include "parser_internal.h"

/**
 * \brief Push an AST node onto the list.
 *
 * \param list_node     The list node for this operation.
 * \param child         The child node to push onto the list. Note that the list
 *                      node takes ownership of this child node on success. This
 *                      node must be a statement.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
LIBSAT_SYM(libsat_ast_list_node_push)(
    LIBSAT_SYM(libsat_ast_node)* list_node, LIBSAT_SYM(libsat_ast_node)* child)
{
    status retval;

    /* the child node must be a statement. */
    if (LIBSAT_PARSER_AST_NODE_TYPE_STATEMENT != child->type)
    {
        retval = ERROR_LIBSAT_PARSER_CHILD_MUST_BE_STATEMENT;
        goto done;
    }

    /* the list node must be a list. */
    if (LIBSAT_PARSER_AST_NODE_TYPE_STATEMENT_LIST != list_node->type)
    {
        retval = ERROR_LIBSAT_PARSER_LIST_NODE_MUST_BE_STATEMENT_LIST;
        goto done;
    }

    /* push this node. */
    child->next = list_node->value.list.head;
    list_node->value.list.head = child;

    /* success. */
    retval = STATUS_SUCCESS;
    goto done;

done:
    return retval;
}
