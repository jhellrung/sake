/*******************************************************************************
 * sake/core/utility/compressed_tuple/operators.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct operators::result_of::extension::equal0< ... >
 * struct operators::result_of::extension::not_equal0< ... >
 * struct operators::result_of::extension::less0< ... >
 * struct operators::result_of::extension::greater0< ... >
 * struct operators::result_of::extension::less_equal0< ... >
 * struct operators::result_of::extension::greater_equal0< ... >
 * struct result_of::extension::cmp0< ... >
 *
 * operator==(compressed_tuple< T0, ... > const & x, compressed_tuple< U0, ... > const & y) -> ...
 * operator!=(compressed_tuple< T0, ... > const & x, compressed_tuple< U0, ... > const & y) -> ...
 * operator<(compressed_tuple< T0, ... > const & x, compressed_tuple< U0, ... > const & y) -> ...
 * operator>(compressed_tuple< T0, ... > const & x, compressed_tuple< U0, ... > const & y) -> ...
 * operator<=(compressed_tuple< T0, ... > const & x, compressed_tuple< U0, ... > const & y) -> ...
 * operator>=(compressed_tuple< T0, ... > const & x, compressed_tuple< U0, ... > const & y) -> ...
 * cmp(compressed_tuple< T0, ... > const & x, compressed_tuple< U0, ... > const & y) -> ...
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_COMPRESSED_TUPLE_OPERATORS_HPP
#define SAKE_CORE_UTILITY_COMPRESSED_TUPLE_OPERATORS_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>

#include <sake/boost_ext/fusion/algorithm/query/cmp.hpp>
#include <sake/boost_ext/fusion/algorithm/query/equal.hpp>
#include <sake/boost_ext/fusion/algorithm/query/not_equal.hpp>

#include <sake/core/functional/operators/equal.hpp>
#include <sake/core/functional/operators/greater.hpp>
#include <sake/core/functional/operators/greater_equal.hpp>
#include <sake/core/functional/operators/less.hpp>
#include <sake/core/functional/operators/less_equal.hpp>
#include <sake/core/functional/operators/not_equal.hpp>
#include <sake/core/math/cmp.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/utility/compressed_tuple/fwd.hpp>

namespace sake
{

#ifndef BOOST_NO_VARIADIC_TEMPLATES
#define class_T0N class... T
#define class_U0N class... U
#define T0N       T...
#define U0N       U...
#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES
#define class_T0N BOOST_PP_ENUM_PARAMS( SAKE_TUPLE_MAX_SIZE, class T )
#define class_U0N BOOST_PP_ENUM_PARAMS( SAKE_TUPLE_MAX_SIZE, class U )
#define T0N       BOOST_PP_ENUM_PARAMS( SAKE_TUPLE_MAX_SIZE, T )
#define U0N       BOOST_PP_ENUM_PARAMS( SAKE_TUPLE_MAX_SIZE, U )
#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

namespace operators {
namespace result_of {
namespace extension {

template< class_T0N, class_U0N >
struct equal0< sake::compressed_tuple< T0N >, sake::compressed_tuple< U0N > >
    : boost_ext::fusion::result_of::equal<
          sake::compressed_tuple< T0N >, sake::compressed_tuple< U0N > >
{ };

template< class_T0N, class_U0N >
struct not_equal0< sake::compressed_tuple< T0N >, sake::compressed_tuple< U0N > >
    : boost_ext::fusion::result_of::not_equal<
          sake::compressed_tuple< T0N >, sake::compressed_tuple< U0N > >
{ };

template< class_T0N, class_U0N >
struct less0< sake::compressed_tuple< T0N >, sake::compressed_tuple< U0N > >
    : sake::operators::result_of::less<
          typename sake::result_of::cmp<
              sake::compressed_tuple< T0N >, sake::compressed_tuple< U0N >
          >::type,
          sake::zero_t
      >
{ };

template< class_T0N, class_U0N >
struct greater0< sake::compressed_tuple< T0N >, sake::compressed_tuple< U0N > >
    : sake::operators::result_of::greater<
          typename sake::result_of::cmp<
              sake::compressed_tuple< T0N >, sake::compressed_tuple< U0N >
          >::type,
          sake::zero_t
      >
{ };

template< class_T0N, class_U0N >
struct less_equal0< sake::compressed_tuple< T0N >, sake::compressed_tuple< U0N > >
    : sake::operators::result_of::less_equal<
          typename sake::result_of::cmp<
              sake::compressed_tuple< T0N >, sake::compressed_tuple< U0N >
          >::type,
          sake::zero_t
      >
{ };

template< class_T0N, class_U0N >
struct greater_equal0< sake::compressed_tuple< T0N >, sake::compressed_tuple< U0N > >
    : sake::operators::result_of::greater_equal<
          typename sake::result_of::cmp<
              sake::compressed_tuple< T0N >, sake::compressed_tuple< U0N >
          >::type,
          sake::zero_t
      >
{ };

} // namespace extension
} // namespace result_of
} // namespace operators

namespace result_of {
namespace extension {

template< class_T0N, class_U0N >
struct cmp0< sake::compressed_tuple< T0N >, sake::compressed_tuple< U0N > >
    : boost_ext::fusion::result_of::cmp<
          sake::compressed_tuple< T0N >, sake::compressed_tuple< U0N > >
{ };

} // namespace extension
} // namespace result_of

namespace compressed_tuple_adl
{

template< class_T0N, class_U0N >
inline typename sake::operators::result_of::equal<
    sake::compressed_tuple< T0N >, sake::compressed_tuple< U0N > >::type
operator==(sake::compressed_tuple< T0N > const & x, sake::compressed_tuple< U0N > const & y)
{ return boost_ext::fusion::equal(x, y); }

template< class_T0N, class_U0N >
inline typename sake::operators::result_of::not_equal<
    sake::compressed_tuple< T0N >, sake::compressed_tuple< U0N > >::type
operator!=(sake::compressed_tuple< T0N > const & x, sake::compressed_tuple< U0N > const & y)
{ return boost_ext::fusion::not_equal(x, y); }

template< class_T0N, class_U0N >
inline typename sake::operators::result_of::less<
    sake::compressed_tuple< T0N >, sake::compressed_tuple< U0N > >::type
operator<(sake::compressed_tuple< T0N > const & x, sake::compressed_tuple< U0N > const & y)
{ return sake::cmp(x,y) < sake::zero; }

template< class_T0N, class_U0N >
inline typename sake::operators::result_of::greater<
    sake::compressed_tuple< T0N >, sake::compressed_tuple< U0N > >::type
operator>(sake::compressed_tuple< T0N > const & x, sake::compressed_tuple< U0N > const & y)
{ return sake::cmp(x,y) > sake::zero; }

template< class_T0N, class_U0N >
inline typename sake::operators::result_of::less_equal<
    sake::compressed_tuple< T0N >, sake::compressed_tuple< U0N > >::type
operator<=(sake::compressed_tuple< T0N > const & x, sake::compressed_tuple< U0N > const & y)
{ return sake::cmp(x,y) <= sake::zero; }

template< class_T0N, class_U0N >
inline typename sake::operators::result_of::greater_equal<
    sake::compressed_tuple< T0N >, sake::compressed_tuple< U0N > >::type
operator>=(sake::compressed_tuple< T0N > const & x, sake::compressed_tuple< U0N > const & y)
{ return sake::cmp(x,y) >= sake::zero; }

template< class_T0N, class_U0N >
inline typename sake::result_of::cmp<
    sake::compressed_tuple< T0N >, sake::compressed_tuple< U0N > >::type
cmp(sake::compressed_tuple< T0N > const & x, sake::compressed_tuple< U0N > const & y)
{ return boost_ext::fusion::cmp(x, y); }

} // namespace compressed_tuple_adl

#undef class_T0N
#undef class_U0N
#undef T0N
#undef U0N

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_COMPRESSED_TUPLE_OPERATORS_HPP
