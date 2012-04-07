/*******************************************************************************
 * sake/boost_ext/preprocessor/seq/eat_nil.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_BOOST_EXT_PP_SEQ_EAT_NIL( seq )
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_PREPROCESSOR_SEQ_EAT_NIL_HPP
#define SAKE_BOOST_EXT_PREPROCESSOR_SEQ_EAT_NIL_HPP

#include <boost/preprocessor/cat.hpp>

#include <sake/boost_ext/preprocessor/seq/is_nil.hpp>

#define SAKE_BOOST_EXT_PP_SEQ_EAT_NIL( seq ) \
    BOOST_PP_CAT( \
        SAKE_BOOST_EXT_PP_SEQ_EAT_NIL_, \
        SAKE_BOOST_EXT_PP_SEQ_IS_NIL( seq ) \
    ) ( seq )
#define SAKE_BOOST_EXT_PP_SEQ_EAT_NIL_0( seq ) seq
#define SAKE_BOOST_EXT_PP_SEQ_EAT_NIL_1( seq )

#endif // #ifndef SAKE_BOOST_EXT_PREPROCESSOR_SEQ_EAT_NIL_HPP
