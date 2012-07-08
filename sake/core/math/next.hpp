/*******************************************************************************
 * sake/core/math/next.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * next(T x) -> T
 * next(T x, D n) -> T
 * next_c<N>(T x) -> T
 * struct functional::next
 *
 * Essentially just extends boost::next to integral types.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_NEXT_HPP
#define SAKE_CORE_MATH_NEXT_HPP

#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/config.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/is_iterator.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace next_private
{

template< class T, bool = sake::is_iterator<T>::value >
struct dispatch;

} // namespace next_private

namespace result_of
{

template< class T, class D = void >
struct next
    : boost_ext::remove_qualifiers<T>
{ };

template< class T, int N >
struct next_c
    : boost_ext::remove_qualifiers<T>
{ };

} // namespace result_of

namespace functional
{

struct next
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::result_of::next, (1,2) )

    template< class T >
    T operator()(T x) const
    { return ++x; }

    template< class T, class D >
    T operator()(T const & x, D const & n) const
    { return next_private::dispatch<T>::apply(x, n); }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace next_adl_barrier
{ sake::functional::next const next = { }; }
using namespace next_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::functional::next const next = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

template< int N, class T >
inline T
next_c(T const & x)
{ return sake::next(x, boost::integral_constant< int, N >()); }

namespace next_private
{

template< class T >
struct dispatch< T, false >
{
    template< class D >
    static T
    apply(T const & x, D const & n)
    { return static_cast<T>(x + n); }
};

template< class T >
struct dispatch< T, true >
{
    template< class D >
    static T
    apply(T x, D n, boost::incrementable_traversal_tag)
    {
        SAKE_ASSERT(!(n < sake::zero));
        if(sake::zero < n)
            do { ++x; } while(!(--n == sake::zero));
        return x;
    }

    template< class D >
    static T
    apply(T x, D n, boost::bidirectional_traversal_tag)
    {
        if(sake::zero < n)
            do { ++x; } while(!(--n == sake::zero));
        else if(n < sake::zero)
            do { --x; } while(!(++n == sake::zero));
        return x;
    }

    template< class D >
    static T
    apply(T const & x, D const n, boost::random_access_traversal_tag)
    { return x + n; }

    template< class D >
    static T
    apply(T const & x, D const n)
    { return apply(x, n, typename sake::iterator_traversal<T>::type()); }
};

} // namespace next_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_NEXT_HPP
