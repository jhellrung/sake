/*******************************************************************************
 * sake/core/iterator/adaptor.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * class iterator::adaptor< Derived, I, Params >
 *
 * Accepted keywords:
 * - iterator::keyword::value
 * - iterator::keyword::reference
 * - iterator::keyword::difference
 * - iterator::keyword::traversal
 * - iterator::keyword::introversal
 * - iterator::keyword::compare_enable
 * - iterator::keyword::difference_enable
 * - iterator::keyword::chained_base
 * - iterator::keyword::member
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ADAPTOR_HPP
#define SAKE_CORE_ITERATOR_ADAPTOR_HPP

#include <boost/config.hpp>
#include <boost/mpl/not.hpp>
#include <boost/preprocessor/tuple/rem.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/iterator/adaptor_fwd.hpp>
#include <sake/core/iterator/at.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/at_ip.hpp>
#include <sake/core/iterator/private/adaptor/at_helper.hpp>
#include <sake/core/iterator/private/adaptor/constructor_param_specs.hpp>
#include <sake/core/iterator/private/adaptor/convert_helper.hpp>
#include <sake/core/iterator/private/adaptor/difference_helper.hpp>
#include <sake/core/iterator/private/adaptor/equal_helper.hpp>
#include <sake/core/iterator/private/adaptor/member_base.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/keyword/arg_pack_tag.hpp>
#include <sake/core/keyword/arg_packer.hpp>
#include <sake/core/math/cmp.hpp>
#include <sake/core/math/sign_t.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/utility/overload.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace iterator
{

template<
    class Derived, class I,
    class Params /*= boost::mpl::map0<>*/
>
class adaptor
    : public adaptor_private::member_base< Derived, I, Params >
{
    typedef adaptor_private::member_base< Derived, I, Params > member_base_;
public:
    SAKE_USING_TYPEDEF( typename member_base_, value_type );
    SAKE_USING_TYPEDEF( typename member_base_, reference );
    SAKE_USING_TYPEDEF( typename member_base_, difference_type );

    typedef I base_type;
    I const &
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
        I, Params >::type constructor_param_specs_;
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

    template< class T, class Introversal >
    typename sake::iterator_relax< I, Introversal >::type
    base_at(T const & x, Introversal)
    {
        return sake::iterator::at(
            base(), adaptor_private::at_helper<I>(x), Introversal());
    }

    friend class sake::iterator::core_access;

    // Dereferenceable
    reference derived_dereference() const
    { return *base(); }

    // Incrementable
    void derived_increment()
    { ++member_base_::protected_base(); }

    // SinglePass / BeginDetect / EndDetect
    template< class T >
    bool derived_equal(T const & x) const
    { return base() == adaptor_private::equal_helper<I>(x); }

    // Bidirectional
    void derived_decrement()
    { --member_base_::protected_base(); }

    // RandomAccess
    void derived_advance_ip(difference_type const n)
    { member_base_::protected_base() += n; }

    template< class Other >
    bool derived_less(Other const & other) const
    { return base() < adaptor_private::convert_helper<I>(other); }
    template< class Other >
    bool derived_less_equal(Other const & other) const
    { return base() <= adaptor_private::convert_helper<I>(other); }

    template< class Other >
    sake::sign_t derived_cmp(Other const & other) const
    { return sake::cmp(base(), adaptor_private::convert_helper<I>(other)); }

    // RandomAccess / +BeginAccess / +EndAccess
    template< class T >
    difference_type derived_difference(T const & x) const
    { return base() - adaptor_private::difference_helper<I>(x); }

    // Introversal / BeginAccess / EndAccess
    template< class T >
    void derived_at_ip(T const & x)
    {
        sake::iterator::at_ip(
            member_base_::protected_base(),
            adaptor_private::at_helper<I>(x)
        );
    }
};

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ADAPTOR_HPP
