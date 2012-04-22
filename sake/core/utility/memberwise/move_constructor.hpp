/*******************************************************************************
 * sake/core/utility/memberwise/move_constructor.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_MEMBERWISE_MOVE_CONSTRUCTOR[_R]( [r,] T, member_seq )
 *
 * Expands to define a move constructor equivalent to member-wise move
 * construction.
 *
 * member_seq should be a Boost.PP sequence of base class specifiers followed by
 * member variable specifiers. A base class specifier is a Boost.PP 1-sequence
 * simply consisting of the base class; a member variable specifier is a
 * Boost.PP 2-sequence of the form ( type )( name ).
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_MEMBERWISE_MOVE_CONSTRUCTOR_HPP
#define SAKE_CORE_UTILITY_MEMBERWISE_MOVE_CONSTRUCTOR_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>

#include <sake/boost_ext/preprocessor/keyword/typename.hpp>

#define SAKE_MEMBERWISE_MOVE_CONSTRUCTOR( T, member_seq ) \
    SAKE_MEMBERWISE_MOVE_CONSTRUCTOR_impl( BOOST_PP_DEDUCE_R(), \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( T ), \
        member_seq \
    )
#define SAKE_MEMBERWISE_MOVE_CONSTRUCTOR_R( r, T, member_seq ) \
    SAKE_MEMBERWISE_MOVE_CONSTRUCTOR_impl( r, \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( T ), \
        member_seq \
    )

#if !defined( BOOST_NO_DEFAULTED_FUNCTIONS ) \
 && !defined( BOOST_NO_RVALUE_REFERENCES )

#define SAKE_MEMBERWISE_MOVE_CONSTRUCTOR_impl( r, T, member_seq ) \
    T(T&&) = default;

#else // #if !defined(...) && !defined(...)

#include <sake/core/utility/memberwise/private/initialization_list.hpp>

#ifndef BOOST_NO_RVALUE_REFERENCES
#define SAKE_MEMBERWISE_MOVE_CONSTRUCTOR_param_type( T ) T&&
#else // #ifndef BOOST_NO_RVALUE_REFERENCES
#define SAKE_MEMBERWISE_MOVE_CONSTRUCTOR_param_type( T ) this_rvalue_param_type
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_MEMBERWISE_MOVE_CONSTRUCTOR_impl( r, T, member_seq ) \
    T(SAKE_MEMBERWISE_MOVE_CONSTRUCTOR_param_type( T ) other) \
    SAKE_MEMBERWISE_initialization_list( r, member_seq ) \
    { }

#endif // #if !defined(...) && !defined(...)

#endif // #ifndef SAKE_CORE_UTILITY_MEMBERWISE_MOVE_CONSTRUCTOR_HPP
