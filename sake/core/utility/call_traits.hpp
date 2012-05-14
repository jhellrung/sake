/*******************************************************************************
 * sake/core/utility/call_traits.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct call_traits<T>
 * struct extension::call_traits< T, Enable = void >
 * struct default_impl::call_traits<T>
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
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/is_object.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/type_traits/add_const_remove_volatile.hpp>
#include <sake/boost_ext/type_traits/is_rvalue_reference.hpp>

#include <sake/core/move/has_move_emulation.hpp>
#include <sake/core/utility/call_traits_fwd.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/is_by_value_optimal.hpp>

namespace sake
{

/*******************************************************************************
 * struct call_traits<T>
 ******************************************************************************/

template< class T >
struct call_traits
    : extension::call_traits<T>
{ };

/*******************************************************************************
 * struct extension::call_traits< T, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T, class Enable /*= void*/ >
struct call_traits
    : default_impl::call_traits<T>
{ };

} // namespace extension

/*******************************************************************************
 * struct default_impl::call_traits<T>
 ******************************************************************************/

namespace default_impl
{

namespace call_traits_private
{

template< class, int = 0 >
struct disabler;

template< class T >
struct dispatch_index
{
    static int const value = boost_ext::mpl::
         if_< sake::is_by_value_optimal<T>, sake::int_tag<2> >::type::template
#ifndef BOOST_NO_RVALUE_REFERENCES
    else_   < sake::int_tag<1> >::type::value;
#else // #ifndef BOOST_NO_RVALUE_REFERENCES
    else_if < sake::has_move_emulation<T>, sake::int_tag<1> >::type::template
    else_   < sake::int_tag<0> >::type::value;
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES
};

template<
    class T,
    int = dispatch_index< typename boost::remove_cv<T>::type >::value
>
class dispatch;

template< class T >
class dispatch<T,2>
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
class dispatch<T,1>
{
    typedef typename boost::remove_cv<T>::type nocv_type;
    typedef typename boost::add_const<T>::type const_type;
public:
    typedef T value_type;
    typedef const_type& param_type;
    typedef nocv_type fwd_param_type;
    typedef SAKE_RV_REF( nocv_type ) fwd_cast_type;
    static fwd_cast_type
    fwd_cast(nocv_type& x)
    { return static_cast< fwd_cast_type >(x); }
};

template< class T >
class dispatch<T,0>
{
    BOOST_STATIC_ASSERT((boost::is_object<T>::value));
    BOOST_STATIC_ASSERT((!boost::is_array<T>::value));
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

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_CALL_TRAITS_HPP
