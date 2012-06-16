/*******************************************************************************
 * sake/core/iterator/adaptor_fwd.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ADAPTOR_FWD_HPP
#define SAKE_CORE_ITERATOR_ADAPTOR_FWD_HPP

#include <boost/mpl/map/map0.hpp>

namespace sake
{

namespace iterator
{

template<
    class Derived, class Base,
    class Params = boost::mpl::map0<>
>
class adaptor;

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ADAPTOR_FWD_HPP
