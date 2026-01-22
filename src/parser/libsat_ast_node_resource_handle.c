/**
 * \file parser/libsat_ast_node_resource_handle.c
 *
 * \brief Get the resource handle for a given \ref libsat_ast_node instance.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <libsat/parser.h>

/**
 * \brief Get the resource associated with a \ref libsat_ast_node.
 *
 * \param node          The node for this operation.
 *
 * \returns the resource handle for this \ref libsat_ast_node.
 */
RCPR_SYM(resource)*
LIBSAT_SYM(libsat_ast_node_resource_handle)(
    LIBSAT_SYM(libsat_ast_node)* node)
{
    return &node->hdr;
}
