/*******************************************************************************
 * sake/core/iterator/private/is_convertible_relax.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_IS_CONVERTIBLE_RELAX_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_IS_CONVERTIBLE_RELAX_HPP

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/is_iterator.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/iterator/traits_fwd.hpp>

namespace sake
{

namespace iterator
{

namespace private_
{

template< class I, class J >
class is_convertible_relax_helper
{
    typedef typename sake::introversal_meet<
        typename sake::iterator_introversal<I>::type,
        typename sake::iterator_introversal<J>::type
    >::type introversal;
public:
    static bool const value = boost_ext::is_convertible<
        typename sake::iterator_relax< I, introversal >::type,
        typename sake::iterator_relax< J, introversal >::type
    >::value;
    typedef is_convertible_relax_helper type;
};

template< class I, class J >
struct is_convertible_relax
    : boost_ext::mpl::and3<
          sake::is_iterator<I>,
          sake::is_iterator<J>,
          is_convertible_relax_helper<I,J>
      >
{ };

} // namespace private_

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_IS_CONVERTIBLE_RELAX_HPP
