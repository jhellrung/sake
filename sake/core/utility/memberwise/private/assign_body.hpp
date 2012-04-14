/*******************************************************************************
 * sake/core/utility/memberwise/private/assign_body.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_MEMBERWISE_assign_body( r, T, base_seq, member_name_seq )
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_PRIVATE_ASSIGN_BODY_HPP
#define SAKE_CORE_UTILITY_DEFINE_NATURAL_PRIVATE_ASSIGN_BODY_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/size.hpp>

#include <sake/boost_ext/preprocessor/seq/is_nil.hpp>
#include <sake/boost_ext/preprocessor/tuple/rem.hpp>

#include <sake/core/move/move.hpp>
#include <sake/core/utility/inherit_cast.hpp>

#define SAKE_MEMBERWISE_assign_body( r, T, base_seq, member_name_seq ) \
    BOOST_PP_SEQ_CAT( \
        ( SAKE_MEMBERWISE_assign_body_ ) \
        ( SAKE_BOOST_EXT_PP_SEQ_IS_NIL( base_seq ) ) \
        ( SAKE_BOOST_EXT_PP_SEQ_IS_NIL( member_name_seq ) ) \
    ) ( r, T, base_seq, member_name_seq )

#define SAKE_MEMBERWISE_assign_body_00( r, T, base_seq, member_name_seq ) \
    BOOST_PP_SEQ_FOR_EACH_R( r, SAKE_MEMBERWISE_assign_base, ~, base_seq ) \
    BOOST_PP_SEQ_FOR_EACH_R( r, SAKE_MEMBERWISE_assign_member, T, member_name_seq ) \
    return *this;

#define SAKE_MEMBERWISE_assign_body_01( r, T, base_seq, member_name_seq ) \
    BOOST_PP_CAT( \
        SAKE_MEMBERWISE_assign_body_01_, \
        BOOST_PP_EQUAL( 1, BOOST_PP_SEQ_SIZE( base_seq ) ) \
    ) ( r, base_seq )
#define SAKE_MEMBERWISE_assign_body_01_0( r, base_seq ) \
    BOOST_PP_SEQ_FOR_EACH_R( r, SAKE_MEMBERWISE_assign_base, ~, base_seq ) \
    return *this;
#define SAKE_MEMBERWISE_assign_body_01_1( r, base_seq ) \
    return ::sake::inherit_cast<T>(SAKE_BOOST_EXT_PP_TUPLE_REM1 base_seq::operator=( \
        ::sake::move(::sake::inherit_cast< SAKE_BOOST_EXT_PP_TUPLE_REM1 base_seq >(other))));

#define SAKE_MEMBERWISE_assign_body_10( r, T, base_seq, member_name_seq ) \
    BOOST_PP_SEQ_FOR_EACH_R( r, SAKE_MEMBERWISE_assign_member, T, member_name_seq ) \
    return *this;

#define SAKE_MEMBERWISE_assign_body_11( r, T, base_seq, member_name_seq ) \
    return *this;

#define SAKE_MEMBERWISE_assign_base( r, data, elem ) \
    elem::operator=(::sake::move(::sake::inherit_cast< elem >(other)));
#define SAKE_MEMBERWISE_assign_member( r, data, elem ) \
    static_cast< void >(sizeof( "assert non-reference", &data::elem )); \
    elem = ::sake::move(other.elem);

#endif // #ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_PRIVATE_ASSIGN_BODY_HPP
