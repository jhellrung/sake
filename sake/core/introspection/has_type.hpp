/*******************************************************************************
 * sake/core/introspection/has_type.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_INTROSPECTION_DEFINE_HAS_TYPE( trait, name )
 *
 * Expands to define a metafunction trait which determines if a given type has
 * a nested typedef or struct of the given name.
 *
 * Example:
 *
 * SAKE_INTROSPECTION_DEFINE_HAS_TYPE( has_type_xxx, xxx )
 *
 * expands to a metafunction definition equivalent to
 *
 * template< class T >
 * struct has_type_xxx
 * {
 *     static const bool value = [true iff T has a nested type named xxx];
 *     typedef has_type_xxx type;
 * };
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_HAS_TYPE_HPP
#define SAKE_CORE_INTROSPECTION_HAS_TYPE_HPP

#include <sake/core/utility/yes_no_tag.hpp>

#define SAKE_INTROSPECTION_DEFINE_HAS_TYPE( trait, name ) \
template< class T > \
class trait \
{ \
    template< class > struct sfinae; \
    template< class U > static ::sake::yes_tag test(sfinae< typename T::name >*); \
    template< class U > static ::sake::no_tag test(...); \
public: \
    static const bool value = sizeof( ::sake::yes_tag ) == sizeof( test<T>(0) ); \
    typedef trait type; \
};

#endif // #ifndef SAKE_CORE_INTROSPECTION_HAS_TYPE_HPP
