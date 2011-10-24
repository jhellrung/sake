/*******************************************************************************
 * sake/core/functional/operators/less.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * operators::less(T0&& x0, T1&& x1)
 *     -> operators::result_of::less< T0, T1 >::type
 * struct operators::functional::less
 *
 * struct operators::result_of::less< T0, T1 >
 * struct operators::result_of::extension::less0< T0, T1, Enable = void >
 * struct operators::result_of::extension::less1< T0, T1, Enable = void >
 * struct operators::result_of::default_impl::less< T0, T1 >
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_LESS_HPP
#define SAKE_CORE_FUNCTIONAL_OPERATORS_LESS_HPP

#define SAKE_OPERATORS_NAME less
#define SAKE_OPERATORS_OP   <
#include <sake/core/functional/operators/private/comparison.ipp>

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_LESS_HPP
