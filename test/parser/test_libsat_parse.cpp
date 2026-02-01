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

TEST_SUITE(libsat_parse);

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

/**
 * Parse a variable as a simple statement.
 */
TEST(simple_variable_statement)
{
    allocator* alloc;
    libsat_context* context;
    libsat_ast_node* base = nullptr;
    libsat_ast_node* node = nullptr;
    const char* input = R"(x)";

    /* create malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create context. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_context_create(&context, alloc));

    /* Parse should succeed. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_parse(&base, context, input));

    /* the node should not be NULL and should be a statement. */
    node = base;
    TEST_ASSERT(nullptr != node);
    TEST_ASSERT(LIBSAT_PARSER_AST_NODE_TYPE_STATEMENT == node->type);
    TEST_ASSERT(NULL != node->value.unary);

    /* The child node should be a variable. */
    node = node->value.unary;
    TEST_ASSERT(LIBSAT_PARSER_AST_NODE_TYPE_VARIABLE == node->type);
    TEST_EXPECT(0 == node->value.variable_index);

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(libsat_ast_node_resource_handle(base)));
    TEST_ASSERT(
        STATUS_SUCCESS ==
            resource_release(libsat_context_resource_handle(context)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Parse a "not" variable statement.
 */
TEST(not_variable_statement)
{
    allocator* alloc;
    libsat_context* context;
    libsat_ast_node* base = nullptr;
    libsat_ast_node* node = nullptr;
    const char* input = R"(¬x)";

    /* create malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create context. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_context_create(&context, alloc));

    /* Parse should succeed. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_parse(&base, context, input));

    /* the node should not be NULL and should be a statement. */
    node = base;
    TEST_ASSERT(nullptr != node);
    TEST_ASSERT(LIBSAT_PARSER_AST_NODE_TYPE_STATEMENT == node->type);
    TEST_ASSERT(NULL != node->value.unary);

    /* The statement child node should be a negation. */
    node = node->value.unary;
    TEST_ASSERT(LIBSAT_PARSER_AST_NODE_TYPE_NEGATION == node->type);
    TEST_ASSERT(NULL != node->value.unary);

    /* The negation child node should be a variable. */
    node = node->value.unary;
    TEST_ASSERT(LIBSAT_PARSER_AST_NODE_TYPE_VARIABLE == node->type);
    TEST_EXPECT(0 == node->value.variable_index);

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(libsat_ast_node_resource_handle(base)));
    TEST_ASSERT(
        STATUS_SUCCESS ==
            resource_release(libsat_context_resource_handle(context)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Parse a simple conjunction statement.
 */
TEST(simple_conjunction)
{
    allocator* alloc;
    libsat_context* context;
    libsat_ast_node* base = nullptr;
    libsat_ast_node* node = nullptr;
    libsat_ast_node* conjunction = nullptr;
    const char* input = R"(x∧y)";

    /* create malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create context. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_context_create(&context, alloc));

    /* Parse should succeed. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_parse(&base, context, input));

    /* the node should not be NULL and should be a statement. */
    node = base;
    TEST_ASSERT(nullptr != node);
    TEST_ASSERT(LIBSAT_PARSER_AST_NODE_TYPE_STATEMENT == node->type);
    TEST_ASSERT(NULL != node->value.unary);

    /* The statement child node should be a conjunction. */
    conjunction = node->value.unary;
    TEST_ASSERT(LIBSAT_PARSER_AST_NODE_TYPE_CONJUNCTION == conjunction->type);
    TEST_ASSERT(NULL != conjunction->value.binary.lhs);
    TEST_ASSERT(NULL != conjunction->value.binary.rhs);

    /* The left-hand side of the conjunction should be a variable. */
    node = conjunction->value.binary.lhs;
    TEST_ASSERT(LIBSAT_PARSER_AST_NODE_TYPE_VARIABLE == node->type);
    TEST_EXPECT(0 == node->value.variable_index);

    /* The right-hand side of the conjunction should be a variable. */
    node = conjunction->value.binary.rhs;
    TEST_ASSERT(LIBSAT_PARSER_AST_NODE_TYPE_VARIABLE == node->type);
    TEST_EXPECT(1 == node->value.variable_index);

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(libsat_ast_node_resource_handle(base)));
    TEST_ASSERT(
        STATUS_SUCCESS ==
            resource_release(libsat_context_resource_handle(context)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Parse a simple disjunction statement.
 */
TEST(simple_disjunction)
{
    allocator* alloc;
    libsat_context* context;
    libsat_ast_node* base = nullptr;
    libsat_ast_node* node = nullptr;
    libsat_ast_node* disjunction = nullptr;
    const char* input = R"(x∨y)";

    /* create malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create context. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_context_create(&context, alloc));

    /* Parse should succeed. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_parse(&base, context, input));

    /* the node should not be NULL and should be a statement. */
    node = base;
    TEST_ASSERT(nullptr != node);
    TEST_ASSERT(LIBSAT_PARSER_AST_NODE_TYPE_STATEMENT == node->type);
    TEST_ASSERT(NULL != node->value.unary);

    /* The statement child node should be a disjunction. */
    disjunction = node->value.unary;
    TEST_ASSERT(LIBSAT_PARSER_AST_NODE_TYPE_DISJUNCTION == disjunction->type);
    TEST_ASSERT(NULL != disjunction->value.binary.lhs);
    TEST_ASSERT(NULL != disjunction->value.binary.rhs);

    /* The left-hand side of the disjunction should be a variable. */
    node = disjunction->value.binary.lhs;
    TEST_ASSERT(LIBSAT_PARSER_AST_NODE_TYPE_VARIABLE == node->type);
    TEST_EXPECT(0 == node->value.variable_index);

    /* The right-hand side of the disjunction should be a variable. */
    node = disjunction->value.binary.rhs;
    TEST_ASSERT(LIBSAT_PARSER_AST_NODE_TYPE_VARIABLE == node->type);
    TEST_EXPECT(1 == node->value.variable_index);

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(libsat_ast_node_resource_handle(base)));
    TEST_ASSERT(
        STATUS_SUCCESS ==
            resource_release(libsat_context_resource_handle(context)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}

/**
 * A conjunction binds more tightly than a disjunction.
 */
TEST(disjunction_conjunction)
{
    allocator* alloc;
    libsat_context* context;
    libsat_ast_node* base = nullptr;
    libsat_ast_node* node = nullptr;
    libsat_ast_node* disjunction = nullptr;
    libsat_ast_node* conjunction = nullptr;
    const char* input = R"(x∨y∧z)";

    /* create malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create context. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_context_create(&context, alloc));

    /* Parse should succeed. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_parse(&base, context, input));

    /* the node should not be NULL and should be a statement. */
    node = base;
    TEST_ASSERT(nullptr != node);
    TEST_ASSERT(LIBSAT_PARSER_AST_NODE_TYPE_STATEMENT == node->type);
    TEST_ASSERT(nullptr != node->value.unary);

    /* The statement child node should be a disjunction. */
    disjunction = node->value.unary;
    TEST_ASSERT(LIBSAT_PARSER_AST_NODE_TYPE_DISJUNCTION == disjunction->type);
    TEST_ASSERT(nullptr != disjunction->value.binary.lhs);
    TEST_ASSERT(nullptr != disjunction->value.binary.rhs);

    /* The left-hand side of the disjunction should be a variable. */
    node = disjunction->value.binary.lhs;
    TEST_ASSERT(LIBSAT_PARSER_AST_NODE_TYPE_VARIABLE == node->type);
    TEST_EXPECT(0 == node->value.variable_index);

    /* The right-hand side of the disjunction should be a conjunction. */
    conjunction = disjunction->value.binary.rhs;
    TEST_ASSERT(LIBSAT_PARSER_AST_NODE_TYPE_CONJUNCTION == conjunction->type);
    TEST_ASSERT(nullptr != conjunction->value.binary.lhs);
    TEST_ASSERT(nullptr != conjunction->value.binary.rhs);

    /* The left-hand side of the conjunction should be a variable. */
    node = conjunction->value.binary.lhs;
    TEST_ASSERT(LIBSAT_PARSER_AST_NODE_TYPE_VARIABLE == node->type);
    TEST_EXPECT(1 == node->value.variable_index);

    /* The right-hand side of the conjunction should be a variable. */
    node = conjunction->value.binary.rhs;
    TEST_ASSERT(LIBSAT_PARSER_AST_NODE_TYPE_VARIABLE == node->type);
    TEST_EXPECT(2 == node->value.variable_index);

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(libsat_ast_node_resource_handle(base)));
    TEST_ASSERT(
        STATUS_SUCCESS ==
            resource_release(libsat_context_resource_handle(context)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}

/**
 * A conjunction binds more tightly than a disjunction.
 */
TEST(conjunction_disjunction)
{
    allocator* alloc;
    libsat_context* context;
    libsat_ast_node* base = nullptr;
    libsat_ast_node* node = nullptr;
    libsat_ast_node* disjunction = nullptr;
    libsat_ast_node* conjunction = nullptr;
    const char* input = R"(x∧y∨z)";

    /* create malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create context. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_context_create(&context, alloc));

    /* Parse should succeed. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_parse(&base, context, input));

    /* the node should not be NULL and should be a statement. */
    node = base;
    TEST_ASSERT(nullptr != node);
    TEST_ASSERT(LIBSAT_PARSER_AST_NODE_TYPE_STATEMENT == node->type);
    TEST_ASSERT(nullptr != node->value.unary);

    /* The statement child node should be a disjunction. */
    disjunction = node->value.unary;
    TEST_ASSERT(LIBSAT_PARSER_AST_NODE_TYPE_DISJUNCTION == disjunction->type);
    TEST_ASSERT(nullptr != disjunction->value.binary.lhs);
    TEST_ASSERT(nullptr != disjunction->value.binary.rhs);

    /* The left-hand side of the disjunction should be a conjunction. */
    conjunction = disjunction->value.binary.lhs;
    TEST_ASSERT(LIBSAT_PARSER_AST_NODE_TYPE_CONJUNCTION == conjunction->type);
    TEST_ASSERT(nullptr != conjunction->value.binary.lhs);
    TEST_ASSERT(nullptr != conjunction->value.binary.rhs);

    /* The left-hand side of the conjunction should be a variable. */
    node = conjunction->value.binary.lhs;
    TEST_ASSERT(LIBSAT_PARSER_AST_NODE_TYPE_VARIABLE == node->type);
    TEST_EXPECT(0 == node->value.variable_index);

    /* The right-hand side of the conjunction should be a variable. */
    node = conjunction->value.binary.rhs;
    TEST_ASSERT(LIBSAT_PARSER_AST_NODE_TYPE_VARIABLE == node->type);
    TEST_EXPECT(1 == node->value.variable_index);

    /* The right-hand side of the disjunction should be a variable. */
    node = disjunction->value.binary.rhs;
    TEST_ASSERT(LIBSAT_PARSER_AST_NODE_TYPE_VARIABLE == node->type);
    TEST_EXPECT(2 == node->value.variable_index);

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(libsat_ast_node_resource_handle(base)));
    TEST_ASSERT(
        STATUS_SUCCESS ==
            resource_release(libsat_context_resource_handle(context)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}
