/*******************************************************************************
 * sake/core/iterator/private/adaptor/convert_helper.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_CONVERT_HELPER_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_CONVERT_HELPER_HPP

#include <boost/mpl/not.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_convertible_wndp2bp.hpp>

#include <sake/core/iterator/adaptor_fwd.hpp>

namespace sake
{

namespace iterator
{

namespace adaptor_private
{

template< class I, class J >
inline typename boost::enable_if_c<
    boost_ext::is_convertible_wndp2bp< J const &, I >::value,
    J const &
>::type
convert_helper(J const & j)
{ return j; }

template< class I, class D, class J, class P >
inline typename boost::enable_if_c<
    boost_ext::mpl::and2<
        boost_ext::is_convertible_wndp2bp< J const &, I >,
        boost::mpl::not_< boost_ext::is_convertible_wndp2bp<
            sake::iterator::adaptor<D,J,P> const &, I > >
    >::value,
    J const &
>::type
convert_helper(sake::iterator::adaptor<D,J,P> const & j)
{ return j.base(); }

} // namespace adaptor_private

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_CONVERT_HELPER_HPP