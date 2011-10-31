/*******************************************************************************
 * sake/boost_ext/fusion/adapted/compressed_tuple.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_FUSION_ADAPTED_COMPRESSED_TUPLE_HPP
#define SAKE_BOOST_EXT_FUSION_ADAPTED_COMPRESSED_TUPLE_HPP

#include <boost/config.hpp>

#include <sake/boost_ext/fusion/adapted/tuple.hpp>

#include <sake/core/utility/compressed_tuple_fwd.hpp>

namespace boost {
namespace fusion {
namespace traits {

#ifndef BOOST_NO_VARIADIC_TEMPLATES
template< class... T >
struct tag_of< sake::compressed_tuple< T... > >
#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES
template< BOOST_PP_ENUM_PARAMS( SAKE_COMPRESSED_TUPLE_MAX_SIZE, class T ) >
struct tag_of< sake::compressed_tuple< BOOST_PP_ENUM_PARAMS( SAKE_COMPRESSED_TUPLE_MAX_SIZE, T ) > >
#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES
{ typedef sake::boost_ext::fusion::tuple_tag type; };

} // namespace traits
} // namespace fusion
} // namespace boost

#endif // #ifndef SAKE_BOOST_EXT_FUSION_ADAPTED_COMPRESSED_TUPLE_HPP
