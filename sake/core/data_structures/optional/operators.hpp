/*******************************************************************************
 * sake/core/data_structures/optional/operators.hpp
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
 * operator==(optional<T> const & x, optional<U> const & y) -> ...
 * operator!=(optional<T> const & x, optional<U> const & y) -> ...
 * operator<(optional<T> const & x, optional<U> const & y) -> ...
 * operator>(optional<T> const & x, optional<U> const & y) -> ...
 * operator<=(optional<T> const & x, optional<U> const & y) -> ...
 * operator>=(optional<T> const & x, optional<U> const & y) -> ...
 * cmp(optional<T> const & x, optional<U> const & y) -> ...
 ******************************************************************************/

#ifndef SAKE_CORE_DATA_STRUCTURES_OPTIONAL_OPERATORS_HPP
#define SAKE_CORE_DATA_STRUCTURES_OPTIONAL_OPERATORS_HPP

#include <sake/boost_ext/type_traits/common_type.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/add_pointer_remove_reference_add_const.hpp>

#include <sake/core/data_structures/optional/fwd.hpp>
#include <sake/core/functional/operators/relational.hpp>
#include <sake/core/math/cmp_fwd.hpp>
#include <sake/core/math/sign_t.hpp>

namespace sake
{

namespace optional_adl
{

namespace operators
{

namespace result_of
{

namespace private_
{

template< class T, class U, template< class T_, class U_ > class ResultOfOp >
struct impl
    : boost_ext::common_type<
          typename ResultOfOp<
              typename boost_ext::add_reference_add_const<
                  typename T::value_type >::type,
              typename boost_ext::add_reference_add_const<
                  typename U::value_type >::type
          >::type,
          bool
      >
{ };

} // namespace private_

#define define_operator( name ) \
template< class T, class U > \
struct name : private_::impl< T, U, sake::operators::result_of::name > { };

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
template< class T, class U > \
inline typename sake::optional_adl::operators::result_of::name< \
    sake::optional<T>, \
    sake::optional<U> \
>::type \
operator op (sake::optional<T> const & x, sake::optional<U> const & y) \
{ \
    typename boost_ext::add_pointer_remove_reference_add_const<T>::type const p = x.get_ptr(); \
    typename boost_ext::add_pointer_remove_reference_add_const<U>::type const q = y.get_ptr(); \
    return expression; \
}

define_operator( ==,     equal, p == q ? true : p && q ? *p == *q : false )
define_operator( !=, not_equal, p != q ? p && q ? *p != *q : true : false )
define_operator( < ,    less      , q ? p ? *p < *q : true : false )
define_operator( > , greater      , p ? q ? *p > *q : true : false )
define_operator( <=,    less_equal, p ? q ? *p <= *q : false : true )
define_operator( >=, greater_equal, q ? p ? *p >= *q : false : true )

#undef define_operator

namespace result_of
{

template< class T, class U >
struct cmp
    : boost_ext::common_type<
          typename sake::result_of::cmp<
              typename boost_ext::add_reference_add_const<
                  typename T::value_type >::type,
              typename boost_ext::add_reference_add_const<
                  typename U::value_type >::type
          >::type,
          sake::sign_t
      >
{ };

} // namespace result_of

template< class T, class U >
inline typename sake::optional_adl::result_of::cmp<
    sake::optional<T>,
    sake::optional<U>
>::type
cmp(sake::optional<T> const & x, sake::optional<U> const & y)
{
    typename boost_ext::add_pointer_remove_reference_add_const<T>::type const p = x.get_ptr();
    typename boost_ext::add_pointer_remove_reference_add_const<U>::type const q = y.get_ptr();
    return p ? q ? sake::cmp(*p,*q)
                 : sake::sign_t(sake::sign_t::positive_tag()) :
               q ? sake::sign_t(sake::sign_t::negative_tag())
                 : sake::sign_t(sake::sign_t::zero_tag());
}

} // namespace optional_adl

namespace operators {
namespace result_of {
namespace extension {

#define define_operator( name ) \
template< class T, class U > \
struct name< sake::optional<T>, sake::optional<U>, void > \
    : sake::optional_adl::operators::result_of::name< \
          sake::optional<T>, \
          sake::optional<U> \
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

template< class T, class U >
struct cmp< sake::optional<T>, sake::optional<U>, void >
    : sake::optional_adl::result_of::cmp<
          sake::optional<T>,
          sake::optional<U>
      >
{ };

} // namespace extension
} // namespace result_of

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_OPTIONAL_OPERATORS_HPP
