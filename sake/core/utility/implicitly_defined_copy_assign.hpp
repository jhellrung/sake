/*******************************************************************************
 * sake/core/utility/implicitly_defined_copy_assign.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_IMPLICITLY_DEFINED_COPY_ASSIGN[_R]( [r,] T, base_seq, member_seq )
 * #define SAKE_IMPLICITLY_DEFINED_COPY_ASSIGN_IF_ANY_UMC[_R]( [r,] T, base_seq, member_seq )
 * #define SAKE_IMPLICITLY_DEFINED_COPY_ASSIGN_IMPL[_R]( [r,], T, base_seq, member_seq )
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_IMPLICITLY_DEFINED_COPY_ASSIGN_HPP
#define SAKE_CORE_UTILITY_IMPLICITLY_DEFINED_COPY_ASSIGN_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>

#include <sake/core/utility/private/implicitly_defined/assign_body.hpp>

#define SAKE_IMPLICITLY_DEFINED_COPY_ASSIGN( T, operator_assign, base_seq, member_seq ) \
    SAKE_IMPLICITLY_DEFINED_COPY_ASSIGN_R( BOOST_PP_DEDUCE_R(), T, base_seq, member_seq )

#define SAKE_IMPLICITLY_DEFINED_COPY_ASSIGN_IF_ANY_UMC( T, base_seq, member_seq ) \
    SAKE_IMPLICITLY_DEFINED_COPY_ASSIGN_IF_ANY_UMC_R( BOOST_PP_DEDUCE_R(), T, base_seq, member_seq )

#if !defined( BOOST_NO_RVALUE_REFERENCES ) \
 && !defined( BOOST_NO_DEFAULTED_FUNCTIONS )

#define SAKE_IMPLICITLY_DEFINED_COPY_ASSIGN_R( r, T, base_seq, member_seq ) \
    T& operator=(T const &) = default;

#define SAKE_IMPLICITLY_DEFINED_COPY_ASSIGN_IF_ANY_UMC_R( r, T, base_seq, member_seq ) \
    T& operator=(T const &) = default;

#else // #if !defined(...) && !defined(...)

#include <boost/mpl/if.hpp>
#include <boost/mpl/not.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/preprocessor/seq/is_nil.hpp>

#include <sake/core/move/is_unfriendly_movable_copyable.hpp>
#include <sake/core/utility/private/implicitly_defined/disabler.hpp>

#define SAKE_IMPLICITLY_DEFINED_COPY_ASSIGN_R( r, T, base_seq, member_seq ) \
    T& operator=(this_copy_assign_param_type other) \
    { SAKE_IMPLICITLY_DEFINED_assign_body( r, T, base_seq, member_seq ) }

#define SAKE_IMPLICITLY_DEFINED_COPY_ASSIGN_IF_ANY_UMC_R( r, T, base_seq, member_seq ) \
    T& operator=(typename ::boost::mpl::if_c< \
        SAKE_IMPLICITLY_DEFINED_COPY_ASSIGN_any_umc( r, T, base_seq, member_seq ) ::value \
        T const &, \
        ::sake::implicitly_defined_private::disabler<T> \
    >::type other) \
    { SAKE_IMPLICITLY_DEFINED_assign_body( r, T, base_seq, member_seq ) }

#define SAKE_IMPLICITLY_DEFINED_COPY_ASSIGN_any_umc( r, T, base_seq, member_seq ) \
    BOOST_PP_CAT( \
        SAKE_IMPLICITLY_DEFINED_COPY_ASSIGN_any_umc_, \
        BOOST_PP_CAT( \
            SAKE_BOOST_EXT_PP_SEQ_IS_NIL( base_seq ), \
            SAKE_BOOST_EXT_PP_SEQ_IS_NIL( member_seq ) \
        ) \
    ) ( r, T, base_seq, member_seq )
#define SAKE_IMPLICITLY_DEFINED_COPY_ASSIGN_any_umc_00( r, T, base_seq, member_seq ) \
    ::sake::boost_ext::mpl::or2< \
        SAKE_IMPLICITLY_DEFINED_COPY_ASSIGN_any_umc_01( r, T, base_seq, member_seq ), \
        SAKE_IMPLICITLY_DEFINED_COPY_ASSIGN_any_umc_10( r, T, base_seq, member_seq ) \
    >
#define SAKE_IMPLICITLY_DEFINED_COPY_ASSIGN_any_umc_01( r, T, base_seq, member_seq ) \
    BOOST_PP_CAT( ::sake::boost_ext::mpl::or, BOOST_PP_SEQ_SIZE( base_seq ) ) < \
        BOOST_PP_SEQ_FOR_EACH_I_R( r, SAKE_IMPLICITLY_DEFINED_COPY_ASSIGN_comma_is_umc, ~, base_seq ) \
    >
#define SAKE_IMPLICITLY_DEFINED_COPY_ASSIGN_any_umc_10( r, T, base_seq, member_seq ) \
    BOOST_PP_CAT( ::sake::boost_ext::mpl::or, BOOST_PP_SEQ_SIZE( member_seq ) ) < \
        BOOST_PP_SEQ_FOR_EACH_I_R( r, SAKE_IMPLICITLY_DEFINED_COPY_ASSIGN_comma_member_is_umc, ~, member_seq )
#define SAKE_IMPLICITLY_DEFINED_COPY_ASSIGN_any_umc_11( r, T, base_seq, member_seq ) \
    ::boost::integral_constant< bool, false >

#define SAKE_IMPLICITLY_DEFINED_COPY_ASSIGN_comma_is_umc( r, data, i, elem ) \
    BOOST_PP_COMMA_IF( i ) ::sake::is_unfriendly_movable_copyable< elem >
#define SAKE_IMPLICITLY_DEFINED_COPY_ASSIGN_comma_member_is_umc( r, data, i, elem ) \
    SAKE_IMPLICITLY_DEFINED_COPY_ASSIGN_comma_is_umc( r, data, i, BOOST_PP_TUPLE_ELEM( 2, 0, elem ) )

#endif // #if !defined(...) && !defined(...)

#define SAKE_IMPLICITLY_DEFINED_COPY_ASSIGN_IMPL( T, operator_assign, base_seq, member_seq ) \
    SAKE_IMPLICITLY_DEFINED_COPY_ASSIGN_IMPL_R( BOOST_PP_DEDUCE_R(), T, base_seq, member_seq )

#define SAKE_IMPLICITLY_DEFINED_COPY_ASSIGN_IMPL_R( r, T, base_seq, member_seq ) \
    T& copy_assign_impl(T const & other) \
    { SAKE_IMPLICITLY_DEFINED_assign_body( r, T, base_seq, member_seq ) }

#endif // #ifndef SAKE_CORE_UTILITY_IMPLICITLY_DEFINED_COPY_ASSIGN_HPP
