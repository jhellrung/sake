/*******************************************************************************
 * sake/core/iterator/private/facade/chained_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_CHAINED_BASE_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_FACADE_CHAINED_BASE_HPP

#include <boost/mpl/has_key.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/at.hpp>
#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>

#include <sake/core/emplacer/constructible.hpp>
#include <sake/core/emplacer/emplacer.hpp>
#include <sake/core/iterator/facade_fwd.hpp>
#include <sake/core/iterator/keyword.hpp>
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

template< class Params >
struct chained_base_index
{
    static int const value = boost::mpl::has_key<
        Params, sake::iterator::keyword::tag::chained_base >::value;
};

template< class Params >
class chained_base< Params, 0 >
{
protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename chained_base,
        BOOST_PP_SEQ_NIL
    )
};

template< class Params >
class chained_base< Params, 1 >
    : public boost_ext::mpl::at<
          Params, sake::iterator::keyword::tag::chained_base >::type
{
protected:
    typedef typename boost_ext::mpl::at<
        Params, sake::iterator::keyword::tag::chained_base
    >::type chained_base_type;

    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename chained_base,
        (( chained_base_type ))
    )

    template< class T >
    explicit chained_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            chained_base, T >::value >::type* = 0)
        : chained_base_type(sake::emplacer_constructible<
              chained_base_type >(sake::forward<T>(x)))
    { }

    template< class V >
    explicit chained_base(sake::emplacer< V ( ) >)
    { }
};

} // namespace private_

} // namespace facade_adl

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_CHAINED_BASE_HPP
