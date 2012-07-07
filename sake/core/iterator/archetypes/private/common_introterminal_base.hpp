/*******************************************************************************
 * sake/core/iterator/archetypes/private/common_introterminal_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ARCHETYPES_PRIVATE_COMMON_INTROTERMINAL_BASE_HPP
#define SAKE_CORE_ITERATOR_ARCHETYPES_PRIVATE_COMMON_INTROTERMINAL_BASE_HPP

#include <sake/core/iterator/archetypes/private/relax.hpp>
#include <sake/core/iterator/categories.hpp>

namespace sake
{

namespace archetypes
{

namespace iterator_private
{

template< class Derived, class Introterminal >
struct common_introterminal_base;

#define define_conversion_operator( tag ) \
    private: typedef typename sake::archetypes::iterator_private::relax< \
        Derived, sake::tag ## _introterminal_tag >::type tag ## _type; \
    public: operator tag ## _type() const \
    { return *static_cast< tag ## _type* >(0); }

template< class Derived >
struct common_introterminal_base<
    Derived, sake::null_introterminal_tag >
{
    define_conversion_operator( null )

    void
    at_ip(null_type)
    { }

    template< class Introterminal >
    typename sake::archetypes::iterator_private::
        relax< Derived, Introterminal >::type
    at(null_type, Introterminal) const
    { return static_cast< Derived const & >(*this); }
};

template< class Derived >
struct common_introterminal_base<
    Derived, sake::begin_detect_introterminal_tag >
    : sake::archetypes::iterator_private::common_introterminal_base<
          Derived, sake::null_introterminal_tag >
{ define_conversion_operator( begin_detect ) };

template< class Derived >
struct common_introterminal_base<
    Derived, sake::begin_access_introterminal_tag >
    : sake::archetypes::iterator_private::common_introterminal_base<
          Derived, sake::begin_detect_introterminal_tag >
{ define_conversion_operator( begin_access ) };

template< class Derived >
struct common_introterminal_base<
    Derived, sake::end_detect_introterminal_tag >
    : sake::archetypes::iterator_private::common_introterminal_base<
          Derived, sake::null_introterminal_tag >
{ define_conversion_operator( end_detect ) };

template< class Derived >
struct common_introterminal_base<
    Derived, sake::end_access_introterminal_tag >
    : sake::archetypes::iterator_private::common_introterminal_base<
          Derived, sake::end_detect_introterminal_tag >
{ define_conversion_operator( end_access ) };

template< class Derived >
struct common_introterminal_base<
    Derived, sake::begin_detect_end_detect_introterminal_tag >
    : sake::archetypes::iterator_private::common_introterminal_base<
          Derived, sake::null_introterminal_tag >
{
    define_conversion_operator( begin_detect )
    define_conversion_operator( end_detect )
    define_conversion_operator( begin_detect_end_detect )
};

template< class Derived >
struct common_introterminal_base<
    Derived, sake::begin_access_end_detect_introterminal_tag >
    : sake::archetypes::iterator_private::common_introterminal_base<
          Derived, sake::begin_detect_end_detect_introterminal_tag >
{
    define_conversion_operator( begin_access )
    define_conversion_operator( begin_access_end_detect )
};

template< class Derived >
struct common_introterminal_base<
    Derived, sake::begin_detect_end_access_introterminal_tag >
    : sake::archetypes::iterator_private::common_introterminal_base<
          Derived, sake::begin_detect_end_detect_introterminal_tag >
{
    define_conversion_operator( end_access )
    define_conversion_operator( begin_detect_end_access )
};

template< class Derived >
struct common_introterminal_base<
    Derived, sake::begin_access_end_access_introterminal_tag >
    : sake::archetypes::iterator_private::common_introterminal_base<
          Derived, sake::begin_detect_end_detect_introterminal_tag >
{
    define_conversion_operator( begin_access )
    define_conversion_operator( end_access )
    define_conversion_operator( begin_access_end_detect )
    define_conversion_operator( begin_detect_end_access )
    define_conversion_operator( begin_access_end_access )
};

#undef define_conversion_operator

} // namespace iterator_private

} // namespace archetypes

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ARCHETYPES_PRIVATE_COMMON_INTROTERMINAL_BASE_HPP
