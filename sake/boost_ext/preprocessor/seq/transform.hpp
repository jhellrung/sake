/*******************************************************************************
 * sake/boost_ext/preprocessor/seq/transform.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_BOOST_EXT_PP_SEQ_TRANSFORM_ ## s ( op, data, seq )
 *
 * This is functionally identical to BOOST_PP_SEQ_TRANSFORM_S, except that it
 * can be used within the macro passed to BOOST_PP_SEQ_TRANSFORM[_S].
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_PREPROCESSOR_SEQ_TRANSFORM_HPP
#define SAKE_BOOST_EXT_PREPROCESSOR_SEQ_TRANSFORM_HPP

#include <boost/preprocessor/seq/fold_left.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#include <sake/boost_ext/preprocessor/seq/private/transform.ipp>
#include <sake/boost_ext/preprocessor/tuple/rem.hpp>

#endif // #ifndef SAKE_BOOST_EXT_PREPROCESSOR_SEQ_TRANSFORM_HPP
