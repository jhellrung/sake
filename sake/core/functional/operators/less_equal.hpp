/*******************************************************************************
 * sake/core/functional/operators/less_equal.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * operators::less_equal(T0&& x0, T1&& x1)
 *     -> operators::result_of::less_equal< T0, T1 >::type
 * struct operators::functional::less_equal
 *
 * struct operators::result_of::less_equal< T0, T1 >
 * struct operators::result_of::extension::less_equal< T0, T1, Enable = void >
 * struct operators::result_of::default_impl::less_equal< T0, T1 >
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_LESS_EQUAL_HPP
#define SAKE_CORE_FUNCTIONAL_OPERATORS_LESS_EQUAL_HPP

#define SAKE_OPERATORS_NAME less_equal
#define SAKE_OPERATORS_OP   <=
#include <sake/core/functional/operators/private/relational.ipp>

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_LESS_EQUAL_HPP
