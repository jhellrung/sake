/*******************************************************************************
 * sake/core/math/clamp.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * clamp(L&& lower, T&& x, U&& upper)
 *     -> result_of::clamp<L,T,U>::type
 * clamp(L&& lower, T&& x, U&& upper, Less less)
 *     -> result_of::clamp<L,T,U>::type
 *
 * Note: One can use sake::unbounded for either lower or upper to only clamp in
 * one direction.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_CLAMP_HPP
#define SAKE_CORE_MATH_CLAMP_HPP

#include <boost/config.hpp>

#include <sake/boost_ext/type_traits/common_result_type.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/config.hpp>
#include <sake/core/math/unbounded_tag.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace result_of
{

namespace clamp_private
{

template<
    class L, class T, class U,
    class L_ = typename boost_ext::remove_qualifiers<L>::type,
    class U_ = typename boost_ext::remove_qualifiers<U>::type
>
struct dispatch
    : boost_ext::common_result_type<
          typename boost_ext::common_result_type<T,L>::type,
          U
      >
{ };

template< class L, class T, class U, class L_ >
struct dispatch< L, T, U, L_, sake::unbounded_tag >
    : boost_ext::common_result_type<T,L>
{ };

template< class L, class T, class U, class U_ >
struct dispatch< L, T, U, sake::unbounded_tag, U_ >
    : boost_ext::common_result_type<T,U>
{ };

template< class L, class T, class U >
struct dispatch< L, T, U, sake::unbounded_tag, sake::unbounded_tag >
{ typedef T type; };

} // namespace clamp_private

template< class L, class T, class U, class Less = void >
struct clamp
    : clamp_private::dispatch<L,T,U>
{ };

} // namespace result_of

namespace functional
{

struct clamp
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::result_of::clamp, (3,4) )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class L, class T, class U >
    typename sake::result_of::clamp<L,T,U>::type
    operator()(L&& lower, T&& x, U&& upper) const
    {
        SAKE_ASSERT(!(upper < lower));
        return !(upper < x) ? !(x < lower) ?
               sake::forward<T>(x) :
               sake::forward<L>(lower) :
               sake::forward<U>(upper);
    }

    template< class L, class T, class U, class Less >
    typename sake::result_of::clamp<L,T,U>::type
    operator()(L&& lower, T&& x, U&& upper, Less less) const
    {
        SAKE_ASSERT(!less(upper, lower));
        return !less(upper, x) ? !less(x, lower) ?
               sake::forward<T>(x) :
               sake::forward<L>(lower) :
               sake::forward<U>(upper);
    }

    template< class L, class T >
    typename sake::result_of::clamp< L, T, sake::unbounded_tag >::type
    operator()(L&& lower, T&& x, sake::unbounded_tag) const
    { return !(x < lower) ? sake::forward<T>(x) : sake::forward<L>(lower); }

    template< class L, class T, class Less >
    typename sake::result_of::clamp< L, T, sake::unbounded_tag >::type
    operator()(L&& lower, T&& x, sake::unbounded_tag, Less less) const
    { return !less(x, lower) ? sake::forward<T>(x) : sake::forward<L>(lower); }

    template< class T, class U >
    typename sake::result_of::clamp< sake::unbounded_tag, T, U >::type
    operator()(sake::unbounded_tag, T&& x, U&& upper) const
    { return !(upper < x) ? sake::forward<T>(x) : sake::forward<U>(upper); }

    template< class T, class U, class Less >
    typename sake::result_of::clamp< sake::unbounded_tag, T, U >::type
    operator()(sake::unbounded_tag, T&& x, U&& upper, Less less) const
    { return !less(upper, x) ? sake::forward<T>(x) : sake::forward<U>(upper); }

    template< class T >
    T operator()(sake::unbounded_tag, T&& x, sake::unbounded_tag) const
    { return sake::forward<T>(x); }

    template< class T, class Less >
    T operator()(sake::unbounded_tag, T&& x, sake::unbounded_tag, const Less&) const
    { return sake::forward<T>(x); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class L, class T, class U >
    typename sake::result_of::clamp< L&, T&, U& >::type
    operator()(L& lower, T& x, U& upper) const
    {
        SAKE_ASSERT(!(upper < lower));
        return !(upper < x) ? !(x < lower) ? x : lower : upper;
    }

    template< class L, class T, class U, class Less >
    typename sake::result_of::clamp< L&, T&, U& >::type
    operator()(L& lower, T& x, U& upper, Less less) const
    {
        SAKE_ASSERT(!less(upper, lower));
        return !less(upper, x) ? !less(x, lower) ? x : lower : upper;
    }

    template< class L, class T, class U >
    typename sake::result_of::clamp< L const &, T const &, U const & >::type
    operator()(L const & lower, T const & x, U const & upper) const
    {
        SAKE_ASSERT(!(upper < lower));
        return !(upper < x) ? !(x < lower) ? x : lower : upper;
    }

    template< class L, class T, class U, class Less >
    typename sake::result_of::clamp< L const &, T const &, U const & >::type
    operator()(L const & lower, T const & x, U const & upper, Less less) const
    {
        SAKE_ASSERT(!less(upper, lower));
        return !less(upper, x) ? !less(x, lower) ? x : lower : upper;
    }

    template< class L, class T >
    typename sake::result_of::clamp< L&, T&, sake::unbounded_tag >::type
    operator()(L& lower, T& x, sake::unbounded_tag) const
    { return !(x < lower) ? x : lower; }

    template< class L, class T, class Less >
    typename sake::result_of::clamp< L&, T&, sake::unbounded_tag >::type
    operator()(L& lower, T& x, sake::unbounded_tag, Less less) const
    { return !less(x, lower) ? x : lower; }

    template< class L, class T >
    typename sake::result_of::clamp< L const &, T const &, sake::unbounded_tag >::type
    operator()(L const & lower, T const & x, sake::unbounded_tag) const
    { return !(x < lower) ? x : lower; }

    template< class L, class T, class Less >
    typename sake::result_of::clamp< L const &, T const &, sake::unbounded_tag >::type
    operator()(L const & lower, T const & x, sake::unbounded_tag, Less less) const
    { return !less(x, lower) ? x : lower; }

    template< class T, class U >
    typename sake::result_of::clamp< sake::unbounded_tag, T&, U& >::type
    operator()(sake::unbounded_tag, T& x, U& upper) const
    { return !(upper < x) ? x : upper; }

    template< class T, class U, class Less >
    typename sake::result_of::clamp< sake::unbounded_tag, T&, U& >::type
    operator()(sake::unbounded_tag, T& x, U& upper, Less less) const
    { return !less(upper, x) ? x : upper; }

    template< class T, class U >
    typename sake::result_of::clamp< sake::unbounded_tag, T const &, U const & >::type
    operator()(sake::unbounded_tag, T const & x, U const & upper) const
    { return !(upper < x) ? x : upper; }

    template< class T, class U, class Less >
    typename sake::result_of::clamp< sake::unbounded_tag, T const &, U const & >::type
    operator()(sake::unbounded_tag, T const & x, U const & upper, Less less) const
    { return !less(upper, x) ? x : upper; }

    template< class T >
    typename sake::result_of::clamp< sake::unbounded_tag, T&, sake::unbounded_tag >::type
    operator()(sake::unbounded_tag, T& x, sake::unbounded_tag) const
    { return x; }

    template< class T, class Less >
    typename sake::result_of::clamp< sake::unbounded_tag, T&, sake::unbounded_tag >::type
    operator()(sake::unbounded_tag, T& x, sake::unbounded_tag, Less const & /*less*/) const
    { return x; }

    template< class T >
    typename sake::result_of::clamp< sake::unbounded_tag, T const &, sake::unbounded_tag >::type
    operator()(sake::unbounded_tag, T const & x, sake::unbounded_tag) const
    { return x; }

    template< class T, class Less >
    typename sake::result_of::clamp< sake::unbounded_tag, T const &, sake::unbounded_tag >::type
    operator()(sake::unbounded_tag, T const & x, sake::unbounded_tag, Less const & /*less*/) const
    { return x; }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace clamp_adl_barrier
{ sake::functional::clamp const clamp = { }; }
using namespace clamp_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::functional::clamp const clamp = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_CLAMP_HPP
