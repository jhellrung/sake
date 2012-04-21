/*******************************************************************************
 * sake/core/utility/cast/implicit.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * implicit_cast<T>(U&& x) -> T
 * implicit_cast(U&& x, type_tag<T>) -> T
 * struct functional::implicit_cast<T>
 * struct functional::implicit_cast< void >
 * struct functional::implicit_cast<>
 *
 * implicit_cast is similar to a static_cast, but it disallows unintentional
 * explicit conversions (e.g., constructors declared explicit).
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_CAST_IMPLICIT_HPP
#define SAKE_CORE_UTILITY_CAST_IMPLICIT_HPP

#include <boost/config.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/is_same_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/move/forward.hpp>
#include <sake/core/move/is_movable.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/type_tag.hpp>

namespace sake
{

namespace implicit_cast_private
{

template< class T, class U > struct enable;
#ifdef BOOST_NO_RVALUE_REFERENCES
template< class T > struct rv_sink;
template< class T, class U > struct enable_ref;
template< class T, class U > struct enable_cref;
#endif // #ifdef BOOST_NO_RVALUE_REFERENCES

} // namespace implicit_cast_private

namespace result_of
{

template< class U, class T >
struct implicit_cast
{
    typedef typename sake::remove_qualified_type_tag<T>::type type;
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename boost_ext::remove_rvalue_reference<U>::type, type >::value));
};

} // namespace result_of

namespace functional
{

template< class T = void >
struct implicit_cast
{
    typedef T result_type;

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    typename implicit_cast_private::enable<T,U>::type
    operator()(U&& x) const
    { return static_cast<T>(sake::forward<U>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    typedef typename sake::rv_sink_traits::rv_param<T>::type rv_param_type;
    typedef typename sake::implicit_cast_private::rv_sink<T>::type rv_sink_type;
public:
    // lvalues + movable explicit rvalues
    template< class U >
    typename sake::implicit_cast_private::enable_ref<T,U>::type
    operator()(U& x) const
    { return static_cast<T>(x); }
    // T rvalues
    T operator()(rv_param_type x) const
    { return static_cast<T>(x); }
    // movable implicit rvalues
    T operator()(rv_sink_type x) const
    { return x(); }
    // const lvalues + non-movable rvalues
    template< class U >
    typename sake::implicit_cast_private::enable_cref<T,U>::type
    operator()(U const & x) const
    { return static_cast<T>(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

template<>
struct implicit_cast< void >
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::result_of::implicit_cast, 2 )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U, class T >
    typename implicit_cast_private::enable<T,U>::type
    operator()(U&& x, sake::type_tag<T>) const
    { return static_cast<T>(sake::forward<U>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    // lvalues + movable explicit rvalues
    template< class U, class T >
    typename sake::implicit_cast_private::enable_ref<T,U>::type
    operator()(U& x, sake::type_tag<T>) const
    { return static_cast<T>(x); }
    // T rvalues
    template< class T >
    T operator()(
        typename sake::rv_sink_traits::rv_param<T>::type x,
        sake::type_tag<T>) const
    { return static_cast<T>(x); }
    // movable implicit rvalues
    template< class T >
    T operator()(
        typename sake::implicit_cast_private::rv_sink<T>::type x,
        sake::type_tag<T>) const
    { return x(); }
    // const lvalues + non-movable rvalues
    template< class U, class T >
    typename sake::implicit_cast_private::enable_cref<T,U>::type
    operator()(U const & x, sake::type_tag<T>) const
    { return static_cast<T>(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

} // namespace functional

#ifndef BOOST_NO_RVALUE_REFERENCES

template< class T, class U >
inline typename implicit_cast_private::enable<T,U>::type
implicit_cast(U&& x)
{ return static_cast<T>(sake::forward<U>(x)); }

template< class U, class T >
inline typename implicit_cast_private::enable<T,U>::type
implicit_cast(U&& x, sake::type_tag<T>)
{ return static_cast<T>(sake::forward<U>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

template< class T, class U >
inline typename sake::implicit_cast_private::enable_ref<T,U>::type
implicit_cast(U& x)
{ return static_cast<T>(x); }

template< class U, class T >
inline typename sake::implicit_cast_private::enable_ref<T,U>::type
implicit_cast(U& x, sake::type_tag<T>)
{ return static_cast<T>(x); }

template< class T >
inline T
implicit_cast(
    typename sake::rv_sink_traits::rv_param<T>::type x)
{ return x; }

template< class T >
inline T
implicit_cast(
    typename sake::rv_sink_traits::rv_param<T>::type x,
    sake::type_tag<T>)
{ return x; }

template< class T >
inline T
implicit_cast(
    typename sake::implicit_cast_private::rv_sink<T>::type x)
{ return x(); }

template< class T >
inline T
implicit_cast(
    typename sake::implicit_cast_private::rv_sink<T>::type x,
    sake::type_tag<T>)
{ return x(); }

template< class T, class U >
inline typename sake::implicit_cast_private::enable_cref<T,U>::type
implicit_cast(U const & x)
{ return static_cast<T>(x); }

template< class U, class T >
inline typename sake::implicit_cast_private::enable_cref<T,U>::type
implicit_cast(U const & x, sake::type_tag<T>)
{ return static_cast<T>(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

namespace implicit_cast_private
{

template< class T, class U >
struct enable
    : boost::enable_if_c< boost_ext::is_convertible<U,T>::value, T >
{ };

#ifdef BOOST_NO_RVALUE_REFERENCES

template< class T >
struct rv_sink
{
    typedef sake::rv_sink<
        sake::functional::implicit_cast<T>, // Visitor
        T, // Result
        boost_ext::mpl::and2<
            boost_ext::is_convertible< boost::mpl::_1, T >,
            boost::mpl::not_< boost::is_same< T, boost::mpl::_1 > >
        > // Pred
    > type;
};

template< class T, class U >
struct enable_ref
    : implicit_cast_private::enable<
          typename boost_ext::remove_rvalue_reference< U& >::type,
          T
      >
{ };

template< class T, class U >
struct enable_cref
    : boost::disable_if_c< boost_ext::mpl::or3<
          boost::mpl::not_< boost_ext::is_convertible< U const &, T > >,
          boost_ext::is_same_sans_qualifiers<
              U, typename sake::rv_sink_traits::rv_param<T>::type >,
          sake::is_movable<U>
      >::value, T >
{ };

#endif // #ifdef BOOST_NO_RVALUE_REFERENCES

} // namespace implicit_cast_private

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_CAST_IMPLICIT_HPP
