/*******************************************************************************
 * sake/core/move/has_move_emulation_and_copyable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * [only for C++03]
 * struct has_move_emulation_and_copyable<T>
 *
 * This is a metafunction evaluating to true only if T is both movable and
 * copyable.
 ******************************************************************************/

#ifndef SAKE_CORE_MOVE_HAS_MOVE_EMULATION_AND_COPYABLE_HPP
#define SAKE_CORE_MOVE_HAS_MOVE_EMULATION_AND_COPYABLE_HPP

#include <boost/config.hpp>

#ifdef BOOST_NO_RVALUE_REFERENCES

#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/config.hpp>
#include <sake/core/introspection/has_mem_fun_operator.hpp>
#include <sake/core/move/has_move_emulation.hpp>
#include <sake/core/move/rv.hpp>

namespace sake
{

template< class T >
struct has_move_emulation_and_copyable
    : boost_ext::mpl::and2<
          sake::has_move_emulation<T>,
#if SAKE_MSC_VERSION <= 1500 \
 || SAKE_GNUC_VERSION <= SAKE_GNUC_VERSION_OF(4,6,3)
          boost_ext::is_convertible< T const &, boost::rv<T> const & >
#else // #if XXX_VERSION <= ...
          sake::has_mem_fun_operator< T const, boost::rv<T> const & >
#endif // #if XXX_VERSION <= ...
      >
{ };

template< class T >
struct has_move_emulation_and_copyable< T& >
    : boost::false_type
{ };

} // namespace sake

#endif // #ifdef BOOST_NO_RVALUE_REFERENCES

#endif // #ifndef SAKE_CORE_MOVE_HAS_MOVE_EMULATION_AND_COPYABLE_HPP
