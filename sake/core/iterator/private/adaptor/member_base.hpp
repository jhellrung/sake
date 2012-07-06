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

#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>

#include <sake/core/config.hpp>
#include <sake/core/emplacer/constructible.hpp>
#include <sake/core/emplacer/make.hpp>
#include <sake/core/iterator/keyword.hpp>
#include <sake/core/iterator/private/adaptor/member_storage.hpp>
#include <sake/core/iterator/private/adaptor/traits.hpp>
#include <sake/core/keyword/arg_pack_tag.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/memberwise/swap.hpp>
#include <sake/core/memberwise/type_trait_tag.hpp>
#include <sake/core/utility/compressed_pair/compressed_pair.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace iterator
{

namespace adaptor_private
{

template< class Params >
struct member_base_bool
    : boost::mpl::has_key<
          Params, sake::iterator::keyword::tag::member
      >
{ };

template<
    class Derived, class I, class Params,
    bool = member_base_bool< Params >::value
>
class member_base
SAKE_WORKAROUND_DEFINE_FRIENDED_PRIMARY_TEMPLATE_WITH_DEFAULT_PARAMETER;

template< class Derived, class I, class Params >
class member_base< Derived, I, Params, false >
    : public adaptor_private::traits< Derived, I, Params >::facade_
{
    typedef adaptor_private::traits< Derived, I, Params > traits_;
    SAKE_USING_TYPEDEF( typename traits_, facade_ );
public:

    SAKE_MEMBERWISE_SWAP(
        typename member_base, (( facade_ )) (( I )( m_base )) )

protected:

    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename member_base,
        (( facade_ )) (( I )( m_base ))
    )
    SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG(
        (( facade_ )) (( I )( m_base )),
        ( has_copy_constructor )
        ( has_nothrow_copy_constructor )
        ( has_nothrow_copy_assign )
    )

    template< class ArgPack >
    member_base(sake::keyword::arg_pack_tag, ArgPack arg_pack)
        : facade_(
              arg_pack[sake::iterator::keyword::_chained_base
                     | sake::make_emplacer()]),
          m_base(sake::emplacer_constructible<I>(
              arg_pack[sake::iterator::keyword::_base
                     | sake::make_emplacer()]))
    { }

    I &
    protected_base()
    { return m_base; }
    I const &
    protected_base() const
    { return m_base; }

private:
    I m_base;
};

template< class Derived, class I, class Params >
class member_base< Derived, I, Params, true >
    : public adaptor_private::traits< Derived, I, Params >::facade_
{
    typedef adaptor_private::traits< Derived, I, Params > traits_;
    SAKE_USING_TYPEDEF( typename traits_, facade_ );
public:
    SAKE_USING_TYPEDEF( typename facade_, iterator_traversal );

    SAKE_MEMBERWISE_SWAP(
        typename member_base, (( facade_ )) (( m_pair_type )( m_pair )) )

protected:
    typedef typename boost_ext::mpl::at<
        Params, sake::iterator::keyword::tag::member
    >::type member_type;
private:
    typedef adaptor_private::member_storage<
        iterator_traversal, member_type
    > member_storage_;
    typedef sake::compressed_pair<
        I, typename member_storage_::type
    > m_pair_type;
protected:

    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename member_base,
        (( facade_ )) (( m_pair_type )( m_pair ))
    )
    SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG(
        (( facade_ )) (( m_pair_type )( m_pair )),
        ( has_copy_constructor )
        ( has_nothrow_copy_constructor )
        ( has_nothrow_copy_assign )
    )

    template< class ArgPack >
    member_base(sake::keyword::arg_pack_tag, ArgPack arg_pack)
        : facade_(
              arg_pack[sake::iterator::keyword::_chained_base
                     | sake::make_emplacer()]),
          m_pair(
              arg_pack[sake::iterator::keyword::_base
                     | sake::make_emplacer()],
              arg_pack[sake::iterator::keyword::_member
                     | sake::make_emplacer()])
    { }

    I &
    protected_base()
    { return m_pair.first(); }
    I const &
    protected_base() const
    { return m_pair.first(); }

    typename boost_ext::add_reference< member_type >::type
    member()
    { return member_storage_::get(m_pair.second()); }
    typename boost_ext::add_reference_add_const< member_type >::type
    member() const
    { return member_storage_::get(m_pair.second()); }

private:
    m_pair_type m_pair;
};

} // namespace adaptor_private

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_MEMBER_BASE_HPP
