/*******************************************************************************
 * sake/core/iterator/traits.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator_traits<I>
 * struct extension::iterator_traits<I>
 * struct default_impl::iterator_traits< I, Introversal = void >
 *
 * struct iterator_value<I>
 * struct iterator_reference<I>
 * struct iterator_pointer<I>
 * struct iterator_difference<I>
 * struct iterator_traversal<I>
 *
 * struct iterator_introversal<I>
 * struct iterator_begin_introversal<I>
 * struct iterator_end_introversal<I>
 * struct iterator_relax< I, Introversal = null_introversal_tag >
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
 *     typedef ... introversal;
 *
 *     template< class Introversal >
 *     struct relax { typedef ... type; };
 *
 *     template< class T >
 *     static I&
 *     at_ip(I& i, T x);
 *     template< class T, class Introversal >
 *     static typename relax< Introversal >::type
 *     at(I i, T x, Introversal);
 * };
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_TRAITS_HPP
#define SAKE_CORE_ITERATOR_TRAITS_HPP

#include <iterator>

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_void.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/is_cv_or.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>

#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/default_impl/at.hpp>
#include <sake/core/iterator/default_impl/at_ip.hpp>
#include <sake/core/iterator/default_impl/introversal.hpp>
#include <sake/core/iterator/default_impl/relax.hpp>
#include <sake/core/iterator/traits_fwd.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

template< class I >
struct iterator_value
{ typedef typename sake::iterator_traits<I>::value_type type; };
template< class I >
struct iterator_reference
{ typedef typename sake::iterator_traits<I>::reference type; };
template< class I >
struct iterator_pointer
{ typedef typename sake::iterator_traits<I>::pointer type; };
template< class I >
struct iterator_difference
{ typedef typename sake::iterator_traits<I>::difference_type type; };
template< class I >
struct iterator_traversal
{ typedef typename sake::iterator_traits<I>::traversal type; };

template< class I >
struct iterator_introversal
{
    typedef typename sake::iterator_traits<I>::introversal type;
    static unsigned int const value = type::value;
};
template< class I >
struct iterator_begin_introversal
    : sake::introversal_meet<
          typename sake::iterator_introversal<I>::type,
          sake::begin_access_introversal_tag
      >
{ };
template< class I >
struct iterator_end_introversal
    : sake::introversal_meet<
          typename sake::iterator_introversal<I>::type,
          sake::end_access_introversal_tag
      >
{ };

template< class I, class Introversal = sake::null_introversal_tag >
struct iterator_relax
{
private:
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        Introversal, sake::null_introversal_tag >::value));
    typedef sake::iterator_traits<I> traits_;
    SAKE_USING_TYPEDEF( typename traits_, introversal );
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        introversal, Introversal >::value));
public:
    typedef typename traits_::template relax< Introversal >::type type;
};

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

template< class I, class Introversal /*= void*/ >
struct iterator_traits
{
    typedef std::iterator_traits<I> std_traits;
    SAKE_USING_TYPEDEF( typename std_traits, value_type );
    SAKE_USING_TYPEDEF( typename std_traits, reference );
    SAKE_USING_TYPEDEF( typename std_traits, pointer );
    SAKE_USING_TYPEDEF( typename std_traits, difference_type );
    typedef typename std_traits::iterator_category category;

    BOOST_STATIC_ASSERT((!boost_ext::is_reference< value_type >::value));
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or< value_type >::value));
    BOOST_STATIC_ASSERT((boost_ext::mpl::or2<
        boost::is_void< difference_type >,
        boost_ext::mpl::and2<
            boost::is_integral< difference_type >,
            boost::is_signed< difference_type >
        >
    >::value));

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

    typedef Introversal introversal;

    template< class Introversal_ >
    struct relax
    {
        BOOST_STATIC_ASSERT((boost_ext::is_convertible<
            Introversal_, sake::null_introversal_tag >::value));
        BOOST_STATIC_ASSERT((boost_ext::is_convertible<
            Introversal, Introversal_ >::value));
        typedef typename sake::iterator::default_impl::
            relax< I, Introversal_ >::type type;
    };

    template< class T >
    static I&
    at_ip(I& i, T const & x)
    {
        BOOST_STATIC_ASSERT((boost_ext::mpl::or3<
            boost::is_same< T, sake::begin_tag >,
            boost::is_same< T, sake::end_tag >,
            sake::iterator::private_::is_interoperable<I,T>
        >::value));
        return sake::iterator::default_impl::at_ip(i, x);
    }

    template< class T, class Introversal >
    static typename relax< Introversal >::type
    at(I const & i, T const & x, Introversal)
    {
        BOOST_STATIC_ASSERT((boost_ext::mpl::or3<
            boost_ext::mpl::and2<
                boost::is_same< T, sake::begin_tag >,
                boost_ext::is_convertible<
                    introversal, sake::begin_access_introversal_tag >
            >,
            boost_ext::mpl::and2<
                boost::is_same< T, sake::end_tag >,
                boost_ext::is_convertible<
                    introversal, sake::end_access_introversal_tag >
            >,
            sake::iterator::private_::is_interoperable<I,T>
        >::value));
        return sake::iterator::default_impl::at(i, x, Introversal());
    }
};

template< class I >
struct iterator_traits< I, void >
    : sake::default_impl::iterator_traits<
          I, typename sake::iterator::default_impl::introversal<I>::type >
{ };

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_TRAITS_HPP
