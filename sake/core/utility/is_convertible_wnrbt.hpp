/*******************************************************************************
 * sake/core/utility/is_convertible_wnrbt.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct is_convertible_wnrbt< From, To >
 * struct extension::is_convertible_wnrbt< From, To, Enable = void >
 * struct default_impl::is_convertible_wnrbt< From, To >
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

#ifndef SAKE_CORE_UTILITY_IS_CONVERTIBLE_WNRBT_HPP
#define SAKE_CORE_UTILITY_IS_CONVERTIBLE_WNRBT_HPP

#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/introspection/has_mem_fun_operator.hpp>
#include <sake/core/utility/is_convertible_wnrbt_fwd.hpp>

namespace sake
{

/*******************************************************************************
 * struct is_convertible_wnrbt< From, To >
 ******************************************************************************/

template< class From, class To >
struct is_convertible_wnrbt
    : boost_ext::is_convertible< From, To >
{ };

template< class From, class To >
struct is_convertible_wnrbt< From, To& >
    : boost_ext::mpl::and2<
          boost_ext::is_convertible< From, To& >,
          sake::extension::is_convertible_wnrbt<
              typename boost::remove_cv< From >::type,
              To&
          >
      >
{ };

/*******************************************************************************
 * struct extension::is_convertible_wnrbt< From, To, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class From, class To, class Enable /*= void*/ >
struct is_convertible_wnrbt
    : sake::default_impl::is_convertible_wnrbt< From, To >
{ };

} // namespace extension

/*******************************************************************************
 * struct default_impl::is_convertible_wnrbt< From, To >
 ******************************************************************************/

namespace default_impl
{

template< class From, class To >
struct is_convertible_wnrbt
    : boost::false_type
{ };

namespace is_convertible_wnrbt_private
{

template< class From, class To >
struct helper1
    : sake::has_mem_fun_operator< From, To& >
{ };

template< class From, class To >
struct helper1< From, To const >
    : boost_ext::mpl::or2<
          sake::has_mem_fun_operator< From, To& >,
          sake::has_mem_fun_operator< From, To const & >
      >
{ };

template< class From, class To >
struct helper1< From, To volatile >
    : boost_ext::mpl::or2<
          sake::has_mem_fun_operator< From, To& >,
          sake::has_mem_fun_operator< From, To volatile & >
      >
{ };

template< class From, class To >
struct helper1< From, To const volatile >
    : boost_ext::mpl::or4<
          sake::has_mem_fun_operator< From, To& >,
          sake::has_mem_fun_operator< From, To const & >,
          sake::has_mem_fun_operator< From, To volatile & >,
          sake::has_mem_fun_operator< From, To const volatile & >
      >
{ };

template< class From, class To >
struct helper0
    : boost_ext::mpl::or4<
          helper1< From, To >,
          helper1< From const, To >,
          helper1< From volatile, To >,
          helper1< From const volatile, To >
      >
{ };

template< class From, class To >
struct helper0< From const, To >
    : boost_ext::mpl::or2<
          helper1< From const, To >,
          helper1< From const volatile, To >
      >
{ };

template< class From, class To >
struct helper0< From volatile, To >
    : boost_ext::mpl::or2<
          helper1< From volatile, To >,
          helper1< From const volatile, To >
      >
{ };

template< class From, class To >
struct helper0< From const volatile, To >
    : helper1< From const volatile, To >
{ };

} // namespace is_convertible_wnrbt_private

template< class From, class To >
struct is_convertible_wnrbt< From&, To& >
    : boost_ext::mpl::or3<
          boost_ext::is_convertible< From*, To* >,
          is_convertible_wnrbt_private::helper0< From, To >,
          sake::is_convertible_wnrbt<
              typename boost::remove_cv< From >::type,
              To&
          >
      >
{ };

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_IS_CONVERTIBLE_WNRBT_HPP
