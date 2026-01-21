/**
 * \file base/test_libsat_parse.cpp
 *
 * \brief Unit tests for libsat_parse.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <libsat/libsat.h>
#include <libsat/parser.h>
#include <libsat/status.h>
#include <minunit/minunit.h>

LIBSAT_IMPORT_base;
LIBSAT_IMPORT_parser;
RCPR_IMPORT_allocator;
RCPR_IMPORT_resource;

/**
 * Attempting to parse an empty string returns ERROR_LIBSAT_PARSER_EMPTY_INPUT.
 */
TEST(empty_string_error)
{
    allocator* alloc;
    libsat_context* context;
    libsat_ast_node* node;
    const char* input = R"()";

    /* create malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create context. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_context_create(&context, alloc));

    /* Parse should return an error. */
    TEST_ASSERT(
        ERROR_LIBSAT_PARSER_EMPTY_INPUT
            == libsat_parse(&node, context, input));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS ==
            resource_release(libsat_context_resource_handle(context)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}
