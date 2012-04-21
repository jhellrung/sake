/*******************************************************************************
 * sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::is_base_of_sans_qualifiers<B,D>
 *
 * Note that, as for boost::is_base_of, this returns true if B and D are the
 * same class type (sans qualifiers).
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_IS_BASE_OF_SANS_QUALIFIERS_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_IS_BASE_OF_SANS_QUALIFIERS_HPP

#include <boost/type_traits/is_base_of.hpp>

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

namespace sake
{

namespace boost_ext
{

template< class B, class D >
struct is_base_of_sans_qualifiers
    : boost::is_base_of<
          typename boost_ext::remove_qualifiers<B>::type,
          typename boost_ext::remove_qualifiers<D>::type
      >
{ };

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_IS_BASE_OF_SANS_QUALIFIERS_HPP
