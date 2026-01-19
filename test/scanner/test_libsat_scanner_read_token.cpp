/**
 * \file base/test_libsat_scanner_read_token.cpp
 *
 * \brief Unit tests for libsat_scanner_read_token.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <libsat/libsat.h>
#include <libsat/scanner.h>
#include <libsat/status.h>
#include <minunit/minunit.h>

LIBSAT_IMPORT_base;
LIBSAT_IMPORT_scanner;
RCPR_IMPORT_allocator;
RCPR_IMPORT_resource;

TEST_SUITE(libsat_scanner_read_token);

/**
 * The empty string returns LIBSAT_SCANNER_TOKEN_TYPE_EOF.
 */
TEST(empty_string_eof)
{
    allocator* alloc;
    libsat_context* context;
    libsat_scanner* scanner;
    libsat_scanner_token details;
    const char* input = R"()";

    /* create malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create context. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_context_create(&context, alloc));

    /* create scanner. */
    TEST_ASSERT(
        STATUS_SUCCESS == libsat_scanner_create(&scanner, context, input));

    /* read a token from the scanner. */
    int token = libsat_scanner_read_token(&details, scanner);

    /* this token should be EOF. */
    TEST_EXPECT(LIBSAT_SCANNER_TOKEN_TYPE_EOF == token);

    /* verify the details. */
    TEST_EXPECT(token == details.type);
    TEST_EXPECT(0 == details.begin_index);
    TEST_EXPECT(0 == details.end_index);
    TEST_EXPECT(1 == details.begin_line);
    TEST_EXPECT(1 == details.end_line);
    TEST_EXPECT(1 == details.begin_col);
    TEST_EXPECT(1 == details.end_col);

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS ==
            resource_release(libsat_scanner_resource_handle(scanner)));
    TEST_ASSERT(
        STATUS_SUCCESS ==
            resource_release(libsat_context_resource_handle(context)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}
