/*******************************************************************************
 * sake/boost_ext/preprocessor/seq/juxt_enum.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * SAKE_BOOST_EXT_PP_SEQ_JUXT_ENUM[_R]( [r,] seq )
 *
 * This enumerates each Boost.Preprocessor seq and juxtaposes the results.  This
 * is useful when having to pass template instantiations (with more than one
 * template parameter) as macro arguments, since the comma delimiting template
 * parameters would be interpretted as separating macro arguments.
 *
 * Example:
 *
 * SAKE_BOOST_EXT_PP_SEQ_JUXT_ENUM( (( f< )) (( T0 )( T1 )( T2 )) (( > )) )
 *
 * expands to
 *
 * f< T0, T1, T2 >
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_PREPROCESSOR_SEQ_JUXT_ENUM_HPP
#define SAKE_BOOST_EXT_PREPROCESSOR_SEQ_JUXT_ENUM_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>
#include <boost/preprocessor/seq/enum.hpp>

#include <sake/boost_ext/preprocessor/seq/for_each.hpp>

#define SAKE_BOOST_EXT_PP_SEQ_JUXT_ENUM( seq ) \
    SAKE_BOOST_EXT_PP_SEQ_JUXT_ENUM_R( BOOST_PP_DEDUCE_R(), seq )

#define SAKE_BOOST_EXT_PP_SEQ_JUXT_ENUM_R( r, seq ) \
    BOOST_PP_CAT( SAKE_BOOST_EXT_PP_SEQ_FOR_EACH_, r ) \
        ( SAKE_BOOST_EXT_PP_SEQ_JUXT_ENUM_for_each_macro, ~, seq )

#define SAKE_BOOST_EXT_PP_SEQ_JUXT_ENUM_for_each_macro( r, data, elem ) BOOST_PP_SEQ_ENUM( elem )

#endif // #ifndef SAKE_BOOST_EXT_PREPROCESSOR_SEQ_JUXT_ENUM_HPP
