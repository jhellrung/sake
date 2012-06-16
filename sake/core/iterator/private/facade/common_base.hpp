/*******************************************************************************
 * sake/core/iterator/private/facade/common_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_COMMON_BASE_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_FACADE_COMMON_BASE_HPP

#include <boost/static_assert.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/core_access.hpp>
#include <sake/core/iterator/facade_fwd.hpp>
#include <sake/core/iterator/private/facade/chained_base.hpp>
#include <sake/core/iterator/private/is_interoperable.hpp>
#include <sake/core/math/sign.hpp>
#include <sake/core/math/sign_t.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/move/forward.hpp>

namespace sake
{

namespace iterator
{

namespace facade_adl
{

namespace private_
{

template< class Derived, class Params >
class common_base
    : public private_::chained_base< Derived, Params >
{
    typedef private_::chained_base< Derived, Params > chained_base_;
public:

    template< class Introversal = sake::null_introversal_tag >
    struct relax
    {
        BOOST_STATIC_ASSERT((boost_ext::is_convertible<
            Introversal, sake::null_introversal_tag >::value));
        typedef typename sake::iterator::core_access::
            relax< Derived, Introversal >::type type;
    };

    template< class Other >
    typename boost::enable_if_c<
        sake::iterator::private_::is_interoperable< Derived, Other >::value,
        Derived&
    >::type
    at_ip(Other const & other)
    {
        sake::iterator::core_access::at_ip(derived(), other);
        return derived();
    }

    template< class Other >
    inline friend Derived&
    at_ip(Derived& this_, Other const & other)
    { return this_.at_ip(other); }

    template< class Other, class Introversal >
    typename boost::lazy_enable_if_c<
        sake::iterator::private_::is_interoperable< Derived, Other >::value,
        relax< Introversal >
    >::type
    at(Other const & other, Introversal) const
    { return sake::iterator::core_access::at(derived(), other, Introversal()); }

    template< class Other, class Introversal >
    inline friend typename relax< Introversal >::type
    at(Derived const & this_, Other const & other, Introversal)
    { return this_.at(other, Introversal()); }

protected:
    Derived& derived()
    { return *static_cast< Derived* >(this); }
    Derived const & derived() const
    { return *static_cast< Derived const * >(this); }

    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename common_base,
        (( chained_base_ ))
    )

    template< class T >
    explicit common_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            common_base, T >::value >::type* = 0)
        : chained_base_(sake::forward<T>(x))
    { }

    friend class sake::iterator::core_access;

    template< class Other >
    bool derived_equal(Other const & other) const
    { return sake::iterator::core_access::cmp(derived(), other) == sake::zero; }
    template< class Other >
    bool derived_less(Other const & other) const
    { return sake::iterator::core_access::cmp(derived(), other) < sake::zero; }
    template< class Other >
    bool derived_less_equal(Other const & other) const
    { return sake::iterator::core_access::cmp(derived(), other) <= sake::zero; }
    template< class Other >
    sake::sign_t derived_cmp(Other const & other) const
    { return sake::sign(derived() - other); }

    template< class Introversal >
    struct derived_relax
    { typedef Derived type; };

    template< class Other, class Introversal >
    typename relax< Introversal >::type
    derived_at(Other const & other, Introversal) const
    {
        Derived result(derived());
        result.at_ip(other);
        return result;
    }
};

} // namespace private_

} // namespace facade_adl

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_COMMON_BASE_HPP
