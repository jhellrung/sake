/*******************************************************************************
 * sake/core/functional/operators/star.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * operators::star(T && x)
 *   -> operators::result_of::star<T>::type
 * struct operators::functional::star
 *
 * struct operators::result_of::star<T>
 * struct operators::result_of::extension::star< T, Enable = void >
 * struct operators::result_of::default_impl::star<T>
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_STAR_HPP
#define SAKE_CORE_FUNCTIONAL_OPERATORS_STAR_HPP

#define SAKE_OPERATORS_NAME star
#define SAKE_OPERATORS_OP   *
#define SAKE_OPERATORS_NO_DEFINE_RESULT_OF_DEFAULT_IMPL_IMPL
#include <sake/core/functional/operators/private/unary.ipp>

#include <boost/mpl/if.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_function.hpp>
#include <boost/type_traits/is_object.hpp>

#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/config.hpp>
#include <sake/core/iterator/is_iterator.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/pointee.hpp>

namespace sake {
namespace operators {
namespace result_of {
namespace default_impl {
namespace star_private {

template< class T >
struct dispatch_index
{
  static int const value = boost::mpl::if_c<
    sake::is_iterator<T>::value,
    sake::int_tag<3>, sake::int_tag<1>
  >::type::value;
};

template< class T >
struct dispatch_index< T * >
{
  static int const value = boost::mpl::if_c<
    boost::is_function<T>::value,
    sake::int_tag<0>, sake::int_tag<2>
  >::type::value;
};

template< class T, int = dispatch_index<T>::value >
struct dispatch;

template< class T >
struct dispatch<T,3>
  : sake::iterator_reference<T>
{ };

template< class T >
struct dispatch< T *, 2 >
{
  BOOST_STATIC_ASSERT((boost::is_object<T>::value));
  typedef T & type;
};

template< class T >
struct dispatch<T,1>
{
private:
  typedef typename sake::pointee<T>::type pointee_;
  BOOST_STATIC_ASSERT((boost::is_object< pointee_ >::value));
public:
  typedef pointee_ & type;
};

template< class T >
struct dispatch< T *, 0 >
{
#if SAKE_MSC_VERSION && SAKE_MSC_VERSION <= 1500
  typedef T * type;
#else // #if SAKE_MSC_VERSION && SAKE_MSC_VERSION <= 1500
  typedef T & type;
#endif // #if SAKE_MSC_VERSION && SAKE_MSC_VERSION <= 1500
};

template< class T >
struct impl
  : dispatch< typename boost_ext::remove_qualifiers<T>::type >
{ };

} // namespace star_private
} // namespace default_impl
} // namespace result_of
} // namespace operators
} // namespace sake

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_STAR_HPP
