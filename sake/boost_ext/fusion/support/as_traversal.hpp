/*******************************************************************************
 * sake/boost_ext/fusion/support/as_traversal.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::fusion::as_traversal< Traversal >
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_FUSION_SUPPORT_AS_TRAVERSAL_HPP
#define SAKE_BOOST_EXT_FUSION_SUPPORT_AS_TRAVERSAL_HPP

#include <boost/fusion/support/category_of.hpp>

#include <sake/core/iterator/categories.hpp>

namespace sake
{

namespace boost_ext
{

namespace fusion
{

namespace traits
{

template< class Traversal >
struct as_traversal;

template<>
struct as_traversal< boost::incrementable_traversal_tag >
{ typedef boost::fusion::incrementable_traversal_tag type; };
template<>
struct as_traversal< boost::single_pass_traversal_tag >
{ typedef boost::fusion::single_pass_traversal_tag type; };
template<>
struct as_traversal< boost::forward_traversal_tag >
{ typedef boost::fusion::forward_traversal_tag type; };
template<>
struct as_traversal< boost::bidirectional_traversal_tag >
{ typedef boost::fusion::bidirectional_traversal_tag type; };
template<>
struct as_traversal< boost::random_access_traversal_tag >
{ typedef boost::fusion::random_access_traversal_tag type; };

} // namespace traits

} // namespace fusion

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_FUSION_SUPPORT_AS_TRAVERSAL_HPP
