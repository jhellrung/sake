/*******************************************************************************
 * sake/core/utility/cast/inherit.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * inherit_cast<T>(U& x) -> result_of::inherit_cast< U&, T >::type
 * inherit_cast<T>(U* p) -> result_of::inherit_cast< U*, T >::type
 * inherit_cast(U& x, type_tag<T>) -> result_of::inherit_cast< U&, T >::type
 * inherit_cast(U* p, type_tag<T>) -> result_of::inherit_cast< U*, T >::type
 * struct functional::inherit_cast<T>
 * struct functional::inherit_cast<>
 *
 * This can be used to explicitly cast between base and derived pointers and
 * objects and get cv-qualifications correctly propagated.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_CAST_INHERIT_HPP
#define SAKE_CORE_UTILITY_CAST_INHERIT_HPP

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_cv_or.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>
#include <sake/boost_ext/type_traits/propagate_cv.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/utility/address_of.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/type_tag.hpp>

namespace sake
{

namespace result_of
{

template< class U, class T >
struct inherit_cast;

} // namespace result_of

namespace inherit_cast_private
{

template< class T, class U > struct enable;

} // namespace inherit_cast_private

template< class T, class U >
inline typename inherit_cast_private::enable< T, U& >::type
inherit_cast(U& x)
{
    // Cast through pointers to avoid triggering implicit constructors.
    return *sake::inherit_cast<T>(sake::address_of(x));
}

template< class U, class T >
inline typename inherit_cast_private::enable< T, U& >::type
inherit_cast(U& x, sake::type_tag<T>)
{ return sake::inherit_cast<T>(x); }

template< class T, class U >
inline typename inherit_cast_private::enable< T, U* >::type
inherit_cast(U* const p)
{
    typedef typename boost_ext::propagate_cv<U,T>::type cv_type;
    return static_cast< cv_type* >(p);
}

template< class U, class T >
inline typename inherit_cast_private::enable< T, U* >::type
inherit_cast(U* const p, sake::type_tag<T>)
{ return sake::inherit_cast<T>(p); }

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
    typename inherit_cast_private::enable< T, U& >::type
    operator()(U& x) const
    { return sake::inherit_cast<T>(x); }

    template< class U >
    typename inherit_cast_private::enable< T, U* >::type
    operator()(U* const p) const
    { return sake::inherit_cast<T>(p); }
};

template<>
struct inherit_cast< void >
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::result_of::inherit_cast, 2 )

    template< class U, class T >
    typename inherit_cast_private::enable< T, U& >::type
    operator()(U& x, sake::type_tag<T>) const
    { return sake::inherit_cast<T>(x); }

    template< class U, class T >
    typename inherit_cast_private::enable< T, U* >::type
    operator()(U* const p, sake::type_tag<T>) const
    { return sake::inherit_cast<T>(p); }
};

} // namespace functional

namespace result_of
{

namespace inherit_cast_private
{

template<
    class U, class T,
    class U_ = typename boost_ext::remove_qualifiers<U>::type
>
class dispatch;

template< class U, class T, class U_ >
class dispatch< U, T, U_* >
{
    BOOST_STATIC_ASSERT((!boost_ext::is_reference<T>::value));
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or<T>::value));
    BOOST_STATIC_ASSERT((!boost::is_pointer<T>::value));
    BOOST_STATIC_ASSERT((
        boost::is_base_of<T,U_>::value || boost::is_base_of<U_,T>::value));
public:
    typedef typename boost_ext::propagate_cv<U_,T>::type * type;
};

template< class U, class T, class U_ >
class dispatch< U&, T, U_ >
{
    BOOST_STATIC_ASSERT((!boost_ext::is_reference<T>::value));
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or<T>::value));
    BOOST_STATIC_ASSERT((!boost::is_pointer<T>::value));
    BOOST_STATIC_ASSERT((
        boost::is_base_of<T,U>::value || boost::is_base_of<U,T>::value));
public:
    typedef typename boost_ext::propagate_cv<U,T>::type & type;
};

template< class U, class T, class U_ >
class dispatch< U&, T, U_* >
    : public dispatch< U, T, U_* >
{ };

} // namespace inherit_cast_private

template< class U, class T >
struct inherit_cast
    : inherit_cast_private::dispatch<
          U, typename sake::remove_qualified_type_tag<T>::type >
{ };

} // namespace result_of

namespace inherit_cast_private
{

template< class T, class U >
struct enable
{ };

template< class T, class U >
struct enable< T, U& >
    : boost::lazy_enable_if_c<
          boost_ext::mpl::or2<
              boost::is_base_of<T,U>,
              boost::is_base_of<U,T>
          >::value,
          sake::result_of::inherit_cast< U&, T >
      >
{ };

template< class T, class U >
struct enable< T, U* >
    : boost::lazy_enable_if_c<
          boost_ext::mpl::or2<
              boost::is_base_of<T,U>,
              boost::is_base_of<U,T>
          >::value,
          sake::result_of::inherit_cast< U*, T >
      >
{ };

} // namespace inherit_cast_private

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_CAST_INHERIT_HPP
