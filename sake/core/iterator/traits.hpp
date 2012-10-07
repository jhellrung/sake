/*******************************************************************************
 * sake/core/iterator/traits.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator_traits<I>
 * struct extension::iterator_traits<I>
 * struct default_impl::iterator_traits< I, Introterminal = void >
 *
 * struct iterator_value<I>
 * struct iterator_reference<I>
 * struct iterator_pointer<I>
 * struct iterator_difference<I>
 * struct iterator_traversal<I>
 *
 * struct iterator_introterminal<I>
 * struct iterator_relax< I, Introterminal = null_introterminal_tag >
 *
 * iterator_traits has the following interface.
 *
 * template< class I >
 * struct iterator_traits
 * {
 *     typedef ... value_type;
 *     typedef ... reference;
 *     typedef ... pointer;
 *     typedef ... difference_type;
 *     typedef ... traversal;
 *
 *     typedef ... introterminal;
 *
 *     template< class Introterminal = null_introterminal_tag >
 *     struct relax { typedef ... type; };
 *
 *     template< class T >
 *     static void
 *     at_ip(I& i, T x);
 *     static typename relax<>::type
 *     at(I i, T x);
 *     template< class T, class Introterminal >
 *     static typename relax< Introterminal >::type
 *     at(I i, T x, Introterminal);
 * };
 *
 * extension::iterator_traits must have the following interface.
 *
 * template< class I, class Enable = void >
 * struct extension::iterator_traits
 * {
 *     typedef ... value_type;
 *     typedef ... reference;
 *     typedef ... pointer;
 *     typedef ... difference_type;
 *     typedef ... traversal;
 *
 *     typedef ... introterminal;
 *
 *     template< class Introterminal = null_introterminal_tag >
 *     struct relax { typedef ... type; };
 *
 *     template< class T >
 *     static void
 *     at_ip(I& i, T x);
 *     template< class T, class Introterminal >
 *     static typename relax< Introterminal >::type
 *     at(I i, T x, Introterminal);
 * };
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_TRAITS_HPP
#define SAKE_CORE_ITERATOR_TRAITS_HPP

#include <iterator>

#include <boost/mpl/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_void.hpp>

#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/has_qualifier.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/default_impl/at.hpp>
#include <sake/core/iterator/default_impl/at_ip.hpp>
#include <sake/core/iterator/default_impl/introterminal.hpp>
#include <sake/core/iterator/default_impl/relax.hpp>
#include <sake/core/iterator/private/at_static_assert_cond.hpp>
#include <sake/core/iterator/private/fwd.hpp>
#include <sake/core/iterator/traits_fwd.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

template< class I >
struct iterator_traits
    : sake::extension::iterator_traits<I>
{
private:
    typedef sake::extension::iterator_traits<I> extension_traits_;
public:
    SAKE_USING_TYPEDEF( typename extension_traits_, value_type );
    SAKE_USING_TYPEDEF( typename extension_traits_, difference_type );
    SAKE_USING_TYPEDEF( typename extension_traits_, introterminal );

    BOOST_STATIC_ASSERT((!boost_ext::has_qualifier< value_type >::value));
    BOOST_STATIC_ASSERT((boost_ext::mpl::or2<
        boost::is_void< difference_type >,
        boost::is_signed< difference_type >
    >::value));

    template< class Introterminal = sake::null_introterminal_tag >
    struct relax
    {
        BOOST_STATIC_ASSERT((boost_ext::is_convertible<
            Introterminal, sake::null_introterminal_tag >::value));
        BOOST_STATIC_ASSERT((boost_ext::is_convertible<
            introterminal, Introterminal >::value));
        typedef typename extension_traits_::template
            relax< Introterminal >::type type;
        BOOST_STATIC_ASSERT((boost_ext::is_convertible< I, type >::value));
    };

    template< class T >
    static void
    at_ip(I& i, T const & x)
    {
        BOOST_STATIC_ASSERT((
            sake::iterator::private_::at_static_assert_cond<I,T>::value));
        extension_traits_::at_ip(i,x);
    }

    template< class T >
    static typename relax<>::type
    at(I const & i, T const & x)
    { return at(i, x, sake::null_introterminal_tag()); }
    template< class T, class Introterminal >
    static typename relax< Introterminal >::type
    at(I const & i, T const & x, Introterminal)
    {
        BOOST_STATIC_ASSERT((
            sake::iterator::private_::at_static_assert_cond<I,T>::value));
        return extension_traits_::at(i, x, Introterminal());
    }
};

namespace extension
{

template< class I, class Enable /*= void*/ >
struct iterator_traits
    : sake::default_impl::iterator_traits<I>
{ };

} // namespace extension

namespace default_impl
{

template< class I, class Introterminal /*= void*/ >
struct iterator_traits
{
    typedef std::iterator_traits<I> std_traits;
    SAKE_USING_TYPEDEF( typename std_traits, value_type );
    SAKE_USING_TYPEDEF( typename std_traits, reference );
    SAKE_USING_TYPEDEF( typename std_traits, pointer );
    SAKE_USING_TYPEDEF( typename std_traits, difference_type );
    typedef typename std_traits::iterator_category category;

#define is_convertible_( tag ) boost_ext::is_convertible< category, tag >
    typedef typename boost_ext::mpl::
    eval_if< is_convertible_( boost::incrementable_traversal_tag ),
        typename boost_ext::mpl::
             if_< is_convertible_( boost::random_access_traversal_tag ),
                  boost::random_access_traversal_tag >::type::template
        else_if < is_convertible_( boost::bidirectional_traversal_tag ),
                  boost::bidirectional_traversal_tag >::type::template
        else_if < is_convertible_( boost::forward_traversal_tag ),
                  boost::forward_traversal_tag >::type::template
        else_if < is_convertible_( boost::single_pass_traversal_tag ),
                  boost::single_pass_traversal_tag >::type::template
        else_   < boost::incrementable_traversal_tag > >::type::template
    else_if< is_convertible_( std::random_access_iterator_tag ),
             boost::random_access_traversal_tag >::type::template
    else_if< is_convertible_( std::bidirectional_iterator_tag ),
             boost::bidirectional_traversal_tag >::type::template
    else_if< is_convertible_( std::forward_iterator_tag ),
             boost::forward_traversal_tag >::type::template
    else_if< is_convertible_( std::input_iterator_tag ),
             boost::single_pass_traversal_tag >::type::template
    else_if< is_convertible_( std::output_iterator_tag ),
             boost::incrementable_traversal_tag >::type::template
    else_  < void >::type traversal;
#undef is_convertible_

    typedef Introterminal introterminal;

    template< class Introterminal_ >
    struct relax
    { typedef typename sake::iterator::default_impl::
        relax< I, Introterminal_ >::type type; };

    template< class T >
    static void
    at_ip(I& i, T const & x)
    { sake::iterator::default_impl::at_ip(i,x); }

    template< class T, class Introterminal_ >
    static typename relax< Introterminal_ >::type
    at(I const & i, T const & x, Introterminal_)
    { return sake::iterator::default_impl::at(i, x, Introterminal_()); }
};

template< class I >
struct iterator_traits< I, void >
    : sake::default_impl::iterator_traits<
          I, typename sake::iterator::default_impl::introterminal<I>::type >
{ };

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_TRAITS_HPP
