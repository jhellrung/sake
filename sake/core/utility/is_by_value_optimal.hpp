/*******************************************************************************
 * sake/core/utility/is_by_value_optimal.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct is_by_value_optimal<T>
 * struct extension::is_by_value_optimal< T, Enable = void >
 * struct default_impl::is_by_value_optimal<T>
 *
 * This is a metafunction evaluating to true if passing T by value is optimal
 * relative to passing T by reference.
 * Types for which pass-by-value is optimal include:
 * - references
 * - scalar types (integral types, floating point types, enumeration, pointer,
 *   and pointer-to-member types)
 * - reference wrappers
 * - stateless function objects
 * - class types with size no larger than void*, a trivial copy constructor, and
 *   a trivial destructor
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_IS_BY_VALUE_OPTIMAL_HPP
#define SAKE_CORE_UTILITY_IS_BY_VALUE_OPTIMAL_HPP

#include <boost/mpl/not.hpp>
#include <boost/type_traits/has_trivial_copy.hpp>
#include <boost/type_traits/has_trivial_destructor.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_empty.hpp>
#include <boost/type_traits/is_pod.hpp>
#include <boost/type_traits/is_scalar.hpp>
#include <boost/type_traits/is_stateless.hpp>
#include <boost/type_traits/is_union.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/has_small_size.hpp>

#include <sake/core/ref/is_reference_wrapper.hpp>
#include <sake/core/utility/is_by_value_optimal_fwd.hpp>

namespace sake
{

/*******************************************************************************
 * struct is_by_value_optimal<T>
 ******************************************************************************/

template< class T >
struct is_by_value_optimal
    : extension::is_by_value_optimal<
          typename boost::remove_cv<T>::type
      >
{ };

template< class T >
struct is_by_value_optimal< T* >
    : boost::true_type
{ };

template< class T >
struct is_by_value_optimal< T& >
    : boost::true_type
{ };

/*******************************************************************************
 * struct extension::is_by_value_optimal< T, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T, class Enable /*= void*/ >
struct is_by_value_optimal
    : default_impl::is_by_value_optimal<T>
{ };

} // namespace extension

/*******************************************************************************
 * struct default_impl::is_by_value_optimal<T>
 ******************************************************************************/

namespace default_impl
{

template< class T >
struct is_by_value_optimal
    : boost_ext::mpl::or4<
          boost::is_scalar<T>,
          sake::is_reference_wrapper<T>,
          boost::is_stateless<T>,
          boost_ext::mpl::and2<
              boost_ext::mpl::or2<
                  boost_ext::has_small_size<T>,
                  boost_ext::mpl::and2<
                      boost::mpl::not_< boost::is_union<T> >,
                      boost::is_empty<T>
                  >
              >,
              boost_ext::mpl::or2<
                  boost::is_pod<T>,
                  boost_ext::mpl::and2<
                      boost::has_trivial_copy<T>,
                      boost::has_trivial_destructor<T>
                  >
              >
          >
      >
{ };

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_IS_BY_VALUE_OPTIMAL_HPP
