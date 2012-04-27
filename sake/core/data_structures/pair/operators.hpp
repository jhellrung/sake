/*******************************************************************************
 * sake/core/data_structures/pair/operators.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct operators::result_of::extension::equal< ... >
 * struct operators::result_of::extension::not_equal< ... >
 * struct operators::result_of::extension::less< ... >
 * struct operators::result_of::extension::greater< ... >
 * struct operators::result_of::extension::less_equal< ... >
 * struct operators::result_of::extension::greater_equal< ... >
 * struct result_of::extension::cmp< ... >
 *
 * operator==(pair<T0,T1> const & x, pair<U0,U1> const & y) -> ...
 * operator!=(pair<T0,T1> const & x, pair<U0,U1> const & y) -> ...
 * operator<(pair<T0,T1> const & x, pair<U0,U1> const & y) -> ...
 * operator>(pair<T0,T1> const & x, pair<U0,U1> const & y) -> ...
 * operator<=(pair<T0,T1> const & x, pair<U0,U1> const & y) -> ...
 * operator>=(pair<T0,T1> const & x, pair<U0,U1> const & y) -> ...
 * cmp(pair<T0,T1> const & x, pair<U0,U1> const & y) -> ...
 ******************************************************************************/

#ifndef SAKE_CORE_DATA_STRUCTURES_PAIR_OPERATORS_HPP
#define SAKE_CORE_DATA_STRUCTURES_PAIR_OPERATORS_HPP

#include <boost/preprocessor/cat.hpp>

#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/common_type.hpp>

#include <sake/core/data_structures/pair/fwd.hpp>
#include <sake/core/functional/operators/logical.hpp>
#include <sake/core/functional/operators/relational.hpp>
#include <sake/core/math/cmp_fwd.hpp>
#include <sake/core/math/zero.hpp>

namespace sake
{

namespace pair_adl
{

namespace private_
{

template<
    template< class _T0, class _T1, class _U0, class _U1 >
    class F,
    class T0, class T1, class U0, class U1
>
struct result_of_helper
    : F<
          typename boost_ext::add_reference_add_const< T0 >::type,
          typename boost_ext::add_reference_add_const< T1 >::type,
          typename boost_ext::add_reference_add_const< U0 >::type,
          typename boost_ext::add_reference_add_const< U1 >::type
      >
{ };

} // namespace private_

namespace operators
{

namespace result_of
{

namespace private_
{

template< class T0, class T1, class U0, class U1 >
struct equal_impl
    : sake::operators::result_of::and_<
          typename sake::operators::result_of::equal< T0, U0 >::type,
          typename sake::operators::result_of::equal< T1, U1 >::type
      >
{ };

template< class T0, class T1, class U0, class U1 >
struct not_equal_impl
    : sake::operators::result_of::or_<
          typename sake::operators::result_of::not_equal< T0, U0 >::type,
          typename sake::operators::result_of::not_equal< T1, U1 >::type
      >
{ };

template< class T0, class T1, class U0, class U1 >
struct less_impl
    : sake::operators::result_of::or_<
          typename sake::operators::result_of::less< T0, U0 >::type,
          typename sake::operators::result_of::and_<
              typename sake::operators::result_of::not_<
                  typename sake::operators::result_of::less< U0, T1 >::type
              >::type,
              typename sake::operators::result_of::less< T1, U1 >::type
          >::type
      >
{ };

template< class T0, class T1, class U0, class U1 >
struct greater_impl
    : sake::operators::result_of::or_<
          typename sake::operators::result_of::greater< T0, U0 >::type,
          typename sake::operators::result_of::and_<
              typename sake::operators::result_of::not_<
                  typename sake::operators::result_of::greater< U0, T1 >::type
              >::type,
              typename sake::operators::result_of::greater< T1, U1 >::type
          >::type
      >
{ };

template< class T0, class T1, class U0, class U1 >
struct less_equal_impl
    : sake::operators::result_of::and_<
          typename sake::operators::result_of::less_equal< T0, U0 >::type,
          typename sake::operators::result_of::or_<
              typename sake::operators::result_of::not_<
                  typename sake::operators::result_of::less_equal< U0, T1 >::type
              >::type,
              typename sake::operators::result_of::less_equal< T1, U1 >::type
          >::type
      >
{ };

template< class T0, class T1, class U0, class U1 >
struct greater_equal_impl
    : sake::operators::result_of::and_<
          typename sake::operators::result_of::greater_equal< T0, U0 >::type,
          typename sake::operators::result_of::or_<
              typename sake::operators::result_of::not_<
                  typename sake::operators::result_of::greater_equal< U0, T0 >::type
              >::type,
              typename sake::operators::result_of::greater_equal< T1, U1 >::type
          >::type
      >
{ };

} // namespace private_

#define define_operator( name ) \
template< class T, class U > struct name; \
template< class T0, class T1, class U0, class U1 > \
struct name< sake::pair< T0, T1 >, sake::pair< U0, U1 > > \
    : sake::pair_adl::private_::result_of_helper< \
          private_::BOOST_PP_CAT( name, _impl ), \
          T0, T1, U0, U1 \
      > \
{ };

define_operator( equal )
define_operator( not_equal )
define_operator( less )
define_operator( greater )
define_operator( less_equal )
define_operator( greater_equal )

#undef define_operator

} // namespace result_of

} // namespace operators

#define define_operator( op, name, expression ) \
template< class T0, class T1, class U0, class U1 > \
inline typename sake::pair_adl::operators::result_of::name< \
    sake::pair<T0,T1>, \
    sake::pair<U0,U1> \
>::type \
operator op (sake::pair<T0,T1> const & x, sake::pair<U0,U1> const & y) \
{ return expression; }

define_operator( ==, equal,
    x.first == y.first && x.second == y.second )
define_operator( !=, not_equal,
    x.first != y.first || x.second != y.second )
define_operator( < , less,
    x.first < y.first || (!(y.first < x.first) && x.second < y.second) )
define_operator( > , greater      ,
    x.first > y.first || (!(y.first > x.first) && x.second > y.second) )
define_operator( <=, less_equal,
    x.first <= y.first && (!(y.first <= x.first) || x.second <= y.second) )
define_operator( >=, greater_equal,
    x.first >= y.first && (!(y.first >= x.first) || x.second >= y.second) )

#undef define_operator

namespace result_of
{

namespace private_
{

template< class T0, class T1, class U0, class U1 >
struct cmp_impl
    : boost_ext::common_type<
          typename sake::result_of::cmp< T0, U0 >::type,
          typename sake::result_of::cmp< T1, U1 >::type
      >
{ };

} // namespace private_

template< class T, class U > struct cmp;
template< class T0, class T1, class U0, class U1 >
struct cmp< sake::pair<T0,T1>, sake::pair<U0,U1> >
    : sake::pair_adl::private_::result_of_helper<
          sake::pair_adl::result_of::private_::cmp_impl,
          T0, T1, U0, U1
      >
{ };

} // namespace result_of

template< class T0, class T1, class U0, class U1 >
inline typename sake::pair_adl::result_of::cmp<
    sake::pair< T0, T1 >,
    sake::pair< U0, U1 >
>::type
cmp(sake::pair< T0, T1 > const & x, sake::pair< U0, U1 > const & y)
{
    typename sake::result_of::cmp<
        typename boost_ext::add_reference_add_const< T0 >::type,
        typename boost_ext::add_reference_add_const< U0 >::type
    >::type const s0 = sake::cmp(x.first, y.first);
    return s0 == sake::zero ? sake::cmp(x.second, y.second) : s0;
}

} // namespace pair_adl

namespace operators {
namespace result_of {
namespace extension {

#define define_operator( name ) \
template< class T0, class T1, class U0, class U1 > \
struct name< sake::pair< T0, T1 >, sake::pair< U0, U1 >, void > \
    : sake::pair_adl::operators::result_of::name< \
          sake::pair< T0, T1 >, \
          sake::pair< U0, U1 > \
      > \
{ };

define_operator( equal )
define_operator( not_equal )
define_operator( less )
define_operator( greater )
define_operator( less_equal )
define_operator( greater_equal )

#undef define_operator

} // namespace extension
} // namespace result_of
} // namespace operators

namespace result_of {
namespace extension {

template< class T0, class T1, class U0, class U1 >
struct cmp< sake::pair< T0, T1 >, sake::pair< U0, U1 >, void >
    : sake::pair_adl::result_of::cmp<
          sake::pair< T0, T1 >,
          sake::pair< U0, U1 >
      >
{ };

} // namespace extension
} // namespace result_of

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_PAIR_OPERATORS_HPP
