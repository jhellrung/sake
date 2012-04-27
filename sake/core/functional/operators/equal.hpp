/*******************************************************************************
 * sake/core/functional/operators/equal.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * operators::equal(T0&& x0, T1&& x1)
 *     -> operators::result_of::equal< T0, T1 >::type
 * struct operators::functional::equal
 *
 * struct operators::result_of::equal< T0, T1 >
 * struct operators::result_of::extension::equal< T0, T1, Enable = void >
 * struct operators::result_of::default_impl::equal< T0, T1 >
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_EQUAL_HPP
#define SAKE_CORE_FUNCTIONAL_OPERATORS_EQUAL_HPP

#define SAKE_OPERATORS_NAME equal
#define SAKE_OPERATORS_OP   ==
#include <sake/core/functional/operators/private/relational.ipp>

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_EQUAL_HPP
