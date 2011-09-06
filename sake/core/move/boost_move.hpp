/*******************************************************************************
 * core/move/boost_move.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost::rv<T>
 *
 * This defines boost::rv.  Ideally, this will just directly include the
 * appropriate boost header file, but if the boost version is too old, it will
 * attempt to define it otherwise.
 ******************************************************************************/

#ifndef SAKE_CORE_MOVE_BOOST_MOVE_HPP
#define SAKE_CORE_MOVE_BOOST_MOVE_HPP

#include <boost/config.hpp>

#if BOOST_VERSION >= 104800

#include <boost/move/move.hpp>

#else // #if BOOST_VERSION >= 104800

#ifdef SAKE_MOVE_USE_BOOST_SANDBOX_MOVE

#include <boost-sandbox/move/boost/move/move.hpp>

#else // #ifdef SAKE_MOVE_USE_BOOST_SANDBOX_MOVE

#ifndef BOOST_MOVE_MOVE_HPP

namespace boost
{

template< class T >
class rv : public T
{
   rv();
   ~rv();
   rv(const rv&);
   void operator=(const rv&);
}
#ifdef __GNUC__
__attribute__ (( __may_alias__ ))
#endif // #ifdef __GNUC__
;

} // namespace boost

#endif // #ifndef BOOST_MOVE_MOVE_HPP

#endif // #ifdef SAKE_MOVE_USE_BOOST_SANDBOX_MOVE

#endif // #if BOOST_VERSION >= 104800

#endif // #ifndef SAKE_CORE_MOVE_BOOST_MOVE_HPP
