/*******************************************************************************
 * sake/core/range/private/traits/size_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_PRIVATE_TRAITS_SIZE_BASE_HPP
#define SAKE_CORE_RANGE_PRIVATE_TRAITS_SIZE_BASE_HPP

#include <cstddef>

#include <boost/mpl/placeholders.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/make_unsigned.hpp>

#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/make_unsigned.hpp>

#include <sake/core/introspection/has_operator_minus.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/facade_fwd.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/iterator/traits_fwd.hpp>
#include <sake/core/range/default_impl/size.hpp>
#include <sake/core/range/static_size.hpp>
#include <sake/core/utility/is_template_base_of.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace default_impl
{

namespace range_traits_private
{

template< class Difference >
struct size_type
    : boost::make_unsigned< Difference >
{ };
template<>
struct size_type< void >
{ typedef std::size_t type; };

template< class R, class Iterator >
struct size_base_bool
{
    typedef sake::iterator_traits< Iterator > iterator_traits;
    SAKE_USING_TYPEDEF( typename iterator_traits, difference_type );
    SAKE_USING_TYPEDEF( typename iterator_traits, traversal );
    typedef typename range_traits_private::size_type<
        difference_type >::type size_type;
    static bool const value = boost_ext::mpl::or4<
        boost_ext::is_convertible<
            traversal, boost::random_access_traversal_tag >,
        sake::range_has_static_size<R>,
        sake::range::default_impl::has_intrinsic_size< R, size_type >,
        sake::is_template_base_of2<
            sake::iterator::facade, Iterator,
            sake::has_operator_minus<
                boost::mpl::_1, boost::mpl::_1, difference_type >
        >
    >::value;
};

template<
    class R, class Iterator,
    bool = size_base_bool< R, Iterator >::value
>
struct size_base;

template< class R, class Iterator >
struct size_base< R, Iterator, false >
{
    typedef typename boost_ext::make_unsigned<
        typename sake::iterator_difference< Iterator >::type >::type size_type;
    typedef boost::false_type size_enable_tag;
};

template< class R, class Iterator >
struct size_base< R, Iterator, true >
{
    typedef typename range_traits_private::size_type<
        typename sake::iterator_difference< Iterator >::type >::type size_type;
    typedef boost::true_type size_enable_tag;
    static size_type
    size(R const & r)
    { return sake::range::default_impl::size(r); }
};

} // namespace range_traits_private

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_PRIVATE_TRAITS_SIZE_BASE_HPP
