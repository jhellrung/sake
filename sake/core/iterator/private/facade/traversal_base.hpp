/*******************************************************************************
 * sake/core/iterator/private/facade/traversal_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_TRAVERSAL_BASE_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_FACADE_TRAVERSAL_BASE_HPP

#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/core_access.hpp>
#include <sake/core/iterator/facade_fwd.hpp>
#include <sake/core/iterator/private/facade/begin_introterminal_base.hpp>
#include <sake/core/iterator/private/facade/post_increment_dispatch.hpp>
#include <sake/core/iterator/private/facade/subscript_dispatch.hpp>
#include <sake/core/iterator/private/facade/traits.hpp>
#include <sake/core/math/cmp.hpp>
#include <sake/core/math/sign_t.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/memberwise/copy_tags.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/memberwise/destructor_tags.hpp>
#include <sake/core/memberwise/swap.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace iterator
{

namespace facade_adl
{

namespace private_
{

template< class Params >
struct traversal_base_index
{
    SAKE_USING_TYPEDEF( typename private_::traits< Params >, iterator_traversal );
    static int const value =
        boost_ext::is_convertible<
            iterator_traversal, boost::random_access_traversal_tag >::value
      + boost_ext::is_convertible<
            iterator_traversal, boost::bidirectional_traversal_tag >::value;
};

template< class Derived, class Params >
class traversal_base< Derived, Params, 0 >
    : public private_::begin_introterminal_base< Derived, Params >
{
    typedef private_::begin_introterminal_base<
        Derived, Params > begin_introterminal_base_;
protected:
    using begin_introterminal_base_::derived;
public:

    SAKE_MEMBERWISE_SWAP(
        typename traversal_base, (( begin_introterminal_base_ )) )

    Derived&
    operator++()
    {
        sake::iterator::core_access::increment(derived());
        return derived();
    }

private:
    typedef private_::traits< Params > traits_;
    typedef private_::post_increment_dispatch<
        Derived,
        typename traits_::facade_value_type,
        typename traits_::reference,
        typename traits_::iterator_traversal
    > post_increment_dispatch_;
public:
    typename post_increment_dispatch_::type
    operator++(int)
    {
        typename post_increment_dispatch_::type result =
            post_increment_dispatch_::apply(derived());
        sake::iterator::core_access::increment(derived());
        return result;
    }
    

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename traversal_base,
        (( begin_introterminal_base_ ))
    )
    SAKE_MEMBERWISE_COPY_TAGS( (( begin_introterminal_base_ )) )
    SAKE_MEMBERWISE_DESTRUCTOR_TAGS( (( begin_introterminal_base_ )) )

    template< class T >
    explicit traversal_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            traversal_base, T >::value >::type* = 0)
        : begin_introterminal_base_(sake::forward<T>(x))
    { }
};

template< class Derived, class Params >
class traversal_base< Derived, Params, 1 >
    : public private_::traversal_base< Derived, Params, 0 >
{
    typedef private_::traversal_base<
        Derived, Params, 0 > traversal_base_;
protected:
    using traversal_base_::derived;
public:

    SAKE_MEMBERWISE_SWAP( typename traversal_base, (( traversal_base_ )) )

    Derived&
    operator--()
    {
        sake::iterator::core_access::decrement(derived());
        return derived();
    }

    Derived
    operator--(int)
    {
        Derived result(derived());
        sake::iterator::core_access::decrement(derived());
        return result;
    }

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename traversal_base,
        (( traversal_base_ ))
    )
    SAKE_MEMBERWISE_COPY_TAGS( (( traversal_base_ )) )
    SAKE_MEMBERWISE_DESTRUCTOR_TAGS( (( traversal_base_ )) )

    template< class T >
    explicit traversal_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            traversal_base, T >::value >::type* = 0)
        : traversal_base_(sake::forward<T>(x))
    { }
};

template< class Derived, class Params >
class traversal_base< Derived, Params, 2 >
    : public private_::traversal_base< Derived, Params, 1 >
{
    typedef private_::traversal_base<
        Derived, Params, 1 > traversal_base_;
protected:
    using traversal_base_::derived;
private:
    typedef private_::traits< Params > traits_;
public:
    SAKE_USING_TYPEDEF( typename traits_, difference_type );

    SAKE_MEMBERWISE_SWAP( typename traversal_base, (( traversal_base_ )) )

private:
    typedef private_::subscript_dispatch<
        Derived,
        typename traits_::facade_value_type,
        typename traits_::reference
    > subscript_dispatch_;
public:

    typename subscript_dispatch_::type
    operator[](difference_type const n) const
    { return subscript_dispatch_::apply(derived() + n); }

    Derived&
    operator+=(difference_type const n)
    {
        sake::iterator::core_access::advance_ip(derived(), n);
        return derived();
    }

    Derived&
    operator-=(difference_type const n)
    { return derived() += (-n); }

    Derived
    operator+(difference_type const n) const
    { return sake::iterator::core_access::advance(derived(), n); }

    Derived
    operator-(difference_type const n) const
    { return derived() + (-n); }

    template< class Derived_ >
    inline friend typename boost::enable_if_c<
        boost::is_same< Derived_, Derived >::value, Derived >::type
    operator+(difference_type const n, Derived_ const & this_)
    { return this_ + n; }

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename traversal_base,
        (( traversal_base_ ))
    )
    SAKE_MEMBERWISE_COPY_TAGS( (( traversal_base_ )) )
    SAKE_MEMBERWISE_DESTRUCTOR_TAGS( (( traversal_base_ )) )

    template< class T >
    explicit traversal_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            traversal_base, T >::value >::type* = 0)
        : traversal_base_(sake::forward<T>(x))
    { }

    friend class sake::iterator::core_access;

    Derived derived_advance(difference_type const n) const
    {
        Derived result(derived());
        return result += n;
    }
};

} // namespace private_

} // namespace facade_adl

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_TRAVERSAL_BASE_HPP
