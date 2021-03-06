/*******************************************************************************
 * sake/core/utility/base_member.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct base_member< T, Tag = void, Enable = void >
 *
 * Inherit from base_member if you want the empty-base optimization (EBO) for a
 * potentially empty lone data member.  base_member<T> inherits from T if
 * boost::is_empty<T> is true, otherwise it stores a data member of type T.
 *
 * Access of the underlying T data member is through the member() protected
 * member function.
 *
 * The Tag type template parameter can be used to differentiate between possibly
 * identical instantiations of base_member.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_BASE_MEMBER_HPP
#define SAKE_CORE_UTILITY_BASE_MEMBER_HPP

#include <boost/config.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_empty.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>

#include <sake/core/emplacer/constructible.hpp>
#include <sake/core/emplacer/emplacer.hpp>
#include <sake/core/emplacer/fwd.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/memberwise/mem_fun.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/movable.hpp>

namespace sake
{

namespace base_member_adl
{

template< class T, class Tag = void, class Enable = void >
struct base_member;

} // namespace base_member_adl

using base_member_adl::base_member;

namespace base_member_adl
{

template< class T, class Tag >
struct base_member< T, Tag,
    typename boost::disable_if_c< boost::is_empty<T>::value >::type >
{
    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
        typename base_member,
        (( T )( m_member ))
    )

    SAKE_MEMBERWISE_MEM_FUN(
        typename base_member,
        ( swap ) ( hash_value ),
        (( T )( m_member ))
    )
protected:
    friend class emplacer_access;

    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename base_member,
        (( T )( m_member ))
    )

private:
    typedef typename boost::remove_cv<T>::type nocv_type;
protected:

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    explicit base_member(U&& x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            base_member, U >::value >::type* = 0)
        : m_member(sake::emplacer_constructible< nocv_type >(sake::forward<U>(x)))
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    explicit base_member(U& x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            base_member, U >::value >::type* = 0)
        : m_member(sake::emplacer_constructible< nocv_type >(x))
    { }

    template< class U >
    explicit base_member(U const & x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            base_member, U >::value >::type* = 0)
        : m_member(sake::emplacer_constructible< nocv_type >(x))
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class V >
    explicit base_member(sake::emplacer< V ( ) >)
    {
        BOOST_STATIC_ASSERT((
            boost::is_void<V>::value
         || boost::is_same< V, nocv_type >::value
        ));
    }

    typename boost_ext::add_reference<T>::type
    member()
    { return m_member; }
    typename boost_ext::add_reference_add_const<T>::type
    member() const
    { return m_member; }

private:
    T m_member;
};

template< class T, class Tag >
struct base_member< T, Tag,
    typename boost::enable_if_c< boost::is_empty<T>::value >::type >
    : T
{
    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
        typename base_member,
        (( T ))
    )

    SAKE_MEMBERWISE_MEM_FUN(
        typename base_member,
        ( swap ) ( hash_value ),
        (( T ))
    )
protected:
    friend class emplacer_access;

    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename base_member,
        (( T ))
    )

private:
    typedef typename boost::remove_cv<T>::type nocv_type;
protected:

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    explicit base_member(U&& x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            base_member, U >::value >::type* = 0)
        : T(sake::emplacer_constructible< nocv_type >(sake::forward<U>(x)))
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    explicit base_member(U& x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            base_member, U >::value >::type* = 0)
        : T(sake::emplacer_constructible< nocv_type >(x))
    { }

    template< class U >
    explicit base_member(U const & x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            base_member, U >::value >::type* = 0)
        : T(sake::emplacer_constructible< nocv_type >(x))
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class V >
    explicit base_member(sake::emplacer< V ( ) >)
    {
        BOOST_STATIC_ASSERT((
            boost::is_void<V>::value
         || boost::is_same< V, nocv_type >::value
        ));
    }

    typename boost_ext::add_reference<T>::type
    member()
    { return *this; }
    typename boost_ext::add_reference_add_const<T>::type
    member() const
    { return *this; }

private:
    // Disable Boost.ResultOf's discovery of a (possible) result_type typedef.
    // This allows the derived class to define a result struct without worrying
    // about T's result_type typedef implicitly overriding it.
    struct result_type_disabler;
    template< result_type_disabler* > struct result_type;
};

} // namespace base_member_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_BASE_MEMBER_HPP
