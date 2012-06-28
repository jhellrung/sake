/*******************************************************************************
 * sake/core/range/concepts/private/DistanceBase.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_CONCEPTS_PRIVATE_DISTANCE_BASE_HPP
#define SAKE_CORE_RANGE_CONCEPTS_PRIVATE_DISTANCE_BASE_HPP

#include <boost/concept/usage.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_signed.hpp>

#include <sake/core/config.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace range
{

namespace concepts
{

namespace private_
{

template< class R, bool = sake::range_distance_enable<R>::value >
struct DistanceBase;

template< class R >
struct DistanceBase< R, false >
{ };

template< class R >
struct DistanceBase< R, true >
{
private:
    typedef sake::range_traits<R> traits_;
public:
    SAKE_USING_TYPEDEF( typename traits_, difference_type );
    BOOST_STATIC_ASSERT((boost::is_signed< difference_type >::value));

    BOOST_CONCEPT_USAGE( DistanceBase )
    {
        assert_result(traits_::distance(r));
    }

private:
    R r;

    void assert_result(difference_type);

    SAKE_SUPPRESS_WARNING_UNINIITIALIZED_IN_CONCEPT_CHECKING_CLASS( DistanceBase )
};

} // namespace private_

} // namespace concepts

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_CONCEPTS_PRIVATE_DISTANCE_BASE_HPP
