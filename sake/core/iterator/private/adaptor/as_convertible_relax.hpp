/*******************************************************************************
 * sake/core/iterator/private/adaptor/as_convertible_relax.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_AS_CONVERTIBLE_RELAX_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_AS_CONVERTIBLE_RELAX_HPP

#include <boost/mpl/placeholders.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sake/boost_ext/mpl/base_type.hpp>
#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/mpl/or.hpp>

#include <sake/core/iterator/adaptor_fwd.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/private/is_convertible_relax.hpp>
#include <sake/core/utility/is_template_base_of.hpp>
#include <sake/core/utility/int_tag.hpp>

namespace sake
{

namespace iterator
{

namespace adaptor_private
{

template< class I, class T >
struct as_convertible_relax_dispatch_index
{
    static int const value = boost_ext::mpl::
    if_<
        boost_ext::mpl::or3<
            boost::is_same< T, sake::begin_tag >,
            boost::is_same< T, sake::end_tag >,
            sake::iterator::private_::is_convertible_relax<T,I>
        >,
        sake::int_tag<2>
    >::type::template
    else_if<
        sake::is_template_base_of3<
            sake::iterator::adaptor, T,
            sake::iterator::private_::is_convertible_relax<
                boost_ext::mpl::base_type< boost::mpl::_1 >, I >
        >,
        sake::int_tag<1>
    >::type::template
    else_<
        sake::int_tag<0>
    >::type::value;
};

template<
    class I, class T,
    int = as_convertible_relax_dispatch_index<I,T>::value
>
struct as_convertible_relax_dispatch;

template< class I, class T >
struct as_convertible_relax_dispatch<I,T,2>
{
    typedef T const & type;
    static type apply(T const & x)
    { return x; }
};

template< class I, class T >
struct as_convertible_relax_dispatch<I,T,1>
{
    typedef typename T::base_type const & type;
    static type apply(T const & x)
    { return x.base(); }
};

template< class I, class T >
struct as_convertible_relax_dispatch<I,T,0>
{ };

// Workaround for MSVC9, which (apparently) doesn't like defaulted non-type
// template parameters in function template instantiations with an explicitly-
// provided template parameter.
template< class I, class T >
struct as_convertible_relax_result
    : as_convertible_relax_dispatch<I,T>
{ };

template< class I, class T >
inline typename as_convertible_relax_result<I,T>::type
as_convertible_relax(T const & x)
{ return as_convertible_relax_dispatch<I,T>::apply(x); }

} // namespace adaptor_private

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_AS_CONVERTIBLE_RELAX_HPP
