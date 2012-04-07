/*******************************************************************************
 * sake/boost_ext/preprocessor/seq/detail/for_each.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * SAKE_BOOST_EXT_PP_SEQ_FOR_EACH_ ## r ( macro, data, seq )
 *
 * This is functionally identical to BOOST_PP_SEQ_FOR_EACH_R, except that it
 * can be used within the macro passed to BOOST_PP_FOR_EACH[_R].
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_PREPROCESSOR_SEQ_FOR_EACH_HPP
#define SAKE_BOOST_EXT_PREPROCESSOR_SEQ_FOR_EACH_HPP

#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/repetition/for.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#include <sake/boost_ext/preprocessor/seq/private/for_each.ipp>
#include <sake/boost_ext/preprocessor/tuple/rem.hpp>

#define SAKE_BOOST_EXT_PP_SEQ_FOR_EACH_for_pred( r, state ) \
    BOOST_PP_TUPLE_ELEM( 4, 0, state )
#define SAKE_BOOST_EXT_PP_SEQ_FOR_EACH_for_op( r, state ) \
    SAKE_BOOST_EXT_PP_SEQ_FOR_EACH_for_op_impl state
#define SAKE_BOOST_EXT_PP_SEQ_FOR_EACH_for_op_impl( n, macro, data, seq ) \
    ( BOOST_PP_DEC( n ), macro, data, BOOST_PP_SEQ_TAIL( seq ) )

#endif // #ifndef SAKE_BOOST_EXT_PREPROCESSOR_SEQ_FOR_EACH_HPP
