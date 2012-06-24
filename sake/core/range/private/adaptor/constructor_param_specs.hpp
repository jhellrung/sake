/*******************************************************************************
 * sake/core/range/private/adaptor/constructor_param_specs.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_PRIVATE_ADAPTOR_CONSTRUCTOR_PARAM_SPECS_HPP
#define SAKE_CORE_RANGE_PRIVATE_ADAPTOR_CONSTRUCTOR_PARAM_SPECS_HPP

#include <boost/mpl/has_key.hpp>
#include <boost/mpl/vector/vector10.hpp>

#include <sake/boost_ext/mpl/at.hpp>

#include <sake/core/range/keyword.hpp>
#include <sake/core/keyword/param_spec.hpp>
#include <sake/core/utility/value_constructor_enable.hpp>

namespace sake
{

namespace range
{

namespace adaptor_private
{

#define keyword_tag sake::range::keyword::tag

template< class Params >
struct constructor_param_specs_index
{
    static int const value =
        boost::mpl::has_key< Params, keyword_tag::member >::value
  + 2 * boost::mpl::has_key< Params, keyword_tag::chained_base >::value;
};

template<
    class I, class Params,
    int = constructor_param_specs_index< Params >::value
>
struct constructor_param_specs;

template< class I, class Params >
struct constructor_param_specs< I, Params, 0 >
{
    typedef boost::mpl::vector1<
        sake::keyword::optional<
            keyword_tag::base,
            sake::value_constructor_enable<I>
        >
    > type;
};

template< class I, class Params >
struct constructor_param_specs< I, Params, 1 >
{
    typedef boost::mpl::vector2<
        sake::keyword::optional<
            keyword_tag::base,
            sake::value_constructor_enable<I>
        >,
        sake::keyword::optional<
            keyword_tag::member,
            sake::value_constructor_enable< typename boost_ext::mpl::at<
                Params, keyword_tag::member >::type >
        >
    > type;
};

template< class I, class Params >
struct constructor_param_specs< I, Params, 2 >
{
    typedef boost::mpl::vector2<
        sake::keyword::optional<
            keyword_tag::base,
            sake::value_constructor_enable<I>
        >,
        sake::keyword::optional<
            keyword_tag::chained_base,
            sake::value_constructor_enable< typename boost_ext::mpl::at<
                Params, keyword_tag::chained_base >::type >
        >
    > type;
};

template< class I, class Params >
struct constructor_param_specs< I, Params, 3 >
{
    typedef boost::mpl::vector3<
        sake::keyword::optional<
            keyword_tag::base,
            sake::value_constructor_enable<I>
        >,
        sake::keyword::optional<
            sake::keyword::deduced< keyword_tag::member >,
            sake::value_constructor_enable< typename boost_ext::mpl::at<
                Params, keyword_tag::member >::type >
        >,
        sake::keyword::optional<
            sake::keyword::deduced< keyword_tag::chained_base >,
            sake::value_constructor_enable< typename boost_ext::mpl::at<
                Params, keyword_tag::chained_base >::type >
        >
    > type;
};

#undef keyword_tag

} // namespace adaptor_private

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_PRIVATE_ADAPTOR_CONSTRUCTOR_PARAM_SPECS_HPP
