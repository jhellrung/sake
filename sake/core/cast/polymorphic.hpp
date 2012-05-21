/*******************************************************************************
 * sake/core/cast/polymorphic.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * polymorphic_cast< T* >(U* p) -> T*
 * polymorphic_cast(U* p, type_tag< T* >) -> T*
 * struct functional::polymorphic_cast< T* >
 * struct functional::polymorphic_cast<>
 ******************************************************************************/

#ifndef SAKE_CORE_CAST_POLYMORPHIC_HPP
#define SAKE_CORE_CAST_POLYMORPHIC_HPP

#include <typeinfo>

#include <boost/static_assert.hpp>
#include <boost/throw_exception.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_polymorphic.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/type_tag.hpp>

namespace sake
{

namespace result_of
{

template< class U, class T >
struct polymorphic_cast;

} // namespace result_of

namespace polymorphic_cast_private
{

template< class T, class U > struct enable;

} // namespace polymorphic_cast_private

template< class T, class U >
inline typename polymorphic_cast_private::enable< T, U* >::type
polymorphic_cast(U* const p)
{ return sake::polymorphic_cast(p, sake::type_tag<T>()); }

template< class U, class T >
inline typename polymorphic_cast_private::enable< T*, U* >::type
polymorphic_cast(U* const p, sake::type_tag< T* >)
{
    T* const q = dynamic_cast< T* >(p);
    if(!q)
        boost::throw_exception(std::bad_cast());
    return q;
}

namespace functional
{

template< class T = void >
struct polymorphic_cast;

template< class T >
struct polymorphic_cast< T* >
{
    typedef T* result_type;
    template< class U >
    typename polymorphic_cast_private::enable< T*, U* >::type
    operator()(U* const p) const
    { return sake::polymorphic_cast< T* >(p); }
};

template<>
struct polymorphic_cast< void >
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::result_of::polymorphic_cast, 2 )

    template< class U, class T >
    typename polymorphic_cast_private::enable< T*, U* >::type
    operator()(U* const p, sake::type_tag< T* >) const
    { return sake::polymorphic_cast< T* >(p); }
};

} // namespace functional

namespace result_of
{

namespace polymorphic_cast_private
{

template< class U, class T >
struct impl;

template< class U, class T >
struct impl< U*, T* >
{
    BOOST_STATIC_ASSERT((
        boost::is_base_of<T,U>::value
    || (boost::is_polymorphic<U>::value && boost::is_base_of<U,T>::value)
    ));
    typedef T* type;
};

} // namespace polymorphic_cast_private

template< class U, class T >
struct polymorphic_cast
    : polymorphic_cast_private::impl<
          typename boost_ext::remove_qualifiers<U>::type,
          typename sake::remove_qualified_type_tag<T>::type
      >
{ };

} // namespace result_of

namespace polymorphic_cast_private
{

template< class T, class U >
struct enable
{ };

template< class T, class U >
struct enable< T*, U* >
    : boost::enable_if_c< boost_ext::mpl::or2<
          boost::is_base_of<T,U>,
          boost_ext::mpl::and2<
              boost::is_polymorphic<U>,
              boost::is_base_of<U,T>
          >
      >::value, T* >
{ };

} // namespace polymorphic_cast_private

} // namespace sake

#endif // #ifndef SAKE_CORE_CAST_POLYMORPHIC_HPP
