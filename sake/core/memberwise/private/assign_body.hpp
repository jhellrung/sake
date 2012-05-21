/*******************************************************************************
 * sake/core/memberwise/private/assign_body.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_MEMBERWISE_assign_body( r, T, member_seq )
 ******************************************************************************/

#ifndef SAKE_CORE_MEMBERWISE_PRIVATE_ASSIGN_BODY_HPP
#define SAKE_CORE_MEMBERWISE_PRIVATE_ASSIGN_BODY_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/to_tuple.hpp>

#include <sake/boost_ext/preprocessor/seq/size_01x.hpp>

#include <sake/core/cast/inherit.hpp>
#include <sake/core/move/as_lvalue.hpp>
#include <sake/core/move/move.hpp>

#define SAKE_MEMBERWISE_assign_body( r, T, member_seq ) \
    BOOST_PP_CAT( \
        SAKE_MEMBERWISE_assign_body_, \
        SAKE_BOOST_EXT_PP_SEQ_SIZE_01X( member_seq, x ) \
    ) ( r, T, member_seq )

#define SAKE_MEMBERWISE_assign_body_0( r, T, member_seq ) \
    return *this;

#define SAKE_MEMBERWISE_assign_body_1( r, T, member_seq ) \
    SAKE_MEMBERWISE_assign_body_1_impl( \
        BOOST_PP_SEQ_SIZE( BOOST_PP_SEQ_HEAD( member_seq ) ), \
        BOOST_PP_SEQ_TO_TUPLE( ( T ) BOOST_PP_SEQ_HEAD( member_seq ) ) \
    )
#define SAKE_MEMBERWISE_assign_body_1_impl( n, T_member_spec ) \
    SAKE_MEMBERWISE_assign_body_1_expand( \
        BOOST_PP_CAT( SAKE_MEMBERWISE_assign_body_1_, n ), \
        T_member_spec \
    )
#define SAKE_MEMBERWISE_assign_body_1_expand( macro, x ) macro x
#define SAKE_MEMBERWISE_assign_body_1_1( T, base ) \
    return ::sake::inherit_cast<T>(base::operator=( \
        ::sake::move(::sake::inherit_cast< base >(SAKE_AS_LVALUE(other)))));
#define SAKE_MEMBERWISE_assign_body_1_2( T, type, name ) \
    name = ::sake::move(other.name); \
    return *this;

#define SAKE_MEMBERWISE_assign_body_x( r, T, member_seq ) \
    BOOST_PP_SEQ_FOR_EACH_R( r, \
        SAKE_MEMBERWISE_assign_body_assign_member, ~, member_seq ) \
    return *this;
#define SAKE_MEMBERWISE_assign_body_assign_member( r, data, elem ) \
    SAKE_MEMBERWISE_assign_body_assign_member_impl( \
        BOOST_PP_SEQ_SIZE( elem ), \
        BOOST_PP_SEQ_TO_TUPLE( elem ) \
    )
#define SAKE_MEMBERWISE_assign_body_assign_member_impl( n, member_spec ) \
    SAKE_MEMBERWISE_assign_body_assign_member_expand( \
        BOOST_PP_CAT( SAKE_MEMBERWISE_assign_body_assign_member_, n ), \
        member_spec \
    )
#define SAKE_MEMBERWISE_assign_body_assign_member_expand( macro, x ) macro x
#define SAKE_MEMBERWISE_assign_body_assign_member_1( base ) \
    base::operator=(::sake::move(::sake::inherit_cast< base >(SAKE_AS_LVALUE(other))));
#define SAKE_MEMBERWISE_assign_body_assign_member_2( type, name ) \
    name = ::sake::move(other.name);

#endif // #ifndef SAKE_CORE_MEMBERWISE_PRIVATE_ASSIGN_BODY_HPP
