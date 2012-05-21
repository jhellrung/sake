/*******************************************************************************
 * sake/core/cast/dynamic.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * dynamic_cast_(U& x, type_tag< T& >) -> T&
 * dynamic_cast_(U* p, type_tag< T* >) -> T*
 * struct functional::dynamic_cast_< T& >
 * struct functional::dynamic_cast_< T* >
 * struct functional::dynamic_cast_<>
 ******************************************************************************/

#ifndef SAKE_CORE_CAST_DYNAMIC_HPP
#define SAKE_CORE_CAST_DYNAMIC_HPP

#include <boost/static_assert.hpp>
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
struct dynamic_cast_;

} // namespace result_of

namespace dynamic_cast_private
{

template< class T, class U > struct enable;

} // namespace dynamic_cast_private

template< class U, class T >
inline typename dynamic_cast_private::enable< T&, U& >::type
dynamic_cast_(U& x, sake::type_tag< T& >)
{ return dynamic_cast< T& >(x); }

template< class U, class T >
inline typename dynamic_cast_private::enable< T*, U* >::type
dynamic_cast_(U* const p, sake::type_tag< T* >)
{ return dynamic_cast< T* >(p); }

namespace functional
{

template< class T = void >
struct dynamic_cast_;

template< class T >
struct dynamic_cast_< T& >
{
    typedef T& result_type;
    template< class U >
    typename dynamic_cast_private::enable< T&, U& >::type
    operator()(U& x) const
    { return dynamic_cast< T& >(x); }
};

template< class T >
struct dynamic_cast_< T* >
{
    typedef T* result_type;
    template< class U >
    typename dynamic_cast_private::enable< T*, U* >::type
    operator()(U* const p) const
    { return dynamic_cast< T* >(p); }
};

template<>
struct dynamic_cast_< void >
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::result_of::dynamic_cast_, 2 )

    template< class U, class T >
    typename dynamic_cast_private::enable< T&, U& >::type
    operator()(U& x, sake::type_tag< T& >) const
    { return dynamic_cast< T& >(x); }

    template< class U, class T >
    typename dynamic_cast_private::enable< T*, U* >::type
    operator()(U* const p, sake::type_tag< T* >) const
    { return dynamic_cast< T* >(p); }
};

} // namespace functional

namespace result_of
{

namespace dynamic_cast_private
{

template<
    class U, class T,
    class U_ = typename boost_ext::remove_qualifiers<U>::type
>
struct dispatch;

template< class U, class T, class U_ >
struct dispatch< U&, T&, U_ >
{
    BOOST_STATIC_ASSERT((
        boost::is_base_of<T,U>::value
    || (boost::is_polymorphic<U>::value && boost::is_base_of<U,T>::value)
    ));
    typedef T& type;
};

template< class U, class T, class U_ >
struct dispatch< U, T*, U_* >
{
    BOOST_STATIC_ASSERT((
        boost::is_base_of<T,U_>::value
    || (boost::is_polymorphic<U_>::value && boost::is_base_of<U_,T>::value)
    ));
    typedef T* type;
};

} // namespace dynamic_cast_private

template< class U, class T >
struct dynamic_cast_
    : dynamic_cast_private::dispatch<
          U, typename sake::remove_qualified_type_tag<T>::type >
{ };

} // namespace result_of

namespace polymorphic_cast_private
{

template< class T, class U >
struct enable
{ };

template< class T&, class U& >
struct enable< T&, U& >
    : boost::enable_if_c< boost_ext::mpl::or2<
          boost::is_base_of<T,U>,
          boost_ext::mpl::and2<
              boost::is_polymorphic<U>,
              boost::is_base_of<U,T>
          >
      >::value, T& >
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

#endif // #ifndef SAKE_CORE_CAST_DYNAMIC_HPP
