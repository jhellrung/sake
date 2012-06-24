/*******************************************************************************
 * sake/core/range/adapt/traits/enable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_ADAPT_TRAITS_ENABLE_HPP
#define SAKE_CORE_RANGE_ADAPT_TRAITS_ENABLE_HPP

#include <boost/config.hpp>

#ifdef BOOST_NO_RVALUE_REFERENCES

#include <boost/mpl/not.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>

#include <sake/core/move/rv.hpp>
#include <sake/core/range/is_adapt_by_value.hpp>
#include <sake/core/type_traits/has_copy_constructor.hpp>

namespace sake
{

namespace range
{

namespace adapt
{

namespace traits
{

template< class R >
struct val_enable
    : boost_ext::mpl::and2<
          sake::range_is_adapt_by_value<R>,
          boost::mpl::not_< sake::has_copy_constructor<R> >
      >
{ };

template< class R >
struct val_enable< boost::rv<R> >
    : boost::false_type
{ };

template< class R >
struct ref_enable
    : boost::mpl::not_< sake::range::adapt::traits::val_enable<R> >
{ };

template< class R, class Result >
struct lazy_val_enabler
    : boost::lazy_enable_if_c<
          sake::range::adapt::traits::val_enable<R>::value,
          Result
      >
{ };

template< class R, class Result >
struct lazy_ref_enabler
    : boost::lazy_disable_if_c<
          sake::range::adapt::traits::val_enable<R>::value,
          Result
      >
{ };

} // namespace traits

} // namespace adapt

} // namespace range

} // namespace sake

#endif // #ifdef BOOST_NO_RVALUE_REFERENCES

#endif // #ifndef SAKE_CORE_RANGE_ADAPT_TRAITS_ENABLE_HPP
