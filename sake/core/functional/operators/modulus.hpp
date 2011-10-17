/*******************************************************************************
 * sake/core/functional/operators/modulus.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * operators::modulus(T0 const & x0, T1 const & x1)
 *     -> operators::result_of::modulus< T0, T1 >::type
 * struct operators::functional::modulus
 *
 * struct operators::result_of::modulus< T0, T1 >
 * struct operators::result_of::extension::modulus0< T0, T1, Enable = void >
 * struct operators::result_of::extension::modulus1< T0, T1, Enable = void >
 * struct operators::result_of::default_impl::modulus< T0, T1 >
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_MODULUS_HPP
#define SAKE_CORE_FUNCTIONAL_OPERATORS_MODULUS_HPP

#define SAKE_OPERATORS_NAME modulus
#define SAKE_OPERATORS_OP   %
#include <sake/core/functional/operators/private/binary_common_type.ipp>

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_MODULUS_HPP
