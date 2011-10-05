/*******************************************************************************
 * sake/core/utility/private/overload/define_filename_x.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#if BOOST_PP_ITERATION_DEPTH() == 0
#define BOOST_PP_FILENAME_1 <sake/core/utility/private/overload/overload.ipp>
#elif BOOST_PP_ITERATION_DEPTH() == 1
#define BOOST_PP_FILENAME_2 <sake/core/utility/private/overload/overload.ipp>
#elif BOOST_PP_ITERATION_DEPTH() == 2
#define BOOST_PP_FILENAME_3 <sake/core/utility/private/overload/overload.ipp>
#elif BOOST_PP_ITERATION_DEPTH() == 3
#define BOOST_PP_FILENAME_4 <sake/core/utility/private/overload/overload.ipp>
#else
#error Unsupported iteration depth.
#endif
