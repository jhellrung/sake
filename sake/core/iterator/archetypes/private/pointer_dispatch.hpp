/*******************************************************************************
 * sake/core/iterator/archetypes/private/pointer_dispatch.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ARCHETYPES_PRIVATE_POINTER_DISPATCH_HPP
#define SAKE_CORE_ITERATOR_ARCHETYPES_PRIVATE_POINTER_DISPATCH_HPP

namespace sake
{

namespace archetypes
{

namespace iterator_private
{

template< class Reference >
struct pointer_dispatch
{
    struct type
    {
        Reference const * operator->() const
        { return static_cast< Reference const * >(0); }
    };
    static type apply()
    { return type(); }
};

template< class T >
struct pointer_dispatch< T& >
{
    typedef T* type;
    static type apply()
    { return static_cast< T* >(0); }
};

} // namespace iterator_private

} // namespace archetypes

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ARCHETYPES_PRIVATE_POINTER_DISPATCH_HPP
