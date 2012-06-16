/*******************************************************************************
 * sake/core/iterator/archetypes/private/begin_introversal_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ARCHETYPES_PRIVATE_BEGIN_INTROVERSAL_BASE_HPP
#define SAKE_CORE_ITERATOR_ARCHETYPES_PRIVATE_BEGIN_INTROVERSAL_BASE_HPP

#include <cstddef>

#include <sake/core/iterator/archetypes/private/relax.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>

namespace sake
{

namespace archetypes
{

namespace iterator_private
{

template< class Derived, class Introversal, bool RandomAccess >
struct begin_introversal_base;

template< class Derived, bool RandomAccess >
struct begin_introversal_base<
    Derived, sake::null_introversal_tag, RandomAccess >
{ };

template< class Derived, bool RandomAccess >
struct begin_introversal_base<
    Derived, sake::begin_detect_introversal_tag, RandomAccess >
{
    inline friend bool operator==(begin_introversal_base, sake::begin_tag)
    { return true; }
    inline friend bool operator!=(begin_introversal_base, sake::begin_tag)
    { return false; }
    inline friend bool operator==(sake::begin_tag, begin_introversal_base)
    { return true; }
    inline friend bool operator!=(sake::begin_tag, begin_introversal_base)
    { return false; }
};

template< class Derived >
struct begin_introversal_base<
    Derived, sake::begin_access_introversal_tag, false >
    : sake::archetypes::iterator_private::begin_introversal_base<
          Derived, sake::begin_detect_introversal_tag, false >
{
    Derived&
    begin_ip()
    { return static_cast< Derived& >(*this); }

    template< class Introversal >
    typename sake::archetypes::iterator_private::
        relax< Derived, Introversal >::type
    begin(Introversal) const
    { return static_cast< Derived const & >(*this); }
};

template< class Derived >
struct begin_introversal_base<
    Derived, sake::begin_access_introversal_tag, true >
    : sake::archetypes::iterator_private::begin_introversal_base<
          Derived, sake::begin_access_introversal_tag, false >
{
    inline friend std::ptrdiff_t operator-(begin_introversal_base, sake::begin_tag)
    { return 0; }
    inline friend std::ptrdiff_t operator-(sake::begin_tag, begin_introversal_base)
    { return 0; }
};

} // namespace iterator_private

} // namespace archetypes

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ARCHETYPES_PRIVATE_BEGIN_INTROVERSAL_BASE_HPP
