/*******************************************************************************
 * sake/core/utility/define_natural/swap.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_DEFINE_NATURAL_SWAP[_R]( [r,] T, base_seq, member_name_seq )
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_SWAP_HPP
#define SAKE_CORE_UTILITY_DEFINE_NATURAL_SWAP_HPP

#include <boost/preprocessor/repetition/deduce_r.hpp>
#include <boost/preprocessor/seq/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

#include <sake/boost_ext/preprocessor/seq/is_nil.hpp>

#include <sake/core/utility/swap.hpp>

#define SAKE_DEFINE_NATURAL_SWAP( T, base_seq, member_name_seq ) \
    SAKE_DEFINE_NATURAL_SWAP_R( BOOST_PP_DEDUCE_R(), T, base_seq, member_name_seq )

#define SAKE_DEFINE_NATURAL_SWAP_R( r, T, base_seq, member_name_seq ) \
    BOOST_PP_SEQ_CAT( \
        ( SAKE_DEFINE_NATURAL_SWAP_ ) \
        ( SAKE_BOOST_EXT_PP_SEQ_IS_NIL( base_seq ) ) \
        ( SAKE_BOOST_EXT_PP_SEQ_IS_NIL( member_name_seq ) ) \
    ) ( r, T, base_seq, member_name_seq ) \
    inline friend void swap(T& x, T& y) \
    { x.swap(y); }

#define SAKE_DEFINE_NATURAL_SWAP_00( r, T, base_seq, member_name_seq ) \
    void swap(T& other) \
    { \
        BOOST_PP_SEQ_FOR_EACH_R( r, SAKE_DEFINE_NATURAL_SWAP_base, ~, base_seq ) \
        BOOST_PP_SEQ_FOR_EACH_R( r, SAKE_DEFINE_NATURAL_SWAP_member, ~, member_name_seq ) \
    }
#define SAKE_DEFINE_NATURAL_SWAP_01( r, T, base_seq, member_name_seq ) \
    void swap(T& other) \
    { BOOST_PP_SEQ_FOR_EACH_R( r, SAKE_DEFINE_NATURAL_SWAP_base, ~, base_seq ) }
#define SAKE_DEFINE_NATURAL_SWAP_10( r, T, base_seq, member_name_seq ) \
    void swap(T& other) \
    { BOOST_PP_SEQ_FOR_EACH_R( r, SAKE_DEFINE_NATURAL_SWAP_member, ~, member_name_seq ) }
#define SAKE_DEFINE_NATURAL_SWAP_11( r, T, base_seq, member_name_seq ) \
    static void swap(T&) \
    { }

#define SAKE_DEFINE_NATURAL_SWAP_base( r, data, elem ) \
    ::sake::swap(static_cast< elem& >(*this), static_cast< elem& >(other));
#define SAKE_DEFINE_NATURAL_SWAP_member( r, data, elem ) \
    static_cast< void >(sizeof( "assert non-reference", &data::elem )); \
    ::sake::swap(elem, other.elem);

#endif // #ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_SWAP_HPP
