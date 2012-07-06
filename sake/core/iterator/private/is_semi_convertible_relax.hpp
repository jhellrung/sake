/*******************************************************************************
 * sake/core/iterator/private/is_semi_convertible_relax.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_IS_SEMI_CONVERTIBLE_RELAX_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_IS_SEMI_CONVERTIBLE_RELAX_HPP

#include <sake/boost_ext/mpl/or.hpp>

#include <sake/core/iterator/private/fwd.hpp>
#include <sake/core/iterator/private/is_convertible_relax.hpp>

namespace sake
{

namespace iterator
{

namespace private_
{

template< class I0, class I1 >
struct is_semi_convertible_relax
    : boost_ext::mpl::or2<
          private_::is_convertible_relax< I0, I1 >,
          private_::is_convertible_relax< I1, I0 >
      >
{ };

} // namespace private_

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_IS_SEMI_CONVERTIBLE_RELAX_HPP
