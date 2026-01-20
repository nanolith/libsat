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

/**
 * A whitespace string returns EOF.
 */
TEST(space_string_eof)
{
    allocator* alloc;
    libsat_context* context;
    libsat_scanner* scanner;
    libsat_scanner_token details;
    const char* input = "  \t \n ";

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
    TEST_EXPECT(6 == details.begin_index);
    TEST_EXPECT(6 == details.end_index);
    TEST_EXPECT(2 == details.begin_line);
    TEST_EXPECT(2 == details.end_line);
    TEST_EXPECT(2 == details.begin_col);
    TEST_EXPECT(2 == details.end_col);

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

/**
 * The empty string returns LIBSAT_SCANNER_TOKEN_TYPE_EOF, even if called twice.
 */
TEST(empty_string_eof_eof)
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

    /* read the next token from the scanner. */
    token = libsat_scanner_read_token(&details, scanner);

    /* this token should still be EOF. */
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

/**
 * We can scan a semicolon.
 */
TEST(semicolon)
{
    allocator* alloc;
    libsat_context* context;
    libsat_scanner* scanner;
    libsat_scanner_token details;
    const char* input = R"( ; )";

    /* create malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create context. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_context_create(&context, alloc));

    /* create scanner. */
    TEST_ASSERT(
        STATUS_SUCCESS == libsat_scanner_create(&scanner, context, input));

    /* read a token from the scanner. */
    int token = libsat_scanner_read_token(&details, scanner);

    /* this token should be SEMICOLON. */
    TEST_EXPECT(LIBSAT_SCANNER_TOKEN_TYPE_SEMICOLON == token);

    /* verify the details. */
    TEST_EXPECT(token == details.type);
    TEST_EXPECT(1 == details.begin_index);
    TEST_EXPECT(1 == details.end_index);
    TEST_EXPECT(1 == details.begin_line);
    TEST_EXPECT(1 == details.end_line);
    TEST_EXPECT(2 == details.begin_col);
    TEST_EXPECT(2 == details.end_col);

    /* read a token from the scanner. */
    token = libsat_scanner_read_token(&details, scanner);

    /* this token should be EOF. */
    TEST_EXPECT(LIBSAT_SCANNER_TOKEN_TYPE_EOF == token);

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

/**
 * We can scan an open parenthesis.
 */
TEST(open_parenthesis)
{
    allocator* alloc;
    libsat_context* context;
    libsat_scanner* scanner;
    libsat_scanner_token details;
    const char* input = R"( ( )";

    /* create malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create context. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_context_create(&context, alloc));

    /* create scanner. */
    TEST_ASSERT(
        STATUS_SUCCESS == libsat_scanner_create(&scanner, context, input));

    /* read a token from the scanner. */
    int token = libsat_scanner_read_token(&details, scanner);

    /* this token should be OPEN_PAREN. */
    TEST_EXPECT(LIBSAT_SCANNER_TOKEN_TYPE_OPEN_PAREN == token);

    /* verify the details. */
    TEST_EXPECT(token == details.type);
    TEST_EXPECT(1 == details.begin_index);
    TEST_EXPECT(1 == details.end_index);
    TEST_EXPECT(1 == details.begin_line);
    TEST_EXPECT(1 == details.end_line);
    TEST_EXPECT(2 == details.begin_col);
    TEST_EXPECT(2 == details.end_col);

    /* read a token from the scanner. */
    token = libsat_scanner_read_token(&details, scanner);

    /* this token should be EOF. */
    TEST_EXPECT(LIBSAT_SCANNER_TOKEN_TYPE_EOF == token);

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

/**
 * We can scan close parenthesis.
 */
TEST(close_parenthesis)
{
    allocator* alloc;
    libsat_context* context;
    libsat_scanner* scanner;
    libsat_scanner_token details;
    const char* input = R"( ) )";

    /* create malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create context. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_context_create(&context, alloc));

    /* create scanner. */
    TEST_ASSERT(
        STATUS_SUCCESS == libsat_scanner_create(&scanner, context, input));

    /* read a token from the scanner. */
    int token = libsat_scanner_read_token(&details, scanner);

    /* this token should be CLOSE_PAREN. */
    TEST_EXPECT(LIBSAT_SCANNER_TOKEN_TYPE_CLOSE_PAREN == token);

    /* verify the details. */
    TEST_EXPECT(token == details.type);
    TEST_EXPECT(1 == details.begin_index);
    TEST_EXPECT(1 == details.end_index);
    TEST_EXPECT(1 == details.begin_line);
    TEST_EXPECT(1 == details.end_line);
    TEST_EXPECT(2 == details.begin_col);
    TEST_EXPECT(2 == details.end_col);

    /* read a token from the scanner. */
    token = libsat_scanner_read_token(&details, scanner);

    /* this token should be EOF. */
    TEST_EXPECT(LIBSAT_SCANNER_TOKEN_TYPE_EOF == token);

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

/**
 * We can scan a true literal.
 */
TEST(true_literal)
{
    allocator* alloc;
    libsat_context* context;
    libsat_scanner* scanner;
    libsat_scanner_token details;
    const char* input = R"( true )";

    /* create malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create context. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_context_create(&context, alloc));

    /* create scanner. */
    TEST_ASSERT(
        STATUS_SUCCESS == libsat_scanner_create(&scanner, context, input));

    /* read a token from the scanner. */
    int token = libsat_scanner_read_token(&details, scanner);

    /* this token should be LITERAL_TRUE. */
    TEST_EXPECT(LIBSAT_SCANNER_TOKEN_TYPE_LITERAL_TRUE == token);

    /* verify the details. */
    TEST_EXPECT(token == details.type);
    TEST_EXPECT(1 == details.begin_index);
    TEST_EXPECT(4 == details.end_index);
    TEST_EXPECT(1 == details.begin_line);
    TEST_EXPECT(1 == details.end_line);
    TEST_EXPECT(2 == details.begin_col);
    TEST_EXPECT(5 == details.end_col);

    /* read a token from the scanner. */
    token = libsat_scanner_read_token(&details, scanner);

    /* this token should be EOF. */
    TEST_EXPECT(LIBSAT_SCANNER_TOKEN_TYPE_EOF == token);

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

/**
 * We can scan a false literal.
 */
TEST(false_literal)
{
    allocator* alloc;
    libsat_context* context;
    libsat_scanner* scanner;
    libsat_scanner_token details;
    const char* input = R"( false )";

    /* create malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create context. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_context_create(&context, alloc));

    /* create scanner. */
    TEST_ASSERT(
        STATUS_SUCCESS == libsat_scanner_create(&scanner, context, input));

    /* read a token from the scanner. */
    int token = libsat_scanner_read_token(&details, scanner);

    /* this token should be LITERAL_FALSE. */
    TEST_EXPECT(LIBSAT_SCANNER_TOKEN_TYPE_LITERAL_FALSE == token);

    /* verify the details. */
    TEST_EXPECT(token == details.type);
    TEST_EXPECT(1 == details.begin_index);
    TEST_EXPECT(5 == details.end_index);
    TEST_EXPECT(1 == details.begin_line);
    TEST_EXPECT(1 == details.end_line);
    TEST_EXPECT(2 == details.begin_col);
    TEST_EXPECT(6 == details.end_col);

    /* read a token from the scanner. */
    token = libsat_scanner_read_token(&details, scanner);

    /* this token should be EOF. */
    TEST_EXPECT(LIBSAT_SCANNER_TOKEN_TYPE_EOF == token);

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

/**
 * We can scan a variable.
 */
TEST(variable)
{
    allocator* alloc;
    libsat_context* context;
    libsat_scanner* scanner;
    libsat_scanner_token details;
    const char* input = R"( x27_3 )";

    /* create malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create context. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_context_create(&context, alloc));

    /* create scanner. */
    TEST_ASSERT(
        STATUS_SUCCESS == libsat_scanner_create(&scanner, context, input));

    /* read a token from the scanner. */
    int token = libsat_scanner_read_token(&details, scanner);

    /* this token should be VARIABLE. */
    TEST_EXPECT(LIBSAT_SCANNER_TOKEN_TYPE_VARIABLE == token);

    /* verify the details. */
    TEST_EXPECT(token == details.type);
    TEST_EXPECT(1 == details.begin_index);
    TEST_EXPECT(5 == details.end_index);
    TEST_EXPECT(1 == details.begin_line);
    TEST_EXPECT(1 == details.end_line);
    TEST_EXPECT(2 == details.begin_col);
    TEST_EXPECT(6 == details.end_col);

    /* read a token from the scanner. */
    token = libsat_scanner_read_token(&details, scanner);

    /* this token should be EOF. */
    TEST_EXPECT(LIBSAT_SCANNER_TOKEN_TYPE_EOF == token);

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

/**
 * We can scan a conjunction.
 */
TEST(conjunction)
{
    allocator* alloc;
    libsat_context* context;
    libsat_scanner* scanner;
    libsat_scanner_token details;
    const char* input = R"( ∧ )";

    /* create malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create context. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_context_create(&context, alloc));

    /* create scanner. */
    TEST_ASSERT(
        STATUS_SUCCESS == libsat_scanner_create(&scanner, context, input));

    /* read a token from the scanner. */
    int token = libsat_scanner_read_token(&details, scanner);

    /* this token should be CONJUNCTION. */
    TEST_EXPECT(LIBSAT_SCANNER_TOKEN_TYPE_CONJUNCTION == token);

    /* verify the details. */
    TEST_EXPECT(token == details.type);
    TEST_EXPECT(1 == details.begin_index);
    TEST_EXPECT(3 == details.end_index);
    TEST_EXPECT(1 == details.begin_line);
    TEST_EXPECT(1 == details.end_line);
    TEST_EXPECT(2 == details.begin_col);
    TEST_EXPECT(2 == details.end_col);

    /* read a token from the scanner. */
    token = libsat_scanner_read_token(&details, scanner);

    /* this token should be EOF. */
    TEST_EXPECT(LIBSAT_SCANNER_TOKEN_TYPE_EOF == token);

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

/**
 * We can scan a disjunction.
 */
TEST(disjunction)
{
    allocator* alloc;
    libsat_context* context;
    libsat_scanner* scanner;
    libsat_scanner_token details;
    const char* input = R"( ∨ )";

    /* create malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create context. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_context_create(&context, alloc));

    /* create scanner. */
    TEST_ASSERT(
        STATUS_SUCCESS == libsat_scanner_create(&scanner, context, input));

    /* read a token from the scanner. */
    int token = libsat_scanner_read_token(&details, scanner);

    /* this token should be DISJUNCTION. */
    TEST_EXPECT(LIBSAT_SCANNER_TOKEN_TYPE_DISJUNCTION == token);

    /* verify the details. */
    TEST_EXPECT(token == details.type);
    TEST_EXPECT(1 == details.begin_index);
    TEST_EXPECT(3 == details.end_index);
    TEST_EXPECT(1 == details.begin_line);
    TEST_EXPECT(1 == details.end_line);
    TEST_EXPECT(2 == details.begin_col);
    TEST_EXPECT(2 == details.end_col);

    /* read a token from the scanner. */
    token = libsat_scanner_read_token(&details, scanner);

    /* this token should be EOF. */
    TEST_EXPECT(LIBSAT_SCANNER_TOKEN_TYPE_EOF == token);

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

/**
 * We can scan an exclusive disjunction.
 */
TEST(exclusive_disjunction)
{
    allocator* alloc;
    libsat_context* context;
    libsat_scanner* scanner;
    libsat_scanner_token details;
    const char* input = R"( ⊻ )";

    /* create malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create context. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_context_create(&context, alloc));

    /* create scanner. */
    TEST_ASSERT(
        STATUS_SUCCESS == libsat_scanner_create(&scanner, context, input));

    /* read a token from the scanner. */
    int token = libsat_scanner_read_token(&details, scanner);

    /* this token should be EXCLUSIVE_DISJUNCTION. */
    TEST_EXPECT(LIBSAT_SCANNER_TOKEN_TYPE_EXCLUSIVE_DISJUNCTION == token);

    /* verify the details. */
    TEST_EXPECT(token == details.type);
    TEST_EXPECT(1 == details.begin_index);
    TEST_EXPECT(3 == details.end_index);
    TEST_EXPECT(1 == details.begin_line);
    TEST_EXPECT(1 == details.end_line);
    TEST_EXPECT(2 == details.begin_col);
    TEST_EXPECT(2 == details.end_col);

    /* read a token from the scanner. */
    token = libsat_scanner_read_token(&details, scanner);

    /* this token should be EOF. */
    TEST_EXPECT(LIBSAT_SCANNER_TOKEN_TYPE_EOF == token);

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

/**
 * We can scan an implication.
 */
TEST(implication)
{
    allocator* alloc;
    libsat_context* context;
    libsat_scanner* scanner;
    libsat_scanner_token details;
    const char* input = R"( → )";

    /* create malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create context. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_context_create(&context, alloc));

    /* create scanner. */
    TEST_ASSERT(
        STATUS_SUCCESS == libsat_scanner_create(&scanner, context, input));

    /* read a token from the scanner. */
    int token = libsat_scanner_read_token(&details, scanner);

    /* this token should be IMPLICATION. */
    TEST_EXPECT(LIBSAT_SCANNER_TOKEN_TYPE_IMPLICATION == token);

    /* verify the details. */
    TEST_EXPECT(token == details.type);
    TEST_EXPECT(1 == details.begin_index);
    TEST_EXPECT(3 == details.end_index);
    TEST_EXPECT(1 == details.begin_line);
    TEST_EXPECT(1 == details.end_line);
    TEST_EXPECT(2 == details.begin_col);
    TEST_EXPECT(2 == details.end_col);

    /* read a token from the scanner. */
    token = libsat_scanner_read_token(&details, scanner);

    /* this token should be EOF. */
    TEST_EXPECT(LIBSAT_SCANNER_TOKEN_TYPE_EOF == token);

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

/**
 * We can scan a biconditional.
 */
TEST(biconditional)
{
    allocator* alloc;
    libsat_context* context;
    libsat_scanner* scanner;
    libsat_scanner_token details;
    const char* input = R"( ↔ )";

    /* create malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create context. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_context_create(&context, alloc));

    /* create scanner. */
    TEST_ASSERT(
        STATUS_SUCCESS == libsat_scanner_create(&scanner, context, input));

    /* read a token from the scanner. */
    int token = libsat_scanner_read_token(&details, scanner);

    /* this token should be BICONDITIONAL. */
    TEST_EXPECT(LIBSAT_SCANNER_TOKEN_TYPE_BICONDITIONAL == token);

    /* verify the details. */
    TEST_EXPECT(token == details.type);
    TEST_EXPECT(1 == details.begin_index);
    TEST_EXPECT(3 == details.end_index);
    TEST_EXPECT(1 == details.begin_line);
    TEST_EXPECT(1 == details.end_line);
    TEST_EXPECT(2 == details.begin_col);
    TEST_EXPECT(2 == details.end_col);

    /* read a token from the scanner. */
    token = libsat_scanner_read_token(&details, scanner);

    /* this token should be EOF. */
    TEST_EXPECT(LIBSAT_SCANNER_TOKEN_TYPE_EOF == token);

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

/**
 * We can scan a negation.
 */
TEST(negation)
{
    allocator* alloc;
    libsat_context* context;
    libsat_scanner* scanner;
    libsat_scanner_token details;
    const char* input = R"( ¬ )";

    /* create malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create context. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_context_create(&context, alloc));

    /* create scanner. */
    TEST_ASSERT(
        STATUS_SUCCESS == libsat_scanner_create(&scanner, context, input));

    /* read a token from the scanner. */
    int token = libsat_scanner_read_token(&details, scanner);

    /* this token should be NEGATION. */
    TEST_EXPECT(LIBSAT_SCANNER_TOKEN_TYPE_NEGATION == token);

    /* verify the details. */
    TEST_EXPECT(token == details.type);
    TEST_EXPECT(1 == details.begin_index);
    TEST_EXPECT(2 == details.end_index);
    TEST_EXPECT(1 == details.begin_line);
    TEST_EXPECT(1 == details.end_line);
    TEST_EXPECT(2 == details.begin_col);
    TEST_EXPECT(2 == details.end_col);

    /* read a token from the scanner. */
    token = libsat_scanner_read_token(&details, scanner);

    /* this token should be EOF. */
    TEST_EXPECT(LIBSAT_SCANNER_TOKEN_TYPE_EOF == token);

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
