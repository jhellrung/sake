/*******************************************************************************
 * sake/core/iterator/private/adaptor/as_convertible.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_AS_CONVERTIBLE_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_AS_CONVERTIBLE_HPP

#include <boost/mpl/placeholders.hpp>

#include <sake/boost_ext/mpl/base_type.hpp>
#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/iterator/adaptor_fwd.hpp>
#include <sake/core/utility/is_template_base_of.hpp>
#include <sake/core/utility/int_tag.hpp>

namespace sake
{

namespace iterator
{

namespace adaptor_private
{

template< class I, class J >
struct as_convertible_dispatch_index
{
    static int const value = boost_ext::mpl::
    if_<
        boost_ext::is_convertible<J,I>,
        sake::int_tag<2>
    >::type::template
    else_if<
        sake::is_template_base_of3<
            sake::iterator::adaptor, J,
            boost_ext::is_convertible<
                boost_ext::mpl::base_type< boost::mpl::_1 >, I >
        >,
        sake::int_tag<1>
    >::type::template
    else_<
        sake::int_tag<0>
    >::type::value;
};

template<
    class I, class J,
    int = as_convertible_dispatch_index<I,J>::value
>
struct as_convertible_dispatch;

template< class I, class J >
struct as_convertible_dispatch<I,J,2>
{
    typedef J const & type;
    static type apply(J const & j)
    { return j; }
};

template< class I, class J >
struct as_convertible_dispatch<I,J,1>
{
    typedef typename J::base_type const & type;
    static type apply(J const & j)
    { return j.base(); }
};

template< class I, class J >
struct as_convertible_dispatch<I,J,0>
{ };

// Workaround for MSVC9, which (apparently) doesn't like non-type default
// template parameters in function template instantiations with an explicitly-
// provided template parameter.
template< class I, class J >
struct as_convertible_result
    : as_convertible_dispatch<I,J>
{ };

template< class I, class J >
inline typename as_convertible_result<I,J>::type
as_convertible(J const & j)
{ return as_convertible_dispatch<I,J>::apply(j); }

} // namespace adaptor_private

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_AS_CONVERTIBLE_HPP
