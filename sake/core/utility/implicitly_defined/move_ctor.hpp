/*******************************************************************************
 * sake/core/utility/implicitly_defined/move_ctor.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_IMPLICITLY_DEFINED_MOVE_CTOR[_R]( [r,] T, base_seq, member_seq )
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_IMPLICITLY_DEFINED_MOVE_CTOR_HPP
#define SAKE_CORE_UTILITY_IMPLICITLY_DEFINED_MOVE_CTOR_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>

#define SAKE_IMPLICITLY_DEFINED_MOVE_CTOR( T, base_seq, member_seq ) \
    SAKE_IMPLICITLY_DEFINED_MOVE_CTOR_R( BOOST_PP_DEDUCE_R(), T, base_seq, member_seq )

#if !defined( BOOST_NO_DEFAULTED_FUNCTIONS ) \
 && !defined( BOOST_NO_RVALUE_REFERENCES )

#define SAKE_IMPLICITLY_DEFINED_MOVE_CTOR_R( r, T, base_seq, member_seq ) \
    T(T&&) = default;

#else // #if !defined(...) && !defined(...)

#include <sake/core/utility/implicitly_defined/private/ctor_init.hpp>

#define SAKE_IMPLICITLY_DEFINED_MOVE_CTOR_R( r, T, base_seq, member_seq ) \
    T(this_rvalue_param_type other) \
    SAKE_IMPLICITLY_DEFINED_ctor_init( r, base_seq, member_seq ) \
    { }

#endif // #if !defined(...) && !defined(...)

#endif // #ifndef SAKE_CORE_UTILITY_IMPLICITLY_DEFINED_MOVE_CTOR_HPP
