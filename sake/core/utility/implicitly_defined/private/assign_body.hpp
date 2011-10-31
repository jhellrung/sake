/*******************************************************************************
 * sake/core/utility/implicitly_defined/private/assign_body.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_IMPLICITLY_DEFINED_assign_body( r, T, base_seq, member_seq )
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_IMPLICITLY_DEFINED_PRIVATE_ASSIGN_BODY_HPP
#define SAKE_CORE_UTILITY_IMPLICITLY_DEFINED_PRIVATE_ASSIGN_BODY_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/detail/is_binary.hpp>
#include <boost/preprocessor/facilities/apply.hpp>
#include <boost/preprocessor/logical/bitand.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/tuple/eat.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#include <sake/boost_ext/preprocessor/seq/is_nil.hpp>

#include <sake/core/move/move.hpp>
#include <sake/core/utility/inherit_cast.hpp>

#define SAKE_IMPLICITLY_DEFINED_assign_body( r, T, base_seq, member_seq ) \
    BOOST_PP_CAT( \
        SAKE_IMPLICITLY_DEFINED_assign_bases_, \
        SAKE_BOOST_EXT_PP_SEQ_IS_NIL( base_seq ) \
    ) ( r, T, base_seq, member_seq ) \

#define SAKE_IMPLICITLY_DEFINED_assign_bases_0( r, T, base_seq, member_seq ) \
    BOOST_PP_CAT( \
        SAKE_IMPLICITLY_DEFINED_assign_bases_0_, \
        BOOST_PP_BITAND( \
            BOOST_PP_EQUAL( 1, BOOST_PP_SEQ_SIZE( base_seq ) ), \
            SAKE_BOOST_EXT_PP_SEQ_IS_NIL( member_seq ) \
        ) \
    ) ( r, T, base_seq, member_seq )
#define SAKE_IMPLICITLY_DEFINED_assign_bases_0_0( r, T, base_seq, member_seq ) \
    BOOST_PP_SEQ_FOR_EACH_R( r, SAKE_IMPLICITLY_DEFINED_assign_base, ~, base_seq ) \
    SAKE_IMPLICITLY_DEFINED_assign_bases_1( r, T, base_seq, member_seq )
#define SAKE_IMPLICITLY_DEFINED_assign_base( r, data, elem ) \
    elem::operator=(::sake::move(::sake::inherit_cast< elem >(other)));
#define SAKE_IMPLICITLY_DEFINED_assign_bases_0_1( r, T, base_seq, member_seq ) \
    return ::sake::inherit_cast<T>( BOOST_PP_APPLY( base_seq ) ::operator=( \
        ::sake::move(::sake::inherit_cast< BOOST_PP_APPLY( base_seq ) >(other)) \
    ));

#define SAKE_IMPLICITLY_DEFINED_assign_bases_1( r, T, base_seq, member_seq ) \
    BOOST_PP_CAT( \
        SAKE_IMPLICITLY_DEFINED_assign_members_, \
        SAKE_BOOST_EXT_PP_SEQ_IS_NIL( member_seq ) \
    ) ( r, T, member_seq ) \
    return *this;
#define SAKE_IMPLICITLY_DEFINED_assign_members_0( r, T, member_seq ) \
    BOOST_PP_SEQ_FOR_EACH_R( r, SAKE_IMPLICITLY_DEFINED_assign_member, T, member_seq )
#define SAKE_IMPLICITLY_DEFINED_assign_member( r, data, elem ) \
    SAKE_IMPLICITLY_DEFINED_assign_member_impl( \
        data, \
        BOOST_PP_IIF( \
            BOOST_PP_IS_BINARY( elem ), \
            BOOST_PP_TUPLE_ELEM, \
            elem BOOST_PP_TUPLE_EAT(3) \
        ) ( 2, 1, elem ) \
    )
// The expression &T::member ensures the member is a non-reference type.
#define SAKE_IMPLICITLY_DEFINED_assign_member_impl( T, member ) \
    static_cast< void >(sizeof( &T::member )); \
    member = ::sake::move(other.member);
#define SAKE_IMPLICITLY_DEFINED_assign_members_1( r, T, member_seq )

#endif // #ifndef SAKE_CORE_UTILITY_IMPLICITLY_DEFINED_PRIVATE_ASSIGN_BODY_HPP
