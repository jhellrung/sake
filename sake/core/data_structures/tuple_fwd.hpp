/*******************************************************************************
 * sake/core/data_structures/tuple_fwd.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_DATA_STRUCTURES_TUPLE_FWD_HPP
#define SAKE_CORE_DATA_STRUCTURES_TUPLE_FWD_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>

#ifndef SAKE_TUPLE_PERFECT_MAX_ARITY
#define SAKE_TUPLE_PERFECT_MAX_ARITY 4
#endif // #ifndef SAKE_TUPLE_PERFECT_MAX_ARITY

#ifndef SAKE_TUPLE_MAX_SIZE
#define SAKE_TUPLE_MAX_SIZE 8
#endif // #ifndef SAKE_TUPLE_MAX_SIZE

namespace sake
{

namespace tuple_adl
{

#ifndef BOOST_NO_VARIADIC_TEMPLATES
template< class... T >
#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES
template< BOOST_PP_ENUM_BINARY_PARAMS( SAKE_TUPLE_MAX_SIZE, class T, = void BOOST_PP_INTERCEPT ) >
#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES
struct tuple;

} // namespace tuple_adl

using tuple_adl::tuple;

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_TUPLE_FWD_HPP
