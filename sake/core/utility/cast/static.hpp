/*******************************************************************************
 * sake/core/utility/cast/static.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * static_cast_(U&& x, type_tag<T>) -> T
 * struct functional::static_cast_<T>
 * struct functional::static_cast_< void >
 * struct functional::static_cast_<>
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_CAST_STATIC_HPP
#define SAKE_CORE_UTILITY_CAST_STATIC_HPP

#include <boost/config.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_same_sans_qualifiers.hpp>

#include <sake/core/move/forward.hpp>
#include <sake/core/move/is_movable.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/type_tag.hpp>

namespace sake
{

#ifdef BOOST_NO_RVALUE_REFERENCE

namespace static_cast_private
{

template< class T > struct rv_sink;
template< class T, class U > struct enable_cref;

} // namespace static_cast_private

#endif // #ifdef BOOST_NO_RVALUE_REFERENCE

namespace result_of
{

template< class U, class T = void >
struct static_cast_
    : sake::remove_qualified_type_tag<T>
{ };

} // namespace result_of

namespace functional
{

template< class T = void >
struct static_cast_
{
    typedef T result_type;

#ifndef BOOST_NO_RVALUE_REFERENCE

    template< class U >
    T operator()(U&& x) const
    { return static_cast<T>(sake::forward<U>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCE

private:
    typedef typename sake::rv_sink_traits::rv_param<T>::type rv_param_type;
    typedef typename sake::static_cast_private::rv_sink<T>::type rv_sink_type;
public:
    // lvalues + movable explicit rvalues
    template< class U >
    T operator()(U& x) const
    { return static_cast<T>(x); }
    // T rvalues
    T operator()(rv_param_type x) const
    { return x; }
    // movable implicit rvalues
    T operator()(rv_sink_type x) const
    { return x(); }
    // const lvalues + non-movable rvalues
    template< class U >
    typename sake::static_cast_private::enable_cref<T,U>::type
    operator()(U const & x) const
    { return static_cast<T>(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCE

};

template<>
struct static_cast_< void >
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::result_of::static_cast_, (1,2) )

    template< class U >
    void operator()(U const &) const
    { }

#ifndef BOOST_NO_RVALUE_REFERENCE

    template< class U, class T >
    T operator()(U&& x, sake::type_tag<T>) const
    { return static_cast<T>(sake::forward<U>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCE

    // lvalues + movable explicit rvalues
    template< class U, class T >
    T operator()(U& x, sake::type_tag<T>) const
    { return static_cast<T>(x); }
    // T rvalues
    template< class T >
    T operator()(
        typename sake::rv_sink_traits::rv_param<T>::type x,
        sake::type_tag<T>) const
    { return x; }
    // movable implicit rvalues
    template< class T >
    T operator()(
        typename sake::static_cast_private::rv_sink<T>::type x,
        sake::type_tag<T>) const
    { return x(); }
    // const lvalues + non-movable rvalues
    template< class U, class T >
    typename sake::static_cast_private::enable_cref<T,U>::type
    operator()(U const & x, sake::type_tag<T>) const
    { return static_cast<T>(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCE

};

} // namespace functional

#ifndef BOOST_NO_RVALUE_REFERENCE

template< class U, class T >
inline T
static_cast_(U&& x, sake::type_tag<T>)
{ return static_cast<T>(sake::forward<U>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCE

template< class U, class T >
inline T
static_cast_(U& x, sake::type_tag<T>)
{ return static_cast<T>(x); }

template< class T >
inline T
static_cast_(
    typename sake::rv_sink_traits::rv_param<T>::type x,
    sake::type_tag<T>)
{ return x; }

template< class T >
inline T
static_cast_(
    typename sake::static_cast_private::rv_sink<T>::type x,
    sake::type_tag<T>)
{ return x(); }

template< class U, class T >
inline typename sake::static_cast_private::enable_cref<T,U>::type
static_cast_(U const & x, sake::type_tag<T>)
{ return static_cast<T>(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCE

#ifdef BOOST_NO_RVALUE_REFERENCE

namespace static_cast_private
{

template< class T >
struct rv_sink
{
    typedef sake::rv_sink<
        sake::functional::static_cast_<T>, // Visitor
        T, // Result
        boost::mpl::not_< boost::is_same< T, boost::mpl::_1 > > // Pred
    > type;
};

template< class T, class U >
struct enable_cref
    : boost::disable_if_c< boost_ext::mpl::or2<
          boost_ext::is_same_sans_qualifiers<
              U, typename sake::rv_sink_traits::rv_param<T>::type >,
          sake::is_movable<U>
      >::value, T >
{ };

} // namespace static_cast_private

#endif // #ifdef BOOST_NO_RVALUE_REFERENCE

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_CAST_STATIC_HPP
