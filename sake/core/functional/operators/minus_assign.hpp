/*******************************************************************************
 * sake/core/functional/operators/minus_assign.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * operators::minus_assign(T0& x0, T1&& x1)
 *     -> operators::result_of::minus_assign< T0&, T1 >::type
 * struct operators::functional::minus_assign
 *
 * struct operators::result_of::minus_assign< T0, T1 >
 * struct operators::result_of::extension::minus_assign< T0, T1, Enable = void >
 * struct operators::result_of::default_impl::minus_assign< T0, T1 >
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_MINUS_ASSIGN_HPP
#define SAKE_CORE_FUNCTIONAL_OPERATORS_MINUS_ASSIGN_HPP

#define SAKE_OPERATORS_NAME minus_assign
#define SAKE_OPERATORS_OP   -=
#include <sake/core/functional/operators/private/binary.ipp>

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_MINUS_ASSIGN_HPP
