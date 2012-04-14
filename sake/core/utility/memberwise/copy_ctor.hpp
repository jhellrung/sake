/*******************************************************************************
 * sake/core/utility/memberwise/copy_ctor.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_MEMBERWISE_COPY_CTOR[_R]( [r,] T, base_seq, member_name_seq )
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_COPY_CTOR_HPP
#define SAKE_CORE_UTILITY_DEFINE_NATURAL_COPY_CTOR_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>

#define SAKE_MEMBERWISE_COPY_CTOR( T, base_seq, member_name_seq ) \
    SAKE_MEMBERWISE_COPY_CTOR_R( BOOST_PP_DEDUCE_R(), T, base_seq, member_name_seq )

#if !defined( BOOST_NO_DEFAULTED_FUNCTIONS ) \
 && !defined( BOOST_NO_RVALUE_REFERENCES )

#define SAKE_MEMBERWISE_COPY_CTOR_R( r, T, base_seq, member_name_seq ) \
    T(T const &) = default;

#else // #if !defined(...) && !defined(...)

#include <sake/core/utility/memberwise/private/ctor_init.hpp>

#define SAKE_MEMBERWISE_COPY_CTOR_R( r, T, base_seq, member_name_seq ) \
    T(T const & other) \
    SAKE_MEMBERWISE_ctor_init( r, base_seq, member_name_seq ) \
    { }

#endif // #if !defined(...) && !defined(...)

#endif // #ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_COPY_CTOR_HPP
