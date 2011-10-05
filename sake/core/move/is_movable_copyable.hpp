/*******************************************************************************
 * sake/core/move/is_movable_copyable.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * [only for C++03]
 * struct is_movable_copyable<T>
 *
 * This is a metafunction evaluating to true only if T is both movable and
 * copyable.
 ******************************************************************************/

#ifndef SAKE_CORE_MOVE_IS_MOVABLE_COPYABLE_HPP
#define SAKE_CORE_MOVE_IS_MOVABLE_COPYABLE_HPP

#include <boost/config.hpp>

#ifdef BOOST_NO_RVALUE_REFERENCES

#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/move/is_movable.hpp>
#include <sake/core/move/rv.hpp>

namespace sake
{

template< class T >
struct is_movable_copyable
    : boost_ext::mpl::and2<
          sake::is_movable<T>,
          boost_ext::is_convertible< T const &, boost::rv<T> const & >
      >
{ };

template< class T >
struct is_movable_copyable< T& >
    : boost::false_type
{ };

} // namespace sake

#endif // #ifdef BOOST_NO_RVALUE_REFERENCES

#endif // #ifndef SAKE_CORE_MOVE_IS_MOVABLE_COPYABLE_HPP
