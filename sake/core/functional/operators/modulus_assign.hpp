/*******************************************************************************
 * sake/core/functional/operators/modulus_assign.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * operators::modulus_assign(T0& x0, T1&& x1)
 *     -> operators::result_of::modulus_assign< T0&, T1 >::type
 * struct operators::functional::modulus_assign
 *
 * struct operators::result_of::modulus_assign< T0, T1 >
 * struct operators::result_of::extension::modulus_assign< T0, T1, Enable = void >
 * struct operators::result_of::default_impl::modulus_assign< T0, T1 >
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_MODULUS_ASSIGN_HPP
#define SAKE_CORE_FUNCTIONAL_OPERATORS_MODULUS_ASSIGN_HPP

#define SAKE_OPERATORS_NAME modulus_assign
#define SAKE_OPERATORS_OP   %=
#include <sake/core/functional/operators/private/binary.ipp>

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_MODULUS_ASSIGN_HPP
