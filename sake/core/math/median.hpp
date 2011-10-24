/*******************************************************************************
 * sake/core/math/median.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * median(T0&& x0, T1&& x1, T2&& x2)
 *     -> result_of::median< T0, T1, T2 >::type
 * median(T0&& x0, T1&& x1, T2&& x2, Less less)
 *     -> result_of::median< T0, T1, T2 >::type
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_MEDIAN_HPP
#define SAKE_CORE_MATH_MEDIAN_HPP

#include <boost/config.hpp>

#include <sake/boost_ext/type_traits/common_type.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/move/forward.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace result_of
{

template< class T0, class T1, class T2, class Less = void >
struct median
    : boost_ext::common_type<
          typename boost_ext::common_type< T0, T1 >::type,
          T2
      >
{ };

} // namespace result_of

namespace functional
{

struct median
{
    SAKE_RESULT_FROM_METAFUNCTION( result_of::median, (3,4) )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T0, class T1, class T2 >
    typename result_of::median< T0, T1, T2 >::type
    operator()(T0&& x0, T1&& x1, T2&& x2) const
    {
        return x1 < x0 ?
               x2 < x1 ? sake::forward< T1 >(x1) :
               x2 < x0 ? sake::forward< T2 >(x2) :
                         sake::forward< T0 >(x0) :
               x2 < x1 ?
               x2 < x0 ? sake::forward< T0 >(x0) :
                         sake::forward< T2 >(x2) :
                         sake::forward< T1 >(x1);
    }

    template< class T0, class T1, class Less >
    typename result_of::median< T0, T1 >::type
    operator()(T0&& x0, T1&& x1, Less less) const
    {
        return less(x1, x0) ?
               less(x2, x1) ? sake::forward< T1 >(x1) :
               less(x2, x0) ? sake::forward< T2 >(x2) :
                              sake::forward< T0 >(x0) :
               less(x2, x1) ?
               less(x2, x0) ? sake::forward< T0 >(x0) :
                              sake::forward< T2 >(x2) :
                              sake::forward< T1 >(x1);
    }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T0, class T1, class T2 >
    typename result_of::median<
        typename boost_ext::remove_rvalue_reference< T0& >::type,
        typename boost_ext::remove_rvalue_reference< T1& >::type,
        typename boost_ext::remove_rvalue_reference< T2& >::type
    >::type
    operator()(T0& x0, T1& x1, T2& x2) const
    {
        return x1 < x0 ?
               x2 < x1 ? x1 : x2 < x0 ? x2 : x0 :
               x2 < x1 ? x2 < x0 ? x0 : x2 : x1;
    }

    template< class T0, class T1, class T2, class Less >
    typename result_of::median<
        typename boost_ext::remove_rvalue_reference< T0& >::type,
        typename boost_ext::remove_rvalue_reference< T1& >::type,
        typename boost_ext::remove_rvalue_reference< T2& >::type
    >::type
    operator()(T0& x0, T1& x1, T2& x2, Less less) const
    {
        return less(x1, x0) ?
               less(x2, x1) ? x1 : less(x2, x0) ? x2 : x0 :
               less(x2, x1) ? less(x2, x0) ? x0 : x2 : x1;
    }

    template< class T0, class T1, class T2 >
    typename result_of::median<
        typename boost_ext::remove_rvalue_reference< T0 const & >::type,
        typename boost_ext::remove_rvalue_reference< T1 const & >::type,
        typename boost_ext::remove_rvalue_reference< T2 const & >::type
    >::type
    operator()(T0 const & x0, T1 const & x1, T2 const & x2) const
    {
        return x1 < x0 ?
               x2 < x1 ? sake::forward< T1 >(x1) :
               x2 < x0 ? sake::forward< T2 >(x2) :
                         sake::forward< T0 >(x0) :
               x2 < x1 ?
               x2 < x0 ? sake::forward< T0 >(x0) :
                         sake::forward< T2 >(x2) :
                         sake::forward< T1 >(x1);
    }

    template< class T0, class T1, class T2, class Less >
    typename result_of::median<
        typename boost_ext::remove_rvalue_reference< T0 const & >::type,
        typename boost_ext::remove_rvalue_reference< T1 const & >::type,
        typename boost_ext::remove_rvalue_reference< T2 const & >::type
    >::type
    operator()(T0 const & x0, T1 const & x1, T2 const & x2, Less less) const
    {
        return less(x1, x0) ?
               less(x2, x1) ? sake::forward< T1 >(x1) :
               less(x2, x0) ? sake::forward< T2 >(x2) :
                              sake::forward< T0 >(x0) :
               less(x2, x1) ?
               less(x2, x0) ? sake::forward< T0 >(x0) :
                              sake::forward< T2 >(x2) :
                              sake::forward< T1 >(x1);
    }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

} // namespace functional

functional::median const median = { };

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_MEDIAN_HPP