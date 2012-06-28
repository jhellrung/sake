/*******************************************************************************
 * sake/core/range/private/facade/distance.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_PRIVATE_FACADE_DISTANCE_HPP
#define SAKE_CORE_RANGE_PRIVATE_FACADE_DISTANCE_HPP

#include <boost/mpl/placeholders.hpp>

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/introspection/has_operator_minus.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/distance.hpp>
#include <sake/core/iterator/facade_fwd.hpp>
#include <sake/core/range/adapt/fwd.hpp>
#include <sake/core/range/adapt/transform.hpp>
#include <sake/core/range/algorithm/accumulate.hpp>
#include <sake/core/range/distance.hpp>
#include <sake/core/range/distance_fwd.hpp>
#include <sake/core/range/multidim_traits.hpp>
#include <sake/core/range/static_size.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/is_template_base_of.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace range
{

namespace facade_adl
{

namespace private_
{

template< class Derived >
inline typename Derived::difference_type
distance_dispatch(Derived const & /*this_*/, sake::int_tag<3>)
{ return sake::range_static_size< Derived >::value; }

template< class Derived >
inline typename Derived::difference_type
distance_dispatch(Derived const & this_, sake::int_tag<2>)
{ return this_.end() - this_.begin(); }

template< class Derived >
inline typename Derived::difference_type
distance_dispatch(Derived const & this_, sake::int_tag<1>)
{
    return sake::range::algorithm::accumulate(
        sake::range_multidim_traits< Derived const >::outer(this_)
      | sake::range::adapt::functional::transform<
            sake::range::functional::distance >(),
        static_cast< typename Derived::difference_type >(0)
    );
}

template< class Derived >
inline typename Derived::difference_type
distance_dispatch(Derived const & this_, sake::int_tag<0>)
{
    return sake::iterator::distance(
        this_.begin(sake::end_detect_introversal_tag()), sake::_end);
}

template< class Derived >
inline typename Derived::difference_type
distance(Derived const & this_)
{
    SAKE_USING_TYPEDEF( typename Derived, difference_type );
    typedef typename boost_ext::mpl::
    if_<
        sake::range_has_static_size< Derived >,
        sake::int_tag<3>
    >::type::template
    else_if<
        boost_ext::mpl::or2<
            boost_ext::is_convertible<
                typename Derived::range_traversal,
                boost::random_access_traversal_tag
            >,
            sake::is_template_base_of2<
                sake::iterator::facade, typename Derived::const_iterator,
                sake::has_operator_minus<
                    boost::mpl::_1, boost::mpl::_1, difference_type >
            >
        >,
        sake::int_tag<2>
    >::type::template
    else_if<
        sake::range_multidim_enable< Derived const >,
        sake::int_tag<1>
    >::type::template
    else_<
        sake::int_tag<0>
    >::type int_tag_;
    return private_::distance_dispatch(this_, int_tag_());
}

} // namespace private_

} // namespace facade_adl

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_PRIVATE_FACADE_DISTANCE_HPP
