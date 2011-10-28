/*******************************************************************************
 * sake/core/utility/implicitly_defined_move_assign.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_IMPLICITLY_DEFINED_MOVE_ASSIGN[_R]( [r,] T, base_seq, member_seq )
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_IMPLICITLY_DEFINED_MOVE_ASSIGN_HPP
#define SAKE_CORE_UTILITY_IMPLICITLY_DEFINED_MOVE_ASSIGN_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>

#define SAKE_IMPLICITLY_DEFINED_MOVE_ASSIGN( T, base_seq, member_seq ) \
    SAKE_IMPLICITLY_DEFINED_MOVE_ASSIGN_R( BOOST_PP_DEDUCE_R(), T, base_seq, member_seq )

#if !defined( BOOST_NO_DEFAULTED_FUNCTIONS ) \
 && !defined( BOOST_NO_RVALUE_REFERENCES )

#define SAKE_IMPLICITLY_DEFINED_MOVE_ASSIGN_R( r, T, base_seq, member_seq ) \
    T& operator=(T&&) = default;

#else // #if !defined(...) && !defined(...)

#include <sake/core/utility/private/implicitly_defined/assign_body.hpp>

#define SAKE_IMPLICITLY_DEFINED_MOVE_ASSIGN_R( r, T, base_seq, member_seq ) \
    T& operator=(this_rvalue_param_type other) \
    { SAKE_IMPLICITLY_DEFINED_assign_body( r, T, base_seq, member_seq ) }

#endif // #if !defined(...) && !defined(...)

#endif // #ifndef SAKE_CORE_UTILITY_IMPLICITLY_DEFINED_MOVE_ASSIGN_HPP
