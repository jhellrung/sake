/*******************************************************************************
 * sake/boost_ext/preprocessor/seq/zip.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * SAKE_BOOST_EXT_PP_SEQ_ZIP[_R]( [r,] seq )
 *
 * This "zips" together the Boost.Preprocessor seqs in seq.
 *
 * Example:
 *
 * SAKE_BOOST_EXT_PP_SEQ_ZIP( (( a )( b )( c )) (( 1 )( 2 )( 3 )) )
 *
 * expands to
 *
 * (( a, 1 )) (( b, 2 )) (( c, 3 ))
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_PREPROCESSOR_SEQ_ZIP_HPP
#define SAKE_BOOST_EXT_PREPROCESSOR_SEQ_ZIP_HPP

#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>
#include <boost/preprocessor/repetition/for.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#define SAKE_BOOST_EXT_PP_SEQ_ZIP( seq ) \
    SAKE_BOOST_EXT_PP_SEQ_ZIP_R( BOOST_PP_DEDUCE_R(), seq )

#define SAKE_BOOST_EXT_PP_SEQ_ZIP_R( r, seq ) \
    BOOST_PP_CAT( BOOST_PP_FOR_, r ) ( \
        ( BOOST_PP_SEQ_SIZE( BOOST_PP_SEQ_HEAD( seq ) ), seq ), \
        SAKE_BOOST_EXT_PP_SEQ_ZIP_for_pred, \
        SAKE_BOOST_EXT_PP_SEQ_ZIP_for_op, \
        SAKE_BOOST_EXT_PP_SEQ_ZIP_for_macro \
    )

#define SAKE_BOOST_EXT_PP_SEQ_ZIP_for_pred( r, state ) \
    BOOST_PP_TUPLE_ELEM( 2, 0, state )

#define SAKE_BOOST_EXT_PP_SEQ_ZIP_for_op( r, state ) \
    ( BOOST_PP_DEC( BOOST_PP_TUPLE_ELEM( 2, 0, state ) ), \
      BOOST_PP_SEQ_FOR_EACH_R( \
          r, \
          SAKE_BOOST_EXT_PP_SEQ_ZIP_tail_elem, \
          ~, \
          BOOST_PP_TUPLE_ELEM( 2, 1, state ) \
      ) )
#define SAKE_BOOST_EXT_PP_SEQ_ZIP_tail_elem( r, data, elem ) \
    ( BOOST_PP_SEQ_TAIL( elem ) )

#define SAKE_BOOST_EXT_PP_SEQ_ZIP_for_macro( r, state ) \
    (( BOOST_PP_SEQ_FOR_EACH_I_R( \
        r, \
        SAKE_BOOST_EXT_PP_SEQ_ZIP_comma_head_elem, \
        ~, \
        BOOST_PP_TUPLE_ELEM( 2, 1, state ) \
    ) ))
#define SAKE_BOOST_EXT_PP_SEQ_ZIP_comma_head_elem( r, data, i, elem ) \
    BOOST_PP_COMMA_IF( i ) BOOST_PP_SEQ_HEAD( elem )

#endif // #ifndef SAKE_BOOST_EXT_PREPROCESSOR_SEQ_ZIP_HPP
