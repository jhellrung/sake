/*******************************************************************************
 * sake/core/utility/base_member.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
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

#include <boost/preprocessor/seq/seq.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_empty.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/move/forward.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/utility/define_natural/mem_fun.hpp>
#include <sake/core/utility/emplacer_fwd.hpp>

namespace sake
{

template< class T, class Tag = void, class Enable = void >
struct base_member;

template< class T, class Tag >
struct base_member< T, Tag,
    typename boost::disable_if_c< boost::is_empty<T>::value >::type >
{
    SAKE_BASIC_MOVABLE_COPYABLE( base_member )

protected:
    friend class emplacer_access;

    SAKE_DEFINE_NATURAL_MEM_FUN(
        typename base_member,
        ( default_ctor ) ( move_ctor )
        ( copy_assign_if_any_umc ) ( move_assign )
        ( swap ),
        BOOST_PP_SEQ_NIL, (( T, m_member ))
    )

    template< class U >
    explicit base_member(SAKE_FWD2_REF( U ) x,
        typename boost::disable_if_c< boost::is_base_of<
            base_member,
            typename boost_ext::remove_qualifiers<U>::type
        >::value >::type* = 0)
        : m_member(sake::emplacer_construct<T>(sake::forward<U>(x)))
    { }

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
    SAKE_BASIC_MOVABLE_COPYABLE( base_member )

protected:
    friend class emplacer_access;

    SAKE_DEFINE_NATURAL_MEM_FUN(
        typename base_member,
        ( default_ctor ) ( move_ctor )
        ( copy_assign_if_any_umc ) ( move_assign )
        ( swap ),
        ( T ), BOOST_PP_SEQ_NIL
    )

    template< class U >
    explicit base_member(SAKE_FWD2_REF( U ) x,
        typename boost::disable_if_c< boost::is_base_of<
            base_member,
            typename boost_ext::remove_qualifiers<U>::type
        >::value >::type* = 0)
        : T(sake::emplacer_construct<T>(sake::forward<U>(x)))
    { }

    typename boost_ext::add_reference<T>::type
    member()
    { return *this; }
    typename boost_ext::add_reference_add_const<T>::type
    member() const
    { return *this; }

private:
    // Disable Boost.ResultOf's discovery of a (possible) result_type typedef.
    // This allows the derived class to define a result struct without worrying
    // about T's result_type typedef overriding it.
    struct result_type_disabler;
    template< result_type_disabler* > struct result_type;
};

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_BASE_MEMBER_HPP
