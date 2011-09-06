/*******************************************************************************
 * boost_ext/type_traits/propagate_qualifiers.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * boost_ext::propagate_qualifiers< From, To >
 *
 * propagate_qualifiers< From         , To > -> To
 * propagate_qualifiers< From const   , To > -> propagate_qualifiers< From, To > const
 * propagate_qualifiers< From volatile, To > -> propagate_qualifiers< From, To > volatile
 * propagate_qualifiers< From &       , To > -> propagate_qualifiers< From, To > &
 * propagate_qualifiers< From &&      , To > -> propagate_qualifiers< From, To > &&
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_PROPAGATE_QUALIFIERS_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_PROPAGATE_QUALIFIERS_HPP

#include <boost/config.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_cv.hpp>
#include <boost/type_traits/add_volatile.hpp>

#include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>

#include <sake/core/rv.hpp>

namespace sake
{

namespace boost_ext
{

template< class From, class To >
struct propagate_qualifiers
{ typedef To type; };

template< class From, class To >
struct propagate_qualifiers< From const , To >
    : boost::add_const<
          typename propagate_qualifiers< From, To >::type
      >
{ };

template< class From, class To >
struct propagate_qualifiers< From volatile , To >
    : boost::add_volatile<
          typename propagate_qualifiers< From, To >::type
      >
{ };

template< class From, class To >
struct propagate_qualifiers< From const volatile, To >
    : boost::add_cv<
          typename propagate_qualifiers< From, To >::type
      >
{ };

template< class From, class To >
struct propagate_qualifiers< From &, To >
    : boost_ext::add_reference<
          typename propagate_qualifiers< From, To >::type
      >
{ };

#ifndef BOOST_NO_RVALUE_REFERENCES

template< class From, class To >
struct propagate_qualifiers< From &&, To >
    : boost_ext::add_rvalue_reference<
          typename propagate_qualifiers< From, To >::type
      >
{ };

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

template< class From, class To >
struct propagate_qualifiers< boost::rv< From > &, To >
    : boost_ext::add_rvalue_reference<
          typename propagate_qualifiers< From, To >::type
      >
{ };

template< class From, class To >
struct propagate_qualifiers< boost::rv< From > const &, To >
    : boost_ext::add_rvalue_reference<
          typename propagate_qualifiers< From const, To >::type
      >
{ };

template< class From, class To >
struct propagate_qualifiers< boost::rv< From > volatile &, To >
    : boost_ext::add_rvalue_reference<
          typename propagate_qualifiers< From volatile, To >::type
      >
{ };

template< class From, class To >
struct propagate_qualifiers< boost::rv< From > const volatile &, To >
    : boost_ext::add_rvalue_reference<
          typename propagate_qualifiers< From const volatile, To >::type
      >
{ };

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_PROPAGATE_QUALIFIERS_HPP
