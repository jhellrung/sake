/*******************************************************************************
 * sake/core/iterator/archetypes/private/reference_dispatch.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ARCHETYPES_PRIVATE_REFERENCE_DISPATCH_HPP
#define SAKE_CORE_ITERATOR_ARCHETYPES_PRIVATE_REFERENCE_DISPATCH_HPP

#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/iterator/archetypes/access_tag.hpp>

namespace sake
{

namespace archetypes
{

namespace iterator_private
{

template<
    class T, class Access,
    bool = boost_ext::is_convertible<
        Access, sake::iterator_archetypes::writable_tag >::value
>
struct proxy_reference;

template< class T, class Access >
struct proxy_reference< T, Access, false >
{
    operator T& () const
    { return *static_cast< T* >(0); }
};

template< class T, class Access >
struct proxy_reference< T, Access, true >
{
    operator T& () const
    { return *static_cast< T* >(0); }
    proxy_reference const & operator=(T const &) const
    { return *this; }
};

template< class Access >
struct proxy_reference< void, Access, true >
{
    template< class T >
    proxy_reference const & operator=(T const &) const
    { return *this; }
};

template<
    class T, class Access,
    bool = boost_ext::is_convertible<
        Access, sake::iterator_archetypes::lvalue_tag >::value
>
struct reference_dispatch;

template< class T, class Access >
struct reference_dispatch< T, Access, false >
{
    typedef proxy_reference< T, Access > type;
    static type apply()
    { return type(); }
};

template< class T, class Access >
struct reference_dispatch< T, Access, true >
{
    typedef T& type;
    static type apply()
    { return *static_cast< T* >(0); }
};

} // namespace iterator_private

} // namespace archetypes

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ARCHETYPES_PRIVATE_REFERENCE_DISPATCH_HPP
