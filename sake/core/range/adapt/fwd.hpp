/*******************************************************************************
 * sake/core/range/adapt/fwd.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_ADAPT_FWD_HPP
#define SAKE_CORE_RANGE_ADAPT_FWD_HPP

#include <boost/mpl/map/map0.hpp>

namespace sake
{

namespace range
{

namespace adapt
{

namespace functional
{

struct move;

template< class F = void, class Params = boost::mpl::map0<> >
struct transform;

} // namespace functional;

} // namespace adapt

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_ADAPT_FWD_HPP
