/*******************************************************************************
 * sake/core/iterator/private/adaptor/member_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_MEMBER_BASE_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_MEMBER_BASE_HPP

#include <boost/iterator/iterator_categories.hpp>
#include <boost/mpl/not.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/at.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/data_structures/optional/optional.hpp>
#include <sake/core/emplacer/emplacer_constructible.hpp>
#include <sake/core/introspection/has_operator_assign.hpp>
#include <sake/core/iterator/keyword.hpp>
#include <sake/core/iterator/private/adaptor/traits.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/type_traits/has_default_constructor.hpp>
#include <sake/core/utility/compressed_pair/compressed_pair.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace iterator_adaptor_private
{

template< class T >
struct member_base_index_dispatch
{
    static int const value = 2 - boost_ext::mpl::and2<
        boost_ext::mpl::or2<
            boost::mpl::not_< boost_ext::is_convertible<
                Traversal, boost::forward_traversal_tag > >,
            sake::has_default_constructor<T>
        >,
        sake::has_operator_assign< T&, void ( T const & ) >
    >::value;
};

template<>
struct member_base_index_dispatch< void >
{ static int const value = 0; };

template< class T >
struct member_base_index_dispatch< T& >
{ static int const value = 2; };

template< class Params >
struct member_base_index
    : member_base_index_dispatch<
          typename boost_ext::mpl::at<
              Params, sake::iterator_keyword::tag::member,
              void
          >::type
      >
{ };

template< class Params >
struct member_base_index< Params, void >
{ static int const value = 0; };

template< class Params, class T >
struct member_base_index< Params, T& >
{ static int const value = 2; };

template<
    class Derived, class Base, class Params,
    int = member_base_index< Params >::value
>
class member_base;

template< class Derived, class Base, class Params >
class member_base< Derived, Base, Params, 0 >
    : public iterator_adaptor_private::traits<
          Derived, Base, Params >::iterator_facade_
{
    typedef iterator_adaptor_private::traits< Derived, Base, Params > traits_;
    SAKE_USING_TYPEDEF( typename traits_, iterator_facade_ );
protected:

    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename member_base,
        (( iterator_facade_ )) (( Base )( m_base ))
    )

    template< class ArgPack >
    explicit member_base(ArgPack arg_pack,
        typename boost::disable_if_c<
            boost::is_base_of_sans_qualifiers< member_base, ArgPack >::value
        >::type* = 0)
        : iterator_facade_(
              arg_pack[sake::iterator_keyword::_chained_base
                     | sake::emplacer< void ( ) >()]),
          m_base(sake::emplacer_constructible< Base >(
              arg_pack[sake::iterator_keyword::_base
                     | sake::emplacer< void ( ) >()]))
    { }

    Base &
    protected_base()
    { return m_base; }
    Base const &
    protected_base() const
    { return m_base; }

private:
    Base m_base;
};

template< class Derived, class Base, class Params >
class member_base< Derived, Base, Params, 1 >
    : public iterator_adaptor_private::traits<
          Derived, Base, Params >::iterator_facade_
{
    typedef iterator_adaptor_private::traits< Derived, Base, Params > traits_;
    SAKE_USING_TYPEDEF( typename traits_, iterator_facade_ );
protected:
    typedef typename boost_ext::mpl::at<
        Params, sake::iterator_keyword::tag::member
    >::type member_type;
private:
    typedef sake::compressed_pair< Base, member_type > m_members_type;
protected:

    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename member_base,
        (( iterator_facade_ )) (( m_members_type )( m_members ))
    )

    template< class ArgPack >
    explicit member_base(ArgPack arg_pack,
        typename boost::disable_if_c<
            boost::is_base_of_sans_qualifiers< member_base, ArgPack >::value
        >::type* = 0)
        : iterator_facade_(
              arg_pack[sake::iterator_keyword::_chained_base
                     | sake::emplacer< void ( ) >()]),
          m_members(
              arg_pack[sake::iterator_keyword::_base
                     | sake::emplacer< void ( ) >()],
              arg_pack[sake::iteraotr_keyword::_member
                     | sake::emplacer< void ( ) >()])
    { }

    Base &
    protected_base()
    { return m_members.first(); }
    Base const &
    protected_base() const
    { return m_members.first(); }

    typename boost_ext::add_reference< member_type >::type
    member()
    { return m_members.second(); }
    typename boost_ext::add_reference_add_const< member_type >::type
    member()
    { return m_members.second(); }

private:
    m_members_type m_members;
};

template< class Derived, class Base, class Params >
class member_base< Derived, Base, Params, 2 >
    : public iterator_adaptor_private::traits<
          Derived, Base, Params >::iterator_facade_
{
    typedef iterator_adaptor_private::traits< Derived, Base, Params > traits_;
    SAKE_USING_TYPEDEF( typename traits_, iterator_facade_ );
protected:
    typedef typename boost_ext::mpl::at<
        Params, sake::iterator_keyword::tag::member
    >::type member_type;
private:
    typedef sake::compressed_pair<
        Base, sake::optional< member_type >
    > m_members_type;
protected:

    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename member_base,
        (( iterator_facade_ )) (( m_members_type )( m_members ))
    )

    template< class ArgPack >
    explicit member_base(ArgPack arg_pack,
        typename boost::disable_if_c<
            boost::is_base_of_sans_qualifiers< member_base, ArgPack >::value
        >::type* = 0)
        : iterator_facade_(
              arg_pack[sake::iterator_keyword::_chained_base
                     | sake::emplacer< void ( ) >()]),
          m_members(
              arg_pack[sake::iterator_keyword::_base
                     | sake::emplacer< void ( ) >()],
              arg_pack[sake::iteraotr_keyword::_member
                     | sake::emplacer< void ( ) >()])
    { }

    Base &
    protected_base()
    { return m_members.first(); }
    Base const &
    protected_base() const
    { return m_members.first(); }

    typename boost_ext::add_reference< member_type >::type
    member()
    { return m_members.second().get(); }
    typename boost_ext::add_reference_add_const< member_type >::type
    member()
    { return m_members.second().get(); }

private:
    m_members_type m_members;
};

} // namespace iterator_adaptor_private

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_MEMBER_BASE_HPP
