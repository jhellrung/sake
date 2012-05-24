/*******************************************************************************
 * sake/core/iterator/traits.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator_traits<I>
 *
 * struct iterator_value_type<I>
 * struct iterator_reference<I>
 * struct iterator_pointer<I>
 * struct iterator_difference_type<I>
 * struct iterator_traversal<I>
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_TRAITS_HPP
#define SAKE_CORE_ITERATOR_TRAITS_HPP

#include <iterator>

#include <boost/iterator/iterator_categories.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_signed.hpp>

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/is_cv_or.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>

#include <sake/core/iterator/traits_fwd.hpp>

namespace sake
{

#define define_metafunction( type_ ) \
template< class I > \
struct iterator_ ## type_ \
{ typedef typename sake::iterator_traits<I>::type_ type; };
define_metafunction( value_type )
define_metafunction( reference )
define_metafunction( pointer )
define_metafunction( difference_type )
define_metafunction( traversal )
#undef define_metafunction

template< class I >
struct iterator_traits
    : sake::extension::iterator_traits<I>
{ };

namespace extension
{

template< class I, class Enable /*= void*/ >
struct iterator_traits
    : sake::default_impl::iterator_traits<I>
{ };

} // namespace extension

namespace default_impl
{

template< class I >
struct iterator_traits
{
    typedef std::iterator_traits<I> std_traits;
    typedef typename std_traits::value_type value_type;
    typedef typename std_traits::reference reference;
    typedef typename std_traits::pointer pointer;
    typedef typename std_traits::difference_type difference_type;
    typedef typename std_traits::iterator_category category;

    BOOST_STATIC_ASSERT((!boost_ext::is_reference< value_type >::value));
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or< value_type >::value));
    BOOST_STATIC_ASSERT((boost::is_integral< difference_type >::value));
    BOOST_STATIC_ASSERT((boost::is_signed< difference_type >::value));

#define is_convertible_( tag ) boost_ext::is_convertible< category, tag >
    typedef typename boost_ext::mpl::
         if_< is_convertible_( boost::incrementable_traversal_tag ),
              category >::type::template
    else_if < is_convertible_( std::random_access_iterator_tag ),
              boost::random_access_traversal_tag >::type::template
    else_if < is_convertible_( std::bidirectional_iterator_tag ),
              boost::bidirectional_traversal_tag >::type::template
    else_if < is_convertible_( std::forward_iterator_tag ),
              boost::forward_traversal_tag >::type::template
    else_if < is_convertible_( std::input_iterator_tag ),
              boost::single_pass_traversal_tag >::type::template
    else_if < is_convertible_( std::output_iterator_tag ),
              boost::incrementable_traversal_tag >::type::template
    else_   < void >::type traversal;
#undef is_convertible_
};

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_TRAITS_HPP
