/**
 * \file scanner/libsat_ast_node_create_from_variable.c
 *
 * \brief Create an AST node from a variable.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <libsat/libsat.h>
#include <libsat/status.h>
#include <rcpr/vtable.h>
#include <string.h>

#include "../base/libsat_base_internal.h"
#include "parser_internal.h"

LIBSAT_IMPORT_base;
LIBSAT_IMPORT_parser;
RCPR_IMPORT_allocator;
RCPR_IMPORT_resource;

/* the vtable entry for the ast node instance. */
RCPR_VTABLE
resource_vtable libsat_ast_node_vtable = {
    &LIBSAT_SYM(libsat_ast_node_resource_release) };

/**
 * \brief Create an AST node from a variable name.
 *
 * \param node          Pointer to receive the node on success.
 * \param context       The \ref libsat_context for this operation.
 * \param var_name      Name of the variable.
 * \param mode          Variable lookup mode.
 *                      @see \ref libsat_context_variable_get_flags.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
LIBSAT_SYM(libsat_ast_node_create_from_variable)(
    LIBSAT_SYM(libsat_ast_node)** node, LIBSAT_SYM(libsat_context)* context,
    const char* var_name, int mode)
{
    status retval, release_retval;
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
    tmp->type = LIBSAT_PARSER_AST_NODE_TYPE_VARIABLE;

    /* get the variable by name. */
    retval =
        libsat_context_variable_get(
            &tmp->value.variable_index, context, var_name, mode);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_tmp;
    }

    /* success. */
    *node = tmp;
    retval = STATUS_SUCCESS;
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
