/*******************************************************************************
 * sake/core/utility/emplacer/traits.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct is_emplacer< T, ValuePred = default_tag >
 * struct is_emplacer_sans_qualifiers< T, ValuePred = default_tag >
 *
 * struct enable_if_is_emplacer< T, Result = void, ValuePred = default_tag >
 * struct disable_if_is_emplacer< T, Result = void, ValuePred = default_tag >
 * struct lazy_enable_if_is_emplacer< T, Result, ValuePred = default_tag >
 * struct lazy_disable_if_is_emplacer< T, Result, ValuePred = default_tag >
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_EMPLACER_TRAITS_HPP
#define SAKE_CORE_UTILITY_EMPLACER_TRAITS_HPP

#include <boost/mpl/apply.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/is_void.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/utility/default_tag.hpp>
#include <sake/core/utility/emplacer/fwd.hpp>

namespace sake
{

/*******************************************************************************
 * struct is_emplacer< T, ValuePred = default_tag >
 ******************************************************************************/

template< class T, class ValuePred = sake::default_tag >
struct is_emplacer
    : boost::false_type
{ };

template< class Signature, class ValuePred >
struct is_emplacer< sake::emplacer< Signature >, ValuePred >
{
    static bool const value = boost::mpl::apply1<
        ValuePred,
        typename sake::emplacer< Signature >::value_type
    >::type::value;
    typedef is_emplacer type;
};

template< class Signature >
struct is_emplacer< sake::emplacer< Signature >, sake::default_tag >
    : boost::true_type
{ };

template< class Signature >
struct is_emplacer<
    sake::emplacer< Signature >,
    boost::mpl::quote1< boost::is_void >
>
    : boost::is_void< typename sake::emplacer< Signature >::value_type >
{ };

template< class Signature >
struct is_emplacer<
    sake::emplacer< Signature >,
    boost::mpl::quote1< boost_ext::is_void >
>
    : sake::is_emplacer<
          sake::emplacer< Signature >,
          boost::mpl::quote1< boost::is_void >
      >
{ };

template< class Signature >
struct is_emplacer<
    sake::emplacer< Signature >,
    boost::is_void< boost::mpl::_1 >
>
    : sake::is_emplacer<
          sake::emplacer< Signature >,
          boost::mpl::quote1< boost::is_void >
      >
{ };

template< class Signature >
struct is_emplacer<
    sake::emplacer< Signature >,
    boost::mpl::quote1< boost_ext::not_is_void >
>
{
    static bool const value = !boost::is_void<
        typename sake::emplacer< Signature >::value_type >::value;
    typedef is_emplacer type;
};

template< class Signature >
struct is_emplacer<
    sake::emplacer< Signature >,
    boost::mpl::not_< boost::is_void< boost::mpl::_1 > >
>
    : sake::is_emplacer<
          sake::emplacer< Signature >,
          boost::mpl::quote1< boost_ext::not_is_void >
      >
{ };

/*******************************************************************************
 * struct is_emplacer_sans_qualifiers< T, ValuePred = default_tag >
 ******************************************************************************/

template< class T, class ValuePred = sake::default_tag >
struct is_emplacer_sans_qualifiers
    : sake::is_emplacer<
          typename boost_ext::remove_qualifiers<T>::type,
          ValuePred
      >
{ };

/*******************************************************************************
 * struct enable_if_is_emplacer< T, Result = void, ValuePred = default_tag >
 * struct disable_if_is_emplacer< T, Result = void, ValuePred = default_tag >
 * struct lazy_enable_if_is_emplacer< T, Result, ValuePred = default_tag >
 * struct lazy_disable_if_is_emplacer< T, Result, ValuePred = default_tag >
 ******************************************************************************/

template< class T, class Result = void, class ValuePred = sake::default_tag >
struct enable_if_is_emplacer
    : boost::enable_if_c<
          sake::is_emplacer< T, ValuePred >::value,
          Result
      >
{ };

template< class T, class Result = void, class ValuePred = sake::default_tag >
struct disable_if_is_emplacer
    : boost::disable_if_c<
          sake::is_emplacer< T, ValuePred >::value,
          Result
      >
{ };

template< class T, class Result, class ValuePred = sake::default_tag >
struct lazy_enable_if_is_emplacer
    : boost::lazy_enable_if_c<
          sake::is_emplacer< T, ValuePred >::value,
          Result
      >
{ };

template< class T, class Result, class ValuePred = sake::default_tag >
struct lazy_disable_if_is_emplacer
    : boost::lazy_disable_if_c<
          sake::is_emplacer< T, ValuePred >::value,
          Result
      >
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_EMPLACER_TRAITS_HPP
