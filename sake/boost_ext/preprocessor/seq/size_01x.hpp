/*******************************************************************************
 * sake/boost_ext/preprocessor/seq/size_01x.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_BOOST_EXT_PP_SEQ_SIZE_01X( seq, x )
 *
 * Expands to 0 if seq is BOOST_PP_SEQ_NIL.
 * Expands to 1 if seq is size 1.
 * Expands to x if seq is size 2 or more.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_PREPROCESSOR_SEQ_SIZE_01X_HPP
#define SAKE_BOOST_EXT_PREPROCESSOR_SEQ_SIZE_01X_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/size.hpp>

#include <sake/boost_ext/preprocessor/seq/private/size_01x.ipp>

#define SAKE_BOOST_EXT_PP_SEQ_SIZE_01X( seq, x ) \
    BOOST_PP_CAT( \
        SAKE_BOOST_EXT_PP_SEQ_SIZE_01X_, \
        BOOST_PP_SEQ_SIZE( seq ( ~ ) ) \
    ) ( x )

#define SAKE_BOOST_EXT_PP_SEQ_SIZE_01X_1( x ) 0
#define SAKE_BOOST_EXT_PP_SEQ_SIZE_01X_2( x ) 1

#endif // #ifndef SAKE_BOOST_EXT_PREPROCESSOR_SEQ_SIZE_01X_HPP
