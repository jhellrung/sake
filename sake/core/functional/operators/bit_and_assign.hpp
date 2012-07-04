/*******************************************************************************
 * sake/core/functional/operators/bit_and_assign.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * operators::bit_and_assign(T0& x0, T1&& x1)
 *     -> operators::result_of::bit_and_assign< T0&, T1 >::type
 * struct operators::functional::bit_and_assign
 *
 * struct operators::result_of::bit_and_assign< T0, T1 >
 * struct operators::result_of::extension::bit_and_assign< T0, T1, Enable = void >
 * struct operators::result_of::default_impl::bit_and_assign< T0, T1 >
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_BIT_AND_ASSIGN_HPP
#define SAKE_CORE_FUNCTIONAL_OPERATORS_BIT_AND_ASSIGN_HPP

#define SAKE_OPERATORS_NAME bit_and_assign
#define SAKE_OPERATORS_OP   &=
#include <sake/core/functional/operators/private/binary.ipp>

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_BIT_AND_ASSIGN_HPP
