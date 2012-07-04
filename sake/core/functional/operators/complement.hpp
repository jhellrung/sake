/*******************************************************************************
 * sake/core/functional/operators/complement.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * operators::complement(T&& x)
 *     -> operators::result_of::complement<T>::type
 * struct operators::functional::complement
 *
 * struct operators::result_of::complement<T>
 * struct operators::result_of::extension::complement< T, Enable = void >
 * struct operators::result_of::default_impl::complement<T>
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_COMPLEMENT_HPP
#define SAKE_CORE_FUNCTIONAL_OPERATORS_COMPLEMENT_HPP

#define SAKE_OPERATORS_NAME complement
#define SAKE_OPERATORS_OP   ~
#include <sake/core/functional/operators/private/unary.ipp>

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_COMPLEMENT_HPP
