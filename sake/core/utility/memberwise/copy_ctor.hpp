/*******************************************************************************
 * sake/core/utility/memberwise/copy_ctor.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_MEMBERWISE_COPY_CTOR[_R]( [r,] T, member_seq )
 *
 * Expands to define a copy constructor equivalent to member-wise copy
 * construction.
 *
 * member_seq should be a Boost.PP sequence of base class specifiers followed by
 * member variable specifiers. A base class specifier is a Boost.PP 1-sequence
 * simply consisting of the base class; a member variable specifier is a
 * Boost.PP 2-sequence of the form ( type )( name ).
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_MEMBERWISE_COPY_CTOR_HPP
#define SAKE_CORE_UTILITY_MEMBERWISE_COPY_CTOR_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>

#include <sake/boost_ext/preprocessor/keyword/typename.hpp>

#define SAKE_MEMBERWISE_COPY_CTOR( T, member_seq ) \
    SAKE_MEMBERWISE_COPY_CTOR_impl( BOOST_PP_DEDUCE_R(), \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( T ), \
        member_seq \
    )
#define SAKE_MEMBERWISE_COPY_CTOR_R( r, T, member_seq ) \
    SAKE_MEMBERWISE_COPY_CTOR_impl( r, \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( T ), \
        member_seq \
    )

#ifndef BOOST_NO_DEFAULTED_FUNCTIONS

#define SAKE_MEMBERWISE_COPY_CTOR_impl( r, T, member_seq ) \
    T(T const &) = default;

#else // #ifndef BOOST_NO_DEFAULTED_FUNCTIONS

#ifndef BOOST_NO_RVALUE_REFERENCES

#include <sake/core/utility/memberwise/private/ctor_init.hpp>

#define SAKE_MEMBERWISE_COPY_CTOR_impl( r, T, member_seq ) \
    T(T const & other) \
    SAKE_MEMBERWISE_ctor_init( r, member_seq ) \
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

// Assume implicitly-defined copy ctor is sufficient.
#define SAKE_MEMBERWISE_COPY_CTOR_impl( r, T, member_seq )

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#endif // #ifndef BOOST_NO_DEFAULTED_FUNCTIONS

#endif // #ifndef SAKE_CORE_UTILITY_MEMBERWISE_COPY_CTOR_HPP
