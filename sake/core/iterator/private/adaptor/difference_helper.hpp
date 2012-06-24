/*******************************************************************************
 * sake/core/iterator/private/adaptor/difference_helper.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_DIFFERENCE_HELPER_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_DIFFERENCE_HELPER_HPP

#include <boost/mpl/not.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>

#include <sake/core/introspection/has_operator_minus.hpp>
#include <sake/core/iterator/adaptor_fwd.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>

namespace sake
{

namespace iterator
{

namespace adaptor_private
{

template< class I, class J >
inline typename boost::enable_if_c<
    sake::has_operator_minus< I const &, J const &, void >::value,
    J const &
>::type
difference_helper(J const & j)
{ return j; }

template< class I, class D, class J, class P >
inline typename boost::enable_if_c<
    boost_ext::mpl::and2<
        sake::has_operator_minus< I const &, J const &, void >,
        boost::mpl::not_< sake::has_operator_minus<
            I const &, sake::iterator::adaptor<D,J,P> const &, void > >
    >::value,
    J const &
>::type
difference_helper(sake::iterator::adaptor<D,J,P> const & j)
{ return j.base(); }

template< class I >
inline sake::begin_tag
difference_helper(sake::begin_tag)
{ return sake::_begin; }

template< class I >
inline sake::end_tag
difference_helper(sake::end_tag)
{ return sake::_end; }

} // namespace adaptor_private

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_DIFFERENCE_HELPER_HPP