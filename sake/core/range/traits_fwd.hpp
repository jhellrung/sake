/*******************************************************************************
 * sake/core/range/traits_fwd.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_TRAITS_FWD_HPP
#define SAKE_CORE_RANGE_TRAITS_FWD_HPP

#include <sake/core/iterator/categories.hpp>

namespace sake
{

template< class R >
struct range_traits;

template< class R, class Introversal = sake::null_introversal_tag >
struct range_iterator;

template< class R >
struct range_value
{ typedef typename sake::range_traits<R>::value_type type; };
template< class R >
struct range_reference
{ typedef typename sake::range_traits<R>::reference type; };
template< class R >
struct range_difference
{ typedef typename sake::range_traits<R>::difference_type type; };
template< class R >
struct range_size
{ typedef typename sake::range_traits<R>::size_type type; };
template< class R >
struct range_traversal
{ typedef typename sake::range_traits<R>::traversal type; };

template< class R >
struct range_distance_enable
{
    static const bool value = sake::range_traits<R>::distance_enable_tag::value;
    typedef range_distance_enable type;
};

template< class R >
struct range_size_enable
{
    static const bool value = sake::range_traits<R>::size_enable_tag::value;
    typedef range_size_enable type;
};

namespace extension
{
template< class R, class Enable = void >
struct range_traits;
} // namespace extension

namespace default_impl
{
template< class R, class I = void >
struct range_traits;
} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_TRAITS_FWD_HPP
