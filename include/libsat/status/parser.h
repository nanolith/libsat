/**
 * \file libsat/status/parser.h
 *
 * \brief parser status codes for libsat.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#pragma once

#include <libsat/status.h>

/**
 * \brief Empty input was encountered.
 */
#define ERROR_LIBSAT_PARSER_EMPTY_INPUT \
    STATUS_CODE(1, LIBSAT_COMPONENT_PARSER, 0x0000)

/**
 * \brief An unexpected token was encountered.
 */
#define ERROR_LIBSAT_PARSER_UNEXPECTED_TOKEN \
    STATUS_CODE(1, LIBSAT_COMPONENT_PARSER, 0x0001)

/**
 * \brief An unsupported AST node type was encountered.
 */
#define ERROR_LIBSAT_PARSER_UNSUPPORTED_AST_NODE_TYPE \
    STATUS_CODE(1, LIBSAT_COMPONENT_PARSER, 0x0002)
