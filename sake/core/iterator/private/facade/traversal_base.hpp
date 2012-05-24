/*******************************************************************************
 * sake/core/iterator/private/facade/traversal_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_TRAVERSAL_BASE_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_FACADE_TRAVERSAL_BASE_HPP

#include <boost/iterator/iterator_categories.hpp>
#include <boost/static_assert.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/iterator/core_access.hpp>
#include <sake/core/iterator/facade_fwd.hpp>
#include <sake/core/iterator/private/facade/begin_introversal_base.hpp>
#include <sake/core/iterator/private/facade/operator_bracket_dispatch.hpp>
#include <sake/core/iterator/private/facade/operator_post_increment_dispatch.hpp>
#include <sake/core/iterator/private/facade/traits.hpp>
#include <sake/core/math/cmp.hpp>
#include <sake/core/math/sign_t.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace iterator_facade_adl
{

namespace private_
{

template< class Params >
struct traversal_base_index
{
    SAKE_USING_TYPEDEF( typename private_::traits< Params >, iterator_traversal );
    static int const value =
        boost_ext::is_convertible< iterator_traversal, boost::random_access_traversal_tag >::value
      + boost_ext::is_convertible< iterator_traversal, boost::bidirectional_traversal_tag >::value;
};

template< class Derived, class Params >
class traversal_base< Derived, Params, 0 >
    : public private_::begin_introversal_base< Derived, Params >
{
    typedef private_::begin_introversal_base<
        Derived, Params
    > begin_introversal_base_;
protected:
    using begin_introversal_base_::derived;
public:

    Derived&
    operator++()
    {
        sake::iterator_core_access::operator_pre_increment(derived());
        return derived();
    }

private:
    typedef private_::traits< Params > traits_;
    typedef private_::operator_post_increment_dispatch<
        Derived,
        typename traits_::facade_value_type,
        typename traits_::reference,
        typename traits_::iterator_traversal
    > operator_post_increment_dispatch_;
public:
    typename operator_post_increment_dispatch_::type
    operator++(int)
    {
        typename operator_post_increment_dispatch_::type result(derived());
        sake::iterator_core_access::operator_pre_decrement(derived());
        return result;
    }
    

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename traversal_base,
        (( begin_introversal_base_ ))
    )

    template< class T >
    explicit traversal_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c<
            boost_ext::is_base_of_sans_qualifiers< traversal_base, T >::value
        >::type* = 0)
        : begin_introversal_base_(sake::forward<T>(x))
    { }
};

template< class Derived, class Params >
class traversal_base< Derived, Params, 1 >
    : public private_::traversal_base< Derived, Params, 0 >
{
    typedef private_::traversal_base<
        Derived, Params, 0
    > traversal_base_;
protected:
    using traversal_base_::derived;
public:

    Derived&
    operator--()
    {
        sake::iterator_core_access::operator_pre_decrement(derived());
        return derived();
    }

    Derived
    operator--(int)
    {
        Derived result(derived());
        sake::iterator_core_access::operator_pre_decrement(derived());
        return result;
    }

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename traversal_base,
        (( traversal_base_ ))
    )
    BOOST_STATIC_ASSERT((has_default_constructor_tag::value));

    template< class T >
    explicit traversal_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c<
            boost_ext::is_base_of_sans_qualifiers< traversal_base, T >::value
        >::type* = 0)
        : traversal_base_(boost_ext::forward<T>(x))
    { }
};

template< class Derived, class Params >
class traversal_base< Derived, Params, 2 >
    : public private_::traversal_base< Derived, Params, 1 >
{
    typedef private_::traversal_base<
        Derived, Params, 1
    > traversal_base_;
protected:
    using traversal_base_::derived;
private:
    typedef private_::traits< Params > traits_;
public:
    SAKE_USING_TYPEDEF( typename traits_, difference_type );
private:
    typedef private_::operator_bracket_dispatch<
        Derived,
        typename traits_::facade_value_type,
        typename traits_::reference
    > operator_bracket_dispatch_;
public:

    typename operator_bracket_dispatch_::type
    operator[](difference_type const n) const
    { return operator_bracket_dispatch_::apply(derived() + n); }

    Derived&
    operator+=(difference_type const n)
    {
        sake::iterator_core_access::operator_plus_equal(derived(), n);
        return derived();
    }

    Derived&
    operator-=(difference_type const n)
    { return derived() += -n; }

    Derived
    operator+(difference_type const n) const
    { return sake::iterator_core_access::operator_plus(derived(), n); }
    inline friend Derived
    operator+(difference_type const n, Derived const & this_)
    { return this_ + n; }

    Derived
    operator-(difference_type const n) const
    { return derived() + (-n); }

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename traversal_base,
        (( traversal_base_ ))
    )
    BOOST_STATIC_ASSERT((has_default_constructor_tag::value));

    template< class T >
    explicit traversal_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c<
            boost_ext::is_base_of_sans_qualifiers< traversal_base, T >::value
        >::type* = 0)
        : traversal_base_(boost_ext::forward<T>(x))
    { }

    friend class sake::iterator_core_access;

    Derived
    operator_plus_impl(difference_type const n) const
    {
        Derived result(derived());
        return result += n;
    }
};

} // namespace private_

} // namespace iterator_facade_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_TRAVERSAL_BASE_HPP
