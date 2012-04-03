/*******************************************************************************
 * sake/core/utility/emplacer_fwd.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct is_emplacer< T, ValuePred = default_tag >
 *
 * struct enable_if_is_emplacer< T, Result = void, ValuePred = default_tag >
 * struct disable_if_is_emplacer< T, Result = void, ValuePred = default_tag >
 * struct lazy_enable_if_is_emplacer< T, Result, ValuePred = default_tag >
 * struct lazy_disable_if_is_emplacer< T, Result, ValuePred = default_tag >
 *
 * emplacer_construct<T>(U&& y, void* p) -> void
 * emplacer_construct<T>(U&& y) -> T [qualified]
 * emplacer_assign(T& x, U&& y) -> T&
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_EMPLACER_FWD_HPP
#define SAKE_CORE_UTILITY_EMPLACER_FWD_HPP

#include <boost/mpl/apply.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_object.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/type_traits/is_volatile.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/is_reference.hpp>
#include <sake/boost_ext/type_traits/is_same_sans_qualifiers.hpp>

#include <sake/core/move/forward.hpp>
#include <sake/core/utility/default_tag.hpp>
#include <sake/core/utility/is_convertible_wnrbt.hpp>

#ifndef SAKE_EMPLACER_PERFECT_MAX_ARITY
#define SAKE_EMPLACER_PERFECT_MAX_ARITY 4
#endif // #ifndef SAKE_EMPLACER_PERFECT_MAX_ARITY

#ifndef SAKE_EMPLACER_REF_MAX_ARITY
#define SAKE_EMPLACER_REF_MAX_ARITY 6
#endif // #ifndef SAKE_EMPLACER_REF_MAX_ARITY

#ifndef SAKE_EMPLACER_CREF_MAX_ARITY
#define SAKE_EMPLACER_CREF_MAX_ARITY 8
#endif // #ifndef SAKE_EMPLACER_CREF_MAX_ARITY

#define SAKE_EMPLACER_MAX_ARITY SAKE_EMPLACER_PERFECT_MAX_ARITY
#if SAKE_EMPLACER_MAX_ARITY < SAKE_EMPLACER_REF_MAX_ARITY
#undef SAKE_EMPLACER_MAX_ARITY
#define SAKE_EMPLACER_MAX_ARITY SAKE_EMPLACER_REF_MAX_ARITY
#endif // #if SAKE_EMPLACER_MAX_ARITY < SAKE_EMPLACER_REF_MAX_ARITY
#if SAKE_EMPLACER_MAX_ARITY < SAKE_EMPLACER_CREF_MAX_ARITY
#undef SAKE_EMPLACER_MAX_ARITY
#define SAKE_EMPLACER_MAX_ARITY SAKE_EMPLACER_CREF_MAX_ARITY
#endif // #if SAKE_EMPLACER_MAX_ARITY < SAKE_EMPLACER_CREF_MAX_ARITY

namespace sake
{

template< class Signature >
class emplacer;

/*******************************************************************************
 * struct is_emplacer< T, ValuePred = default_tag >
 ******************************************************************************/

template< class T, class ValuePred = sake::default_tag >
struct is_emplacer
    : boost::false_type
{ };

template< class Signature, class ValuePred >
struct is_emplacer< emplacer< Signature >, ValuePred >
{
    static bool const value = boost::mpl::apply1<
        ValuePred,
        typename emplacer< Signature >::value_type
    >::type::value;
    typedef is_emplacer type;
};

template< class Signature >
struct is_emplacer< emplacer< Signature >, sake::default_tag >
    : boost::true_type
{ };

template< class Signature >
struct is_emplacer<
    emplacer< Signature >,
    boost::mpl::quote1< boost::is_void >
>
    : boost::is_void< typename emplacer< Signature >::value_type >
{ };

template< class Signature >
struct is_emplacer<
    emplacer< Signature >,
    boost::is_void< boost::mpl::_1 >
>
    : boost::is_void< typename emplacer< Signature >::value_type >
{ };

template< class Signature >
struct is_emplacer<
    emplacer< Signature >,
    boost::mpl::not_< boost::is_void< boost::mpl::_1 > >
>
{
    static bool const value = boost::is_void< typename emplacer< Signature >::value_type >::value;
    typedef is_emplacer type;
};

/*******************************************************************************
 * struct enable_if_is_emplacer< T, Result = void, ValuePred = default_tag >
 * struct disable_if_is_emplacer< T, Result = void, ValuePred = default_tag >
 * struct lazy_enable_if_is_emplacer< T, Result, ValuePred = default_tag >
 * struct lazy_disable_if_is_emplacer< T, Result, ValuePred = default_tag >
 ******************************************************************************/

template< class T, class Result = void, class ValuePred = sake::default_tag >
struct enable_if_is_emplacer
    : boost::enable_if_c< is_emplacer< T, ValuePred >::value, Result >
{ };

template< class T, class Result = void, class ValuePred = sake::default_tag >
struct disable_if_is_emplacer
    : boost::disable_if_c< is_emplacer< T, ValuePred >::value, Result >
{ };

template< class T, class Result, class ValuePred = sake::default_tag >
struct lazy_enable_if_is_emplacer
    : boost::lazy_enable_if_c< is_emplacer< T, ValuePred >::value, Result >
{ };

template< class T, class Result, class ValuePred = sake::default_tag >
struct lazy_disable_if_is_emplacer
    : boost::lazy_disable_if_c< is_emplacer< T, ValuePred >::value, Result >
{ };

/*******************************************************************************
 * emplacer_construct<T>(U&& y, void* p) -> void
 * emplacer_construct<T>(U&& y) -> T [qualified]
 * emplacer_assign(T& x, U&& y) -> T&
 ******************************************************************************/

template< class T, class U >
inline void
emplacer_construct(SAKE_FWD2_REF( U ) y, void* const p)
{
    BOOST_STATIC_ASSERT((boost::is_object<T>::value));
    BOOST_STATIC_ASSERT(!(boost::is_const<T>::value));
    BOOST_STATIC_ASSERT(!(boost::is_volatile<T>::value));
    new(p) T(sake::forward<U>(y));
}

template< class T, class U >
inline typename boost::enable_if_c<
    boost_ext::is_same_sans_qualifiers<T,U>::value,
    SAKE_FWD2_REF( U )
>::type
emplacer_construct(SAKE_FWD2_REF( U ) y)
{
    BOOST_STATIC_ASSERT(!(boost::is_const<T>::value));
    BOOST_STATIC_ASSERT(!(boost::is_volatile<T>::value));
    BOOST_STATIC_ASSERT((
       !boost_ext::is_reference<T>::value
     || boost_ext::is_reference< SAKE_FWD2_PARAM( U ) >::value
    ));
    return sake::forward<U>(y);
}

template< class T, class U >
inline typename boost::disable_if_c<
    boost_ext::is_same_sans_qualifiers<T,U>::value,
    T
>::type
emplacer_construct(SAKE_FWD2_REF( U ) y)
{
    BOOST_STATIC_ASSERT(!(boost::is_const<T>::value));
    BOOST_STATIC_ASSERT(!(boost::is_volatile<T>::value));
    BOOST_STATIC_ASSERT((
       !boost_ext::is_reference<T>::value
     || sake::is_convertible_wnrbt< SAKE_FWD2_PARAM( U ), T >::value
    ));
    return static_cast<T>(sake::forward<U>(y));
}

template< class T, class U >
inline T&
emplacer_assign(T& x, SAKE_FWD2_REF( U ) y)
{ return x = sake::forward<U>(y); }

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_EMPLACER_FWD_HPP
