/**
 * \file libsat/component.h
 *
 * \brief Define components and component families.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#pragma once

#include <rcpr/component.h>

/* C++ compatibility. */
# ifdef   __cplusplus
extern "C" {
# endif /*__cplusplus*/

/* Largely a formality; 0x06 is reserved for libsat in RCPR. */
#ifdef RCPR_HAS_COMPONENT_FAMILY_LIBSAT
# define LIBSAT_RESERVED_COMPONENT_FAMILY COMPONENT_FAMILY_LIBSAT
#else
# define LIBSAT_RESERVED_COMPONENT_FAMILY 0x06
#endif

/**
 * \brief libsat subcomponents.
 */
enum libsat_subcomponents
{
    /** \brief Base subcomponent. */
    LIBSAT_SUBCOMPONENT_BASE =                                            0x00,

    /** \brief Parser subcomponent. */
    LIBSAT_SUBCOMPONENT_PARSER =                                          0x01,
};

/** \brief Base component scope. */
#define LIBSAT_COMPONENT_BASE \
    COMPONENT_MAKE(LIBSAT_RESERVED_COMPONENT_FAMILY, LIBSAT_SUBCOMPONENT_BASE)

/** \brief Parser component scope. */
#define LIBSAT_COMPONENT_PARSER \
    COMPONENT_MAKE(LIBSAT_RESERVED_COMPONENT_FAMILY, LIBSAT_SUBCOMPONENT_PARSER)

/* C++ compatibility. */
# ifdef   __cplusplus
}
# endif /*__cplusplus*/
