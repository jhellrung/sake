/*******************************************************************************
 * sake/core/range/private/facade/common_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_PRIVATE_FACADE_COMMON_BASE_HPP
#define SAKE_CORE_RANGE_PRIVATE_FACADE_COMMON_BASE_HPP

#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/at.hpp>
#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>

#include <sake/core/emplacer/constructible.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/memberwise/swap.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/range/facade_fwd.hpp>

namespace sake
{

namespace range
{

namespace facade_adl
{

namespace private_
{

struct dummy_base { };

template< class Derived, class Params >
struct common_base
    : boost_ext::mpl::at<
          Params, sake::range::keyword::tag::chained_base,
          private_::dummy_base
      >
{
protected:
    typedef typename boost_ext::mpl::at<
        Params, sake::range::keyword::tag::chained_base,
        private_::dummy_base
    >::type chained_base_type;
public:
    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
        typename common_base,
        (( chained_base_type ))
    )
    SAKE_MEMBERWISE_SWAP(
        typename common_base,
        (( chained_base_type ))
    )
protected:
    Derived& derived()
    { return *static_cast< Derived* >(this); }
    Derived const & derived() const
    { return *static_cast< Derived const * >(this); }

    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename common_base,
        (( chained_base_type ))
    )

    template< class T >
    explicit common_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            common_base, T >::value >::type* = 0)
        : chained_base_type(sake::emplacer_constructible<
              chained_base_type >(sake::forward<T>(x)))
    { }
};

} // namespace private_

} // namespace facade_adl

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_PRIVATE_FACADE_COMMON_BASE_HPP
