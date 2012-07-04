/*******************************************************************************
 * sake/core/functional/operators/shift_left_assign.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * operators::shift_left_assign(T0& x0, T1&& x1)
 *     -> operators::result_of::shift_left_assign< T0&, T1 >::type
 * struct operators::functional::shift_left_assign
 *
 * struct operators::result_of::shift_left_assign< T0, T1 >
 * struct operators::result_of::extension::shift_left_assign< T0, T1, Enable = void >
 * struct operators::result_of::default_impl::shift_left_assign< T0, T1 >
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_SHIFT_LEFT_ASSIGN_HPP
#define SAKE_CORE_FUNCTIONAL_OPERATORS_SHIFT_LEFT_ASSIGN_HPP

#define SAKE_OPERATORS_NAME shift_left_assign
#define SAKE_OPERATORS_OP   <<=
#include <sake/core/functional/operators/private/binary.ipp>

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_SHIFT_LEFT_ASSIGN_HPP
