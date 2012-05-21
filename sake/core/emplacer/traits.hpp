/*******************************************************************************
 * sake/core/emplacer/traits.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct is_emplacer< Emplacer, ValuePred = default_tag >
 * struct is_emplacer_sans_qualifiers< Emplacer, ValuePred = default_tag >
 * struct is_emplacer_with_value< Emplacer, T >
 * struct is_emplacer_sans_qualifiers_with_value< Emplacer, T >
 ******************************************************************************/

#ifndef SAKE_CORE_EMPLACER_TRAITS_HPP
#define SAKE_CORE_EMPLACER_TRAITS_HPP

#include <boost/mpl/apply.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_void.hpp>

#include <sake/boost_ext/type_traits/is_void.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/emplacer/fwd.hpp>
#include <sake/core/utility/default_tag.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

/*******************************************************************************
 * struct is_emplacer< Emplacer, ValuePred = default_tag >
 ******************************************************************************/

template< class Emplacer, class ValuePred = sake::default_tag >
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
 * struct is_emplacer_sans_qualifiers< Emplacer, ValuePred = default_tag >
 ******************************************************************************/

template< class Emplacer, class ValuePred = sake::default_tag >
struct is_emplacer_sans_qualifiers
    : sake::is_emplacer<
          typename boost_ext::remove_qualifiers<Emplacer>::type,
          ValuePred
      >
{ };

/*******************************************************************************
 * struct is_emplacer_with_value< Emplacer, T >
 ******************************************************************************/

template< class Emplacer, class T >
struct is_emplacer_with_value
    : boost::false_type
{ };

template< class Signature, class T >
struct is_emplacer_with_value< sake::emplacer< Signature >, T >
{
private:
    SAKE_USING_TYPEDEF( typename sake::emplacer< Signature >, value_type );
public:
    static bool const value =
        boost::is_void< value_type >::value
     || boost::is_same< value_type, T >::value;
    typedef is_emplacer_with_value type;
};

template< class Signature, class T >
struct is_emplacer_with_value< sake::emplacer< Signature >, T const >
    : sake::is_emplacer_with_value< sake::emplacer< Signature >, T >
{ };

template< class Signature, class T >
struct is_emplacer_with_value< sake::emplacer< Signature >, T volatile >
    : sake::is_emplacer_with_value< sake::emplacer< Signature >, T >
{ };

template< class Signature, class T >
struct is_emplacer_with_value< sake::emplacer< Signature >, T const volatile >
    : sake::is_emplacer_with_value< sake::emplacer< Signature >, T >
{ };

/*******************************************************************************
 * struct is_emplacer_sans_qualifiers_with_value< Emplacer, T >
 ******************************************************************************/

template< class Emplacer, class T >
struct is_emplacer_sans_qualifiers_with_value
    : sake::is_emplacer_with_value<
          typename boost_ext::remove_qualifiers<Emplacer>::type, T >
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_EMPLACER_TRAITS_HPP
