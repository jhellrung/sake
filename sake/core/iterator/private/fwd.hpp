/*******************************************************************************
 * sake/core/iterator/private/fwd.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_FWD_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_FWD_HPP

namespace sake
{

namespace iterator
{

namespace private_
{

template< class I, class J >
struct is_convertible_relax;

template< class I0, class I1 >
struct is_semi_convertible_relax;

} // namespace private_

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_FWD_HPP
