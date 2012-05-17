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
 * explicit conversions (e.g., explicit constructors).
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_CAST_IMPLICIT_HPP
#define SAKE_CORE_UTILITY_CAST_IMPLICIT_HPP

#include <boost/config.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/curry_quote.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/move/forward.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/type_tag.hpp>

namespace sake
{

#ifdef BOOST_NO_RVALUE_REFERENCES

namespace implicit_cast_private
{

template< class T > struct rv_sink_traits;

} // namespace implicit_cast_private

#endif // #ifdef BOOST_NO_RVALUE_REFERENCES

namespace result_of
{

template< class U, class T >
struct implicit_cast
{
    typedef typename sake::remove_qualified_type_tag<T>::type type;
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or< type >::value));
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename boost_ext::remove_rvalue_reference<U>::type, type >::value));
};

} // namespace result_of

namespace functional
{

template< class T = void >
struct implicit_cast
{
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or<T>::value));

    typedef T result_type;

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    typename boost::enable_if_c<
        boost_ext::is_convertible<U,T>::value, T >::type
    operator()(U&& x) const
    { return static_cast<T>(sake::forward<U>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    typedef implicit_cast_private::rv_sink_traits<T> rv_sink_traits_;
    typedef typename rv_sink_traits_::template
        default_< implicit_cast > rv_sink_default_type;
public:
    // lvalues + movable explicit rvalues
    template< class U >
    typename rv_sink_traits_::template ref_enabler<U,T>::type
    operator()(U& x) const
    { return static_cast<T>(x); }
    // T rvalues
    T operator()(typename rv_sink_traits_::primary_type x) const
    { return sake::move(x.value); }
    // movable implicit rvalues
    T operator()(rv_sink_default_type x) const
    { return x(); }
    // const lvalues + non-movable rvalues
    template< class U >
    typename rv_sink_traits_::template cref_enabler<U,T>::type
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
    typename boost::enable_if_c<
        boost_ext::is_convertible<U,T>::value, T >::type
    operator()(U&& x, sake::type_tag<T>) const
    { return static_cast<T>(sake::forward<U>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    // lvalues + movable explicit rvalues
    template< class U, class T >
    typename implicit_cast_private::rv_sink_traits<T>::template
        ref_enabler<U,T>::type
    operator()(U& x, sake::type_tag<T>) const
    { return static_cast<T>(x); }
    // T rvalues
    template< class T >
    T operator()(
        typename implicit_cast_private::rv_sink_traits<T>::primary_type x,
        sake::type_tag<T>) const
    { return sake::move(x.value); }
    // movable implicit rvalues
    template< class T >
    T operator()(
        typename implicit_cast_private::rv_sink_traits<T>::template
            default_< sake::functional::implicit_cast<T> > x,
        sake::type_tag<T>) const
    { return x(); }
    // const lvalues + non-movable rvalues
    template< class U, class T >
    typename implicit_cast_private::rv_sink_traits<T>::template
        cref_enabler<U,T>::type
    operator()(U const & x, sake::type_tag<T>) const
    { return static_cast<T>(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

} // namespace functional

#ifndef BOOST_NO_RVALUE_REFERENCES

template< class T, class U >
inline typename boost::enable_if_c<
    boost_ext::is_convertible<U,T>::value, T >::type
implicit_cast(U&& x)
{ return static_cast<T>(sake::forward<U>(x)); }

template< class U, class T >
inline typename boost::enable_if_c<
    boost_ext::is_convertible<U,T>::value, T >::type
implicit_cast(U&& x, sake::type_tag<T>)
{ return static_cast<T>(sake::forward<U>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

template< class T, class U >
inline typename implicit_cast_private::rv_sink_traits<T>::template
    ref_enabler<U,T>::type
implicit_cast(U& x)
{ return static_cast<T>(x); }

template< class U, class T >
inline typename implicit_cast_private::rv_sink_traits<T>::template
    ref_enabler<U,T>::type
implicit_cast(U& x, sake::type_tag<T>)
{ return static_cast<T>(x); }

template< class T >
inline T
implicit_cast(
    typename implicit_cast_private::rv_sink_traits<T>::primary_type x)
{ return sake::move(x.value); }

template< class T >
inline T
implicit_cast(
    typename implicit_cast_private::rv_sink_traits<T>::primary_type x,
    sake::type_tag<T>)
{ return sake::move(x.value); }

template< class T >
inline T
implicit_cast(
    typename implicit_cast_private::rv_sink_traits<T>::template
        default_< sake::functional::implicit_cast<T> > x)
{ return x(); }

template< class T >
inline T
implicit_cast(
    typename implicit_cast_private::rv_sink_traits<T>::template
        default_< sake::functional::implicit_cast<T> > x,
    sake::type_tag<T>)
{ return x(); }

template< class T, class U >
inline typename implicit_cast_private::rv_sink_traits<T>::template
    cref_enabler<U,T>::type
implicit_cast(U const & x)
{ return static_cast<T>(x); }

template< class U, class T >
inline typename implicit_cast_private::rv_sink_traits<T>::template
    cref_enabler<U,T>::type
implicit_cast(U const & x, sake::type_tag<T>)
{ return static_cast<T>(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#ifdef BOOST_NO_RVALUE_REFERENCES

namespace implicit_cast_private
{

template< class T >
struct rv_sink_traits
    : sake::rv_sink_traits1<
          T, typename boost_ext::mpl::curry_quote2<
              boost_ext::is_convertible >::apply<T>::type >
{ };

} // namespace implicit_cast_private

#endif // #ifdef BOOST_NO_RVALUE_REFERENCES

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_CAST_IMPLICIT_HPP
