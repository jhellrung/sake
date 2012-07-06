/*******************************************************************************
 * sake/core/range/private/traits/distance_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_PRIVATE_TRAITS_DISTANCE_BASE_HPP
#define SAKE_CORE_RANGE_PRIVATE_TRAITS_DISTANCE_BASE_HPP

#include <boost/mpl/placeholders.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_signed.hpp>

#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/introspection/has_operator_minus.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/facade_fwd.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/iterator/traits_fwd.hpp>
#include <sake/core/range/default_impl/distance.hpp>
#include <sake/core/range/private/traits/size_base.hpp>
#include <sake/core/range/static_size.hpp>
#include <sake/core/utility/is_template_base_of.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace default_impl
{

namespace range_traits_private
{

template< class R, class Iterator >
struct distance_base_bool
{
    typedef sake::iterator_traits< Iterator > iterator_traits;
    SAKE_USING_TYPEDEF( typename iterator_traits, difference_type );
    SAKE_USING_TYPEDEF( typename iterator_traits, traversal );
    static bool const value = boost_ext::mpl::or4<
        boost_ext::is_convertible<
            traversal, boost::forward_traversal_tag >,
        sake::range_has_static_size<R>,
        sake::range::default_impl::has_intrinsic_distance< R, difference_type >,
        sake::is_template_base_of2<
            sake::iterator::facade, Iterator,
            sake::has_operator_minus<
                boost::mpl::_1, boost::mpl::_1, difference_type >
        >
    >::value;
};

template<
    class R, class Iterator,
    bool = distance_base_bool< R, Iterator >::value
>
struct distance_base;

template< class R, class Iterator >
struct distance_base< R, Iterator, false >
    : range_traits_private::size_base< R, Iterator >
{ typedef boost::false_type distance_enable_tag; };

template< class R, class Iterator >
struct distance_base< R, Iterator, true >
    : range_traits_private::size_base< R, Iterator >
{
    typedef typename sake::iterator_difference<
        Iterator >::type difference_type;
    BOOST_STATIC_ASSERT((boost::is_signed< difference_type >::value));
    typedef boost::true_type distance_enable_tag;
    static difference_type
    distance(R const & r)
    { return sake::range::default_impl::distance(r); }
};

} // namespace range_traits_private

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_PRIVATE_TRAITS_DISTANCE_BASE_HPP
