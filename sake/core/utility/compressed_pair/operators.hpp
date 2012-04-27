/*******************************************************************************
 * sake/core/utility/compressed_pair/operators.hpp
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
 * operator==(compressed_pair< T0, T1 > const & x, compressed_pair< U0, U1 > const & y) -> ...
 * operator!=(compressed_pair< T0, T1 > const & x, compressed_pair< U0, U1 > const & y) -> ...
 * operator<(compressed_pair< T0, T1 > const & x, compressed_pair< U0, U1 > const & y) -> ...
 * operator>(compressed_pair< T0, T1 > const & x, compressed_pair< U0, U1 > const & y) -> ...
 * operator<=(compressed_pair< T0, T1 > const & x, compressed_pair< U0, U1 > const & y) -> ...
 * operator>=(compressed_pair< T0, T1 > const & x, compressed_pair< U0, U1 > const & y) -> ...
 * cmp(compressed_pair< T0, T1 > const & x, compressed_pair< U0, U1 > const & y) -> ...
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_COMPRESSED_PAIR_OPERATORS_HPP
#define SAKE_CORE_UTILITY_COMPRESSED_PAIR_OPERATORS_HPP

#include <boost/preprocessor/cat.hpp>

#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/common_type.hpp>

#include <sake/core/data_structures/pair/operators.hpp>
#include <sake/core/math/cmp_fwd.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/utility/compressed_pair/fwd.hpp>

namespace sake
{

namespace compressed_pair_adl
{

namespace operators
{

namespace result_of
{

#define define_operator( name ) \
template< class T, class U > struct name; \
template< class T0, class T1, class U0, class U1 > \
struct name< sake::compressed_pair< T0, T1 >, \
             sake::compressed_pair< U0, U1 > > \
    : sake::pair_adl::private_::result_of_helper< \
          sake::pair_adl::operators::result_of::private_:: \
              BOOST_PP_CAT( name, _impl ), \
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
inline typename sake::compressed_pair_adl::operators::result_of::name< \
    sake::compressed_pair<T0,T1>, \
    sake::compressed_pair<U0,U1> \
>::type \
operator op (sake::compressed_pair<T0,T1> const & x, \
             sake::compressed_pair<U0,U1> const & y) \
{ return expression; }

define_operator( ==, equal,
    x.first() == y.first() && x.second() == y.second() )
define_operator( !=, not_equal,
    x.first() != y.first() || x.second() != y.second() )
define_operator( < , less,
    x.first() < y.first() || (!(y.first() < x.first()) && x.second() < y.second()) )
define_operator( > , greater      ,
    x.first() > y.first() || (!(y.first() > x.first()) && x.second() > y.second()) )
define_operator( <=, less_equal,
    x.first() <= y.first() && (!(y.first() <= x.first()) || x.second() <= y.second()) )
define_operator( >=, greater_equal,
    x.first() >= y.first() && (!(y.first() >= x.first()) || x.second() >= y.second()) )

#undef define_operator

namespace result_of
{

template< class T, class U > struct cmp;
template< class T0, class T1, class U0, class U1 >
struct cmp< sake::compressed_pair<T0,T1>, sake::compressed_pair<U0,U1> >
    : sake::pair_adl::private_::result_of_helper<
          sake::pair_adl::result_of::private_::cmp_impl,
          T0, T1, U0, U1
      >
{ };

} // namespace result_of

template< class T0, class T1, class U0, class U1 >
inline typename sake::compressed_pair_adl::result_of::cmp<
    sake::compressed_pair< T0, T1 >,
    sake::compressed_pair< U0, U1 >
>::type
cmp(sake::compressed_pair< T0, T1 > const & x,
    sake::compressed_pair< U0, U1 > const & y)
{
    typename sake::result_of::cmp<
        typename boost_ext::add_reference_add_const< T0 >::type,
        typename boost_ext::add_reference_add_const< U0 >::type
    >::type const s0 = sake::cmp(x.first(), y.first());
    return s0 == sake::zero ? sake::cmp(x.second(), y.second()) : s0;
}

} // namespace compressed_pair_adl

namespace operators {
namespace result_of {
namespace extension {

#define define_operator( name ) \
template< class T0, class T1, class U0, class U1 > \
struct name< sake::compressed_pair< T0, T1 >, \
             sake::compressed_pair< U0, U1 >, void > \
    : sake::compressed_pair_adl::operators::result_of::name< \
          sake::compressed_pair< T0, T1 >, \
          sake::compressed_pair< U0, U1 > \
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
struct cmp< sake::compressed_pair< T0, T1 >,
            sake::compressed_pair< U0, U1 >, void >
    : sake::compressed_pair_adl::result_of::cmp<
          sake::compressed_pair< T0, T1 >,
          sake::compressed_pair< U0, U1 >
      >
{ };

} // namespace extension
} // namespace result_of

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_COMPRESSED_PAIR_OPERATORS_HPP
