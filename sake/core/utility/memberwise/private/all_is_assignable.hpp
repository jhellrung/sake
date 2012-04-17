/*******************************************************************************
 * sake/core/utility/memberwise/private/all_is_assignable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_MEMBERWISE_all_is_assignable( r, member_seq )
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_MEMBERWISE_PRIVATE_ALL_IS_ASSIGNABLE_HPP
#define SAKE_CORE_UTILITY_MEMBERWISE_PRIVATE_ALL_IS_ASSIGNABLE_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/size.hpp>

#include <sake/boost_ext/mpl/and.hpp>

#include <sake/core/utility/memberwise/private/is_assignable.hpp>

#define SAKE_MEMBERWISE_all_is_assignable( r, member_seq ) \
    BOOST_PP_CAT( \
        SAKE_MEMBERWISE_all_is_assignable_, \
        BOOST_PP_EQUAL( 1, BOOST_PP_SEQ_SIZE( member_seq ) ) \
    ) ( r, member_seq )

#define SAKE_MEMBERWISE_all_is_assignable_0( r, member_seq ) \
    ::sake::boost_ext::mpl::BOOST_PP_CAT( and, BOOST_PP_SEQ_SIZE( member_seq ) )< \
        BOOST_PP_SEQ_FOR_EACH_I_R( r, \
            SAKE_MEMBERWISE_comma_is_assignable_member, ~, member_seq ) \
    >
#define SAKE_MEMBERWISE_comma_is_assignable_member( r, data, i, elem ) \
    BOOST_PP_COMMA_IF( i ) \
    ::sake::memberwise_private::is_assignable< BOOST_PP_SEQ_HEAD( elem ) >

#define SAKE_MEMBERWISE_all_is_assignable_1( r, member_seq ) \
    ::sake::memberwise_private::is_assignable< \
        BOOST_PP_SEQ_HEAD( BOOST_PP_SEQ_HEAD( member_seq ) ) >

#endif // #ifndef SAKE_CORE_UTILITY_MEMBERWISE_PRIVATE_ALL_IS_ASSIGNABLE_HPP
