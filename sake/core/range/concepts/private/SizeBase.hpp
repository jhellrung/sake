/*******************************************************************************
 * sake/core/range/concepts/private/SizeBase.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_CONCEPTS_PRIVATE_SIZE_BASE_HPP
#define SAKE_CORE_RANGE_CONCEPTS_PRIVATE_SIZE_BASE_HPP

#include <boost/concept/usage.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_unsigned.hpp>

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

template< class R, bool = sake::range_size_enable<R>::value >
struct SizeBase;

template< class R >
struct SizeBase< R, false >
{ };

template< class R >
struct SizeBase< R, true >
{
private:
    typedef sake::range_traits<R> traits_;
public:
    SAKE_USING_TYPEDEF( typename traits_, size_type );
    BOOST_STATIC_ASSERT((boost::is_unsigned< size_type >::value));

    BOOST_CONCEPT_USAGE( SizeBase )
    {
        assert_result(traits_::size(r));
    }

private:
    R const r;

    void assert_result(size_type);

    SAKE_SUPPRESS_WARNING_UNINIITIALIZED_IN_CONCEPT_CHECKING_CLASS( SizeBase )
};

} // namespace private_

} // namespace concepts

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_CONCEPTS_PRIVATE_SIZE_BASE_HPP
