/*******************************************************************************
 * sake/core/data_structures/tuple/operators.hpp
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
 * operator==(tuple< T0, ... > const & x, tuple< U0, ... > const & y) -> ...
 * operator!=(tuple< T0, ... > const & x, tuple< U0, ... > const & y) -> ...
 * operator<(tuple< T0, ... > const & x, tuple< U0, ... > const & y) -> ...
 * operator>(tuple< T0, ... > const & x, tuple< U0, ... > const & y) -> ...
 * operator<=(tuple< T0, ... > const & x, tuple< U0, ... > const & y) -> ...
 * operator>=(tuple< T0, ... > const & x, tuple< U0, ... > const & y) -> ...
 * cmp(tuple< T0, ... > const & x, tuple< U0, ... > const & y) -> ...
 ******************************************************************************/

#ifndef SAKE_CORE_DATA_STRUCTURES_TUPLE_OPERATORS_HPP
#define SAKE_CORE_DATA_STRUCTURES_TUPLE_OPERATORS_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>

#include <sake/boost_ext/fusion/algorithm/query/cmp.hpp>
#include <sake/boost_ext/fusion/algorithm/query/equal.hpp>
#include <sake/boost_ext/fusion/algorithm/query/not_equal.hpp>

#include <sake/core/data_structures/tuple/fwd.hpp>
#include <sake/core/functional/operators/relational.hpp>
#include <sake/core/math/cmp.hpp>
#include <sake/core/math/zero.hpp>

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

namespace sake
{

namespace tuple_adl
{

namespace operators
{

namespace result_of
{

#define define_operator( name ) \
template< class T, class U > struct name; \
template< class_T0N, class_U0N > \
struct name< sake::tuple< T0N >, sake::tuple< U0N > > \
    : boost_ext::fusion::result_of::name< \
          sake::tuple< T0N >, \
          sake::tuple< U0N > \
      > \
{ };

define_operator( equal )
define_operator( not_equal )

#undef define_operator

#define define_operator( name ) \
template< class T, class U > struct name; \
template< class_T0N, class_U0N > \
struct name< sake::tuple< T0N >, sake::tuple< U0N > > \
    : sake::operators::result_of::name< \
          typename sake::result_of::cmp< \
              sake::tuple< T0N >, \
              sake::tuple< U0N > \
          >::type, \
          sake::zero_t \
      > \
{ };

define_operator( less )
define_operator( greater )
define_operator( less_equal )
define_operator( greater_equal )

#undef define_operator

} // namespace result_of

} // namespace operators

#define define_operator( op, name ) \
template< class_T0N, class_U0N > \
inline typename sake::tuple_adl::operators::result_of::name< \
    sake::tuple< T0N >, \
    sake::tuple< U0N > \
>::type \
operator op (sake::tuple< T0N > const & x, sake::tuple< U0N > const & y) \
{ return boost_ext::fusion::name(x, y); }

define_operator( ==, equal )
define_operator( !=, not_equal )

#undef define_operator

#define define_operator( op, name ) \
template< class_T0N, class_U0N > \
inline typename sake::tuple_adl::operators::result_of::less< \
    sake::tuple< T0N >, \
    sake::tuple< U0N > \
>::type \
operator op (sake::tuple< T0N > const & x, sake::tuple< U0N > const & y) \
{ return sake::cmp(x,y) op sake::zero; }

define_operator( <, less )
define_operator( >, greater )
define_operator( <=, less_equal )
define_operator( >=, greater_equal )

#undef define_operator

namespace result_of
{

template< class T, class U > struct cmp;
template< class_T0N, class_U0N >
struct cmp< sake::tuple< T0N >, sake::tuple< U0N > >
    : boost_ext::fusion::result_of::cmp<
          sake::tuple< T0N >,
          sake::tuple< U0N >
      >
{ };

} // namespace result_of

template< class_T0N, class_U0N >
inline typename sake::tuple_adl::result_of::cmp<
    sake::tuple< T0N >,
    sake::tuple< U0N >
>::type
cmp(sake::tuple< T0N > const & x, sake::tuple< U0N > const & y)
{ return boost_ext::fusion::cmp(x, y); }

} // namespace tuple_adl

namespace operators {
namespace result_of {
namespace extension {

#define define_operator( name ) \
template< class_T0N, class_U0N > \
struct name< sake::tuple< T0N >, sake::tuple< U0N >, void > \
    : sake::tuple_adl::operators::result_of::name< \
          sake::tuple< T0N >, \
          sake::tuple< U0N > \
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

template< class_T0N, class_U0N >
struct cmp< sake::tuple< T0N >, sake::tuple< U0N >, void >
    : sake::tuple_adl::result_of::cmp<
          sake::tuple< T0N >,
          sake::tuple< U0N >
      >
{ };

} // namespace extension
} // namespace result_of

} // namespace sake

#undef class_T0N
#undef class_U0N
#undef T0N
#undef U0N

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_TUPLE_OPERATORS_HPP
