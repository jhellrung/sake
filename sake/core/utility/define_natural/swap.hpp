/*******************************************************************************
 * sake/core/utility/define_natural/swap.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_DEFINE_NATURAL_SWAP[_R]( [r,] T, base_seq, member_seq )
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_SWAP_HPP
#define SAKE_CORE_UTILITY_DEFINE_NATURAL_SWAP_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/detail/is_binary.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/tuple/eat.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#include <sake/boost_ext/preprocessor/seq/is_nil.hpp>

#include <sake/core/utility/swap.hpp>

#define SAKE_DEFINE_NATURAL_SWAP( T, base_seq, member_seq ) \
    SAKE_DEFINE_NATURAL_SWAP_R( BOOST_PP_DEDUCE_R(), T, base_seq, member_seq )

#define SAKE_DEFINE_NATURAL_SWAP_R( r, T, base_seq, member_seq ) \
    void swap(T& other) \
    { BOOST_PP_CAT( \
        SAKE_DEFINE_NATURAL_SWAP_bases_, \
        SAKE_BOOST_EXT_PP_SEQ_IS_NIL( base_seq ) \
    ) ( r, T, base_seq, member_seq ) }

#define SAKE_DEFINE_NATURAL_SWAP_bases_0( r, T, base_seq, member_seq ) \
    BOOST_PP_SEQ_FOR_EACH_R( r, SAKE_DEFINE_NATURAL_SWAP_base, ~, base_seq ) \
    SAKE_DEFINE_NATURAL_SWAP_bases_1( r, T, base_seq, member_seq )
#define SAKE_DEFINE_NATURAL_SWAP_base( r, data, elem ) \
    ::sake::swap(static_cast< elem& >(*this), static_cast< elem& >(other));
#define SAKE_DEFINE_NATURAL_SWAP_bases_1( r, T, base_seq, member_seq ) \
    BOOST_PP_IIF( \
        SAKE_BOOST_EXT_PP_SEQ_IS_NIL( member_seq ), \
        BOOST_PP_SEQ_FOR_EACH_R, \
        BOOST_PP_TUPLE_EAT(4) \
    ) ( r, SAKE_DEFINE_NATURAL_SWAP_member, T, member_seq )
#define SAKE_DEFINE_NATURAL_SWAP_member( r, data, elem ) \
    SAKE_DEFINE_NATURAL_SWAP_member_impl( \
        data, \
        BOOST_PP_IIF( \
            BOOST_PP_IS_BINARY( elem ), \
            BOOST_PP_TUPLE_ELEM, \
            elem BOOST_PP_TUPLE_EAT(3) \
        ) ( 2, 1, elem ) \
    )
#define SAKE_DEFINE_NATURAL_SWAP_member_impl( T, member ) \
    static_cast< void >(("assert " BOOST_PP_STRINGIZE( member ) " is non-reference", sizeof( &T::member ))); \
    ::sake::swap(member, other.member);

#endif // #ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_SWAP_HPP
