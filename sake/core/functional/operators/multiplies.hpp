/*******************************************************************************
 * sake/core/functional/operators/multiplies.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * operators::multiplies(T0 const & x0, T1 const & x1)
 *     -> operators::result_of::multiplies< T0, T1 >::type
 * struct operators::functional::multiplies
 *
 * struct operators::result_of::multiplies< T0, T1 >
 * struct operators::result_of::extension::multiplies0< T0, T1, Enable = void >
 * struct operators::result_of::extension::multiplies1< T0, T1, Enable = void >
 * struct operators::result_of::default_impl::multiplies< T0, T1 >
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_MULTIPLIES_HPP
#define SAKE_CORE_FUNCTIONAL_OPERATORS_MULTIPLIES_HPP

#define SAKE_OPERATORS_NAME multiplies
#define SAKE_OPERATORS_OP   *
#include <sake/core/functional/operators/private/binary_common_type.ipp>

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_MULTIPLIES_HPP
