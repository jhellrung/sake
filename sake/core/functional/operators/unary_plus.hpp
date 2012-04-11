/*******************************************************************************
 * sake/core/functional/operators/unary_plus.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * operators::unary_plus(T&& x)
 *     -> operators::result_of::unary_plus<T>::type
 * struct operators::functional::unary_plus
 *
 * struct operators::result_of::unary_plus<T>
 * struct operators::result_of::extension::unary_plus< T, Enable = void >
 * struct operators::result_of::default_impl::unary_plus<T>
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_UNARY_PLUS_HPP
#define SAKE_CORE_FUNCTIONAL_OPERATORS_UNARY_PLUS_HPP

#define SAKE_OPERATORS_NAME unary_plus
#define SAKE_OPERATORS_OP   +
#include <sake/core/functional/operators/private/unary.ipp>

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_UNARY_PLUS_HPP
