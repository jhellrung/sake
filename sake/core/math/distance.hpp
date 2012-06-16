/*******************************************************************************
 * sake/core/math/distance.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * distance(T0 const & x0, T1 const & x1)
 *     -> result_of::distance< T0, T1 >::type
 * struct functional::distance
 *
 * Essentially just extends std::distance to integral types.
 *
 * The result of distance is always a signed integral type, even if the
 * parameter types are unsigned integral types.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_DISTANCE_HPP
#define SAKE_CORE_MATH_DISTANCE_HPP

#include <boost/mpl/if.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_void.hpp>

#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/common_result_type.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/distance.hpp>
#include <sake/core/iterator/is_iterator.hpp>
#include <sake/core/math/integer_difference.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace result_of
{

template< class T0, class T1 = T0 >
struct distance;

} // namespace result_of

namespace distance_private
{

template< class T0, class T1 >
struct dispatch_index;

template<
    class T0, class T1,
    int = dispatch_index< T0, T1 >::value
>
struct dispatch;

} // namespace distance_private

namespace functional
{

struct distance
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::result_of::distance, 2 )

    template< class T0, class T1 >
    typename sake::result_of::distance< T0, T1 >::type
    operator()(T0 const & x0, T1 const & x1) const
    { return distance_private::dispatch< T0, T1 >::apply(x0, x1); }
};

} // namespace functional

sake::functional::distance const distance = { };

namespace result_of
{

template< class T0, class T1 /*= T0*/ >
struct distance
    : sake::distance_private::dispatch<
          typename boost_ext::remove_qualifiers< T0 >::type,
          typename boost_ext::remove_qualifiers< T1 >::type
      >
{ };

} // namespace result_of

namespace distance_private
{

template< class T >
struct is_iterator_or_begin_end_tag
    : sake::is_iterator<T>
{ };

template<>
struct is_iterator_or_begin_end_tag< sake::begin_tag >
    : boost::true_type
{ };

template<>
struct is_iterator_or_begin_end_tag< sake::end_tag >
    : boost::true_type
{ };

template< class T0, class T1 >
struct dispatch_index_integer
{
    typedef typename boost_ext::common_result_type< T0, T1 >::type common_type_;
    BOOST_STATIC_ASSERT((!boost::is_void< common_type_ >::value));
    typedef typename sake::integer_difference< common_type_ >::type type;
    static int const value = boost::mpl::if_c<
        boost_ext::is_convertible< common_type_, type >,
        sake::int_tag<1>,
        sake::int_tag<0>
    >::type::value;
};

template< class T0, class T1 >
struct dispatch_index
{
    static int const value = boost::mpl::if_c<
        boost_ext::mpl::and2<
            distance_private::is_iterator_or_begin_end_tag< T0 >,
            distance_private::is_iterator_or_begin_end_tag< T1 >
        >::value,
        sake::int_tag<2>,
        distance_private::dispatch_index_integer< T0, T1 >
    >::type::value;
};

template< class I0, class I1 >
struct dispatch< I0, I1, 2 >
{
    typedef typename sake::iterator::result_of::distance< I0, I1 >::type type;
    static type apply(I0 const & i0, I1 const & i1)
    { return sake::iterator::distance(i0, i1); }
};

template< class T0, class T1 >
struct dispatch< T0, T1, 1 >
{
    typedef typename boost_ext::common_result_type< T0, T1 >::type common_type_;
    typedef typename sake::integer_difference< common_type_ >::type type;
    static type apply(T0 const & x0, T1 const & x1)
    { return static_cast< type >(x1) - static_cast< type >(x0); }
};

template< class T0, class T1 >
struct dispatch< T0, T1, 0 >
{
    typedef typename boost_ext::common_result_type< T0, T1 >::type common_type_;
    typedef typename sake::integer_difference< common_type_ >::type type;
    static type apply(T0 const & x0, T1 const & x1)
    { return x1 - x0; }
};

} // namespace distance_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_DISTANCE_HPP
