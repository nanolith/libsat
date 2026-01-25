/**
 * \file scanner/libsat_ast_node_create_as_implication.c
 *
 * \brief Create an implication AST node.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <libsat/status.h>
#include <rcpr/vtable.h>
#include <string.h>

#include "../base/libsat_base_internal.h"
#include "parser_internal.h"

LIBSAT_IMPORT_parser;
RCPR_IMPORT_allocator;
RCPR_IMPORT_resource;

/* the vtable entry for the ast node instance. */
RCPR_VTABLE
resource_vtable libsat_ast_node_vtable = {
    &LIBSAT_SYM(libsat_ast_node_resource_release) };

/**
 * \brief Create an AST node from as an implication of two nodes.
 *
 * \param node          Pointer to receive the node on success.
 * \param context       The \ref libsat_context for this operation.
 * \param lhs           The left-hand side of the implication. Note that this
 *                      node takes ownership of this left-hand-side node on
 *                      success.
 * \param rhs           The right-hand side of the implication. Note that this
 *                      node takes ownership of this right-hand-side node on
 *                      success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
LIBSAT_SYM(libsat_ast_node_create_as_implication)(
    LIBSAT_SYM(libsat_ast_node)** node, LIBSAT_SYM(libsat_context)* context,
    LIBSAT_SYM(libsat_ast_node)* lhs, LIBSAT_SYM(libsat_ast_node)* rhs)
{
    status retval;
    libsat_ast_node* tmp;

    /* allocate memory for the node. */
    retval = allocator_allocate(context->alloc, (void**)&tmp, sizeof(*tmp));
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* initialize node. */
    memset(tmp, 0, sizeof(*tmp));
    resource_init(&tmp->hdr, &libsat_ast_node_vtable);
    tmp->alloc = context->alloc;
    tmp->type = LIBSAT_PARSER_AST_NODE_TYPE_IMPLICATION;
    tmp->value.binary.lhs = lhs;
    tmp->value.binary.rhs = rhs;

    /* success. */
    *node = tmp;
    retval = STATUS_SUCCESS;
    goto done;

done:
    return retval;
}
