/*******************************************************************************
 * sake/boost_ext/type_traits/is_convertible_wndp2bp.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::is_convertible_wndp2bp< From, To >
 *
 * WNDP2BP = With No Derived Pointer To Base Pointer
 *
 * This is identical to boost_ext::is_convertible except when From and To are
 * pointer types, in which case this excludes derived pointer to base pointer
 * conversions.
 *
 * This is useful to detect iterator conversions while disallowing derived
 * pointer to base pointer conversions.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_IS_CONVERTIBLE_WNDP2BP_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_IS_CONVERTIBLE_WNDP2BP_HPP

#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_void.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/is_same_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

namespace sake
{

namespace boost_ext
{

namespace is_convertible_wndp2bp_private
{

template<
    class From, class To,
    class = typename boost_ext::remove_qualifiers< From >::type,
    class = typename boost_ext::remove_qualifiers< To >::type
>
struct dispatch
    : boost_ext::is_convertible< From, To >
{ };

template< class From, class To, class F, class T >
struct dispatch< From, To, F*, T* >
    : boost::mpl::and_<
          boost_ext::is_convertible< From, To >,
          boost::mpl::or_<
              boost::is_void<T>,
              boost_ext::is_same_sans_qualifiers<F,T>
          >
      >
{ };

} // namespace is_convertible_wndp2bp_private

template< class From, class To >
struct is_convertible_wndp2bp
    : is_convertible_wndp2bp_private::dispatch< From, To >
{ };

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_IS_CONVERTIBLE_WNDP2BP_HPP
