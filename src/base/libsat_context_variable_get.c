/**
 * \file base/libsat_context_variable_get.c
 *
 * \brief Get or create a variable in the context.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <libsat/libsat.h>
#include <libsat/status.h>

#include "libsat_base_internal.h"

LIBSAT_IMPORT_base_internal;
RCPR_IMPORT_rbtree;
RCPR_IMPORT_resource;

/* forward decls. */
static status check_flags(int flags);

/**
 * \brief Get or create a variable by name.
 *
 * \param var_id        Pointer to the variable id to be set on success.
 * \param context       The context for this operation.
 * \param var_name      The name of the variable.
 * \param flags         The flags to use for this get operation.
 *                      \see libsat_context_variable_get_flags.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
LIBSAT_SYM(libsat_context_variable_get)(
    size_t* var_id, LIBSAT_SYM(libsat_context)* context, const char* var_name,
    int flags)
{
    status retval, release_retval;
    intern_entry* tmp = NULL;

    /* verify that the flags are sane. */
    retval = check_flags(flags);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* if this is a unique variable, just increment the variable count and set
     * the id. */
    if (flags & LIBSAT_VARIABLE_GET_UNIQUE)
    {
        *var_id = context->variable_count;
        context->variable_count += 1;

        retval = STATUS_SUCCESS;
        goto done;
    }

    /* look up the variable by name. */
    retval = rbtree_find((resource**)&tmp, context->string_to_intern, var_name);

    /* decode the returned value. */
    switch (retval)
    {
        case STATUS_SUCCESS:
            /* if the reference is found, but this is a CREATE, that is an */
            /* error. */
            if (flags & LIBSAT_VARIABLE_GET_CREATE)
            {
                retval = ERROR_LIBSAT_BASE_VARIABLE_GET_CREATE_ALREADY_EXISTS;
                goto done;
            }

            /* otherwise, return the variable reference. */
            *var_id = tmp->string_index;
            goto done;

        case ERROR_RBTREE_NOT_FOUND:
            /* if this entry was not found, but this is a REF request, return */
            /* an error. */
            if (flags & LIBSAT_VARIABLE_GET_REF)
            {
                retval = ERROR_LIBSAT_BASE_VARIABLE_GET_REF_NOT_FOUND;
                goto done;
            }

            /* fall into the base case at the end of the function. */
            break;

        /* some other error occurred. */
        default:
            goto done;
    }

    /* create an intern entry. */
    retval =
        intern_entry_create(
            &tmp, context->alloc, var_name, context->variable_count);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* insert this entry into the string_to_intern tree. */
    retval = rbtree_insert(context->string_to_intern, &tmp->hdr);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_tmp;
    }

    /* increment the reference on tmp so we can insert it again. */
    tmp->ref_count += 1;

    /* insert this entry into the intern_to_string tree. */
    retval = rbtree_insert(context->intern_to_string, &tmp->hdr);
    if (STATUS_SUCCESS != retval)
    {
        goto delete_string_mapping;
    }

    /* success: increment variable count and return the new variable ref. */
    *var_id = tmp->string_index;
    context->variable_count += 1;
    retval = STATUS_SUCCESS;
    goto done;

delete_string_mapping:
    release_retval = rbtree_delete(NULL, context->string_to_intern, var_name);
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

cleanup_tmp:
    release_retval = resource_release(&tmp->hdr);
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

done:
    return retval;
}

/**
 * \brief Check that the flag combinations are sane.
 */
static status check_flags(int flags)
{
    /* CREATE and REF are incompatible. */
    if (   (flags & LIBSAT_VARIABLE_GET_CREATE)
        && (flags & LIBSAT_VARIABLE_GET_REF))
    {
        return ERROR_LIBSAT_BASE_VARIABLE_GET_INCOMPATIBLE_FLAGS;
    }

    /* UNIQUE requires CREATE. */
    if (    (flags & LIBSAT_VARIABLE_GET_UNIQUE)
        && !(flags & LIBSAT_VARIABLE_GET_CREATE))
    {
        return ERROR_LIBSAT_BASE_VARIABLE_GET_INCOMPATIBLE_FLAGS;
    }

    return STATUS_SUCCESS;
}
