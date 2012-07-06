/*******************************************************************************
 * sake/core/range/adaptor.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * class range::adaptor< Derived, R, Params >
 *
 * Accepted keywords:
 * - range::keyword::iterator
 * - range::keyword::const_iterator
 * - range::keyword::size_enable
 * - range::keyword::chained_base
 * - range::keyword::member
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_ADAPTOR_HPP
#define SAKE_CORE_RANGE_ADAPTOR_HPP

#include <boost/config.hpp>
#include <boost/mpl/not.hpp>
#include <boost/preprocessor/tuple/rem.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/propagate_const.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/keyword/arg_pack_tag.hpp>
#include <sake/core/keyword/arg_packer.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/memberwise/swap.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/range/adaptor_fwd.hpp>
#include <sake/core/range/is_adapt_by_value.hpp>
#include <sake/core/range/has_value_ownership.hpp>
#include <sake/core/range/private/adaptor/base_iter_helper.hpp>
#include <sake/core/range/private/adaptor/constructor_param_specs.hpp>
#include <sake/core/range/private/adaptor/member_base.hpp>
#include <sake/core/range/static_size.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/range/traits_fwd.hpp>
#include <sake/core/utility/overload.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace range
{

template<
    class Derived, class R,
    class Params /*= boost::mpl::map0<>*/
>
class adaptor
    : public adaptor_private::member_base< Derived, R, Params >
{
    typedef adaptor_private::member_base< Derived, R, Params > member_base_;
public:
    SAKE_USING_TYPEDEF( typename member_base_, difference_type );
    SAKE_USING_TYPEDEF( typename member_base_, size_type );

    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
        typename adaptor,
        (( member_base_ ))
    )
    SAKE_MEMBERWISE_SWAP(
        typename adaptor,
        (( member_base_ ))
    )

    typedef typename sake::range_is_adapt_by_value<R>::type is_adapt_by_value_tag;
    typedef typename sake::range_has_value_ownership<R>::type has_value_ownership_tag;
    typedef typename sake::range_static_size<
        typename boost_ext::remove_qualifiers<R>::type
    >::type static_size_tag;

    typedef R base_type;
    typename boost_ext::add_reference_add_const<R>::type
    base() const
    { return member_base_::protected_base(); }

protected:
    using member_base_::derived;

    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename adaptor,
        (( member_base_ ))
    )

private:
    typedef typename adaptor_private::constructor_param_specs<
        R, Params >::type constructor_param_specs_;
    typedef sake::keyword::arg_packer<
        constructor_param_specs_ > constructor_arg_packer;

    template< class T0 >
    struct explicit_constructor_enable
        : boost_ext::mpl::and2<
              boost::mpl::not_< boost_ext::is_base_of_sans_qualifiers<
                  adaptor, T0 > >,
              typename constructor_arg_packer::template enable< T0 >
          >
    { };
    template< class T0 >
    struct explicit_constructor_enabler
        : boost::enable_if_c< explicit_constructor_enable<
              typename boost_ext::remove_rvalue_reference< T0 >::type
          >::value >
    { };
protected:

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T0 >
    explicit adaptor(T0&& x0,
        typename explicit_constructor_enabler< T0 >::type* = 0)
        : member_base_(
              sake::keyword::arg_pack_tag(),
              constructor_arg_packer()(sake::forward< T0 >(x0)))
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T0 >
    explicit adaptor(T0& x0,
        typename explicit_constructor_enabler< T0& >::type* = 0)
        : member_base_(
              sake::keyword::arg_pack_tag(),
              constructor_arg_packer()(x0))
    { }

    template< class T0 >
    explicit adaptor(T0 const & x0,
        typename explicit_constructor_enabler< T0 const & >::type* = 0)
        : member_base_(
              sake::keyword::arg_pack_tag(),
              constructor_arg_packer()(x0))
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_OVERLOAD_CONSTRUCTOR_NAME \
    adaptor
#define SAKE_OVERLOAD_ENABLE( r, n, T_tuple ) \
    constructor_arg_packer::template enable< BOOST_PP_TUPLE_REM_CTOR( n, T_tuple ) >
#define SAKE_OVERLOAD_INITIALIZATION_LIST( \
    r, n, T_tuple, x_tuple, forward_x_tuple ) \
    member_base_( \
        sake::keyword::arg_pack_tag(), \
        constructor_arg_packer() forward_x_tuple)
#define SAKE_OVERLOAD_BODY( r, n, T_tuple, x_tuple, forward_x_tuple )
#define SAKE_OVERLOAD_MIN_ARITY         2
#define SAKE_OVERLOAD_PERFECT_MAX_ARITY 3
#include SAKE_OVERLOAD_GENERATE()

    template< class CDerived >
    struct base_traits_of
    {
        typedef sake::range_traits<
            typename boost_ext::remove_reference<
                typename boost_ext::propagate_const< CDerived, R >::type
            >::type
        > type;
    };

    template< class CDerived, class Introversal >
    struct base_iterator_with_of
    {
        typedef typename base_traits_of< CDerived >::type::template
            iterator_with< Introversal >::type type;
    };

    template< class CDerived, class T, class Introversal >
    static typename base_iterator_with_of< CDerived, Introversal >::type
    base_iter_at(CDerived& this_, T const & x, Introversal)
    {
        typedef typename boost_ext::remove_reference<
            typename boost_ext::propagate_const< CDerived, R >::type
        >::type cbase_type;
        return base_traits_of< CDerived >::type::iter_at(
            this_.protected_base(),
            adaptor_private::base_iter_helper< cbase_type >(x),
            Introversal()
        );
    }

    template< class CDerived, class Begin, class End >
    struct base_subrange_with_of
    { typedef typename base_traits_of< CDerived >::type::template
        subrange_with< void, void >::type type; };
    template< class CDerived, class End >
    struct base_subrange_with_of< CDerived, sake::begin_tag, End >
    { typedef typename base_traits_of< CDerived >::type::template
        subrange_with< sake::begin_tag, void >::type type; };
    template< class CDerived, class Begin >
    struct base_subrange_with_of< CDerived, Begin, sake::end_tag >
    { typedef typename base_traits_of< CDerived >::type::template
        subrange_with< void, sake::end_tag >::type type; };
    template< class CDerived >
    struct base_subrange_with_of< CDerived, sake::begin_tag, sake::end_tag >
    { typedef typename base_traits_of< CDerived >::type::template
        subrange_with< sake::begin_tag, sake::end_tag >::type type; };

    template< class CDerived, class Begin, class End >
    static typename base_subrange_with_of< CDerived, Begin, End >::type
    base_sub(CDerived& this_, Begin const & b, End const & e)
    {
        typedef typename boost_ext::remove_reference<
            typename boost_ext::propagate_const< CDerived, R >::type
        >::type cbase_type;
        return base_traits_of< CDerived >::type::sub(
            this_.protected_base(),
            adaptor_private::base_iter_helper< cbase_type >(b),
            adaptor_private::base_iter_helper< cbase_type >(e)
        );
    }

    template< class CDerived >
    struct base_reference_of
    { typedef typename base_traits_of< CDerived >::type::reference type; };

    template< class CDerived, class T >
    static typename base_reference_of< CDerived >::type
    base_at(CDerived& this_, T const x)
    { return base_traits_of< CDerived >::type::at(this_.protected_base(), x); }

    bool base_empty() const
    { return base_traits_of< Derived const >::type::empty(base()); }
    difference_type base_distance() const
    { return base_traits_of< Derived const >::type::distance(base()); }
    size_type base_size() const
    { return base_traits_of< Derived const >::type::size(base()); }

    friend class sake::range::core_access;
};

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_ADAPTOR_HPP
