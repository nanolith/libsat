/**
 * \file libsat/function_decl.h
 *
 * \brief Function declaration macros for libsat.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#pragma once

#include <libsat/config.h>
#include <libsat/macro_tricks.h>

/*
 * With GCC 3.4 and clang, we can force a warning / error if the return value
 * of a function isn't checked.
 */
#if defined(__GNUC__) && defined(__GNUC_MINOR__)
# if (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || (__GNUC__ > 3)
#  define FN_DECL_MUST_CHECK __attribute__((warn_unused_result))
# endif
#endif

/*
 * For other compilers, skip this check.
 */
#ifndef FN_DECL_MUST_CHECK
# define FN_DECL_MUST_CHECK
#endif

/*
 * The LIBSAT_UNIQUE_NAME is a UUID-based symbol.
 */
#define LIBSAT_UNIQUE_NAME 52ec1971_5c81_4ad9_b86d_a788e5770163

/*
 * Simple concat macro.
 */
#define LIBSAT_CONCAT(x,y) x ## y

/*
 * Symbol expansion and combination macro.
 */
#define LIBSAT_SYM_COMBINE(x, y, z) libsat ## _ ## x ## _ ## y ## _ ## z
#define LIBSAT_SYM_COMBINE1(x, y, z) LIBSAT_SYM_COMBINE(x, y, z)

/*
 * The LIBSAT_SYM macro elevates a given symbol to the LIBSAT namespace.
 */
#define LIBSAT_SYM(sym) LIBSAT_SYM_COMBINE1(LIBSAT_UNIQUE_NAME, LIBSAT_VERSION_SYM, sym)

/**
 * Begin an export section.
 */
#define LIBSAT_BEGIN_EXPORT \
    _Pragma("GCC diagnostic push") \
    _Pragma("GCC diagnostic ignored \"-Wunused-function\"")

/**
 * End an export section.
 */
#define LIBSAT_END_EXPORT \
    _Pragma("GCC diagnostic pop")
