/*******************************************************************************
 * sake/core/range/archetypes/private/reference_dispatch.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_ARCHETYPES_PRIVATE_REFERENCE_DISPATCH_HPP
#define SAKE_CORE_RANGE_ARCHETYPES_PRIVATE_REFERENCE_DISPATCH_HPP

namespace sake
{

namespace archetypes
{

namespace range_private
{

template< class T >
struct reference_dispatch
{ static T apply() { return T(); } };

template< class T >
struct reference_dispatch< T& >
{ static T& apply() { return *static_cast< T* >(0); } };

} // namespace range_private

} // namespace archetypes

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_ARCHETYPES_PRIVATE_REFERENCE_DISPATCH_HPP
