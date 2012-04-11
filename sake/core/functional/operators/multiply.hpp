/*******************************************************************************
 * sake/core/functional/operators/multiply.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * operators::multiply(T0&& x0, T1&& x1)
 *     -> operators::result_of::multiply< T0, T1 >::type
 * struct operators::functional::multiply
 *
 * struct operators::result_of::multiply< T0, T1 >
 * struct operators::result_of::extension::multiply0< T0, T1, Enable = void >
 * struct operators::result_of::extension::multiply1< T0, T1, Enable = void >
 * struct operators::result_of::default_impl::multiply< T0, T1 >
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_MULTIPLY_HPP
#define SAKE_CORE_FUNCTIONAL_OPERATORS_MULTIPLY_HPP

#define SAKE_OPERATORS_NAME multiply
#define SAKE_OPERATORS_OP   *
#include <sake/core/functional/operators/private/binary.ipp>

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_MULTIPLY_HPP
