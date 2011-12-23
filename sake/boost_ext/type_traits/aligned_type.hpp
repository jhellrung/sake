/*******************************************************************************
 * sake/boost_ext/type_traits/aligned_type.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * (Largely borrowed from Boost.TypeTrait's type_with_alignment.)
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_ALIGNED_TYPE_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_ALIGNED_TYPE_HPP

#include <cstddef>

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/alignment_of.hpp>

#include <sake/boost_ext/type_traits/max_align.hpp>
#include <sake/boost_ext/type_traits/types.hpp>

namespace sake
{

namespace boost_ext
{

namespace aligned_type_private
{

template< std::size_t Align >
struct select;

} // namespace aligned_type_private

template< std::size_t Align >
struct aligned_type
{
    BOOST_STATIC_ASSERT(((Align & (Align - 1)) == 0));
    typedef typename aligned_type_private::select< Align >::type type;
};

namespace aligned_type_private
{

template< std::size_t Align, class T = void >
struct select_helper
{
    typedef T type;
    template<
        class U,
        bool = (Align <= boost::alignment_of<U>::value
             && sizeof( U ) < sizeof( T ))
    >
    struct apply;
    template< class U >
    struct apply< U, false >
    { typedef select_helper type; };
    template< class U >
    struct apply< U, true >
    { typedef select_helper< Align, U > type; };
};

template< std::size_t Align >
struct select_helper< Align, void >
{
    typedef void type;
    template<
        class U,
        bool = (Align <= boost::alignment_of<U>::value)
    >
    struct apply;
    template< class U >
    struct apply< U, false >
    { typedef select_helper type; };
    template< class U >
    struct apply< U, true >
    { typedef select_helper< Align, U > type; };
};

#define apply_elem_type( r, data, elem ) ::template apply< elem >::type

template< std::size_t Align >
struct select
    : select_helper< Align >
    BOOST_PP_SEQ_FOR_EACH( apply_elem_type, ~, SAKE_BOOST_EXT_ALIGNMENT_TYPES )
{ };

#undef apply_elem_type

#ifdef __GNUC__
struct aligned_2   { unsigned char _[  2]; } __attribute__((__aligned__(  2)));
struct aligned_4   { unsigned char _[  4]; } __attribute__((__aligned__(  4)));
struct aligned_8   { unsigned char _[  8]; } __attribute__((__aligned__(  8)));
struct aligned_16  { unsigned char _[ 16]; } __attribute__((__aligned__( 16)));
struct aligned_32  { unsigned char _[ 32]; } __attribute__((__aligned__( 32)));
struct aligned_64  { unsigned char _[ 64]; } __attribute__((__aligned__( 64)));
struct aligned_128 { unsigned char _[128]; } __attribute__((__aligned__(128)));
#endif // #ifdef __GNUC__

#ifdef _MSC_VER
struct __declspec(align(  2)) aligned_2   { unsigned char _[  2]; };
struct __declspec(align(  4)) aligned_4   { unsigned char _[  4]; };
struct __declspec(align(  8)) aligned_8   { unsigned char _[  8]; };
struct __declspec(align( 16)) aligned_16  { unsigned char _[ 16]; };
struct __declspec(align( 32)) aligned_32  { unsigned char _[ 32]; };
struct __declspec(align( 64)) aligned_64  { unsigned char _[ 64]; };
struct __declspec(align(128)) aligned_128 { unsigned char _[128]; };
#endif // #ifdef _MSC_VER

#if defined( __GNUC__ ) || defined( _MSC_VER )

BOOST_STATIC_ASSERT((boost_ext::max_align <= 128));

BOOST_STATIC_ASSERT((boost::alignment_of< unsigned char >::value == 1));
BOOST_STATIC_ASSERT((boost::alignment_of< aligned_2   >::value ==   2));
BOOST_STATIC_ASSERT((boost::alignment_of< aligned_4   >::value ==   4));
BOOST_STATIC_ASSERT((boost::alignment_of< aligned_8   >::value ==   8));
BOOST_STATIC_ASSERT((boost::alignment_of< aligned_16  >::value ==  16));
BOOST_STATIC_ASSERT((boost::alignment_of< aligned_32  >::value ==  32));
BOOST_STATIC_ASSERT((boost::alignment_of< aligned_64  >::value ==  64));
BOOST_STATIC_ASSERT((boost::alignment_of< aligned_128 >::value == 128));

template<> struct select<  1> { typedef unsigned char type; };
template<> struct select<  2> { typedef aligned_2   type; };
template<> struct select<  4> { typedef aligned_4   type; };
template<> struct select<  8> { typedef aligned_8   type; };
template<> struct select< 16> { typedef aligned_16  type; };
template<> struct select< 32> { typedef aligned_32  type; };
template<> struct select< 64> { typedef aligned_64  type; };
template<> struct select<128> { typedef aligned_128 type; };

#endif // #if defined( __GNUC__ ) || defined( _MSC_VER )

} // namespace aligned_type_private

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_ALIGNED_TYPE_HPP
