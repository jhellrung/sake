/*******************************************************************************
 * sake/core/functional/operators/post_decrement.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * operators::post_decrement(T& x)
 *     -> operators::result_of::post_decrement< T& >::type
 * struct operators::functional::post_decrement
 *
 * struct operators::result_of::post_decrement<T>
 * struct operators::result_of::extension::post_decrement< T, Enable = void >
 * struct operators::result_of::default_impl::post_decrement<T>
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_POST_DECREMENT_HPP
#define SAKE_CORE_FUNCTIONAL_OPERATORS_POST_DECREMENT_HPP

#define SAKE_OPERATORS_NAME       post_decrement
#define SAKE_OPERATORS_APPLY( x ) x--
#include <sake/core/functional/operators/private/unary.ipp>

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_POST_DECREMENT_HPP
