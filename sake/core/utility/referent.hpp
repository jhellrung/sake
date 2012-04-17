/*******************************************************************************
 * sake/core/utility/referent.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct referent<T>
 *
 * This is a metafunction that strips reference qualifiers and unwraps reference
 * wrappers.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_REFERENT_HPP
#define SAKE_CORE_UTILITY_REFERENT_HPP

#include <sake/core/utility/ref/unwrap_reference.hpp>

namespace sake
{

template< class T >
struct referent
    : sake::unwrap_reference<T>
{ };

template< class T >
struct referent< T& >
    : referent<T>
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_REFERENT_HPP
