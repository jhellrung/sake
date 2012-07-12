/*******************************************************************************
 * sake/core/memberwise/copy_constructor.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_MEMBERWISE_COPY_CONSTRUCTOR[_R]( [r,] T, member_seq )
 *
 * Expands to define a copy constructor equivalent to member-wise copy
 * construction.
 *
 * member_seq should be a Boost.PP sequence of base class specifiers followed by
 * member variable specifiers. A base class specifier is a Boost.PP 1-sequence
 * simply consisting of the base class; a member variable specifier is a
 * Boost.PP 2-sequence of the form ( type )( name ).
 ******************************************************************************/

#ifndef SAKE_CORE_MEMBERWISE_COPY_CONSTRUCTOR_HPP
#define SAKE_CORE_MEMBERWISE_COPY_CONSTRUCTOR_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>

#include <sake/boost_ext/preprocessor/keyword/typename.hpp>

#include <sake/core/memberwise/copy_constructor_tags.hpp>

#define SAKE_MEMBERWISE_COPY_CONSTRUCTOR( T, member_seq ) \
    SAKE_MEMBERWISE_COPY_CONSTRUCTOR_R( BOOST_PP_DEDUCE_R(), T, member_seq )
#define SAKE_MEMBERWISE_COPY_CONSTRUCTOR_R( r, T, member_seq ) \
    SAKE_MEMBERWISE_COPY_CONSTRUCTOR_TAGS_R( r, member_seq ) \
    SAKE_MEMBERWISE_COPY_CONSTRUCTOR_impl( r, \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( T ), \
        member_seq \
    )

#ifndef BOOST_NO_DEFAULTED_FUNCTIONS

#define SAKE_MEMBERWISE_COPY_CONSTRUCTOR_impl( r, T, member_seq ) \
    T(T const &) = default;

#else // #ifndef BOOST_NO_DEFAULTED_FUNCTIONS

#ifndef BOOST_NO_RVALUE_REFERENCES

#include <sake/core/memberwise/private/initialization_list.hpp>

#define SAKE_MEMBERWISE_COPY_CONSTRUCTOR_impl( r, T, member_seq ) \
    T(T const & other) \
        BOOST_NOEXCEPT_IF((has_nothrow_copy_constructor_tag::value)) \
    SAKE_MEMBERWISE_initialization_list( r, member_seq ) \
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

// Assume implicitly-defined copy constructor is sufficient.
#define SAKE_MEMBERWISE_COPY_CONSTRUCTOR_impl( r, T, member_seq )

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#endif // #ifndef BOOST_NO_DEFAULTED_FUNCTIONS

#endif // #ifndef SAKE_CORE_MEMBERWISE_COPY_CONSTRUCTOR_HPP
