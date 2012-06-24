/*******************************************************************************
 * sake/core/range/adaptors/fwd.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_ADAPTORS_FWD_HPP
#define SAKE_CORE_RANGE_ADAPTORS_FWD_HPP

#include <boost/mpl/map/map0.hpp>

namespace sake
{

namespace range
{

namespace adaptors
{

template< class R >
class by_value;

template< class R >
class move;

template< class R, class F, class Params = boost::mpl::map0<> >
class transform;

} // namespace adaptors

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_ADAPTORS_MOVE_HPP
