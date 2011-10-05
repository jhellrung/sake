/*******************************************************************************
 * sake/core/move/forward.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_FWD_REF( T )
 * #define SAKE_FWD_PARAM( T )
 * #define SAKE_FWD2_REF( T )
 * #define SAKE_FWD2_PARAM( T )
 *
 * forward<T>(U&& x) -> T&&
 *
 * These provide specific forwarding strategies.  If BOOST_NO_RVALUE_REFERENCES
 * is not defined, there is no difference between FWD_REF and FWD2_REF and both
 * expand to T&&.
 *
 * Use SAKE_FWD_REF( T ) / SAKE_FWD_PARAM( T ) for a deduced template parameter
 * which must be bindable to rvalues.  Unfortunately, if
 * BOOST_NO_RVALUE_REFERENCES, no distinction can be made between
 * references-to-const and references-to-non-const, so this is only appropriate
 * for parameters whose values will only be read or moved.  Also, if
 * BOOST_NO_RVALUE_REFERENCES is not defined, rvalues will *not* be deduced to
 * emulated rvalue references unless wrapped with SAKE_MOVE_RVALUE (see
 * move_rvalue.hpp).  Only *explicit* emulated rvalue references produced by,
 * e.g., sake::move or sake::forward, will be deduced correctly.
 *
 * Example:
 *
 * template< class T >
 * typename result_of::f< SAKE_FWD_PARAM( T ) >::type
 * f(SAKE_FWD_REF( T ) x)
 * { return g(sake::forward<T>(x)); }
 *
 * Use SAKE_FWD2_REF( T ) / SAKE_FWD2_PARAM( T ) for a deduced template
 * parameter which only has to bind to lvalue references or (emulated) rvalue
 * references (e.g., the result of a sake::move or sake::forward).  Thus, this
 * is the appropriate forwarding reference for a function which is only called
 * by other generic forwarding functions.
 *
 * Example:
 *
 * template< class T >
 * typename result_of::f< SAKE_FWD_PARAM( T ) >::type
 * f(SAKE_FWD_REF( T ) x)
 * { return g(sake::forward<T>(x)); }
 *
 * // This overload of f is helpful when BOOST_NO_RVALUE_REFERENCES is defined
 * // and you want to capture non-const references.
 * template< class T >
 * typename result_of::f< T& >::type
 * f(T& x)
 * { return g(x); } // No need for a forward call.
 *
 * template< class T >
 * typename result_of::g< SAKE_FWD2_PARAM( T ) >::type
 * g(SAKE_FWD2_REF( T ) x)
 * { return h(sake::forward<T>(x)); }
 *
 * Note: SAKE_FWD_PARAM( T ) and SAKE_FWD2_PARAM( T ) are the function parameter
 * types with (emulated) rvalue references removed.  In other words, these are
 * the appropriate types to use with Boost.ResultOf-style protocols.
 ******************************************************************************/

#ifndef SAKE_CORE_MOVE_FORWARD_HPP
#define SAKE_CORE_MOVE_FORWARD_HPP

#include <boost/config.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_convertible.hpp>

#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>
#include <sake/boost_ext/type_traits/is_lvalue_reference.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/move/rv.hpp>

#ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_FWD_REF( T )    T &&
#define SAKE_FWD_PARAM( T )  T
#define SAKE_FWD2_REF( T )   T &&
#define SAKE_FWD2_PARAM( T ) T

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_FWD_REF( T )    T const &
#define SAKE_FWD_PARAM( T )  typename ::sake::fwd_param<T>::type
#define SAKE_FWD2_REF( T )   T       &
#define SAKE_FWD2_PARAM( T ) typename ::sake::fwd2_param<T>::type

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

namespace sake
{

#ifndef BOOST_NO_RVALUE_REFERENCES

template< class T, class U >
inline T&&
forward(U&& x)
{
    BOOST_STATIC_ASSERT((!boost_ext::is_lvalue_reference<T>::value ||
                          boost_ext::is_lvalue_reference<U>::value));
    BOOST_STATIC_ASSERT((boost::is_convertible<
        typename boost_ext::remove_reference<U>::type*,
        typename boost_ext::remove_reference<T>::type*
    >::value));
    return static_cast< T&& >(x);
}

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

template< class T >
struct fwd_param
    : boost_ext::remove_rvalue_reference< T const & >
{ };

template< class T >
struct fwd2_param
    : boost_ext::remove_rvalue_reference< T& >
{ };

namespace forward_private
{

template< class T, class U >
struct dispatch
{
    BOOST_STATIC_ASSERT((boost::is_convertible< U*, T* >::value));
    typedef typename boost_ext::add_rvalue_reference<T>::type result_type;
    static result_type apply(boost::rv<U> const & x)
    { return static_cast< result_type >(const_cast< boost::rv<U>& >(x)); }
};

template< class T, class U >
struct dispatch< T, U& >
{
    BOOST_STATIC_ASSERT((boost::is_convertible< U*, T* >::value));
    typedef typename boost_ext::add_rvalue_reference<T>::type result_type;
    static result_type apply(U& x)
    { return static_cast< result_type >(x); }
};

template< class T, class U >
struct dispatch< T&, U >
{ /* Disable forwarding an rvalue as an lvalue. */ };

template< class T, class U >
struct dispatch< T&, U& >
{
    typedef T& result_type;
    static result_type apply(U& x)
    {
        // Need this assert at member function scope (rather than at class
        // scope) since the full signature of both forward overloads below need
        // to be known before the correct overload can be selected.
        BOOST_STATIC_ASSERT((boost::is_convertible< U*, T* >::value));
        return static_cast< result_type >(x);
    }
};

} // namespace forward_private

/*******************************************************************************
 * In the absence of rvalue references, we settle on forward having the result
 * types described in the table below.  Keep in mind that the result types are
 * primarily motivated by the use case typified by
 *
 * template< class T >
 * void f1(SAKE_FWD_REF( T ) x) { g1(sake::forward<T>(x)); }
 * template< class T >
 * void f2(SAKE_FWD2_REF( T ) x) { g2(sake::forward<T>(x); }
 *
 * Thus, if T is not an emulated rvalue, sake::forward<T>(x) should have a
 * result type of T& or T const & (depending on the const-qualification of its
 * argument, x).  On the other hand, if T *is* an emulated rvalue,
 * sake::forward<T>(x) should have a result type of boost::rv<T>&.
 *
 * "U&", "U const &", and "rv<U> {const} &" indicate a generic lvalue
 * reference-to-non-const, lvalue reference-to-const, and emulated rvalue
 * reference, respectively.
 * "T{&}" indicates a generic non-const object or lvalue reference-to-non-const.
 * "T const {&}" indicates a generic const object or lvalue reference-to-const.
 * "rv<T> {const} {&}" indicates a generic emulated rvalue reference (possibly
 * const-qualified, and possibly without an actual reference qualifier).
 *
 *             T \ U | U&        | U const & | rv<U> {const} &
 * ------------------+-----------+-----------+----------------
 * T{&}              | T&        | T const & | disabled
 * T const {&}       | T const & | T const & | disabled
 * rv<T> {const} {&} | rv<T>&    | disabled  | rv<T>&
 ******************************************************************************/

template< class T, class U >
inline typename forward_private::dispatch<
    typename boost_ext::remove_rvalue_reference<
        typename boost_ext::add_reference<T>::type
    >::type,
    typename boost_ext::remove_rvalue_reference< U& >::type
>::result_type
forward(U& x)
{
    return forward_private::dispatch<
        typename boost_ext::remove_rvalue_reference<
            typename boost_ext::add_reference<T>::type
        >::type,
        typename boost_ext::remove_rvalue_reference< U& >::type
    >::apply(x);
}

template< class T, class U >
inline typename forward_private::dispatch<
    typename boost_ext::remove_rvalue_reference<
        typename boost_ext::add_reference_add_const<T>::type
    >::type,
    typename boost_ext::remove_rvalue_reference< U const & >::type
>::result_type
forward(U const & x)
{
    return forward_private::dispatch<
        typename boost_ext::remove_rvalue_reference<
            typename boost_ext::add_reference_add_const<T>::type
        >::type,
        typename boost_ext::remove_rvalue_reference< U const & >::type
    >::apply(x);
}

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

} // namespace sake

#endif // #ifndef SAKE_CORE_MOVE_FORWARD_HPP
