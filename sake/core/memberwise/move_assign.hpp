/*******************************************************************************
 * sake/core/memberwise/move_assign.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_MEMBERWISE_MOVE_ASSIGN[_R]( [r,] typenameT, member_seq )
 *
 * Expands to define a move assignment operator equivalent to member-wise move
 * assignment.
 *
 * member_seq should be a Boost.PP sequence of base class specifiers followed by
 * member variable specifiers. A base class specifier is a Boost.PP 1-sequence
 * simply consisting of the base class; a member variable specifier is a
 * Boost.PP 2-sequence of the form ( type )( name ).
 ******************************************************************************/

#ifndef SAKE_CORE_MEMBERWISE_MOVE_ASSIGN_HPP
#define SAKE_CORE_MEMBERWISE_MOVE_ASSIGN_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>

#include <sake/boost_ext/preprocessor/keyword/typename.hpp>

#include <sake/core/memberwise/move_assign_tags.hpp>

#define SAKE_MEMBERWISE_MOVE_ASSIGN( typenameT, member_seq ) \
    SAKE_MEMBERWISE_MOVE_ASSIGN_R( BOOST_PP_DEDUCE_R(), typenameT, member_seq )
#define SAKE_MEMBERWISE_MOVE_ASSIGN_R( r, typenameT, member_seq ) \
    SAKE_MEMBERWISE_MOVE_ASSIGN_TAGS_R( r, member_seq ) \
    SAKE_MEMBERWISE_MOVE_ASSIGN_impl( r, \
        SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_TYPENAME( typenameT ) BOOST_PP_EMPTY, \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( typenameT ), \
        member_seq \
    )

#if !defined( BOOST_NO_DEFAULTED_FUNCTIONS ) \
 && !defined( BOOST_NO_RVALUE_REFERENCES )

#define SAKE_MEMBERWISE_MOVE_ASSIGN_impl( r, typename, T, member_seq ) \
    T& operator=(T&&) = default;

#else // #if !defined(...) && !defined(...)

#include <boost/preprocessor/cat.hpp>
#include <boost/mpl/if.hpp>

#include <sake/boost_ext/preprocessor/seq/is_nil.hpp>

#include <sake/core/memberwise/private/assign_body.hpp>
#include <sake/core/memberwise/private/move_assign_enable.hpp>

#ifndef BOOST_NO_RVALUE_REFERENCES
#define SAKE_MEMBERWISE_MOVE_ASSIGN_param_type( T ) T&&
#else // #ifndef BOOST_NO_RVALUE_REFERENCES
#define SAKE_MEMBERWISE_MOVE_ASSIGN_param_type( T ) this_rvalue_param_type
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_MEMBERWISE_MOVE_ASSIGN_impl( r, typename, T, member_seq ) \
    BOOST_PP_CAT( \
        SAKE_MEMBERWISE_MOVE_ASSIGN_impl, \
        SAKE_BOOST_EXT_PP_SEQ_IS_NIL( member_seq ) \
    ) ( r, typename, T, member_seq )

#define SAKE_MEMBERWISE_MOVE_ASSIGN_impl0( r, typename, T, member_seq ) \
    typedef typename() ::boost::mpl::if_c< \
        SAKE_MEMBERWISE_move_assign_enable( r, member_seq )::value, \
        SAKE_MEMBERWISE_MOVE_ASSIGN_param_type( T ), \
        ::sake::memberwise_move_assign_private::disabler<T>& \
    >::type _sake_memberwise_move_assign_param_type; \
    T& operator=(_sake_memberwise_move_assign_param_type other) \
        BOOST_NOEXCEPT_IF((has_nothrow_move_assign_tag::value)) \
    { SAKE_MEMBERWISE_assign_body( r, T, member_seq ) }

#define SAKE_MEMBERWISE_MOVE_ASSIGN_impl1( r, typename, T, member_seq ) \
    T& operator=(SAKE_MEMBERWISE_MOVE_ASSIGN_param_type( T )) \
        BOOST_NOEXCEPT \
    { return *this; }

namespace sake
{

namespace memberwise_move_assign_private
{

template< class T >
class disabler : public T
{
    disabler();
    disabler(disabler&);
    ~disabler();
    void operator=(disabler&);
};

} // namespace memberwise_move_assign_private

} // namespace sake

#endif // #if !defined(...) && !defined(...)

#endif // #ifndef SAKE_CORE_MEMBERWISE_MOVE_ASSIGN_HPP
