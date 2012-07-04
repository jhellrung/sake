/*******************************************************************************
 * sake/core/functional/operators/private/binary_common.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_PRIVATE_BINARY_COMMON_HPP
#define SAKE_CORE_FUNCTIONAL_OPERATORS_PRIVATE_BINARY_COMMON_HPP

#include <cstddef>

#include <boost/logic/tribool.hpp>
#include <boost/mpl/vector/vector20.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_promotion.hpp>

#include <sake/boost_ext/type_traits/common_result_type.hpp>
#include <sake/boost_ext/type_traits/is_cv_or.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>
#include <sake/boost_ext/type_traits/make_signed.hpp>
#include <sake/boost_ext/type_traits/make_unsigned.hpp>

#include <sake/core/math/indeterminate_fwd.hpp>

namespace sake
{

namespace operators
{

namespace result_of
{

namespace default_impl
{

template< class T0, class T1 >
class binary_result_types
{
    BOOST_STATIC_ASSERT((!boost_ext::is_reference< T0 >::value));
    BOOST_STATIC_ASSERT((!boost_ext::is_reference< T1 >::value));
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or< T0 >::value));
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or< T1 >::value));
    typedef typename boost_ext::common_result_type< T0, T1 >::type common_type_;
public:
    typedef boost::mpl::vector11<
        common_type_,
        T0&, // op=, op?=; op<<, op>> (for std::ios)
        T0, T1, // op-, op+ (for pointers)
        std::ptrdiff_t, // op- (for pointers)
        bool, // logical
        boost::logic::tribool, // logical
        sake::functional::indeterminate, // logical
        typename boost::integral_promotion< T0 >::type, // shift
        typename boost_ext::make_signed< common_type_ >::type,
        typename boost_ext::make_unsigned< common_type_ >::type
    > type;
};

} // namespace default_impl

} // namespace result_of

} // namespace operators

} // namespace sake

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_PRIVATE_BINARY_COMMON_HPP
