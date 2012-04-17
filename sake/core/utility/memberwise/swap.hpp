/*******************************************************************************
 * sake/core/utility/memberwise/swap.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_MEMBERWISE_SWAP[_R]( [r,] T, member_seq )
 *
 * Expands to define a swap member function and friend function equivalent to
 * member-wise swap.
 *
 * member_seq should be a Boost.PP sequence of base class specifiers followed by
 * member variable specifiers. A base class specifier is a Boost.PP 1-sequence
 * simply consisting of the base class; a member variable specifier is a
 * Boost.PP 2-sequence of the form ( type )( name ).
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_MEMBERWISE_SWAP_HPP
#define SAKE_CORE_UTILITY_MEMBERWISE_SWAP_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/to_tuple.hpp>

#include <sake/boost_ext/preprocessor/keyword/typename.hpp>
#include <sake/boost_ext/preprocessor/seq/is_nil.hpp>

#include <sake/core/utility/swap.hpp>

#define SAKE_MEMBERWISE_SWAP( T, member_seq ) \
    SAKE_MEMBERWISE_SWAP_impl( BOOST_PP_DEDUCE_R(), \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( T ), \
        member_seq \
    )
#define SAKE_MEMBERWISE_SWAP_R( r, T, member_seq ) \
    SAKE_MEMBERWISE_SWAP_impl( r, \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( T ), \
        member_seq \
    )

#define SAKE_MEMBERWISE_SWAP_impl( r, T, member_seq ) \
    BOOST_PP_CAT( \
        SAKE_MEMBERWISE_SWAP_impl, \
        SAKE_BOOST_EXT_PP_SEQ_IS_NIL( member_seq ) \
    ) ( r, T, member_seq ) \
    inline friend void swap(T& x, T& y) \
    { x.swap(y); }

#define SAKE_MEMBERWISE_SWAP_impl0( r, T, member_seq ) \
    void swap(T& other) \
    { BOOST_PP_SEQ_FOR_EACH_R( r, SAKE_MEMBERWISE_SWAP_swap_member, ~, member_seq ) }
#define SAKE_MEMBERWISE_SWAP_swap_member( r, data, elem ) \
    SAKE_MEMBERWISE_SWAP_swap_member_impl( \
        BOOST_PP_SEQ_SIZE( elem ), \
        BOOST_PP_SEQ_TO_TUPLE( elem ) \
    )
#define SAKE_MEMBERWISE_SWAP_swap_member_impl( n, member_spec ) \
    SAKE_MEMBERWISE_SWAP_swap_member_expand( \
        BOOST_PP_CAT( SAKE_MEMBERWISE_SWAP_swap_member_, n ), \
        member_spec \
    )
#define SAKE_MEMBERWISE_SWAP_swap_member_expand( macro, x ) macro x
#define SAKE_MEMBERWISE_SWAP_swap_member_1( base ) \
    ::sake::swap(static_cast< base& >(*this), static_cast< base& >(other));
#define SAKE_MEMBERWISE_SWAP_swap_member_2( type, name ) \
    ::sake::swap(name, other.name);

#define SAKE_MEMBERWISE_SWAP_impl1( r, T, member_seq ) \
    static void swap(T&) \
    { }

#endif // #ifndef SAKE_CORE_UTILITY_MEMBERWISE_SWAP_HPP
