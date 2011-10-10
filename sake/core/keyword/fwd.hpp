/*******************************************************************************
 * sake/core/keyword/fwd.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_KEYWORD_FWD_HPP
#define SAKE_CORE_KEYWORD_FWD_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>

#ifndef SAKE_KEYWORD_MAX_ARITY
#define SAKE_KEYWORD_MAX_ARITY 8
#endif // #ifndef SAKE_KEYWORD_MAX_ARITY

namespace sake
{

namespace keyword
{

#ifndef BOOST_NO_VARIADIC_TEMPLATES

template< class... A >
struct arg_pack;

#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES

template< BOOST_PP_ENUM_BINARY_PARAMS(
    SAKE_KEYWORD_MAX_ARITY, class A, = void BOOST_PP_INTERCEPT ) >
struct arg_pack;

#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

} // namespace keyword

} // namespace sake

#endif // #ifndef SAKE_CORE_KEYWORD_FWD_HPP
