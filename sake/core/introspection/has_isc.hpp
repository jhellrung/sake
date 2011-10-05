/*******************************************************************************
 * sake/core/introspection/has_isc.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_INTROSPECTION_DEFINE_HAS_ISC( trait, name )
 *
 * Expands to define a metafunction trait which determines if a given type has
 * a nested integral static constants (ISC) of the given name.
 *
 * Example:
 *
 * SAKE_INTROSPECTION_DEFINE_HAS_ISC( has_type_xxx, xxx )
 *
 * expands to a metafunction definition equivalent to
 *
 * template< class T >
 * struct has_type_xxx
 * {
 *     static bool const value = [true iff T has an ISC named xxx];
 *     typedef has_type_xxx type;
 * };
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_HAS_ISC_HPP
#define SAKE_CORE_INTROSPECTION_HAS_ISC_HPP

#include <sake/core/utility/yes_no_tag.hpp>

#define SAKE_INTROSPECTION_DEFINE_HAS_ISC( trait, name ) \
template< class T > \
class trait \
{ \
    template< int > struct sfinae; \
    template< class U > static ::sake::yes_tag test(sfinae< T::name >*); \
    template< class U > static ::sake::no_tag test(...); \
public: \
    static bool const value = sizeof( ::sake::yes_tag ) == sizeof( test<T>(0) ); \
    typedef trait type; \
};

#endif // #ifndef SAKE_CORE_INTROSPECTION_HAS_ISC_HPP
