/*******************************************************************************
 * sake/core/iterator/private/adaptor/member_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_MEMBER_BASE_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_MEMBER_BASE_HPP

#include <boost/mpl/has_key.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>

#include <sake/core/config.hpp>
#include <sake/core/emplacer/constructible.hpp>
#include <sake/core/emplacer/make.hpp>
#include <sake/core/iterator/adaptor_fwd.hpp>
#include <sake/core/iterator/keyword.hpp>
#include <sake/core/iterator/private/adaptor/chained_base_constructible.hpp>
#include <sake/core/iterator/private/adaptor/member_storage.hpp>
#include <sake/core/iterator/private/adaptor/traits.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/utility/compressed_pair/compressed_pair.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace iterator_adaptor_private
{

template< class Params >
struct member_base_bool
    : boost::mpl::has_key<
          Params, sake::iterator_keyword::tag::member
      >
{ };

template<
    class Derived, class Base, class Params,
    bool = member_base_bool< Params >::value
>
class member_base
SAKE_WORKAROUND_DEFINE_FRIENDED_PRIMARY_TEMPLATE_WITH_DEFAULT_PARAMETER;

template< class Derived, class Base, class Params >
class member_base< Derived, Base, Params, false >
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
            boost_ext::is_base_of_sans_qualifiers< member_base, ArgPack >::value
        >::type* = 0)
        : iterator_facade_(
              arg_pack[sake::iterator_keyword::_chained_base | sake::make_emplacer()]),
          m_base(sake::emplacer_constructible< Base >(
              arg_pack[sake::iterator_keyword::_base | sake::make_emplacer()]))
    { }

    template< class D, class B, class P >
    explicit member_base(sake::iterator_adaptor<D,B,P>& other,
        typename boost::disable_if_c<
            boost::is_base_of< member_base, sake::iterator_adaptor<D,B,P> >::value
        >::type* = 0)
        : iterator_facade_(iterator_adaptor_private::chained_base_constructible(other)),
          m_base(other.base())
    { }

    template< class D, class B, class P >
    explicit member_base(sake::iterator_adaptor<D,B,P> const & other,
        typename boost::disable_if_c<
            boost::is_base_of< member_base, sake::iterator_adaptor<D,B,P> >::value
        >::type* = 0)
        : iterator_facade_(iterator_adaptor_private::chained_base_constructible(other)),
          m_base(other.base())
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
class member_base< Derived, Base, Params, true >
    : public iterator_adaptor_private::traits<
          Derived, Base, Params >::iterator_facade_
{
    typedef iterator_adaptor_private::traits< Derived, Base, Params > traits_;
    SAKE_USING_TYPEDEF( typename traits_, iterator_facade_ );
public:
    SAKE_USING_TYPEDEF( typename iterator_facade_, iterator_traversal );
protected:
    typedef typename boost_ext::mpl::at<
        Params, sake::iterator_keyword::tag::member
    >::type member_type;
private:
    typedef iterator_adaptor_private::member_storage<
        iterator_traversal, member_type
    > member_storage_;
    typedef sake::compressed_pair<
        Base, typename member_storage_::type
    > m_base_and_member_type;
protected:

    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename member_base,
        (( iterator_facade_ )) (( m_base_and_member_type )( m_base_and_member ))
    )

    template< class ArgPack >
    explicit member_base(ArgPack arg_pack,
        typename boost::disable_if_c<
            boost_ext::is_base_of_sans_qualifiers< member_base, ArgPack >::value
        >::type* = 0)
        : iterator_facade_(
              arg_pack[sake::iterator_keyword::_chained_base | sake::make_emplacer()]),
          m_base_and_member(
              arg_pack[sake::iterator_keyword::_base | sake::make_emplacer()],
              arg_pack[sake::iterator_keyword::_member | sake::make_emplacer()])
    { }

    template< class D, class B, class P >
    explicit member_base(sake::iterator_adaptor<D,B,P>& other,
        typename boost::disable_if_c<
            boost::is_base_of< member_base, sake::iterator_adaptor<D,B,P> >::value
        >::type* = 0)
        : iterator_facade_(iterator_adaptor_private::chained_base_constructible(other)),
          m_base_and_member(other.base(), other.member())
    { }

    template< class D, class B, class P >
    explicit member_base(sake::iterator_adaptor<D,B,P> const & other,
        typename boost::disable_if_c<
            boost::is_base_of< member_base, sake::iterator_adaptor<D,B,P> >::value
        >::type* = 0)
        : iterator_facade_(iterator_adaptor_private::chained_base_constructible(other)),
          m_base_and_member(other.base(), other.member())
    { }

    Base &
    protected_base()
    { return m_base_and_member.first(); }
    Base const &
    protected_base() const
    { return m_base_and_member.first(); }

    template< class, class, class, bool >
    friend class sake::iterator_adaptor_private::member_base;

    typename boost_ext::add_reference< member_type >::type
    member()
    { return member_storage_::get(m_base_and_member.second()); }
    typename boost_ext::add_reference_add_const< member_type >::type
    member()
    { return member_storage_::get(m_base_and_member.second()); }

private:
    m_base_and_member_type m_base_and_member;
};

} // namespace iterator_adaptor_private

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_MEMBER_BASE_HPP
