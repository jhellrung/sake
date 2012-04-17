/*******************************************************************************
 * sake/boost_ext/preprocessor/seq/transpose.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * SAKE_BOOST_EXT_PP_SEQ_TRANSPOSE( seq )
 *
 * Expands to the "transpose" of the given Boost.PP sequence-of-sequences.
 *
 * Example:
 *
 * SAKE_BOOST_EXT_PP_SEQ_TRANSPOSE( (( a )( b )( c )) (( 1 )( 2 )( 3 )) )
 *
 * expands to
 *
 * (( a )( 1 )) (( b )( 2 )) (( c )( 3 ))
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_PREPROCESSOR_SEQ_TRANSPOSE_HPP
#define SAKE_BOOST_EXT_PREPROCESSOR_SEQ_TRANSPOSE_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/size.hpp>

#include <sake/boost_ext/preprocessor/seq/private/transpose.ipp>

#define SAKE_BOOST_EXT_PP_SEQ_TRANSPOSE( seq ) \
    SAKE_BOOST_EXT_PP_SEQ_TRANSPOSE_impl( seq, BOOST_PP_SEQ_SIZE( seq ) )

#define SAKE_BOOST_EXT_PP_SEQ_TRANSPOSE_impl( seq, n ) \
    BOOST_PP_CAT( \
        SAKE_BOOST_EXT_PP_SEQ_TRANSPOSE_impl_, \
        BOOST_PP_SEQ_SIZE( BOOST_PP_SEQ_HEAD( seq ) ) \
    ) ( \
        seq, \
        BOOST_PP_CAT( SAKE_BOOST_EXT_PP_SEQ_TRANSPOSE_head_, n ), \
        BOOST_PP_CAT( SAKE_BOOST_EXT_PP_SEQ_TRANSPOSE_tail_, n ) \
    )

#define SAKE_BOOST_EXT_PP_SEQ_TRANSPOSE_impl_1( seq, head, tail ) ( head seq )
#define SAKE_BOOST_EXT_PP_SEQ_TRANSPOSE_head_1( seq ) ( BOOST_PP_SEQ_HEAD( seq ) )
#define SAKE_BOOST_EXT_PP_SEQ_TRANSPOSE_tail_1( seq ) ( BOOST_PP_SEQ_TAIL( seq ) )

#endif // #ifndef SAKE_BOOST_EXT_PREPROCESSOR_SEQ_TRANSPOSE_HPP
