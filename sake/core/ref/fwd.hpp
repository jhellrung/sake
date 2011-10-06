/*******************************************************************************
 * sake/core/ref/fwd.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_REF_FWD_HPP
#define SAKE_CORE_REF_FWD_HPP

#include <boost/type_traits/integral_constant.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/is_convertible_wnrbt_fwd.hpp>
//#include <sake/core/range/cursor_fwd.hpp>
//#include <sake/core/range/is_view_fwd.hpp>
//#include <sake/core/range/static_size_fwd.hpp>
#include <sake/core/ref/ref_tag.hpp>

namespace sake
{

template< class T >
struct is_reference_wrapper;

template< class T >
struct unwrap_reference;

template< class T, class Tags = ref_tag::default_tags >
class reference_wrapper;

#if 0
/*******************************************************************************
 * struct ext::range_mutable_cursor< T, Introversal, ... >
 * struct ext::range_const_cursor< T, Introversal, ... >
 * struct ext::range_is_view< T, ... >
 * struct ext::range_static_size< T, ... >
 ******************************************************************************/

namespace ext
{

template< class T, class Introversal >
struct range_mutable_cursor<
    T, Introversal,
    typename boost::enable_if_c< sake::is_reference_wrapper<T>::value >::type
>
    : sake::range_cursor< typename sake::unwrap_reference<T>::type, Introversal >
{ };

template< class T, class Introversal >
struct range_const_cursor<
    T, Introversal,
    typename boost::enable_if_c< sake::is_reference_wrapper<T>::value >::type
>
    : sake::range_cursor< typename sake::unwrap_reference<T>::type, Introversal >
{ };

template< class T >
struct range_is_view<
    T,
    typename boost::enable_if_c< sake::is_reference_wrapper<T>::value >::type
>
    : boost::true_type
{ };

template< class T >
struct range_static_size<
    T,
    typename boost::enable_if_c< sake::is_reference_wrapper<T>::value >::type
>
    : sake::range_static_size< typename sake::unwrap_reference<T>::type >
{ };

} // namespace ext
#endif // #if 0

/*******************************************************************************
 * struct boost_ext::ext::is_convertible_wnrbt< T, U, ... >
 ******************************************************************************/

namespace boost_ext
{

namespace ext
{

template< class T, class U >
struct is_convertible_wnrbt<
    T, U,
    typename boost::enable_if_c< sake::is_reference_wrapper<T>::value >::type
>
    : boost_ext::is_convertible_wnrbt<
          typename sake::unwrap_reference<T>::type &,
          U
      >
{ };

} // namespace ext

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_CORE_REF_FWD_HPP