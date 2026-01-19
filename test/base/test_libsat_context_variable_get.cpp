/**
 * \file base/test_libsat_context_variable_get.cpp
 *
 * \brief Unit tests for libsat_context_variable_get.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <libsat/libsat.h>
#include <libsat/status.h>
#include <minunit/minunit.h>

LIBSAT_IMPORT_base;
RCPR_IMPORT_allocator;
RCPR_IMPORT_resource;

TEST_SUITE(libsat_context_variable_get);

/**
 * CREATE and REF can't be used in the same variable get call.
 */
TEST(CREATE_REF)
{
    allocator* alloc;
    libsat_context* context;
    size_t var_id = 0;

    /* create malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create context. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_context_create(&context, alloc));

    /* CREATE and REF cannot be combined in flags. */
    TEST_ASSERT(
        ERROR_LIBSAT_BASE_VARIABLE_GET_INCOMPATIBLE_FLAGS
            == libsat_context_variable_get(
                    &var_id, context, "x",
                    LIBSAT_VARIABLE_GET_CREATE | LIBSAT_VARIABLE_GET_REF));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(libsat_context_resource_handle(context)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}

/**
 * UNIQUE must be used with CREATE.
 */
TEST(UNIQUE_fails_without_CREATE)
{
    allocator* alloc;
    libsat_context* context;
    size_t var_id = 0;

    /* create malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create context. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_context_create(&context, alloc));

    /* UNIQUE can't be used without CREATE. */
    TEST_ASSERT(
        ERROR_LIBSAT_BASE_VARIABLE_GET_INCOMPATIBLE_FLAGS
            == libsat_context_variable_get(
                    &var_id, context, "x",
                    LIBSAT_VARIABLE_GET_UNIQUE));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(libsat_context_resource_handle(context)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}

/**
 * REF returns an error if the variable doesn't already exist.
 */
TEST(REF_fails_if_variable_not_found)
{
    allocator* alloc;
    libsat_context* context;
    size_t var_id = 0;

    /* create malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create context. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_context_create(&context, alloc));

    /* REF fails, because this variable has not yet been inserted. */
    TEST_ASSERT(
        ERROR_LIBSAT_BASE_VARIABLE_GET_REF_NOT_FOUND
            == libsat_context_variable_get(
                    &var_id, context, "x",
                    LIBSAT_VARIABLE_GET_REF));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(libsat_context_resource_handle(context)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}

/**
 * Base case: a new variable is created.
 */
TEST(base_case)
{
    allocator* alloc;
    libsat_context* context;
    size_t var_id = 100;

    /* create malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create context. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_context_create(&context, alloc));

    /* Insertion of a new variable succeeds. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == libsat_context_variable_get(
                    &var_id, context, "x",
                    LIBSAT_VARIABLE_GET_DEFAULT));

    /* the variable reference is 0. */
    TEST_EXPECT(0 == var_id);

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(libsat_context_resource_handle(context)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}

/**
 * CREATE fails if the variable already exists.
 */
TEST(CREATE_fails_when_variable_already_exists)
{
    allocator* alloc;
    libsat_context* context;
    size_t var_id = 100;

    /* create malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create context. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_context_create(&context, alloc));

    /* Insertion of a new variable succeeds. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == libsat_context_variable_get(
                    &var_id, context, "x",
                    LIBSAT_VARIABLE_GET_DEFAULT));

    /* Attempting to create this variable a second time fails. */
    TEST_ASSERT(
        ERROR_LIBSAT_BASE_VARIABLE_GET_CREATE_ALREADY_EXISTS
            == libsat_context_variable_get(
                    &var_id, context, "x",
                    LIBSAT_VARIABLE_GET_CREATE));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(libsat_context_resource_handle(context)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}

/**
 * REF succeeds after a variable has been created.
 */
TEST(REF_after_insert)
{
    allocator* alloc;
    libsat_context* context;
    size_t var_id = 100;

    /* create malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == malloc_allocator_create(&alloc));

    /* create context. */
    TEST_ASSERT(STATUS_SUCCESS == libsat_context_create(&context, alloc));

    /* Insertion of a new variable succeeds. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == libsat_context_variable_get(
                    &var_id, context, "x",
                    LIBSAT_VARIABLE_GET_DEFAULT));

    /* REF now succeeds. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == libsat_context_variable_get(
                    &var_id, context, "x",
                    LIBSAT_VARIABLE_GET_REF));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(libsat_context_resource_handle(context)));
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(allocator_resource_handle(alloc)));
}
