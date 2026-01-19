/**
 * \file libsat/status/base.h
 *
 * \brief base status codes for libsat.
 *
 * \copyright 2025-2026 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#pragma once

#include <libsat/status.h>

/**
 * \brief An incompatible flag setting was encountered.
 */
#define ERROR_LIBSAT_BASE_VARIABLE_GET_INCOMPATIBLE_FLAGS \
    STATUS_CODE(1, LIBSAT_COMPONENT_BASE, 0x0000)
