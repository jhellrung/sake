/*******************************************************************************
 * sake/core/utility/debug.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_DEBUG_HPP
#define SAKE_CORE_UTILITY_DEBUG_HPP

#if defined( SAKE_DEBUG ) && defined( SAKE_NDEBUG )

#error Must not define both SAKE_DEBUG and SAKE_NDEBUG

#else // #if defined( SAKE_DEBUG ) && defined( SAKE_NDEBUG )

#if !defined( SAKE_DEBUG ) && !defined( SAKE_NDEBUG )
#ifdef NDEBUG
#define SAKE_NDEBUG
#else // #ifdef NDEBUG
#define SAKE_DEBUG
#endif // #ifdef NDEBUG
#endif // #if !defined( SAKE_DEBUG ) && !defined( SAKE_NDEBUG )

#endif // #if defined( SAKE_DEBUG ) && defined( SAKE_NDEBUG )

#endif // #ifndef SAKE_CORE_UTILITY_DEBUG_HPP
