/*******************************************************************************
 * sake/core/iterator/archetypes/private/traversal_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ARCHETYPES_PRIVATE_TRAVERSAL_BASE_HPP
#define SAKE_CORE_ITERATOR_ARCHETYPES_PRIVATE_TRAVERSAL_BASE_HPP

#include <cstddef>

#include <sake/core/iterator/categories.hpp>

namespace sake
{

namespace archetypes
{

namespace iterator_private
{

template< class Derived, class Traversal >
struct traversal_base;

template< class Derived >
struct traversal_base< Derived, boost::incrementable_traversal_tag >
{
    typedef void difference_type;

    Derived& operator++()
    { return *static_cast< Derived* >(this); }
    Derived operator++(int)
    { return *static_cast< Derived* >(this); }

protected:
    // Disable default constructibility
    explicit traversal_base(int)
    { }
};

template< class Derived >
struct traversal_base< Derived, boost::single_pass_traversal_tag >
    : sake::archetypes::iterator_private::traversal_base<
          Derived, boost::incrementable_traversal_tag >
{
    inline friend bool
    operator==(traversal_base, traversal_base)
    { return true; }
    inline friend bool
    operator!=(traversal_base, traversal_base)
    { return false; }

protected:
    // Disable default constructibility
    explicit traversal_base(int)
        : traversal_base< Derived, boost::incrementable_traversal_tag >(0)
    { }
};

template< class Derived >
struct traversal_base< Derived, boost::forward_traversal_tag >
    : sake::archetypes::iterator_private::traversal_base<
          Derived, boost::single_pass_traversal_tag >
{
    typedef std::ptrdiff_t difference_type;

    traversal_base()
        : traversal_base< Derived, boost::single_pass_traversal_tag >(0)
    { }
};

template< class Derived >
struct traversal_base< Derived, boost::bidirectional_traversal_tag >
    : sake::archetypes::iterator_private::traversal_base<
          Derived, boost::forward_traversal_tag >
{
    Derived& operator--()
    { return *static_cast< Derived* >(this); }
    Derived operator--(int)
    { return *static_cast< Derived* >(this); }
};

template< class Derived >
struct traversal_base< Derived, boost::random_access_traversal_tag >
    : sake::archetypes::iterator_private::traversal_base<
          Derived, boost::bidirectional_traversal_tag >
{
    typedef std::ptrdiff_t difference_type;

    Derived& operator+=(difference_type)
    { return *static_cast< Derived* >(this); }
    Derived& operator-=(difference_type)
    { return *static_cast< Derived* >(this); }

    Derived operator+(difference_type) const
    { return *static_cast< Derived const * >(this); }
    Derived operator-(difference_type) const
    { return *static_cast< Derived const * >(this); }

    inline friend Derived
    operator+(difference_type, traversal_base)
    { return Derived(); }

    inline friend difference_type
    operator-(traversal_base, traversal_base)
    { return 0; }

    inline friend bool
    operator<(traversal_base, traversal_base)
    { return false; }
    inline friend bool
    operator>(traversal_base, traversal_base)
    { return false; }
    inline friend bool
    operator<=(traversal_base, traversal_base)
    { return true; }
    inline friend bool
    operator>=(traversal_base, traversal_base)
    { return true; }
};

} // namespace iterator_private

} // namespace archetypes

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ARCHETYPES_PRIVATE_TRAVERSAL_BASE_HPP
