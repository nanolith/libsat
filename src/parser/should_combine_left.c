/**
 * \file parser/should_combine_left.c
 *
 * \brief Given two operators, determine whether terms should combine left.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <libsat/libsat.h>

#include "parser_internal.h"

LIBSAT_IMPORT_base;

/* forward decls. */
static int get_associativity(int token);
static int get_priority(int token);

/**
 * \brief Given two operator tokens, return true if the parser should combine to
 * the left, and false otherwise.
 *
 * \param left          The operator token to the left.
 * \param right         The operator token to the right.
 *
 * \returns true if the parser should combine to the left and false otherwise.
 */
bool LIBSAT_SYM(should_combine_left)(int left, int right)
{
    int left_assoc = get_associativity(left);
    int left_priority = get_priority(left);
    int right_priority = get_priority(right);

    /* if left is a higher priority than right, then combine to the left. */
    if (left_priority < right_priority)
    {
        return true;
    }
    /* if left associates left and is of equal priority, combine left. */
    if (left_priority == right_priority && LIBSAT_ASSOC_LEFT == left_assoc)
    {
        return true;
    }
    /* if right priority is greater, or if priorities are equal and left is
     * right associative, combine to the right. */
    else
    {
        return false;
    }
}

/**
 * \brief Get the associativity of the given operator token.
 *
 * \param token             The token to decode.
 *
 * \returns the associativity of this token.
 */
static int get_associativity(int token)
{
    switch (token)
    {
        case LIBSAT_SCANNER_TOKEN_TYPE_NEGATION:
        case LIBSAT_SCANNER_TOKEN_TYPE_IMPLICATION:
            return LIBSAT_ASSOC_RIGHT;

        case LIBSAT_SCANNER_TOKEN_TYPE_CONJUNCTION:
        case LIBSAT_SCANNER_TOKEN_TYPE_EXCLUSIVE_DISJUNCTION:
        case LIBSAT_SCANNER_TOKEN_TYPE_DISJUNCTION:
        case LIBSAT_SCANNER_TOKEN_TYPE_BICONDITIONAL:
            return LIBSAT_ASSOC_LEFT;

        /* TODO - this should trigger an assertion failure. */
        default:
            return LIBSAT_ASSOC_LEFT;
    }
}

/**
 * \brief Get the priority of the given operator token.
 *
 * \param token             The token to decode.
 *
 * \returns the priority of this operator.
 */
static int get_priority(int token)
{
    switch (token)
    {
        case LIBSAT_SCANNER_TOKEN_TYPE_NEGATION:
            return LIBSAT_OPERATOR_PRIORITY_NEGATION;

        case LIBSAT_SCANNER_TOKEN_TYPE_CONJUNCTION:
            return LIBSAT_OPERATOR_PRIORITY_CONJUNCTION;

        case LIBSAT_SCANNER_TOKEN_TYPE_EXCLUSIVE_DISJUNCTION:
            return LIBSAT_OPERATOR_PRIORITY_EXCLUSIVE_DISJUNCITON;

        case LIBSAT_SCANNER_TOKEN_TYPE_DISJUNCTION:
            return LIBSAT_OPERATOR_PRIORITY_DISJUNCITON;

        case LIBSAT_SCANNER_TOKEN_TYPE_IMPLICATION:
            return LIBSAT_OPERATOR_PRIORITY_IMPLICATION;

        case LIBSAT_SCANNER_TOKEN_TYPE_BICONDITIONAL:
            return LIBSAT_OPERATOR_PRIORITY_BICONDITIONAL;

        default:
            /* TODO - this should trigger an assertion failure. */
            return 100;
    }
}
