/*******************************************************************************
 * sake/core/move/is_movable.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * [only for C++03]
 * struct is_movable<T>
 *
 * is_movable is a Boost.MPL metafunction that determines if the given type has
 * move emulation enabled.
 ******************************************************************************/

#ifndef SAKE_CORE_MOVE_IS_MOVABLE_HPP
#define SAKE_CORE_MOVE_IS_MOVABLE_HPP

#include <boost/config.hpp>

#ifdef BOOST_NO_RVALUE_REFERENCES

#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_class.hpp>

#include <sake/boost_ext/mpl/and.hpp>

#include <sake/core/introspection/has_mem_fun_operator.hpp>
#include <sake/core/move/rv.hpp>

namespace sake
{

template< class T >
struct is_movable
    : boost_ext::mpl::and2<
          boost::is_class<T>,
          sake::has_mem_fun_operator< T, boost::rv<T>& >
      >
{ };

template< class T >
struct is_movable< T const >
    : boost::false_type
{ };

} // namespace sake

#endif // #ifdef BOOST_NO_RVALUE_REFERENCES

#endif // #ifndef SAKE_CORE_MOVE_IS_MOVABLE_HPP
