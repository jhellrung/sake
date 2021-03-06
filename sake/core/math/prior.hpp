/*******************************************************************************
 * sake/core/math/prior.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * prior(T x) -> T
 * prior(T x, D n) -> T
 * prior_c<N>(T x) -> T
 * struct functional::prior
 *
 * Essentially just extends boost::prior to integral types.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_PRIOR_HPP
#define SAKE_CORE_MATH_PRIOR_HPP

#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/config.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/is_iterator.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace prior_private
{

template< class T, bool = sake::is_iterator<T>::value >
struct dispatch;

} // namespace prior_private

namespace result_of
{

template< class T, class D = void >
struct prior
    : boost_ext::remove_qualifiers<T>
{ };

template< class T, int N >
struct prior_c
    : boost_ext::remove_qualifiers<T>
{ };

} // namespace result_of

namespace functional
{

struct prior
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::result_of::prior, (1,2) )

    template< class T >
    T operator()(T x) const
    { return --x; }

    template< class T, class D >
    T operator()(T const & x, D const & n) const
    { return prior_private::dispatch<T>::apply(x, n); }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace prior_adl_barrier
{ sake::functional::prior const prior = { }; }
using namespace prior_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::functional::prior const prior = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

template< int N, class T >
inline T
prior_c(T const & x)
{ return sake::prior(x, boost::integral_constant< int, N >()); }

namespace prior_private
{

template< class T >
struct dispatch< T, false >
{
    template< class D >
    static T
    apply(T const & x, D const & n)
    { return x - n; }
};

template< class T >
struct dispatch< T, true >
{
    template< class D >
    static T
    apply(T x, D n, boost::bidirectional_traversal_tag)
    {
        if(sake::zero < n)
            do { --x; } while(!(--n == sake::zero));
        else if(n < sake::zero)
            do { ++x; } while(!(++n == sake::zero));
        return x;
    }

    template< class D >
    static T
    apply(T const & x, D const n, boost::random_access_traversal_tag)
    { return x - n; }

    template< class D >
    static T
    apply(T const & x, D const n)
    {
        typedef typename sake::iterator_traversal<T>::type iterator_traversal_;
        BOOST_STATIC_ASSERT((boost_ext::is_convertible<
            iterator_traversal_,
            boost::bidirectional_traversal_tag
        >::value));
        return apply(x, n, iterator_traversal_());
    }
};

} // namespace prior_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_PRIOR_HPP
