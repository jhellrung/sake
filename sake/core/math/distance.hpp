/*******************************************************************************
 * sake/core/math/distance.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * distance(T0 const & x0, T1 const & x1)
 *     -> result_of::distance< T0, T1 >::type
 * distance(T0 const & x0, T1 const & x1, K const & k)
 *     -> result_of::distance< T0, T1, K >::type
 * struct functional::distance
 *
 * distance essentially just extends std::distance to integral types.
 * The result of distance is always a signed integral type, even if the
 * parameter types are unsigned integral types.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_DISTANCE_HPP
#define SAKE_CORE_MATH_DISTANCE_HPP

#include <boost/iterator/iterator_categories.hpp>
#include <boost/iterator/iterator_traits.hpp>

#include <sake/boost_ext/type_traits/common_type.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/iterator/is_iterator.hpp>
#include <sake/core/math/integer_difference.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace result_of
{

template< class T0, class T1 = T0, class K = void >
struct distance;

} // namespace result_of

namespace distance_private
{

template< class T0, class T1, class K >
struct dispatch_index;

template<
    class T0, class T1,
    class K = void,
    unsigned int = dispatch_index< T0, T1, K >::value
>
struct dispatch;

} // namespace distance_private

namespace functional
{

struct distance
{
    SAKE_RESULT_FROM_METAFUNCTION( result_of::distance, (2,3) )

    template< class T0, class T1 >
    typename result_of::distance< T0, T1 >::type
    operator()(T0 const & x0, T1 const & x1) const
    { return distance_private::dispatch< T0, T1 >::apply(x0, x1); }

    template< class T0, class T1, class K >
    typename result_of::distance< T0, T1, K >::type
    operator()(T0 const & x0, T1 const & x1, K const & k) const
    { return distance_private::dispatch< T0, T1, K >::apply(x0, x1, k); }
};

} // namespace functional

functional::distance const distance = { };

namespace result_of
{

namespace distance_private
{

template< class T, bool = sake::is_iterator<T>::value >
struct dispatch;

template< class T >
struct dispatch< T, false >
    : sake::integer_difference<T>
{ };

template< class T >
struct dispatch< T, true >
    : boost::iterator_difference<T>
{ };

} // namespace distance_private

template< class T0, class T1 /*= T0*/, class K /*= void*/ >
struct distance
    : boost_ext::common_type< K, typename distance< T0, T1 >::type >
{ };

template< class T0, class T1 >
struct distance< T0, T1, void >
    : distance_private::dispatch<
          typename boost_ext::common_type<
              typename boost_ext::remove_qualifiers< T0 >::type,
              typename boost_ext::remove_qualifiers< T1 >::type
          >::type
      >
{ };

} // namespace result_of

namespace distance_private
{

template< class T0, class T1, class K >
struct dispatch_index
{
    typedef typename boost_ext::common_type< T0, T1 >::type common_type_;
    typedef typename result_of::distance< T0, T1, K >::type result_type;
    static unsigned int const _ =
        (1 << 2) * sake::is_iterator< common_type_ >::value
      | (1 << 1) * boost_ext::is_convertible< common_type_, result_type >::value
      | (1 << 0);
    static unsigned int const value = sake::static_intlog2_c<_>::value;
};

template< class T0, class T1 >
struct dispatch< T0, T1, void, 2 >
{
    typedef typename boost_ext::common_type< T0, T1 >::type common_type_;
    typedef typename result_of::distance< T0, T1 >::type result_type;

    static result_type
    apply(T0 i0, T1 const i1, boost::incrementable_traversal_tag)
    {
        result_type k = 0;
        for(; i0 != i1; ++i0, ++k);
        return k;
    }

    static result_type
    apply(T0 const & i0, T1 const & i1, boost::random_access_traversal_tag)
    { return i1 - i0; }

    static result_type
    apply(T0 const & i0, T1 const & i1)
    { return apply(i0, i1, typename boost::iterator_traversal< common_type_ >::type()); }
};

template< class T0, class T1, class K >
struct dispatch< T0, T1, K, 2 >
{
    typedef typename boost_ext::common_type< T0, T1 >::type common_type_;
    typedef typename result_of::distance< T0, T1, K >::type result_type;

    static result_type
    apply(T0 i0, const T1 i1, result_type k, boost::incrementable_traversal_tag)
    {
        SAKE_ASSERT_RELATION( k, ==, sake::zero );
        for(; i0 != i1; ++i0, ++k);
        return k;
    }

    static result_type
    apply(T0 const & i0, T1 const & i1, K const &, boost::random_access_traversal_tag)
    { return i1 - i0; }

    static result_type
    apply(T0 const & i0, T1 const & i1, K const & k)
    { return apply(i0, i1, k, typename boost::iterator_traversal< common_type_ >::type()); }
};

template< class T0, class T1 >
struct dispatch< T0, T1, void, 1 >
{
    typedef typename result_of::distance< T0, T1 >::type result_type;
    static result_type
    apply(T0 const & i0, T1 const & i1)
    { return static_cast< result_type >(i1) - static_cast< result_type >(i0); }
};

template< class T0, class T1, class K >
struct dispatch< T0, T1, K, 1 >
{
    typedef typename result_of::distance< T0, T1, K >::type result_type;
    static result_type
    apply(T0 const & i0, T1 const & i1, K const &)
    { return static_cast< result_type >(i1) - static_cast< result_type >(i0); }
};

template< class T0, class T1 >
struct dispatch< T0, T1, void, 0 >
{
    typedef typename result_of::distance< T0, T1 >::type result_type;
    static result_type
    apply(T0 const & i0, T1 const & i1)
    { return i1 - i0; }
};

template< class T0, class T1, class K >
struct dispatch< T0, T1, K, 0 >
{
    typedef typename result_of::distance< T0, T1, K >::type result_type;
    static result_type
    apply(T0 const & i0, T1 const & i1, K const &)
    { return i1 - i0; }
};

} // namespace distance_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_DISTANCE_HPP
