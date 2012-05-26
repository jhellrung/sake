/*******************************************************************************
 * sake/core/iterator/adaptor.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * clas iterator_adaptor< Derived, Base, Params >
 *
 * Accepted keywords:
 * - iterator_keyword::value
 * - iterator_keyword::reference
 * - iterator_keyword::difference
 * - iterator_keyword::traversal
 * - iterator_keyword::introversal
 * - iterator_keyword::compare_enable
 * - iterator_keyword::difference_enable
 * - iterator_keyword::chained_base
 * - iterator_keyword::member
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

#include <sake/core/cursor/traits.hpp>
#include <sake/core/keyword/arg_packer.hpp>
#include <sake/core/iterator/adaptor_fwd.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/private/adaptor/constructor_param_specs.hpp>
#include <sake/core/iterator/private/adaptor/converting_constructor_enable.hpp>
#include <sake/core/iterator/private/adaptor/member_base.hpp>
#include <sake/core/math/cmp.hpp>
#include <sake/core/math/sign_t.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/utility/overload.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

template<
    class Derived, class Base,
    class Params /*= boost::mpl::map0<>*/
>
class iterator_adaptor
    : public iterator_adaptor_private::member_base< Derived, Base, Params >
{
    typedef iterator_adaptor_private::member_base<
        Derived, Base, Params
    > member_base_;
public:
    SAKE_USING_TYPEDEF( typename member_base_, value_type );
    SAKE_USING_TYPEDEF( typename member_base_, reference );
    SAKE_USING_TYPEDEF( typename member_base_, difference_type );

    typedef Base base_type;
    base_type const &
    base() const
    { return protected_base(); }

protected:
    using member_base_::derived;

    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename iterator_adaptor,
        (( member_base_ ))
    )

private:
    typedef typename iterator_adaptor_private::constructor_param_specs<
        Base, Params
    >::type constructor_param_specs_;
    typedef sake::keyword::arg_packer<
        constructor_param_specs_
    > constructor_arg_packer;

    template< class T0 >
    struct explicit_constructor_enable
        : boost_ext::mpl::and2<
              boost::mpl::not_< boost_ext::is_base_of_sans_qualifiers<
                  iterator_adaptor, T0 > >,
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
    explicit iterator_adaptor(T0&& x0,
        typename explicit_constructor_enabler< T0 >::type* = 0)
        : member_base_(constructor_arg_packer()(sake::forward< T0 >(x0)))
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T0 >
    explicit iterator_adaptor(T0& x0,
        typename explicit_constructor_enabler< T0& >::type* = 0)
        : member_base_(constructor_arg_packer()(x0))
    { }

    template< class T0 >
    explicit iterator_adaptor(T0 const & x0,
        typename explicit_constructor_enabler< T0 const & >::type* = 0)
        : member_base_(constructor_arg_packer()(x0))
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_OVERLOAD_CONSTRUCTOR_NAME \
    iterator_adaptor
#define SAKE_OVERLOAD_ENABLE( r, n, T_tuple ) \
    constructor_arg_packer::template enable< BOOST_PP_TUPLE_REM_CTOR( n, T_tuple ) >
#define SAKE_OVERLOAD_INITIALIZATION_LIST( r, n, T_tuple, x_tuple, forward_x_tuple ) \
    member_base_(constructor_arg_packer() forward_x_tuple )
#define SAKE_OVERLOAD_BODY( r, n, T_tuple, x_tuple, forward_x_tuple )
#define SAKE_OVERLOAD_MIN_ARITY         2
#define SAKE_OVERLOAD_PERFECT_MAX_ARITY 3
#include SAKE_OVERLOAD_GENERATE()

private:
    template< class D, class B, class P >
    struct converting_constructor_enabler
        : iterator_adaptor_private::converting_constructor_enabler<
              Base, Params, D, B, P >
    { };
public:
    template< class D, class B, class P >
    iterator_adaptor(iterator_adaptor<D,B,P>& other,
        typename converting_constructor_enabler<D,B,P>::type* = 0)
        : member_base_(other)
    { }
    template< class D, class B, class P >
    iterator_adaptor(iterator_adaptor<D,B,P> const & other,
        typename converting_constructor_enabler< D const, B, P >::type* = 0)
        : member_base_(other)
    { }

    using member_base_::protected_base;

    friend class sake::iterator_core_access;

    // Dereferenceable
    reference
    derived_dereference() const
    { return *base(); }

    // Incrementable
    void
    derived_increment()
    { ++protected_base(); }

    // SinglePass
    template< class Other >
    bool
    derived_equal(Other const & other) const
    { return base() == other.base(); }

    // Bidirectional
    void
    derived_decrement()
    { --protected_base(); }

    // RandomAccess
    void
    derived_plus_assign(difference_type const n)
    { protected_base() += n; }

    template< class Other >
    bool
    derived_less(Other const & other) const
    { return base() < other.base(); }
    template< class Other >
    bool
    derived_less_equal(Other const & other) const
    { return base() <= other.base(); }

    template< class Other >
    sake::sign_t
    derived_cmp(Other const & other) const
    { return sake::cmp(base(), other.base()); }

    template< class Other >
    difference_type
    derived_difference(Other const & other) const
    { return base() - other.base(); }

    // BeginDetect
    bool
    derived_at_begin() const
    { return sake::cursor_traits< Base >::at_begin(base()); }
    // BeginAccess
    Derived
    derived_begin() const
    {
        Derived result(derived());
        result.base() = sake::cursor_traits< Base >::begin(base());
        return result;
    }
    // EndDetect
    bool
    derived_at_end() const
    { return sake::cursor_traits< Base >::at_end(base()); }
    // EndAccess
    Derived
    derived_end() const
    {
        Derived result(derived());
        result.base() = sake::cursor_traits< Base >::end(base());
        return result;
    }

    // RandomAccess + BeginAccess
    difference_type
    derived_difference_with_begin() const
    { return derived() - sake::begin_tag(); }
    // RandomAccess + EndAccess
    difference_type
    derived_difference_with_end() const
    { return derived() - sake::end_tag(); }
};

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ADAPTOR_HPP
