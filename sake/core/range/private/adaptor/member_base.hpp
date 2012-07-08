/*******************************************************************************
 * sake/core/range/private/adaptor/member_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_PRIVATE_ADAPTOR_MEMBER_BASE_HPP
#define SAKE_CORE_RANGE_PRIVATE_ADAPTOR_MEMBER_BASE_HPP

#include <boost/mpl/has_key.hpp>

#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>

#include <sake/core/config.hpp>
#include <sake/core/emplacer/construct.hpp>
#include <sake/core/emplacer/constructible.hpp>
#include <sake/core/keyword/arg_pack_tag.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/memberwise/swap.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/range/adaptor_fwd.hpp>
#include <sake/core/range/keyword.hpp>
#include <sake/core/range/private/adaptor/traits.hpp>
#include <sake/core/utility/compressed_pair/compressed_pair.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace range
{

namespace adaptor_private
{

template< class Params >
struct member_base_bool
    : boost::mpl::has_key<
          Params, sake::range::keyword::tag::member
      >
{ };

template<
    class Derived, class R, class Params,
    bool = member_base_bool< Params >::value
>
class member_base
SAKE_WORKAROUND_DEFINE_FRIENDED_PRIMARY_TEMPLATE_WITH_DEFAULT_PARAMETER;

template< class Derived, class R, class Params >
class member_base< Derived, R, Params, false >
    : public adaptor_private::traits< Derived, R, Params >::facade_
{
    typedef adaptor_private::traits< Derived, R, Params > traits_;
    SAKE_USING_TYPEDEF( typename traits_, facade_ );
public:
    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
        typename member_base,
        (( facade_ )) (( R )( m_base ))
    )
    SAKE_MEMBERWISE_SWAP(
        typename member_base,
        (( facade_ )) (( R )( m_base ))
    )
protected:

    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename member_base,
        (( facade_ )) (( R )( m_base ))
    )

    template< class ArgPack >
    member_base(sake::keyword::arg_pack_tag, ArgPack arg_pack)
        : facade_(
              arg_pack[sake::range::keyword::_chained_base
                     | sake::construct::emplacer()]),
          m_base(sake::emplacer_constructible<R>(
              arg_pack[sake::range::keyword::_base
                     | sake::construct::emplacer()]))
    { }

    typename boost_ext::add_reference<R>::type
    protected_base()
    { return m_base; }
    typename boost_ext::add_reference_add_const<R>::type
    protected_base() const
    { return m_base; }

private:
    R m_base;
};

template< class Derived, class R, class Params >
class member_base< Derived, R, Params, true >
    : public adaptor_private::traits< Derived, R, Params >::facade_
{
    typedef adaptor_private::traits< Derived, R, Params > traits_;
    SAKE_USING_TYPEDEF( typename traits_, facade_ );
protected:
    typedef typename boost_ext::mpl::at<
        Params, sake::range::keyword::tag::member
    >::type member_type;
private:
    typedef sake::compressed_pair< R, member_type > m_pair_type;
public:
    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
        typename member_base,
        (( facade_ )) (( m_pair_type )( m_pair ))
    )
    SAKE_MEMBERWISE_SWAP(
        typename member_base,
        (( facade_ )) (( m_pair_type )( m_pair ))
    )
protected:

    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename member_base,
        (( facade_ )) (( m_pair_type )( m_pair ))
    )

    template< class ArgPack >
    member_base(sake::keyword::arg_pack_tag, ArgPack arg_pack)
        : facade_(
              arg_pack[sake::range::keyword::_chained_base
                     | sake::construct::emplacer()]),
          m_pair(
              arg_pack[sake::range::keyword::_base
                     | sake::construct::emplacer()],
              arg_pack[sake::range::keyword::_member
                     | sake::construct::emplacer()])
    { }

    typename boost_ext::add_reference<R>::type
    protected_base()
    { return m_pair.first(); }
    typename boost_ext::add_reference_add_const<R>::type
    protected_base() const
    { return m_pair.first(); }

    typename boost_ext::add_reference< member_type >::type
    member()
    { return m_pair.second(); }
    typename boost_ext::add_reference_add_const< member_type >::type
    member() const
    { return m_pair.second(); }

private:
    m_pair_type m_pair;
};

} // namespace adaptor_private

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_PRIVATE_ADAPTOR_MEMBER_BASE_HPP
