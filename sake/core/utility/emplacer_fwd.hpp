/*******************************************************************************
 * sake/core/utility/emplacer_fwd.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct is_emplacer< T, ValuePred = default_tag >
 * struct is_not_emplacer< T, ValuePred = default_tag >
 *
 * struct enable_if_is_emplacer< T, Result = void, ValuePred = default_tag >
 * struct disable_if_is_emplacer< T, Result = void, ValuePred = default_tag >
 * struct lazy_enable_if_is_emplacer< T, Result, ValuePred = default_tag >
 * struct lazy_disable_if_is_emplacer< T, Result, ValuePred = default_tag >
 *
 * emplacer_construct<T>(U&& y, void* p) -> void
 * emplacer_construct<T>(U&& y) -> T
 * emplacer_assign(T& x, U&& y) -> void
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_EMPLACER_FWD_HPP
#define SAKE_CORE_UTILITY_EMPLACER_FWD_HPP

#include <boost/mpl/apply.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_object.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_convertible_wnrbt.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/move/forward.hpp>
#include <sake/core/utility/default_tag.hpp>

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
 * struct is_not_emplacer< T, ValuePred = default_tag >
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
    boost::mpl::not_< boost::is_void< boost::mpl::_1 > >
>
    : boost::mpl::not_< boost::is_void< typename emplacer< Signature >::value > >
{ };

template< class T, class ValuePred = sake::default_tag >
struct is_not_emplacer
    : boost::mpl::not_< is_emplacer< T, ValuePred > >
{ };

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
 * emplacer_construct<T>(U&& y) -> T
 * emplacer_assign(T& x, U&& y) -> void
 ******************************************************************************/

namespace emplacer_private
{

template<
    class T, class U,
    class U_ = typename boost_ext::remove_qualifiers<U>::type,
    bool = is_emplacer< U_, boost::mpl::quote1< boost::is_void > >::value,
    bool = is_emplacer< U_, boost_ext::is_convertible_wnrbt< boost::mpl::_1, T > >::value
>
struct construct_assign_dispatch;

} // namespace emplacer_private

template< class T, class U >
inline void
emplacer_construct(SAKE_FWD2_REF( U ) y, void* const p)
{ emplacer_private::construct_assign_dispatch<T,U>::apply(sake::forward<U>(y), p); }

template< class T, class U >
inline T
emplacer_construct(SAKE_FWD2_REF( U ) y)
{ return emplacer_private::construct_assign_dispatch<T,U>::apply(sake::forward<U>(y)); }

template< class T, class U >
inline void
emplacer_assign(T& x, SAKE_FWD2_REF( U ) y)
{ emplacer_private::construct_assign_dispatch<T,U>::apply(x, sake::forward<U>(y)); }

namespace emplacer_private
{

template< class T, class U, class U_ >
struct construct_assign_dispatch< T, U, U_, false, false >
{
    static void apply(SAKE_FWD2_REF( U ) y, void* const p)
    {
        BOOST_STATIC_ASSERT((boost::is_object<T>::value));
        new(p) T(sake::forward<U>(y));
    }
    static T apply(SAKE_FWD2_REF( U ) y)
    {
        BOOST_STATIC_ASSERT((boost_ext::mpl::or2<
            boost::mpl::not_< boost_ext::is_reference<T> >,
            boost_ext::is_convertible_wnrbt< SAKE_FWD2_PARAM( U ), T >
        >::value));
        return static_cast<T>(sake::forward<U>(y));
    }
    static void apply(typename boost_ext::add_reference<T>::type x, SAKE_FWD2_REF( U ) y)
    { x = sake::forward<U>(y); }
};

template< class T, class U, class U_ >
struct construct_assign_dispatch< T, U, U_, true, false >
{
    static void apply(U_ e, void* const p)
    { e.template construct<T>(p); }
    static T apply(U_ e)
    { return e.template construct<T>(); }
    static void apply(typename boost_ext::add_reference<T>::type x, U_ e)
    { e.assign(x); }
};

template< class T, class U, class U_ >
struct construct_assign_dispatch< T, U, U_, false, true >
{
    static void apply(U_ e, void* const p)
    { e.construct(p); }
    static T apply(U_ e)
    { return e.construct(); }
    static void apply(typename boost_ext::add_reference<T>::type x, U_ e)
    { e.assign(x); }
};

} // namespace emplacer_private

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_EMPLACER_FWD_HPP
