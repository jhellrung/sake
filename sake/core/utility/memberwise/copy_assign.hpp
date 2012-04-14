/*******************************************************************************
 * sake/core/utility/memberwise/copy_assign.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_MEMBERWISE_COPY_ASSIGN[_R]( [r,] T, base_seq, member_name_seq )
 * #define SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_UMC[_R]( [r,] typenameT, base_seq, member_type_name_seq )
 * #define SAKE_MEMBERWISE_COPY_ASSIGN_IMPL[_R]( [r,], T, base_seq, member_name_seq )
 *
 * Expands to define a copy assignment operator or copy_assign_impl member
 * function equivalent to member-wise copy assignment.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_COPY_ASSIGN_HPP
#define SAKE_CORE_UTILITY_DEFINE_NATURAL_COPY_ASSIGN_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>

#include <sake/core/utility/memberwise/private/assign_body.hpp>

#define SAKE_MEMBERWISE_COPY_ASSIGN( T, base_seq, member_name_seq ) \
    SAKE_MEMBERWISE_COPY_ASSIGN_R( BOOST_PP_DEDUCE_R(), \
        T, base_seq, member_name_seq )

#define SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_UMC( typenameT, base_seq, member_type_name_seq ) \
    SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_UMC_R( BOOST_PP_DEDUCE_R(), \
        typenameT, base_seq, member_type_name_seq )

#if !defined( BOOST_NO_DEFAULTED_FUNCTIONS ) \
 && !defined( BOOST_NO_RVALUE_REFERENCES )

#define SAKE_MEMBERWISE_COPY_ASSIGN_R( r, T, base_seq, member_name_seq ) \
    T& operator=(T const &) = default;

#else // #if !defined(...) && !defined(...)

#define SAKE_MEMBERWISE_COPY_ASSIGN_R( r, T, base_seq, member_name_seq ) \
    T& operator=(this_copy_assign_param_type other) \
    { SAKE_MEMBERWISE_assign_body( r, T, base_seq, member_name_seq ) }

#endif // #if !defined(...) && !defined(...)

#ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_UMC_R( r, typenameT, base_seq, member_type_name_seq )

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

#include <boost/mpl/if.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/seq/cat.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/size.hpp>

#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/preprocessor/keyword/typename.hpp>
#include <sake/boost_ext/preprocessor/seq/is_nil.hpp>
#include <sake/boost_ext/preprocessor/seq/join.hpp>
#include <sake/boost_ext/preprocessor/tuple/rem.hpp>

#include <sake/core/move/is_unfriendly_movable_copyable.hpp>
#include <sake/core/utility/memberwise/private/disabler.hpp>
#include <sake/core/utility/memberwise/private/seq_transform_tuple_elem.hpp>

#define SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_UMC_R( r, typenameT, base_seq, member_type_name_seq ) \
    SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_UMC_impl0( r, \
        SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_TYPENAME( typenameT ) BOOST_PP_EMPTY, \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( typenameT ), \
        base_seq, \
        SAKE_MEMBERWISE_seq_transform_tuple_elem( r, 0, member_type_name_seq ), \
        SAKE_MEMBERWISE_seq_transform_tuple_elem( r, 0, member_type_name_seq ) \
    )
#define SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_UMC_impl0( r, \
    typename, T, base_seq, member_type_seq, member_name_seq ) \
    SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_UMC_impl1( r, \
        typename, T, \
        SAKE_BOOST_EXT_PP_SEQ_JOIN( ( base_seq ) ( member_type_seq ) ), \
        base_seq, \
        member_name_seq \
    )
#define SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_UMC_impl1( r, typename, T, type_seq, base_seq, member_name_seq ) \
    typedef SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_UMC_param_type( r, typename, T, type_seq ) \
        _sake_memberwise_copy_assign_if_any_umc_param_type; \
    T& operator=(_sake_memberwise_copy_assign_if_any_umc_param_type other) \
    { SAKE_MEMBERWISE_assign_body( r, T, base_seq, member_name_seq ) }

#define SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_UMC_param_type( r, typename, T, type_seq ) \
    BOOST_PP_CAT( \
        SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_UMC_param_type_, \
        SAKE_BOOST_EXT_PP_SEQ_IS_NIL( type_seq ) \
    ) ( r, typename, T, type_seq )
#define SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_UMC_param_type_0( r, typename, T, type_seq ) \
    typename() ::boost::mpl::if_c< \
        SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_UMC_any_umc( r, type_seq ) ::value, \
        T const &, \
        ::sake::memberwise_private::disabler<T> \
    >::type
#define SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_UMC_param_type_1( r, typename, T, type_seq ) \
    ::sake::memberwise_private::disabler<T>

#define SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_UMC_any_umc( r, type_seq ) \
    BOOST_PP_CAT( \
        SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_UMC_any_umc_, \
        BOOST_PP_EQUAL( 1, BOOST_PP_SEQ_SIZE( type_seq ) ) \
    ) ( r, type_seq )
#define SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_UMC_any_umc_0( r, type_seq ) \
    ::sake::boost_ext::mpl::BOOST_PP_CAT( or, BOOST_PP_SEQ_SIZE( type_seq ) )< \
        BOOST_PP_SEQ_FOR_EACH_I_R( r, \
            SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_UMC_comma_is_umc, ~, type_seq ) \
    >
#define SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_UMC_comma_is_umc( r, data, i, elem ) \
    BOOST_PP_COMMA_IF( i ) ::sake::is_unfriendly_movable_copyable< elem >
#define SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_UMC_any_umc_1( r, type_seq ) \
    ::sake::is_unfriendly_movable_copyable< SAKE_BOOST_EXT_PP_TUPLE_REM1 type_seq >

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_MEMBERWISE_COPY_ASSIGN_IMPL( T, operator_assign, base_seq, member_name_seq ) \
    SAKE_MEMBERWISE_COPY_ASSIGN_IMPL_R( BOOST_PP_DEDUCE_R(), \
        T, base_seq, member_name_seq )

#define SAKE_MEMBERWISE_COPY_ASSIGN_IMPL_R( r, T, base_seq, member_name_seq ) \
    T& copy_assign_impl(T const & other) \
    { SAKE_MEMBERWISE_assign_body( r, T, base_seq, member_name_seq ) }

#endif // #ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_COPY_ASSIGN_HPP
