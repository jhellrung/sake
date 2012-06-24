/*******************************************************************************
 * sake/core/range/debug.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_DEBUG_HPP
#define SAKE_CORE_RANGE_DEBUG_HPP

#include <sake/core/utility/debug.hpp>

#if defined( SAKE_RANGE_DEBUG ) && defined( SAKE_RANGE_NDEBUG )

#error Must not define both SAKE_RANGE_DEBUG and SAKE_RANGE_NDEBUG

#else // #if defined( SAKE_RANGE_DEBUG ) && defined( SAKE_RANGE_NDEBUG )

#if !defined( SAKE_RANGE_DEBUG ) && !defined( SAKE_RANGE_NDEBUG )
#ifdef SAKE_DEBUG
#define SAKE_RANGE_DEBUG
#else // #ifdef SAKE_DEBUG
#define SAKE_RANGE_NDEBUG
#endif // #ifdef SAKE_DEBUG
#endif // #if !defined( SAKE_RANGE_DEBUG ) && !defined( SAKE_RANGE_NDEBUG )

#endif // #if defined( SAKE_RANGE_DEBUG ) && defined( SAKE_RANGE_NDEBUG )

#endif // #ifndef SAKE_CORE_RANGE_DEBUG_HPP
