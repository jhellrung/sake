/*******************************************************************************
 * sake/boost_ext/preprocessor/seq/join.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_BOOST_EXT_PP_SEQ_JOIN( seq )
 *
 * Expands to a concatenation of seq, where seq is a Boost.PP Seq of Boost.PP
 * Seqs.  Any such sub-Seq within seq may be BOOST_PP_SEQ_NIL, indicating an
 * empty Seq.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_PREPROCESSOR_SEQ_JOIN_HPP
#define SAKE_BOOST_EXT_PREPROCESSOR_SEQ_JOIN_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/config/config.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/size.hpp>

#include <sake/boost_ext/preprocessor/seq/eat_nil.hpp>

#if BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_EDG()
#define SAKE_BOOST_EXT_PP_SEQ_JOIN( seq ) \
    SAKE_BOOST_EXT_PP_SEQ_JOIN_I( seq )
#define SAKE_BOOST_EXT_PP_SEQ_JOIN_I( seq ) \
    SAKE_BOOST_EXT_PP_SEQ_JOIN_II( \
        BOOST_PP_CAT( SAKE_BOOST_EXT_PP_SEQ_JOIN_, BOOST_PP_SEQ_SIZE( seq ) ) seq )
#elif BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MWCC()
#define SAKE_BOOST_EXT_PP_SEQ_JOIN( seq ) \
    SAKE_BOOST_EXT_PP_SEQ_JOIN_I( BOOST_PP_SEQ_SIZE( seq ), seq )
#define SAKE_BOOST_EXT_PP_SEQ_JOIN_I( size, seq ) \
    SAKE_BOOST_EXT_PP_SEQ_JOIN_II( \
        BOOST_PP_CAT( SAKE_BOOST_EXT_PP_SEQ_JOIN_, size ) seq )
#else
#define SAKE_BOOST_EXT_PP_SEQ_JOIN( seq ) \
    SAKE_BOOST_EXT_PP_SEQ_JOIN_II( \
        BOOST_PP_CAT( SAKE_BOOST_EXT_PP_SEQ_JOIN_, BOOST_PP_SEQ_SIZE( seq ) ) seq )
#endif
#define SAKE_BOOST_EXT_PP_SEQ_JOIN_II( seq ) \
    BOOST_PP_SEQ_NIL seq

#define SAKE_BOOST_EXT_PP_SEQ_JOIN_1( seq ) SAKE_BOOST_EXT_PP_SEQ_EAT_NIL( seq )

#include <sake/boost_ext/preprocessor/seq/private/join.ipp>

#endif // #ifndef SAKE_BOOST_EXT_PREPROCESSOR_SEQ_JOIN_HPP
