/*******************************************************************************
 * sake/boost_ext/preprocessor/seq/juxt_enum.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * SAKE_BOOST_EXT_PP_SEQ_JUXT_ENUM( seq )
 *
 * This enumerates each given Boost.PP Seq and juxtaposes the results.  This
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
#include <boost/preprocessor/config/config.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/size.hpp>

#if BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_EDG()
#define SAKE_BOOST_EXT_PP_SEQ_JUXT_ENUM( seq ) \
    SAKE_BOOST_EXT_PP_SEQ_JUXT_ENUM_I( seq )
#define SAKE_BOOST_EXT_PP_SEQ_JUXT_ENUM_I( seq ) \
    BOOST_PP_CAT( SAKE_BOOST_EXT_PP_SEQ_JUXT_ENUM_, BOOST_PP_SEQ_SIZE( seq ) ) seq
#elif BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MWCC()
#define SAKE_BOOST_EXT_PP_SEQ_JUXT_ENUM( seq ) \
    SAKE_BOOST_EXT_PP_SEQ_JUXT_ENUM_I( BOOST_PP_SEQ_SIZE( seq ), seq )
#define SAKE_BOOST_EXT_PP_SEQ_JUXT_ENUM_I( size, seq ) \
    BOOST_PP_CAT( SAKE_BOOST_EXT_PP_SEQ_JUXT_ENUM_, size ) seq
#else
#define SAKE_BOOST_EXT_PP_SEQ_JUXT_ENUM( seq ) \
    BOOST_PP_CAT( SAKE_BOOST_EXT_PP_SEQ_JUXT_ENUM_, BOOST_PP_SEQ_SIZE( seq ) ) seq
#endif

#define SAKE_BOOST_EXT_PP_SEQ_JUXT_ENUM_1( seq ) BOOST_PP_SEQ_ENUM( seq )

#include <sake/boost_ext/preprocessor/seq/private/juxt_enum.ipp>

#endif // #ifndef SAKE_BOOST_EXT_PREPROCESSOR_SEQ_JUXT_ENUM_HPP
