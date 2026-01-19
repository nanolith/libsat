/**
 * \file libsat/scanner.h
 *
 * \brief Scanner for libsat.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#pragma once

#include <libsat/function_decl.h>
#include <libsat/libsat_fwd.h>
#include <rcpr/allocator.h>
#include <rcpr/resource.h>
#include <rcpr/resource/protected.h>

/* C++ compatibility. */
# ifdef   __cplusplus
extern "C" {
# endif /*__cplusplus*/

/**
 * \brief Token types.
 */
enum LIBSAT_SYM(libsat_scanner_token_type)
{
    /** \brief A variable. */
    LIBSAT_SCANNER_TOKEN_TYPE_VARIABLE,

    /** \brief Open parenthesis. */
    LIBSAT_SCANNER_TOKEN_TYPE_OPEN_PAREN,

    /** \brief Close parenthesis. */
    LIBSAT_SCANNER_TOKEN_TYPE_CLOSE_PAREN,

    /** \brief Semicolon ends an expression. */
    LIBSAT_SCANNER_TOKEN_TYPE_SEMICOLON,
};

/**
 * \brief Token details.
 */
typedef struct LIBSAT_SYM(libsat_scanner_token)
LIBSAT_SYM(libsat_scanner_token);
struct LIBSAT_SYM(libsat_scanner_token)
{
    int type;
    size_t begin_index;
    size_t end_index;
    size_t begin_line;
    size_t begin_col;
    size_t end_line;
    size_t end_col;
    union
    {
        size_t istring;
        uint64_t u64;
        int64_t i64;
        bool boolean;
    } value;
};

/**
 * \brief A scanner capable of generating tokens based on input strings for
 * creating propositional logic statements.
 */
typedef struct LIBSAT_SYM(libsat_scanner) LIBSAT_SYM(libsat_scanner);

/******************************************************************************/
/* Start of model checking properties.                                        */
/******************************************************************************/

/**
 * \brief Valid scanner property.
 *
 * \param scanner       The scanner instance to be verified.
 *
 * \returns true if the scanner instance is valid.
 */
bool
LIBSAT_SYM(prop_scanner_valid)(
    const LIBSAT_SYM(libsat_scanner)* scanner);

/******************************************************************************/
/* Start of constructors.                                                     */
/******************************************************************************/

/**
 * \brief Create a scanner instance.
 *
 * \param scanner       Pointer to the scanner pointer to be set to this created
 *                      scanner instance on success.
 * \param context       The context for this operation.
 * \param input         The input string to scan.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status FN_DECL_MUST_CHECK
LIBSAT_SYM(libsat_scanner_create)(
    LIBSAT_SYM(libsat_scanner)** scanner, LIBSAT_SYM(libsat_context)* context,
    const char* input);

/******************************************************************************/
/* Start of public methods.                                                   */
/******************************************************************************/

/**
 * \brief Given a \ref libsat_scanner instance, return the resource handle for
 * this instance.
 *
 * \param scanner      The \ref libsat_scanner instance from which the resource
 *                      handle is returned.
 *
 * \returns the resource handle for this scanner instance.
 */
RCPR_SYM(resource)*
LIBSAT_SYM(libsat_scanner_resource_handle)(
    LIBSAT_SYM(libsat_scanner)* scanner);

/******************************************************************************/
/* Start of public exports.                                                   */
/******************************************************************************/
#define __INTERNAL_LIBSAT_IMPORT_scanner_sym(sym) \
    LIBSAT_BEGIN_EXPORT \
    typedef LIBSAT_SYM(libsat_scanner) sym ## libsat_scanner; \
    static inline status FN_DECL_MUST_CHECK sym ## libsat_scanner_create( \
        LIBSAT_SYM(libsat_scanner)** x, LIBSAT_SYM(libsat_context)* y, \
        const char* z) { \
            return LIBSAT_SYM(libsat_scanner_create)(x,y,z); } \
    static inline RCPR_SYM(resource)* \
    sym ## libsat_scanner_resource_handle( \
        LIBSAT_SYM(libsat_scanner)* x) { \
            return LIBSAT_SYM(libsat_scanner_resource_handle)(x); } \
    LIBSAT_END_EXPORT \
    REQUIRE_SEMICOLON_HERE
#define LIBSAT_IMPORT_scanner_as(sym) \
    __INTERNAL_LIBSAT_IMPORT_scanner_sym(sym ## _)
#define LIBSAT_IMPORT_scanner \
    __INTERNAL_LIBSAT_IMPORT_scanner_sym()

/* C++ compatibility. */
# ifdef   __cplusplus
}
# endif /*__cplusplus*/
