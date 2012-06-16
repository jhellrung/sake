/*******************************************************************************
 * sake/core/iterator/private/adaptor/assign_helper.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_ASSIGN_HELPER_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_ASSIGN_HELPER_HPP

#include <boost/utility/enable_if.hpp>

#include <sake/core/introspection/has_operator_assign.hpp>
#include <sake/core/iterator/adaptor_fwd.hpp>

namespace sake
{

namespace iterator
{

namespace adaptor_private
{

template< class I, class J >
inline typename boost::enable_if_c<
    sake::has_operator_assign< I&, J const & >::value, J const & >::type
assign_helper(J const & j)
{ return j; }

template< class I, class D, class J, class P >
inline typename boost::enable_if_c<
    sake::has_operator_assign< I&, J const & >::value, J const & >::type
assign_helper(sake::iterator::adaptor<D,J,P> const & j)
{ return j.base(); }

} // namespace adaptor_private

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_ASSIGN_HELPER_HPP
