/*******************************************************************************
 * sake/core/utility/private/overload/define_iteration_depth.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#if BOOST_PP_ITERATION_DEPTH() == 0
#define SAKE_OVERLOAD_ITERATION_DEPTH 0
#elif BOOST_PP_ITERATION_DEPTH() == 1
#define SAKE_OVERLOAD_ITERATION_DEPTH 1
#elif BOOST_PP_ITERATION_DEPTH() == 2
#define SAKE_OVERLOAD_ITERATION_DEPTH 2
#elif BOOST_PP_ITERATION_DEPTH() == 3
#define SAKE_OVERLOAD_ITERATION_DEPTH 3
#else
#error Unsupported iteration depth.
#endif
