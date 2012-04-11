/*******************************************************************************
 * sake/core/functional/operators/unary_minus.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * operators::unary_minus(T&& x)
 *     -> operators::result_of::unary_minus<T>::type
 * struct operators::functional::unary_minus
 *
 * struct operators::result_of::unary_minus<T>
 * struct operators::result_of::extension::unary_minus< T, Enable = void >
 * struct operators::result_of::default_impl::unary_minus<T>
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_UNARY_MINUS_HPP
#define SAKE_CORE_FUNCTIONAL_OPERATORS_UNARY_MINUS_HPP

#define SAKE_OPERATORS_NAME unary_minus
#define SAKE_OPERATORS_OP   -
#include <sake/core/functional/operators/private/unary.ipp>

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_UNARY_MINUS_HPP
