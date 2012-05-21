/*******************************************************************************
 * sake/core/cast/static.hpp
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

#ifndef SAKE_CORE_CAST_STATIC_HPP
#define SAKE_CORE_CAST_STATIC_HPP

#include <boost/config.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/is_cv_or.hpp>

#include <sake/core/move/forward.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/type_tag.hpp>

namespace sake
{

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
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or<T>::value));

    typedef T result_type;

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    T operator()(U&& x) const
    { return static_cast<T>(sake::forward<U>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    typedef sake::rv_sink_traits1<T> rv_sink_traits_;
    typedef typename rv_sink_traits_::template
        default_< static_cast_ > rv_sink_default_type;
public:
    // lvalues + movable explicit rvalues
    template< class U >
    T operator()(U& x) const
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
struct static_cast_< void >
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::result_of::static_cast_, (1,2) )

    template< class U >
    void operator()(U const &) const
    { }

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U, class T >
    T operator()(U&& x, sake::type_tag<T>) const
    { return static_cast<T>(sake::forward<U>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    // lvalues + movable explicit rvalues
    template< class U, class T >
    T operator()(U& x, sake::type_tag<T>) const
    { return static_cast<T>(x); }
    // T rvalues
    template< class T >
    T operator()(
        typename sake::rv_sink_traits1<T>::primary_type x,
        sake::type_tag<T>) const
    { return sake::move(x.value); }
    // movable implicit rvalues
    template< class T >
    T operator()(
        typename sake::rv_sink_traits1<T>::template default_< static_cast_ > x,
        sake::type_tag<T>) const
    { return x(); }
    // const lvalues + non-movable rvalues
    template< class U, class T >
    typename sake::rv_sink_traits1<T>::template cref_enabler<U,T>::type
    operator()(U const & x, sake::type_tag<T>) const
    { return static_cast<T>(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

} // namespace functional

#ifndef BOOST_NO_RVALUE_REFERENCES

template< class U, class T >
inline T
static_cast_(U&& x, sake::type_tag<T>)
{ return static_cast<T>(sake::forward<U>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

template< class U, class T >
inline T
static_cast_(U& x, sake::type_tag<T>)
{ return static_cast<T>(x); }

template< class T >
inline T
static_cast_(
    typename sake::rv_sink_traits1<T>::primary_type x,
    sake::type_tag<T>)
{ return sake::move(x.value); }

template< class T >
inline T
static_cast_(
    typename sake::rv_sink_traits1<T>::template
        default_< sake::functional::static_cast_ > x,
    sake::type_tag<T>)
{ return x(); }

template< class U, class T >
inline typename sake::rv_sink_traits1<T>::template cref_enabler<U,T>::type
static_cast_(U const & x, sake::type_tag<T>)
{ return static_cast<T>(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

} // namespace sake

#endif // #ifndef SAKE_CORE_CAST_STATIC_HPP
