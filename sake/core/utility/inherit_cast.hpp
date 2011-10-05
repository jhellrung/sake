/*******************************************************************************
 * sake/core/utility/inherit_cast.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * inherit_cast<T>(U* x) -> result_of::inherit_cast< U*, T >::type
 * inherit_cast<T>(U& x) -> result_of::inherit_cast< U&, T >::type
 * struct functional::inherit_cast< T = void >
 *
 * This can be used to safely and explicitly cast between base and derived
 * pointers and objects.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_INHERIT_CAST_HPP
#define SAKE_CORE_UTILITY_INHERIT_CAST_HPP

#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/propagate_cv.hpp>

#include <sake/core/utility/address_of.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/type_tag.hpp>

namespace sake
{

namespace result_of
{

template< class U, class T >
class inherit_cast;

template< class U, class T >
class inherit_cast< U*, T >
{
    typedef typename sake::remove_type_tag<T>::type T_;
    BOOST_STATIC_ASSERT((!boost_ext::is_reference< T_ >::value));
private:
    typedef typename boost_ext::propagate_cv< U, T_ >::type * type;
};

template< class U, class T >
class inherit_cast< U&, T >
{
    typedef typename sake::remove_type_tag<T>::type T_;
    BOOST_STATIC_ASSERT((!boost_ext::is_reference< T_ >::value));
private:
    typedef typename boost_ext::propagate_cv< U, T_ >::type & type;
};

} // namespace result_of

template< class T, class U >
inline typename result_of::inherit_cast< U*, T >::type
inherit_cast(U* const p)
{
    typedef typename boost_ext::propagate_const<U,T>::type T_;
    return static_cast< T_* >(p);
}

template< class T, class U >
inline typename result_of::inherit_cast< U&, T >::type
inherit_cast(U& x)
{ return *sake::inherit_cast<T>(sake::address_of(x)); }

template< class U, class T >
inline typename result_of::inherit_cast< U*, T >::type
inherit_cast(U* const p, sake::type_tag<T>)
{ return sake::inherit_cast<T>(p); }

template< class U, class T >
inline typename result_of::inherit_cast< U&, T >::type
inherit_cast(U& x, sake::type_tag<T>)
{ return sake::inherit_cast<T>(x); }

namespace functional
{

template< class T = void >
struct inherit_cast
{
    BOOST_STATIC_ASSERT((!boost_ext::is_reference<T>::value));

    template< class > struct result;
    template< class This, class U >
    struct result< This ( U ) >
        : result_of::inherit_cast<U,T>
    { };

    template< class U >
    typename result_of::inherit_cast< U*, T >::type
    operator()(U* const p) const
    { return sake::inherit_cast<T>(p); }

    template< class U >
    typename result_of::inherit_cast< U&, T >::type
    operator()(U& x) const
    { return sake::inherit_cast<T>(x); }
};

template<>
struct inherit_cast< void >
{
    SAKE_RESULT_FROM_METAFUNCTION( 2, result_of::inherit_cast )

    template< class U, class T >
    typename result_of::inherit_cast< U&, T >::type
    operator()(U* const p, sake::type_tag<T>) const
    { return sake::inherit_cast<T>(p); }

    template< class U, class T >
    typename result_of::inherit_cast< U&, T >::type
    operator()(U& x, sake::type_tag<T>) const
    { return sake::inherit_cast<T>(x); }
};

} // namespace functional

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_INHERIT_CAST_HPP
