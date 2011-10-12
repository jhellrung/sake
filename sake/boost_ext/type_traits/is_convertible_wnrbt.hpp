/*******************************************************************************
 * sake/boost_ext/type_traits/is_convertible_wnrbt.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::is_convertible_wnrbt< From, To >
 * struct boost_ext::extension::is_convertible_wnrbt< From, To, Enable = void >
 * struct boost_ext::default_impl::is_convertible_wnrbt< From, To >
 *
 * WNRBT = With No Reference-Bound Temporary
 *
 * This is similar to is_convertible, except that when To is a reference type,
 * conversions which end up binding the To reference to a temporary are
 * excluded.  Since, in general, it's impossible to know if a conversion
 * involves the binding of a temporary to a reference, some user-supplied help
 * may be necessary in some cases.
 *
 * This is necessary to SFINAE-out constructors that would otherwise initialize
 * reference members with temporaries (and thus create dangling references), and
 * may be useful in determining return types to avoid returning references to
 * local variables.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_IS_CONVERTIBLE_WNRBT_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_IS_CONVERTIBLE_WNRBT_HPP

#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/is_convertible_wnrbt_fwd.hpp>

namespace sake
{

namespace boost_ext
{

/*******************************************************************************
 * struct boost_ext::is_convertible_wnrbt< From, To >
 ******************************************************************************/

template< class From, class To >
struct is_convertible_wnrbt
    : boost_ext::is_convertible< From, To >
{ };

template< class From, class To >
struct is_convertible_wnrbt< From, To& >
    : boost_ext::mpl::and2<
          boost_ext::is_convertible< From, To& >,
          boost_ext::extension::is_convertible_wnrbt<
              typename boost::remove_cv< From >::type,
              To&
          >
      >
{ };

/*******************************************************************************
 * struct boost_ext::extension::is_convertible_wnrbt< From, To, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class From, class To, class Enable /*= void*/ >
struct is_convertible_wnrbt
    : boost_ext::default_impl::is_convertible_wnrbt< From, To >
{ };

} // namespace extension

/*******************************************************************************
 * struct boost_ext::default_impl::is_convertible_wnrbt< From, To >
 ******************************************************************************/

namespace default_impl
{

template< class From, class To >
struct is_convertible_wnrbt
    : boost::false_type
{ };

template< class From, class To >
struct is_convertible_wnrbt< From&, To& >
    : boost_ext::mpl::or2<
          boost_ext::is_convertible< From*, To* >,
          boost_ext::is_convertible_wnrbt<
              typename boost::remove_cv< From >::type,
              To&
          >
      >
{ };

} // namespace default_impl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_IS_CONVERTIBLE_WNRBT_HPP
