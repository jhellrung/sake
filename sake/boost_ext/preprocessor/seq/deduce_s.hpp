/*******************************************************************************
 * sake/boost_ext/preprocessor/seq/deduce_s.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_BOOST_EXT_PP_DEDUCE_S()
 *
 * This expands to the next available BOOST_PP_SEQ_FOLD_LEFT fold step.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_PREPROCESSOR_SEQ_DEDUCE_S_HPP
#define SAKE_BOOST_EXT_PREPROCESSOR_SEQ_DEDUCE_S_HPP

#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/seq/fold_left.hpp>

#define SAKE_BOOST_EXT_PP_DEDUCE_S() \
    BOOST_PP_DEC( BOOST_PP_SEQ_FOLD_LEFT( SAKE_BOOST_EXT_PP_DEDUCE_S_s, ~, ( ~ ) ) )

#define SAKE_BOOST_EXT_PP_DEDUCE_S_s( s, state, elem ) s

#endif // #ifndef SAKE_BOOST_EXT_PREPROCESSOR_SEQ_DEDUCE_S_HPP
