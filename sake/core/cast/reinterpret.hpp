/*******************************************************************************
 * sake/core/cast/reinterpret.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * reinterpret_cast_< T* >(U* p) -> T*
 * reinterpret_cast_(U* p, type_tag< T* >) -> T*
 * struct functional::reinterpret_cast_< T* >
 * struct functional::reinterpret_cast_<>
 ******************************************************************************/

#ifndef SAKE_CORE_CAST_REINTERPRET_HPP
#define SAKE_CORE_CAST_REINTERPRET_HPP

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/type_tag.hpp>

namespace sake
{

namespace result_of
{

template< class U, class T >
struct reinterpret_cast_;

} // namespace result_of

template< class T, class U >
inline T
reinterpret_cast_(U* const p)
{ return sake::reinterpret_cast_(p, sake::type_tag<T>()); }

template< class U, class T >
inline T*
reinterpret_cast_(U* const p, sake::type_tag< T* >)
{ return reinterpret_cast< T* >(p); }

namespace functional
{

template< class T = void >
struct reinterpret_cast_;

template< class T >
struct reinterpret_cast_< T* >
{
    typedef T* result_type;
    template< class U >
    T* operator()(U* const p) const
    { return reinterpret_cast< T* >(p); }
};

template<>
struct reinterpret_cast_< void >
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::result_of::reinterpret_cast_, 2 )

    template< class U, class T >
    T* operator()(U* const p, sake::type_tag< T* >) const
    { return reinterpret_cast< T* >(p); }
};

} // namespace functional

namespace result_of
{

namespace reinterpret_cast_private
{

template< class U, class T >
struct impl;

template< class U, class T >
struct impl< U*, T* >
{ typedef T* type; };

} // namespace reinterpret_cast_private

template< class U, class T >
struct reinterpret_cast_
    : reinterpret_cast_private::impl<
          typename boost_ext::remove_qualifiers<U>::type,
          typename sake::remove_qualified_type_tag<T>::type
      >
{ };

} // namespace result_of

} // namespace sake

#endif // #ifndef SAKE_CORE_CAST_REINTERPRET_HPP
