/*******************************************************************************
 * sake/core/memberwise/default_constructor.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR[_R]( [r,] T, member_seq )
 * #define SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR_BODY()
 *
 * Expands to define a default constructor equivalent to member-wise default
 * construction. Additionally defines a has_default_constructor_tag typedef
 * based on whether each member specified in member_seq has a default
 * constructor (as determined by sake::has_default_constructor).
 *
 * member_seq should be a Boost.PP sequence of base class specifiers followed by
 * member variable specifiers. A base class specifier is a Boost.PP 1-sequence
 * simply consisting of the base class; a member variable specifier is a
 * Boost.PP 2-sequence of the form ( type )( name ).
 ******************************************************************************/

#ifndef SAKE_CORE_MEMBERWISE_DEFAULT_CONSTRUCTOR_HPP
#define SAKE_CORE_MEMBERWISE_DEFAULT_CONSTRUCTOR_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>

#include <sake/boost_ext/preprocessor/keyword/typename.hpp>

#include <sake/core/memberwise/default_constructor_tags.hpp>

#define SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR( T, member_seq ) \
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR_impl( BOOST_PP_DEDUCE_R(), \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( T ), \
        member_seq \
    )
#define SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR_R( r, T, member_seq ) \
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR_impl( r, \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( T ), \
        member_seq \
    )

#define SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR_impl( r, T, member_seq ) \
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR_TAGS_R( r, member_seq ) \
    T() SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR_BODY()

#ifndef BOOST_NO_DEFAULTED_FUNCTIONS
#define SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR_BODY() = default;
#else // #ifndef BOOST_NO_DEFAULTED_FUNCTIONS
#define SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR_BODY() \
    BOOST_NOEXCEPT_IF((has_nothrow_default_constructor_tag::value)) { }
#endif // #ifndef BOOST_NO_DEFAULTED_FUNCTIONS

#endif // #ifndef SAKE_CORE_MEMBERWISE_DEFAULT_CONSTRUCTOR_HPP
