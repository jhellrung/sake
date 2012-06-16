/*******************************************************************************
 * sake/core/iterator/archetypes/private/operator_bracket_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ARCHETYPES_PRIVATE_OPERATOR_BRACKET_BASE_HPP
#define SAKE_CORE_ITERATOR_ARCHETYPES_PRIVATE_OPERATOR_BRACKET_BASE_HPP

#include <cstddef>

#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/iterator/archetypes/access_tag.hpp>
#include <sake/core/iterator/archetypes/private/reference_dispatch.hpp>
#include <sake/core/iterator/categories.hpp>

namespace sake
{

namespace archetypes
{

namespace iterator_private
{

template<
    class T, class Access,
    bool = boost_ext::is_convertible<
        Access, sake::iterator::archetypes::writable_tag >::value
>
class operator_bracket_proxy;

template< class T, class Access >
class operator_bracket_proxy< T, Access, false >
{
    typedef sake::archetypes::iterator_private::reference_dispatch<
        T const, Access
    > reference_dispatch_;
public:
    operator typename reference_dispatch_::type () const
    { return reference_dispatch_::apply(); }
};

template< class T, class Access >
class operator_bracket_proxy< T, Access, true >
{
    typedef sake::archetypes::iterator_private::reference_dispatch<
        T, Access
    > reference_dispatch_;
public:
    operator typename reference_dispatch_::type () const
    { return reference_dispatch_::apply(); }
    operator_bracket_proxy const & operator=(T const &)
    { return *this; }
};

template<
    class T, class Traversal, class Access,
    bool = boost_ext::is_convertible<
        Traversal, boost::random_access_traversal_tag >::value
>
class operator_bracket_base;

template< class T, class Traversal, class Access >
class operator_bracket_base< T, Traversal, Access, false >
{ };

template< class T, class Traversal, class Access >
class operator_bracket_base< T, Traversal, Access, true >
{
    typedef operator_bracket_proxy< T, Access > type;
public:
    type operator[](std::ptrdiff_t) const
    { return type(); }
};

} // namespace iterator_private

} // namespace archetypes

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ARCHETYPES_PRIVATE_OPERATOR_BRACKET_BASE_HPP
