/*******************************************************************************
 * sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::is_base_of_sans_qualifiers<B,D>
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_IS_BASE_OF_SANS_QUALIFIERS_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_IS_BASE_OF_SANS_QUALIFIERS_HPP

#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

namespace sake
{

namespace boost_ext
{

namespace is_base_of_sans_qualifiers_private
{

template< class B, class D >
struct dispatch
    : boost::is_base_of<B,D>
{ };

template< class B >
struct dispatch<B,B>
    : boost::true_type
{ };

} // namespace is_base_of_sans_qualifiers_private

template< class B, class D >
struct is_base_of_sans_qualifiers
    : is_base_of_sans_qualifiers_private::dispatch<
          typename boost_ext::remove_qualifiers<B>::type,
          typename boost_ext::remove_qualifiers<D>::type
      >
{ };

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_IS_BASE_OF_SANS_QUALIFIERS_HPP
