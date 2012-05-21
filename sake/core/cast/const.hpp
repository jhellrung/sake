/*******************************************************************************
 * sake/core/cast/const.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * const_cast_(U& x, type_tag< T& >) -> T&
 * const_cast_(U* p, type_tag< T* >) -> T*
 * struct functional::const_cast_< T& >
 * struct functional::const_cast_< T* >
 * struct functional::const_cast_<>
 ******************************************************************************/

#ifndef SAKE_CORE_CAST_CONST_HPP
#define SAKE_CORE_CAST_CONST_HPP

#include <boost/static_assert.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/is_same_sans_cv.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/type_tag.hpp>

namespace sake
{

namespace result_of
{

template< class U, class T >
struct const_cast_;

} // namespace result_of

namespace const_cast_private
{

template< class T, class U > struct enable;

} // namespace const_cast_private

template< class U, class T >
inline typename const_cast_private::enable< T&, U& >::type
const_cast_(U& x, sake::type_tag< T& >)
{ return const_cast< T& >(x); }

template< class U, class T >
inline typename const_cast_private::enable< T*, U* >::type
const_cast_(U* const p, sake::type_tag< T* >)
{ return const_cast< T* >(p); }

namespace functional
{

template< class T = void >
struct const_cast_;

template< class T >
struct const_cast_< T& >
{
    typedef T& result_type;
    template< class U >
    typename const_cast_private::enable< T&, U& >::type
    operator()(U& x) const
    { return const_cast< T& >(x); }
};

template< class T >
struct const_cast_< T* >
{
    typedef T* result_type;
    typename const_cast_private::enable< T*, U* >::type
    operator()(U* const p) const
    { return const_cast< T* >(p); }
};

template<>
struct const_cast_< void >
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::result_of::const_cast_, 2 )

    template< class U, class T >
    typename const_cast_private::enable< T&, U& >::type
    operator()(U& x, sake::type_tag< T& >) const
    { return const_cast< T& >(x); }

    template< class U, class T >
    typename const_cast_private::enable< T*, U* >::type
    operator()(U* const p, sake::type_tag< T* >) const
    { return const_cast< T* >(p); }
};

} // namespace functional

namespace result_of
{

namespace const_cast_private
{

template<
    class U, class T,
    class U_ = typename boost_ext::remove_qualifiers<U>::type
>
struct dispatch;

template< class U, class T, class U_ >
struct dispatch< U&, T&, U_ >
{
    BOOST_STATIC_ASSERT((boost_ext::is_same_sans_cv<T,U>::value));
    typedef T& type;
};

template< class U, class T, class U_ >
struct dispatch< U, T*, U_* >
{
    BOOST_STATIC_ASSERT((boost_ext::is_same_sans_cv<T,U_>::value));
    typedef T* type;
};

} // namespace const_cast_private

template< class U, class T >
struct const_cast_
    : const_cast_private::dispatch<
          U, typename sake::remove_qualified_type_tag<T>::type >
{ };

} // namespace result_of

namespace const_cast_private
{

template< class T, class U >
struct enable
{ };

template< class T, class U >
struct enable< T&, U& >
    : boost::enable_if_c<
          boost_ext::is_same_sans_cv<T,U>::value,
          T&
      >
{ };

template< class T, class U >
struct enable< T*, U* >
    : boost::enable_if_c<
          boost_ext::is_same_sans_cv<T,U>::value,
          T*
      >
{ };

} // namespace const_cast_private

} // namespace sake

#endif // #ifndef SAKE_CORE_CAST_CONST_HPP
