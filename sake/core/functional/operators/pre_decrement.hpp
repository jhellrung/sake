/*******************************************************************************
 * sake/core/functional/operators/pre_decrement.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * operators::pre_decrement(T& x)
 *     -> operators::result_of::pre_decrement< T& >::type
 * struct operators::functional::pre_decrement
 *
 * struct operators::result_of::pre_decrement<T>
 * struct operators::result_of::extension::pre_decrement< T, Enable = void >
 * struct operators::result_of::default_impl::pre_decrement<T>
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_PRE_DECREMENT_HPP
#define SAKE_CORE_FUNCTIONAL_OPERATORS_PRE_DECREMENT_HPP

#define SAKE_OPERATORS_NAME pre_decrement
#define SAKE_OPERATORS_OP   --
#include <sake/core/functional/operators/private/unary.ipp>

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_PRE_DECREMENT_HPP
