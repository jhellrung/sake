/*******************************************************************************
 * sake/core/concepts/MoveConstructible.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct concepts::MoveConstructible<T>
 ******************************************************************************/

#ifndef SAKE_CORE_CONCEPTS_MOVE_CONSTRUCTIBLE_HPP
#define SAKE_CORE_CONCEPTS_MOVE_CONSTRUCTIBLE_HPP

#include <sake/core/concepts/Constructible.hpp>

namespace sake
{

namespace concepts
{

template< class T >
struct MoveConstructible
    : sake::concepts::Constructible< T ( T ) >
{ };

} // namespace concepts

} // namespace sake

#endif // #ifndef SAKE_CORE_CONCEPTS_MOVE_CONSTRUCTIBLE_HPP
