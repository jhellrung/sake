/*******************************************************************************
 * sake/core/iterator/private/adaptor/constructor_param_specs.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_CONSTRUCTOR_PARAM_SPECS_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_CONSTRUCTOR_PARAM_SPECS_HPP

#include <boost/mpl/vector/vector10.hpp>

#include <sake/boost_ext/mpl/at.hpp>

#include <sake/core/iterator/keyword.hpp>
#include <sake/core/keyword/param_spec.hpp>
#include <sake/core/utility/value_constructor_enable.hpp>

namespace sake
{

namespace iterator
{

namespace adaptor_private
{

template< class I, class Params >
struct constructor_param_specs
{
    typedef boost::mpl::vector3<
        sake::keyword::optional<
            sake::iterator::keyword::tag::base,
            sake::value_constructor_enable<I>
        >,
        sake::keyword::optional<
            sake::keyword::deduced< sake::iterator::keyword::tag::member >,
            sake::value_constructor_enable<
                typename boost_ext::mpl::at<
                    Params, sake::iterator::keyword::tag::member,
                    void
                >::type
            >
        >,
        sake::keyword::optional<
            sake::keyword::deduced< sake::iterator::keyword::tag::chained_base >,
            sake::value_constructor_enable<
                typename boost_ext::mpl::at<
                    Params, sake::iterator::keyword::tag::chained_base,
                    void
                >::type
            >
        >
    > type;
};

} // namespace adaptor_private

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_CONSTRUCTOR_PARAM_SPECS_HPP
