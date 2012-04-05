/*******************************************************************************
 * sake/boost_ext/type_traits/types.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_BOOST_EXT_INTEGRAL_TYPES
 * #define SAKE_BOOST_EXT_FLOATING_POINT_TYPES
 * #define SAKE_BOOST_EXT_POINTER_TYPES
 * #define SAKE_BOOST_EXT_ALIGNMENT_TYPES
 *
 * (Largely borrowed from Boost.TypeTrait's type_with_alignment.)
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_TYPES_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_TYPES_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/seq/transform.hpp>

#ifndef BOOST_NO_LONG_LONG
#define SAKE_BOOST_EXT_LONG_LONG_SEQ_ELEM ( long long int )
#define SAKE_BOOST_EXT_UNSIGNED_LONG_LONG_SEQ_ELEM ( unsigned long long int )
#else // #ifndef BOOST_NO_LONG_LONG
#define SAKE_BOOST_EXT_LONG_LONG_SEQ_ELEM
#define SAKE_BOOST_EXT_UNSIGNED_LONG_LONG_SEQ_ELEM
#endif // #ifndef BOOST_NO_LONG_LONG

#define SAKE_BOOST_EXT_SIGNED_INTEGRAL_TYPES \
    ( signed char ) \
    ( short int ) \
    ( int ) \
    ( long int ) \
    SAKE_BOOST_EXT_LONG_LONG_SEQ_ELEM
#define SAKE_BOOST_EXT_UNSIGNED_INTEGRAL_TYPES \
    ( unsigned char ) \
    ( unsigned short int ) \
    ( unsigned int ) \
    ( unsigned long int ) \
    SAKE_BOOST_EXT_UNSIGNED_LONG_LONG_SEQ_ELEM
#define SAKE_BOOST_EXT_INTEGRAL_TYPES \
    ( bool ) \
    ( char ) \
    SAKE_BOOST_EXT_SIGNED_INTEGRAL_TYPES \
    SAKE_BOOST_EXT_UNSIGNED_INTEGRAL_TYPES

#define SAKE_BOOST_EXT_FLOATING_POINT_TYPES \
    ( float ) \
    ( double ) \
    ( long double )

#define SAKE_BOOST_EXT_POINTER_TYPES \
    ( void* ) \
    ( ::sake::boost_ext::type_traits_private::fun_ptr_type ) \
    ( ::sake::boost_ext::type_traits_private::mem_obj_s_ptr_type ) \
    ( ::sake::boost_ext::type_traits_private::mem_obj_m_ptr_type ) \
    ( ::sake::boost_ext::type_traits_private::mem_obj_v_ptr_type ) \
    ( ::sake::boost_ext::type_traits_private::mem_obj_i_ptr_type ) \
    ( ::sake::boost_ext::type_traits_private::mem_fun_s_ptr_type ) \
    ( ::sake::boost_ext::type_traits_private::mem_fun_m_ptr_type ) \
    ( ::sake::boost_ext::type_traits_private::mem_fun_v_ptr_type ) \
    ( ::sake::boost_ext::type_traits_private::mem_fun_i_ptr_type )

#define SAKE_BOOST_EXT_BUILTIN_TYPES \
    SAKE_BOOST_EXT_INTEGRAL_TYPES \
    SAKE_BOOST_EXT_FLOATING_POINT_TYPES \
    SAKE_BOOST_EXT_POINTER_TYPES

#define SAKE_BOOST_EXT_ALIGNMENT_TYPES \
    SAKE_BOOST_EXT_UNSIGNED_INTEGRAL_TYPES \
    SAKE_BOOST_EXT_FLOATING_POINT_TYPES \
    SAKE_BOOST_EXT_POINTER_TYPES \
    BOOST_PP_SEQ_TRANSFORM( \
        SAKE_BOOST_EXT_TYPE_TRAITS_PRIVATE_struct_with_member, \
        ~, \
        SAKE_BOOST_EXT_UNSIGNED_INTEGRAL_TYPES \
        SAKE_BOOST_EXT_FLOATING_POINT_TYPES \
        SAKE_BOOST_EXT_POINTER_TYPES \
    )

#define SAKE_BOOST_EXT_TYPE_TRAITS_PRIVATE_struct_with_member( s, data, elem ) \
    ::sake::boost_ext::type_traits_private::struct_with_member< elem >

namespace sake
{

namespace boost_ext
{

namespace type_traits_private
{

struct dummy_base_a { };
struct dummy_base_b { };

struct dummy_s : dummy_base_a { };
struct dummy_m : dummy_base_a, dummy_base_b { };
struct dummy_v : virtual dummy_base_a { };
struct dummy_i;

typedef void (*fun_ptr_type)();

typedef void* dummy_s::*mem_obj_s_ptr_type;
typedef void* dummy_m::*mem_obj_m_ptr_type;
typedef void* dummy_v::*mem_obj_v_ptr_type;
typedef void* dummy_i::*mem_obj_i_ptr_type;

typedef void (dummy_s::*mem_fun_s_ptr_type)();
typedef void (dummy_m::*mem_fun_m_ptr_type)();
typedef void (dummy_v::*mem_fun_v_ptr_type)();
typedef void (dummy_i::*mem_fun_i_ptr_type)();

template< class T >
struct struct_with_member
{ T _; };

} // namespace type_traits_private

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_TYPES_HPP
