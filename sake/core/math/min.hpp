/*******************************************************************************
 * sake/core/math/min.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * min(T0&& x0, T1&& x1) -> result_of::min< T0, T1 >::type
 * min(T0&& x0, T1&& x1, Less less) -> result_of::min< T0, T1 >::type
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_MIN_HPP
#define SAKE_CORE_MATH_MIN_HPP

#include <boost/config.hpp>

#include <sake/boost_ext/type_traits/common_type.hpp>

#include <sake/core/move/forward.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace result_of
{

template< class T0, class T1, class Less = void >
struct min
    : boost_ext::common_type< T0, T1 >
{ };

} // namespace result_of

namespace functional
{

struct min
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::result_of::min, (2,3) )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T0, class T1 >
    typename sake::result_of::min< T0, T1 >::type
    operator()(T0&& x0, T1&& x1) const
    { return x1 < x0 ? sake::forward< T0 >(x1) : sake::forward< T1 >(x0); }

    template< class T0, class T1, class Less >
    typename sake::result_of::min< T0, T1 >::type
    operator()(T0&& x0, T1&& x1, Less less) const
    { return less(x1, x0) ? sake::forward< T0 >(x1) : sake::forward< T1 >(x0); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T0, class T1 >
    typename sake::result_of::min< T0&, T1& >::type
    operator()(T0& x0, T1& x1) const
    { return x1 < x0 ? x1 : x0; }

    template< class T0, class T1, class Less >
    typename sake::result_of::min< T0&, T1& >::type
    operator()(T0& x0, T1& x1, Less less) const
    { return less(x1, x0) ? x1 : x0; }

    template< class T0, class T1 >
    typename sake::result_of::min< T0&, T1 const & >::type
    operator()(T0& x0, T1 const & x1) const
    { return x1 < x0 ? x1 : x0; }

    template< class T0, class T1, class Less >
    typename sake::result_of::min< T0&, T1 const & >::type
    operator()(T0& x0, T1 const & x1, Less less) const
    { return less(x1, x0) ? x1 : x0; }

    template< class T0, class T1 >
    typename sake::result_of::min< T0 const &, T1& >::type
    operator()(T0 const & x0, T1& x1) const
    { return x1 < x0 ? x1 : x0; }

    template< class T0, class T1, class Less >
    typename sake::result_of::min< T0 const &, T1& >::type
    operator()(T0 const & x0, T1& x1, Less less) const
    { return less(x1, x0) ? x1 : x0; }

    template< class T0, class T1 >
    typename sake::result_of::min< T0 const &, T1 const & >::type
    operator()(T0 const & x0, T1 const & x1) const
    { return x1 < x0 ? x1 : x0; }

    template< class T0, class T1, class Less >
    typename sake::result_of::min< T0 const &, T1 const & >::type
    operator()(T0 const & x0, T1 const & x1, Less less) const
    { return less(x1, x0) ? x1 : x0; }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

} // namespace functional

sake::functional::min const min = { };

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_MIN_HPP
