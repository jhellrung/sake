/*******************************************************************************
 * sake/core/math/rational/traits.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_RATIONAL_FWD_HPP
#define SAKE_CORE_MATH_RATIONAL_FWD_HPP

#include <boost/mpl/apply.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/math/rational/fwd.hpp>
#include <sake/core/utility/default_tag.hpp>

namespace sake
{

template< class T, class IntPred = sake::default_tag >
struct is_rational
    : boost::false_type
{ };

template< class T >
struct is_rational< sake::rational<T>, sake::default_tag >
    : boost::true_type
{ };

template< class T, class IntPred >
struct is_rational< sake::rational<T>, IntPred >
{
    static bool const value = boost::mpl::apply1< IntPred, T >::type::value;
    typedef is_rational type;
};

template< class T, class IntPred = sake::default_tag >
struct is_rational_sans_qualifiers
    : sake::is_rational<
          typename boost_ext::remove_qualifiers<T>::type,
          IntPred
      >
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_RATIONAL_FWD_HPP
