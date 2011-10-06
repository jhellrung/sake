/*******************************************************************************
 * sake/core/utility/call_traits.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * class call_traits<T>
 *
 * This provides similar functionality as boost::call_traits.  See
 *     http://www.boost.org/libs/utility/call_traits.htm
 *
 * call_traits<T> has the following member types:
 * - param_type
 * - fwd_param_type
 * - fwd_cast_type
 * - fwd_cast
 *
 * param_type is the function parameter type one should use to receive (read-
 * only) values of type T.  If T is a reference type, this will be identical to
 * T; otherwise, this will be either T const or T const &, depending on which is
 * more efficient as dictated by is_by_value_optimal.
 *
 * fwd_param_type and fwd_cast_type can be used in generic code to forward
 * parameters of a templated type:
 *
 * template< class T >
 * struct some_class
 * {
 *     void f(typename call_traits<T>::fwd_param_type x)
 *     { g(static_cast< typename call_traits<T>::fwd_cast_type >(x)); }
 *     // or g(call_traits<T>::fwd_cast(x)) for short
 * };
 *
 * Note that call_traits<T> fails (on purpose) for array types, unlike
 * boost::call_traits.  I'm not convinced array types are appropriate to use
 * with this kind of utility.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_CALL_TRAITS_HPP
#define SAKE_CORE_UTILITY_CALL_TRAITS_HPP

#include <boost/config.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/is_object.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <sake/boost_ext/type_traits/add_const_remove_volatile.hpp>
#include <sake/boost_ext/type_traits/is_rvalue_reference.hpp>

#include <sake/core/move/is_movable.hpp>
#include <sake/core/utility/extension.hpp>
#include <sake/core/utility/is_by_value_optimal.hpp>

namespace sake
{

template< class T >
struct call_traits;

namespace no_ext
{

namespace call_traits_private
{

template< class, int = 0 >
struct disabler;

template<
    class T,
    bool = sake::is_by_value_optimal<T>::value,
#ifndef BOOST_NO_RVALUE_REFERENCES
    bool = true
#else // #ifndef BOOST_NO_RVALUE_REFERENCES
    bool = sake::is_movable< typename boost::remove_cv<T>::type >::value
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES
>
class dispatch;

template< class T >
class dispatch< T, false, false >
{
    BOOST_STATIC_ASSERT((boost::is_object<T>::value));
    BOOST_STATIC_ASSERT((boost::is_array<T>::value));
    typedef typename boost_ext::add_const_remove_volatile<T>::type const_type;
public:
    typedef T value_type;
    typedef const_type& param_type;
    typedef const_type& fwd_param_type;
    typedef const_type& fwd_cast_type;
    static fwd_cast_type
    fwd_cast(const_type& x)
    { return x; }
};

template< class T, bool IsMovable >
class dispatch< T, true, IsMovable >
{
    typedef typename boost_ext::add_const_remove_volatile<T>::type const_type;
public:
    typedef T value_type;
    typedef const_type param_type;
    typedef const_type fwd_param_type;
    typedef const_type fwd_cast_type;
    static fwd_cast_type
    fwd_cast(const_type x)
    { return x; }
};

template< class T >
class dispatch< T, false, true >
{
    typedef typename boost_ext::add_const_remove_volatile<T>::type const_type;
    typedef typename boost::remove_cv<T>::type nocv_type;
public:
    typedef T value_type;
    typedef const_type& param_type;
    typedef nocv_type fwd_param_type;
    typedef SAKE_RV_REF( nocv_type ) fwd_cast_type;
    static fwd_cast_type
    fwd_cast(nocv_type& x)
    { return static_cast< fwd_cast_type >(x); }
};

} // namespace call_traits_private

template< class T >
struct call_traits
    : call_traits_private::dispatch<T>
{ };

template< class T >
struct call_traits< T& >
{
    BOOST_STATIC_ASSERT((!boost_ext::is_rvalue_reference< T& >::value));
    typedef T& value_type;
    typedef T& param_type;
    typedef T& fwd_param_type;
    typedef T& fwd_cast_type;
    static fwd_cast_type
    fwd_cast(T& x)
    { return x; }
};

#ifndef BOOST_NO_RVALUE_REFERENCES

template< class T >
struct call_traits< T&& >
{
    typedef T&& value_type;
    typedef T&& param_type;
    typedef T&& fwd_param_type;
    typedef T&& fwd_cast_type;
    static fwd_cast_type
    fwd_cast(T& x)
    { return static_cast< T&& >(x); }
};

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

template< class T >
struct call_traits< boost::rv<T>& >
{
    typedef boost::rv<T>& value_type;
    typedef boost::rv<T>& param_type;
    typedef boost::rv<T>& fwd_param_type;
    typedef boost::rv<T>& fwd_cast_type;
    static fwd_cast_type
    fwd_cast(T& x)
    { return x; }
};

template< class T >
struct call_traits< boost::rv<T> const & >
{
    typedef boost::rv<T> const & value_type;
    typedef boost::rv<T> const & param_type;
    typedef boost::rv<T> const & fwd_param_type;
    typedef boost::rv<T> const & fwd_cast_type;
    static fwd_cast_type
    fwd_cast(T const & x)
    { return x; }
};

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

} // namespace no_ext

SAKE_EXTENSION_UNARY_CLASS( call_traits )

template< class T >
struct call_traits
    : ext::call_traits<T>
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_CALL_TRAITS_HPP
