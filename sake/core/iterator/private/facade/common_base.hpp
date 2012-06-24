/*******************************************************************************
 * sake/core/iterator/private/facade/common_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_COMMON_BASE_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_FACADE_COMMON_BASE_HPP

#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>

#include <sake/core/iterator/facade_fwd.hpp>
#include <sake/core/iterator/private/facade/chained_base.hpp>
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
    : public private_::chained_base< Params >
{
    typedef private_::chained_base< Params > chained_base_;
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

private:
    struct dummy_param { };
protected:
    void derived_equal(dummy_param);
    void derived_difference(dummy_param);
};

} // namespace private_

} // namespace facade_adl

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_COMMON_BASE_HPP
