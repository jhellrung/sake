/*******************************************************************************
 * sake/boost_ext/type_traits/common_arithmetic_type.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::common_arithmetic_type< T0, T1 >
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_COMMON_ARITHMETIC_TYPE_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_COMMON_ARITHMETIC_TYPE_HPP

#include <boost/config.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_void.hpp>

#include <sake/boost_ext/type_traits/is_arithmetic_or_enum.hpp>
#include <sake/boost_ext/type_traits/make_signed.hpp>
#include <sake/boost_ext/type_traits/make_unsigned.hpp>

#include <sake/core/expr_traits/typeof.hpp>
#include <sake/core/utility/declval.hpp>

namespace sake
{

namespace boost_ext
{

template< class T0, class T1 >
class common_arithmetic_type
{
  BOOST_STATIC_ASSERT((boost_ext::is_arithmetic_or_enum< T0 >::value));
  BOOST_STATIC_ASSERT((boost_ext::is_arithmetic_or_enum< T1 >::value));

#ifndef BOOST_NO_DECLTYPE

public:
  typedef decltype( sake::declval< bool >() ?
    sake::declval< T0 >() : sake::declval< T1 >() ) type;

#else // #ifndef BOOST_NO_DECLTYPE

  typedef boost::mpl::vector8<
    int, unsigned int, T0, T1,
    typename boost_ext::make_signed< T0 >::type,
    typename boost_ext::make_signed< T1 >::type,
    typename boost_ext::make_unsigned< T0 >::type,
    typename boost_ext::make_unsigned< T1 >::type
  > candidate_types;
public:
  SAKE_EXPR_TYPEOF_TYPEDEF(
    typename sake::declval< bool >() ?
      sake::declval< T0 >() : sake::declval< T1 >(),
    candidate_types,
    type
  );
  BOOST_STATIC_ASSERT((!boost::is_void< type >::value));

#endif // #ifndef BOOST_NO_DECLTYPE

};

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_COMMON_ARITHMETIC_TYPE_HPP
