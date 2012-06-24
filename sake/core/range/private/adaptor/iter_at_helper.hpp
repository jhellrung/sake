/*******************************************************************************
 * sake/core/range/private/adaptor/iter_at_helper.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_PRIVATE_ADAPTOR_AT_HELPER_HPP
#define SAKE_CORE_RANGE_PRIVATE_ADAPTOR_AT_HELPER_HPP

#include <boost/mpl/not.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/iterator/adaptor_fwd.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/range/traits_fwd.hpp>

namespace sake
{

namespace range
{

namespace adaptor_private
{

template< class R, class I >
inline typename boost::enable_if_c<
    boost_ext::is_convertible<
        I const &, typename sake::range_iterator<R>::type >::value,
    I const &
>::type
iter_at_helper(I const & i)
{ return i; }

template< class R, class D, class I, class P >
inline typename boost::enable_if_c<
    boost_ext::mpl::and2<
        boost_ext::is_convertible<
            I const &, typename sake::range_iterator<R>::type >,
        boost::mpl::not_< boost_ext::is_convertible<
            sake::iterator::adaptor<D,I,P> const &,
            typename sake::range_iterator<R>::type
        > >
    >::value,
    I const &
>::type
iter_at_helper(sake::iterator::adaptor<D,I,P> const & i)
{ return i.base(); }

template< class R >
inline sake::begin_tag
iter_at_helper(sake::begin_tag)
{ return sake::_begin; }

template< class R >
inline sake::end_tag
iter_at_helper(sake::end_tag)
{ return sake::_end; }

} // namespace adaptor_private

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_PRIVATE_ADAPTOR_AT_HELPER_HPP
