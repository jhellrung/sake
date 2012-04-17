/*******************************************************************************
 * sake/core/utility/memberwise/dtor.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_MEMBERWISE_DTOR[_R]( [r,] T, member_seq )
 * #define SAKE_MEMBERWISE_DTOR_BODY()
 *
 * Expands to define a destructor equivalent to member-wise destruction.
 *
 * Currently, member_seq is not used, but for forward compatibility, member_seq
 * should be a Boost.PP sequence of base class specifiers followed by member
 * variable specifiers. A base class specifier is a Boost.PP 1-sequence simply
 * consisting of the base class; a member variable specifier is a Boost.PP
 * 2-sequence of the form ( type )( name ).
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_MEMBERWISE_DTOR_HPP
#define SAKE_CORE_UTILITY_MEMBERWISE_DTOR_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>

#include <sake/boost_ext/preprocessor/keyword/typename.hpp>

#define SAKE_MEMBERWISE_DTOR( T, member_seq ) \
    SAKE_MEMBERWISE_DTOR_impl( BOOST_PP_DEDUCE_R(), \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( T ), \
        member_seq \
    )
#define SAKE_MEMBERWISE_DTOR_R( r, T, member_seq ) \
    SAKE_MEMBERWISE_DTOR_impl( r, \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( T ), \
        member_seq \
    )

#define SAKE_MEMBERWISE_DTOR_impl( r, T, member_seq ) \
    ~T() SAKE_MEMBERWISE_DTOR_BODY()

#ifndef BOOST_NO_DEFAULTED_FUNCTIONS
#define SAKE_MEMBERWISE_DTOR_BODY() = default;
#else // #ifndef BOOST_NO_DEFAULTED_FUNCTIONS
#define SAKE_MEMBERWISE_DTOR_BODY() { }
#endif // #ifndef BOOST_NO_DEFAULTED_FUNCTIONS

#endif // #ifndef SAKE_CORE_UTILITY_MEMBERWISE_DTOR_HPP
