/*******************************************************************************
 * sake/core/utility/memberwise/copy_assign.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_MEMBERWISE_COPY_ASSIGN[_R]( [r,] typenameT, member_seq )
 * #define SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_HAS_UME[_R]( [r,] typenameT, member_seq )
 * #define SAKE_MEMBERWISE_COPY_ASSIGN_IMPL[_R]( [r,], T, member_seq )
 *
 * Expands to define a copy assignment operator or copy_assign_impl member
 * function equivalent to member-wise copy assignment.
 *
 * member_seq should be a Boost.PP sequence of base class specifiers followed by
 * member variable specifiers. A base class specifier is a Boost.PP 1-sequence
 * simply consisting of the base class; a member variable specifier is a
 * Boost.PP 2-sequence of the form ( type )( name ).
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_MEMBERWISE_COPY_ASSIGN_HPP
#define SAKE_CORE_UTILITY_MEMBERWISE_COPY_ASSIGN_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>

#include <sake/boost_ext/preprocessor/keyword/typename.hpp>

#include <sake/core/type_traits/has_nothrow_copy_assign.hpp>
#include <sake/core/utility/memberwise/type_trait_tag.hpp>

/*******************************************************************************
 * #define SAKE_MEMBERWISE_COPY_ASSIGN[_R]( [r,] typenameT, member_seq )
 ******************************************************************************/

#define SAKE_MEMBERWISE_COPY_ASSIGN( typenameT, member_seq ) \
    SAKE_MEMBERWISE_COPY_ASSIGN_R( BOOST_PP_DEDUCE_R(), typenameT, member_seq )
#define SAKE_MEMBERWISE_COPY_ASSIGN_R( r, typenameT, member_seq ) \
    SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG_R( r, member_seq, has_nothrow_copy_assign ) \
    SAKE_MEMBERWISE_COPY_ASSIGN_impl( r, \
        SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_TYPENAME( typenameT ) BOOST_PP_EMPTY, \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( typenameT ), \
        member_seq \
    )

#ifndef BOOST_NO_DEFAULTED_FUNCTIONS

#define SAKE_MEMBERWISE_COPY_ASSIGN_impl( r, typename, T, member_seq ) \
    T& operator=(T const &) = default;

#else // #ifndef BOOST_NO_DEFAULTED_FUNCTIONS

#include <boost/preprocessor/cat.hpp>
#include <boost/mpl/if.hpp>

#include <sake/boost_ext/preprocessor/seq/is_nil.hpp>

#include <sake/core/utility/memberwise/private/assign_body.hpp>
#include <sake/core/utility/memberwise/private/copy_assign_enable.hpp>

#ifndef BOOST_NO_RVALUE_REFERENCES
#define SAKE_MEMBERWISE_COPY_ASSIGN_param_type( T ) T const & 
#else // #ifndef BOOST_NO_RVALUE_REFERENCES
#define SAKE_MEMBERWISE_COPY_ASSIGN_param_type( T ) this_copy_assign_param_type
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_MEMBERWISE_COPY_ASSIGN_impl( r, typename, T, member_seq ) \
    BOOST_PP_CAT( \
        SAKE_MEMBERWISE_COPY_ASSIGN_impl, \
        SAKE_BOOST_EXT_PP_SEQ_IS_NIL( member_seq ) \
    ) ( r, typename, T, member_seq )

#define SAKE_MEMBERWISE_COPY_ASSIGN_impl0( r, typename, T, member_seq ) \
    typedef typename() ::boost::mpl::if_c< \
        SAKE_MEMBERWISE_copy_assign_enable( r, member_seq )::value, \
        SAKE_MEMBERWISE_COPY_ASSIGN_param_type( T ), \
        ::sake::memberwise_copy_assign_private::disabler<T>& \
    >::type _sake_memberwise_copy_assign_param_type; \
    T& operator=(_sake_memberwise_copy_assign_param_type other) \
        BOOST_NOEXCEPT_IF((has_nothrow_copy_assign_tag::value)) \
    { SAKE_MEMBERWISE_assign_body( r, T, member_seq ) }

#define SAKE_MEMBERWISE_COPY_ASSIGN_impl1( r, typename, T, member_seq ) \
    T& operator=(SAKE_MEMBERWISE_COPY_ASSIGN_param_type( T )) \
        BOOST_NOEXCEPT \
    { return *this; }

#endif // #ifndef BOOST_NO_DEFAULTED_FUNCTIONS

/*******************************************************************************
 * #define SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_HAS_UME[_R]( [r,] typenameT, member_seq )
 ******************************************************************************/

#define SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_HAS_UME( typenameT, member_seq ) \
    SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_HAS_UME_R( BOOST_PP_DEDUCE_R(), typenameT, member_seq )
#define SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_HAS_UME_R( r, typenameT, member_seq ) \
    SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG_R( r, member_seq, has_nothrow_copy_assign ) \
    SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_HAS_UME_impl( r, \
        SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_TYPENAME( typenameT ) BOOST_PP_EMPTY, \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( typenameT ), \
        member_seq \
    )

#ifndef BOOST_NO_RVALUE_REFERENCES

#ifndef BOOST_NO_DEFAULTED_FUNCTIONS

#define SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_HAS_UME_impl( r, typename, T, member_seq ) \
    T& operator=(T const &) = default;

#else // #ifndef BOOST_NO_DEFAULTED_FUNCTIONS

#define SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_HAS_UME_impl( r, typename, T, member_seq )

#endif // #ifndef BOOST_NO_DEFAULTED_FUNCTIONS

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

#include <boost/mpl/if.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/size.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/preprocessor/seq/is_nil.hpp>

#include <sake/core/move/has_unfriendly_move_emulation.hpp>
#include <sake/core/utility/memberwise/private/assign_body.hpp>
#include <sake/core/utility/memberwise/private/copy_assign_enable.hpp>

#define SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_HAS_UME_impl( r, typename, T, member_seq ) \
    BOOST_PP_CAT( \
        SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_HAS_UME_impl, \
        SAKE_BOOST_EXT_PP_SEQ_IS_NIL( member_seq ) \
    ) ( r, typename, T, member_seq )

#define SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_HAS_UME_impl0( r, typename, T, member_seq ) \
    typedef typename() ::boost::mpl::if_c< \
        ::sake::boost_ext::mpl::and2< \
            SAKE_MEMBERWISE_copy_assign_enable( r, member_seq ), \
            SAKE_MEMBERWISE_COPY_ASSIGN_any_has_ume( r, member_seq ) \
        >::value, \
        T const &, \
        ::sake::memberwise_copy_assign_private::disabler<T>& \
    >::type _sake_memberwise_copy_assign_param_type; \
    T& operator=(_sake_memberwise_copy_assign_param_type other) \
        BOOST_NOEXCEPT_IF((has_nothrow_copy_assign_tag::value)) \
    { SAKE_MEMBERWISE_assign_body( r, T, member_seq ) }

#define SAKE_MEMBERWISE_COPY_ASSIGN_any_has_ume( r, member_seq ) \
    BOOST_PP_CAT( \
        SAKE_MEMBERWISE_COPY_ASSIGN_any_has_ume_, \
        BOOST_PP_EQUAL( 1, BOOST_PP_SEQ_SIZE( member_seq ) ) \
    ) ( r, member_seq )
#define SAKE_MEMBERWISE_COPY_ASSIGN_any_has_ume_0( r, member_seq ) \
    ::sake::boost_ext::mpl::BOOST_PP_CAT( or, BOOST_PP_SEQ_SIZE( member_seq ) )< \
        BOOST_PP_SEQ_FOR_EACH_I_R( r, \
            SAKE_MEMBERWISE_COPY_ASSIGN_comma_has_ume_member, ~, member_seq ) \
    >
#define SAKE_MEMBERWISE_COPY_ASSIGN_comma_has_ume_member( r, data, i, elem ) \
    BOOST_PP_COMMA_IF( i ) \
    ::sake::has_unfriendly_move_emulation< BOOST_PP_SEQ_HEAD( elem ) >
#define SAKE_MEMBERWISE_COPY_ASSIGN_any_has_ume_1( r, member_seq ) \
    ::sake::has_unfriendly_move_emulation< BOOST_PP_SEQ_HEAD( BOOST_PP_SEQ_HEAD( member_seq ) ) >

#define SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_HAS_UME_impl1( r, typename, T, member_seq )

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

/*******************************************************************************
 * #define SAKE_MEMBERWISE_COPY_ASSIGN_IMPL[_R]( [r,], T, member_seq )
 ******************************************************************************/

#define SAKE_MEMBERWISE_COPY_ASSIGN_IMPL( T, member_seq ) \
    SAKE_MEMBERWISE_COPY_ASSIGN_IMPL_R( BOOST_PP_DEDUCE_R(), T, member_seq )
#define SAKE_MEMBERWISE_COPY_ASSIGN_IMPL_R( r, T, member_seq ) \
    SAKE_MEMBERWISE_COPY_ASSIGN_IMPL_impl( r, \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( T ), \
        member_seq \
    )

#define SAKE_MEMBERWISE_COPY_ASSIGN_IMPL_impl( r, T, member_seq ) \
    SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG_R( r, member_seq, has_nothrow_copy_assign ) \
    T& copy_assign_impl(T const & other) \
        BOOST_NOEXCEPT_IF((has_nothrow_copy_assign_tag::value)) \
    { SAKE_MEMBERWISE_assign_body( r, T, member_seq ) }

namespace sake
{

namespace memberwise_copy_assign_private
{

template< class T >
class disabler : public T
{
    disabler();
    disabler(disabler&);
    ~disabler();
    void operator=(disabler&);
};

} // namespace memberwise_copy_assign_private

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_MEMBERWISE_COPY_ASSIGN_HPP
