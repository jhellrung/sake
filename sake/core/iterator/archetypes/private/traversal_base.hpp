/*******************************************************************************
 * sake/core/iterator/archetypes/private/traversal_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ARCHETYPES_TRAVERSAL_BASE_HPP
#define SAKE_CORE_ITERATOR_ARCHETYPES_TRAVERSAL_BASE_HPP

#include <cstddef>

#include <boost/iterator/iterator_categories.hpp>

namespace sake
{

namespace archetypes
{

namespace iterator_private
{

template< class Traversal >
struct traversal_base;

template<>
struct traversal_base< boost::incrementable_traversal_tag >
{
    typedef void difference_type;

    traversal_base& operator++()
    { return *this; }
    traversal_base operator++(int)
    { return *this; }

protected:
    // Disable default constructibility
    explicit traversal_base(int)
    { }
};

template<>
struct traversal_base< boost::single_pass_traversal_tag >
    : traversal_base< boost::incrementable_traversal_tag >
{
    inline friend bool operator==(traversal_base, traversal_base)
    { return true; }
    inline friend bool operator!=(traversal_base, traversal_base)
    { return false; }
};

template<>
struct traversal_base< boost::forward_traversal_tag >
    : traversal_base< boost::single_pass_traversal_tag >
{
    typedef std::ptrdiff_t difference_type;

    traversal_base()
        : traversal_base< boost::single_pass_traversal_tag >(0)
    { }
};

template<>
struct traversal_base< boost::bidirectional_traversal_tag >
    : traversal_base< boost::forward_traversal_tag >
{
    traversal_base& operator--()
    { return *this; }
    traversal_base operator--(int)
    { return *this; }
};

template<>
struct traversal_base< boost::random_access_traversal_tag >
    : traversal_base< boost::bidirectional_traversal_tag >
{
    typedef std::ptrdiff_t difference_type;

    traversal_base& operator+=(difference_type)
    { return *this; }
    traversal_base& operator-=(difference_type)
    { return *this; }

    traversal_base operator+(difference_type)
    { return *this; }
    traversal_base operator-(difference_type)
    { return *this; }

    inline friend traversal_base operator+(difference_type, traversal_base)
    { return traversal_base(); }

    inline friend difference_type operator-(traversal_base, traversal_base)
    { return 0; }

    inline friend bool operator<(traversal_base, traversal_base)
    { return false; }
    inline friend bool operator>(traversal_base, traversal_base)
    { return false; }
    inline friend bool operator<=(traversal_base, traversal_base)
    { return true; }
    inline friend bool operator>=(traversal_base, traversal_base)
    { return true; }
};

} // namespace iterator_private

} // namespace archetypes

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ARCHETYPES_TRAVERSAL_BASE_HPP
