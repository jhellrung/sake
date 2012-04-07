/*******************************************************************************
 * sake/boost_ext/preprocessor/tuple/rem.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_BOOST_EXT_PP_TUPLE_REM1( x0 )
 * #define SAKE_BOOST_EXT_PP_TUPLE_REM2( x0, x1 )
 * #define SAKE_BOOST_EXT_PP_TUPLE_REM3( x0, x1, x2 )
 * #define SAKE_BOOST_EXT_PP_TUPLE_REM4( x0, x1, x2, x3 )
 * #define SAKE_BOOST_EXT_PP_TUPLE_REM5( x0, x1, x2, x3, x4 )
 * #define SAKE_BOOST_EXT_PP_TUPLE_REM6( x0, x1, x2, x3, x4, x5 )
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_PREPROCESSOR_TUPLE_REM_HPP
#define SAKE_BOOST_EXT_PREPROCESSOR_TUPLE_REM_HPP

#define SAKE_BOOST_EXT_PP_TUPLE_REM1( x0 ) x0
#define SAKE_BOOST_EXT_PP_TUPLE_REM2( x0, x1 ) x0, x1
#define SAKE_BOOST_EXT_PP_TUPLE_REM3( x0, x1, x2 ) x0, x1, x2
#define SAKE_BOOST_EXT_PP_TUPLE_REM4( x0, x1, x2, x3 ) x0, x1, x2, x3
#define SAKE_BOOST_EXT_PP_TUPLE_REM5( x0, x1, x2, x3, x4 ) x0, x1, x2, x3, x4
#define SAKE_BOOST_EXT_PP_TUPLE_REM6( x0, x1, x2, x3, x4, x5 ) x0, x1, x2, x3, x4, x5

#endif // #ifndef SAKE_BOOST_EXT_PREPROCESSOR_TUPLE_REM_HPP
