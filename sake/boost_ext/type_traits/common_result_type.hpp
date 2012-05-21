/*******************************************************************************
 * sake/boost_ext/type_traits/common_result_type.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * class boost_ext::common_result_type< T0, T1 >
 *
 * This is identical to boost_ext::common_type except it may have reference
 * qualifiers removed to make it safe to use as a return type (it prevents
 * returning references to temporaries).
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_COMMON_RETURN_TYPE_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_COMMON_RETURN_TYPE_HPP

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/common_type.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/utility/is_convertible_wnrbt.hpp>

namespace sake
{

namespace boost_ext
{

template< class T0, class T1 >
class common_result_type
{
    typedef typename boost_ext::common_type< T0, T1 >::type nominal_type;
public:
    typedef typename boost::mpl::eval_if_c<
        boost_ext::mpl::and2<
            sake::is_convertible_wnrbt<
                typename boost_ext::remove_rvalue_reference< T0 >::type,
                nominal_type
            >,
            sake::is_convertible_wnrbt<
                typename boost_ext::remove_rvalue_reference< T1 >::type,
                nominal_type
            >
        >::value,
        boost::mpl::identity< nominal_type >,
        boost_ext::remove_qualifiers< nominal_type >
    >::type type;
};

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_COMMON_RETURN_TYPE_HPP
