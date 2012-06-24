/*******************************************************************************
 * sake/core/iterator/adaptors/fwd.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ADAPTORS_FWD_HPP
#define SAKE_CORE_ITERATOR_ADAPTORS_FWD_HPP

#include <boost/mpl/map/map0.hpp>

namespace sake
{

namespace iterator
{

namespace adaptors
{

template< class I >
class move;

template< class I, class F, class Params = boost::mpl::map0<> >
class transform;

} // namespace adaptors

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ADAPTORS_FWD_HPP
