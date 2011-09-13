/*******************************************************************************
 * unit_test/test/core/introspection/sfinae.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * This source file outlines the SFINAE "tricks" used to implement the
 * introspection metafunctions.
 ******************************************************************************/

#include <boost/mpl/if.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_member_object_pointer.hpp>
#include <boost/type_traits/is_member_function_pointer.hpp>

#include <sake/core/utility/yes_no_type.hpp>

namespace
{

using sake::yes_type;
using sake::no_type;

struct X { };
struct X_type { struct x; };
struct X_isc { static const int x = 0; };
struct X_class_template { template< class > struct x; };
struct X_mem_obj { int x; };
struct X_mem_fn { void x(); };
struct X_mem_fn_template { template< class T > void x(T); };

template< class T >
struct has_member
{
    struct base { int x; };
    template< int base::* > struct sfinae;
    struct derived : T, base { };
    template< class U > static no_type test(sfinae< &U::x >*);
    template< class U > static yes_type test(...);
    static const bool value = sizeof( yes_type ) == sizeof( test< derived >(0) );
};

BOOST_STATIC_ASSERT(!(has_member< X                 >::value));
BOOST_STATIC_ASSERT( (has_member< X_type            >::value));
BOOST_STATIC_ASSERT( (has_member< X_isc             >::value));
BOOST_STATIC_ASSERT( (has_member< X_class_template  >::value));
BOOST_STATIC_ASSERT( (has_member< X_mem_obj         >::value));
BOOST_STATIC_ASSERT( (has_member< X_mem_fn          >::value));
BOOST_STATIC_ASSERT( (has_member< X_mem_fn_template >::value));

template< class T >
struct has_type
{
    template< class > struct sfinae;
    template< class U > static yes_type test(sfinae< typename U::x >*);
    template< class U > static no_type test(...);
    static const bool value = sizeof( yes_type ) == sizeof( test<T>(0) );
};

BOOST_STATIC_ASSERT(!(has_type< X                 >::value));
BOOST_STATIC_ASSERT( (has_type< X_type            >::value));
BOOST_STATIC_ASSERT(!(has_type< X_isc             >::value));
BOOST_STATIC_ASSERT(!(has_type< X_class_template  >::value));
BOOST_STATIC_ASSERT(!(has_type< X_mem_obj         >::value));
BOOST_STATIC_ASSERT(!(has_type< X_mem_fn          >::value));
BOOST_STATIC_ASSERT(!(has_type< X_mem_fn_template >::value));

template< class T >
struct has_type_or_template
{
    template< class U > static yes_type test(typename U::x*);
    template< class U > static no_type test(...);
    static const bool value = sizeof( yes_type ) == sizeof( test<T>(0) );
};

BOOST_STATIC_ASSERT(!(has_type_or_template< X                 >::value));
BOOST_STATIC_ASSERT( (has_type_or_template< X_type            >::value));
BOOST_STATIC_ASSERT(!(has_type_or_template< X_isc             >::value));
BOOST_STATIC_ASSERT( (has_type_or_template< X_class_template  >::value));
BOOST_STATIC_ASSERT(!(has_type_or_template< X_mem_obj         >::value));
BOOST_STATIC_ASSERT(!(has_type_or_template< X_mem_fn          >::value));
BOOST_STATIC_ASSERT(!(has_type_or_template< X_mem_fn_template >::value));

template< class T >
struct has_isc
{
    template< int > struct sfinae { typedef yes_type type; };
    template< class U > static yes_type test(sfinae< U::x >*);
    template< class U > static no_type test(...);
    static const bool value = sizeof( yes_type ) == sizeof( test<T>(0) );
};

BOOST_STATIC_ASSERT(!(has_isc< X                 >::value));
BOOST_STATIC_ASSERT(!(has_isc< X_type            >::value));
BOOST_STATIC_ASSERT( (has_isc< X_isc             >::value));
BOOST_STATIC_ASSERT(!(has_isc< X_class_template  >::value));
BOOST_STATIC_ASSERT(!(has_isc< X_mem_obj         >::value));
BOOST_STATIC_ASSERT(!(has_isc< X_mem_fn          >::value));
BOOST_STATIC_ASSERT(!(has_isc< X_mem_fn_template >::value));

template< class T >
struct has_mem_obj
{
    template< int T::* > struct sfinae;
    template< class U > static yes_type test(sfinae< &U::x >*);
    template< class U > static no_type test(...);
    static const bool value = sizeof( yes_type ) == sizeof( test<T>(0) );
};

BOOST_STATIC_ASSERT(!(has_mem_obj< X                 >::value));
BOOST_STATIC_ASSERT(!(has_mem_obj< X_type            >::value));
//BOOST_STATIC_ASSERT(!(has_mem_obj< X_isc             >::value));
BOOST_STATIC_ASSERT(!(has_mem_obj< X_class_template  >::value));
BOOST_STATIC_ASSERT( (has_mem_obj< X_mem_obj         >::value));
BOOST_STATIC_ASSERT(!(has_mem_obj< X_mem_fn          >::value));
BOOST_STATIC_ASSERT(!(has_mem_obj< X_mem_fn_template >::value));

} // namespace
