/*******************************************************************************
 * sake/core/math/max.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * max(T0&& x0, T1&& x1) -> result_of::max< T0, T1 >::type
 * max(T0&& x0, T1&& x1, Less less) -> result_of::max< T0, T1 >::type
 *
 * Unlike std::max, sake::max returns the *second* parameter when the given
 * parameters are equivalently ordered.  Thus, sake::max is a *stable* selection
 * algorithm (it preserves the order of its equivalently ordered inputs).
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_MAX_HPP
#define SAKE_CORE_MATH_MAX_HPP

#include <boost/config.hpp>

#include <sake/boost_ext/type_traits/common_result_type.hpp>

#include <sake/core/config.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace result_of
{

template< class T0, class T1, class Less = void >
struct max
    : boost_ext::common_result_type< T0, T1 >
{ };

} // namespace result_of

namespace functional
{

struct max
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::result_of::max, (2,3) )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T0, class T1 >
    typename sake::result_of::max< T0, T1 >::type
    operator()(T0&& x0, T1&& x1) const
    { return x1 < x0 ? sake::forward< T0 >(x0) : sake::forward< T1 >(x1); }

    template< class T0, class T1, class Less >
    typename sake::result_of::max< T0, T1 >::type
    operator()(T0&& x0, T1&& x1, Less less) const
    { return less(x1, x0) ? sake::forward< T0 >(x0) : sake::forward< T1 >(x1); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T0, class T1 >
    typename sake::result_of::max< T0&, T1& >::type
    operator()(T0& x0, T1& x1) const
    { return x1 < x0 ? x0 : x1; }

    template< class T0, class T1, class Less >
    typename sake::result_of::max< T0&, T1& >::type
    operator()(T0& x0, T1& x1, Less less) const
    { return less(x1, x0) ? x0 : x1; }

    template< class T0, class T1 >
    typename sake::result_of::max< T0&, T1 const & >::type
    operator()(T0& x0, T1 const & x1) const
    { return x1 < x0 ? x0 : x1; }

    template< class T0, class T1, class Less >
    typename sake::result_of::max< T0&, T1 const & >::type
    operator()(T0& x0, T1 const & x1, Less less) const
    { return less(x1, x0) ? x0 : x1; }

    template< class T0, class T1 >
    typename sake::result_of::max< T0 const &, T1& >::type
    operator()(T0 const & x0, T1& x1) const
    { return x1 < x0 ? x0 : x1; }

    template< class T0, class T1, class Less >
    typename sake::result_of::max< T0 const &, T1& >::type
    operator()(T0 const & x0, T1& x1, Less less) const
    { return less(x1, x0) ? x0 : x1; }

    template< class T0, class T1 >
    typename sake::result_of::max< T0 const &, T1 const & >::type
    operator()(T0 const & x0, T1 const & x1) const
    { return x1 < x0 ? x0 : x1; }

    template< class T0, class T1, class Less >
    typename sake::result_of::max< T0 const &, T1 const & >::type
    operator()(T0 const & x0, T1 const & x1, Less less) const
    { return less(x1, x0) ? x0 : x1; }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace max_adl_barrier
{ sake::functional::max const max = { }; }
using namespace max_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::functional::max const max = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_MAX_HPP
