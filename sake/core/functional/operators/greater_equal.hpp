/*******************************************************************************
 * sake/core/functional/operators/greater_equal.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * operators::greater_equal(T0&& x0, T1&& x1)
 *     -> operators::result_of::greater_equal< T0, T1 >::type
 * struct operators::functional::greater_equal
 *
 * struct operators::result_of::greater_equal< T0, T1 >
 * struct operators::result_of::extension::greater_equal0< T0, T1, Enable = void >
 * struct operators::result_of::extension::greater_equal1< T0, T1, Enable = void >
 * struct operators::result_of::default_impl::greater_equal< T0, T1 >
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_GREATER_EQUAL_HPP
#define SAKE_CORE_FUNCTIONAL_OPERATORS_GREATER_EQUAL_HPP

#define SAKE_OPERATORS_NAME greater_equal
#define SAKE_OPERATORS_OP   >=
#include <sake/core/functional/operators/private/comparison.ipp>

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_GREATER_EQUAL_HPP
