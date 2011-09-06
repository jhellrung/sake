/*******************************************************************************
 * boost_ext/type_traits/is_convertible_wnrbt_fwd.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_IS_CONVERTIBLE_WNRBT_FWD_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_IS_CONVERTIBLE_WNRBT_FWD_HPP

namespace sake
{

namespace boost_ext
{

template< class From, class To >
struct is_convertible_wnrbt;

namespace ext
{

template< class From, class To, class Enable >
struct is_convertible_wnrbt;

} // namespace ext

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_IS_CONVERTIBLE_WNRBT_FWD_HPP
