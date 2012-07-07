/*******************************************************************************
 * sake/core/range/traits.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct range_traits<R>
 *
 * struct range_iterator< R, Introterminal = null_introterminal_tag >
 * struct range_value<R>
 * struct range_reference<R>
 * struct range_difference<R>
 * struct range_size<R>
 * struct range_traversal<R>
 *
 * struct range_distance_enable<R>
 * struct range_size_enable<R>
 *
 * range_traits has the following interface.
 *
 * template< class R >
 * struct range_traits
 * {
 *     typedef ... iterator;
 *     typedef ... value_type;
 *     typedef ... reference;
 *     typedef ... difference_type;
 *     typedef ... size_type;
 *     typedef ... traversal;
 *
 *     template< class Introterminal >
 *     struct iterator_with { typedef ... type; };
 *     template< class Begin, class End >
 *     struct subrange_with { typedef ... type; };
 *
 *     static iterator
 *     begin(R& r);
 *     template< class Introterminal >
 *     static typename iterator_with< Introterminal >::type
 *     begin(R& r, Introterminal);
 *
 *     static iterator
 *     end(R& r);
 *     template< class Introterminal >
 *     static typename iterator_with< Introterminal >::type
 *     end(R& r, Introterminal);
 *
 *     template< class T, class Introterminal >
 *     static typename iterator_with< Introterminal >::type
 *     iter_at(R& r, T x, Introterminal);
 *
 *     template< class Begin, class End >
 *     static typename subrange_with< Begin, End >::type
 *     sub(R& r, Begin b, End e);
 *
 *     template< class T >
 *     static reference
 *     at(R& r, T x);
 *
 *     static bool
 *     empty(R const & r);
 *
 *     typedef ... distance_enable_tag;
 *     // If distance_enable_tag::value == true
 *     static difference_type
 *     distance(R const & r);
 *
 *     typedef ... size_enable_tag;
 *     // If size_enable_tag::value == true
 *     static size_type
 *     size(R const & r);
 * };
 *
 * extension::range_traits must have the following interface.
 *
 * template< class R, class Enable = void >
 * struct extension::range_traits
 * {
 *     typedef ... iterator;
 *     typedef ... size_type;
 *
 *     template< class Introterminal >
 *     struct iterator_with { typedef ... type; };
 *     template< class Begin, class End >
 *     struct subrange_with { typedef ... type; };
 *
 *     template< class T, class Introterminal >
 *     static typename iterator_with< Introterminal >::type
 *     iter_at(R& r, T x, Introterminal);
 *
 *     template< class Begin, class End >
 *     static typename subrange_with< Begin, End >::type
 *     sub(R& r, Begin b, End e);
 *
 *     template< class T >
 *     static reference
 *     at(R& r, T x);
 *
 *     static bool
 *     empty(R const & r);
 *
 *     typedef ... distance_enable_tag;
 *     // If distance_enable_tag::value == true
 *     static difference_type
 *     distance(R const & r);
 *
 *     typedef ... size_enable_tag;
 *     // If size_enable_tag::value == true
 *     static size_type
 *     size(R const & r);
 * };
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_TRAITS_HPP
#define SAKE_CORE_RANGE_TRAITS_HPP

#include <cstddef>

#include <boost/mpl/assert.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/not.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/iterator/adapt_introterminal.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/range/default_impl/at.hpp>
#include <sake/core/range/default_impl/empty.hpp>
#include <sake/core/range/default_impl/iterator.hpp>
#include <sake/core/range/default_impl/iter_at.hpp>
#include <sake/core/range/default_impl/iterator_with.hpp>
#include <sake/core/range/default_impl/sub.hpp>
#include <sake/core/range/default_impl/subrange_with.hpp>
#include <sake/core/range/private/iter_at_static_assert_cond.hpp>
#include <sake/core/range/private/traits/distance_base.hpp>
#include <sake/core/range/traits_fwd.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

template< class R >
struct range_traits
    : sake::extension::range_traits<R>
{
private:
    typedef sake::extension::range_traits<R> extension_traits_;
public:
    SAKE_USING_TYPEDEF( typename extension_traits_, iterator );
    typedef sake::iterator_traits< iterator > iterator_traits;
    SAKE_USING_TYPEDEF( typename iterator_traits, value_type );
    SAKE_USING_TYPEDEF( typename iterator_traits, reference );
    SAKE_USING_TYPEDEF( typename iterator_traits, difference_type );
    SAKE_USING_TYPEDEF( typename iterator_traits, traversal );

    SAKE_USING_TYPEDEF( typename extension_traits_, size_type );

    template< class Introterminal >
    struct iterator_with
    {
        BOOST_STATIC_ASSERT((boost_ext::is_convertible<
            Introterminal, sake::null_introterminal_tag >::value));
        typedef typename extension_traits_::template
            iterator_with< Introterminal >::type type;
        BOOST_MPL_ASSERT((boost_ext::mpl::or2<
            boost::mpl::not_< boost::is_same<
                Introterminal, sake::null_introterminal_tag > >,
            boost::is_same< type, iterator >
        >));
    };

    template< class T >
    static iterator
    iter_at(R& r, T const & x)
    { return iter_at(r, x, sake::null_introterminal_tag()); }

    template< class T, class Introterminal >
    static typename iterator_with< Introterminal >::type
    iter_at(R& r, T const & x, Introterminal)
    {
        BOOST_STATIC_ASSERT((
            sake::range::private_::iter_at_static_assert_cond<R,T>::value));
        BOOST_STATIC_ASSERT((boost_ext::is_convertible<
            Introterminal, sake::null_introterminal_tag >::value));
        return extension_traits_::iter_at(r, x, Introterminal());
    }

    static iterator
    begin(R& r)
    { return iter_at(r, sake::_begin, sake::null_introterminal_tag()); }
    template< class Introterminal >
    static typename iterator_with< Introterminal >::type
    begin(R& r, Introterminal)
    { return iter_at(r, sake::_begin, Introterminal()); }

    static iterator
    end(R& r)
    { return iter_at(r, sake::_end, sake::null_introterminal_tag()); }
    template< class Introterminal >
    static typename iterator_with< Introterminal >::type
    end(R& r, Introterminal)
    { return iter_at(r, sake::_end, Introterminal()); }

    template< class Begin, class End >
    struct subrange_with
    {
        typedef typename extension_traits_::template subrange_with<
            typename boost::mpl::if_c<
                boost::is_same< Begin, sake::begin_tag >::value,
                sake::begin_tag, void
            >::type,
            typename boost::mpl::if_c<
                boost::is_same< End, sake::end_tag >::value,
                sake::end_tag, void
            >::type
        >::type type;
        BOOST_STATIC_ASSERT((boost_ext::is_convertible<
            typename sake::range_iterator< type >::type, iterator >::value));
        BOOST_STATIC_ASSERT((boost_ext::is_convertible<
            iterator, typename sake::range_iterator< type >::type >::value));
    };

    template< class Begin, class End >
    static typename subrange_with< Begin, End >::type
    sub(R& r, Begin const & b, End const & e)
    {
        BOOST_STATIC_ASSERT((boost_ext::mpl::or2<
            boost::is_same< Begin, sake::begin_tag >,
            boost_ext::is_convertible< Begin, iterator >
        >::value));
        BOOST_STATIC_ASSERT((boost_ext::mpl::or2<
            boost::is_same< End, sake::end_tag >,
            boost_ext::is_convertible< End, iterator >
        >::value));
        return extension_traits_::sub(r,b,e);
    }

    template< class T >
    static reference
    at(R& r, T const x)
    {
        BOOST_STATIC_ASSERT((boost_ext::mpl::or3<
            boost::is_same< T, sake::begin_tag >,
            boost::is_same< T, sake::end_tag >,
            boost_ext::mpl::and2<
                boost_ext::is_convertible<
                    traversal, boost::random_access_traversal_tag >,
                boost_ext::is_convertible< T, size_type >
            >
        >::value));
        return extension_traits_::at(r,x);
    }
};

namespace extension
{

template< class R, class Enable /*= void*/ >
struct range_traits
    : sake::default_impl::range_traits<R>
{ };

} // namespace extension

namespace default_impl
{

template< class R, class Iterator /*= void*/ >
struct range_traits
    : range_traits_private::distance_base< R, Iterator >
{
private:
    typedef range_traits_private::size_base< R, Iterator > size_base_;
public:
    typedef Iterator iterator;
    typedef sake::iterator_traits< Iterator > iterator_traits;
    SAKE_USING_TYPEDEF( typename iterator_traits, reference );

    template< class Introterminal >
    struct iterator_with
    { typedef typename sake::range::default_impl::
        iterator_with< R, Iterator, Introterminal >::type type; };

    template< class Begin, class End >
    struct subrange_with
    {
        typedef typename sake::range::default_impl::subrange_with<
            R, Iterator,
            typename boost::mpl::if_c<
                boost::is_same< Begin, sake::begin_tag >::value,
                sake::begin_tag, void
            >::type,
            typename boost::mpl::if_c<
                boost::is_same< End, sake::end_tag >::value,
                sake::end_tag, void
            >::type
        >::type type;
    };

    template< class T, class Introterminal >
    static typename iterator_with< Introterminal >::type
    iter_at(R& r, T const & x, Introterminal)
    { return sake::range::default_impl::iter_at(r, x, Introterminal()); }

    template< class Begin, class End >
    static typename subrange_with< Begin, End >::type
    sub(R& r, Begin const & b, End const & e)
    { return sake::range::default_impl::sub(r,b,e); }

    template< class T >
    static reference
    at(R& r, T const x)
    { return sake::range::default_impl::at(r,x); }

    static bool
    empty(R const & r)
    { return sake::range::default_impl::empty(r); }
};

template< class R >
struct range_traits< R, void >
    : sake::default_impl::range_traits<
          R, typename sake::range::default_impl::iterator<R>::type >
{ };

template< class T, std::size_t N >
struct range_traits< T[N], void >
{
    typedef T this_type[N];

    typedef T* iterator;
    typedef std::size_t size_type;

    template< class Introterminal >
    struct iterator_with
        : sake::iterator::adapt_introterminal< T*, Introterminal >
    { };
    template< class Begin, class End >
    struct subrange_with
    { typedef typename sake::range::default_impl::
        subrange_with< void, T*, Begin, End >::type type; };

    static T*
    iter_at(T* const p, sake::begin_tag, sake::null_introterminal_tag)
    { return p; }
    static T*
    iter_at(T* const p, sake::end_tag, sake::null_introterminal_tag)
    { return p + N; }

    template< class U, class Introterminal >
    static typename iterator_with< Introterminal >::type
    iter_at(this_type& r, U const & x, Introterminal)
    { return sake::range::default_impl::iter_at(r, x, Introterminal()); }

    template< class Begin, class End >
    static typename subrange_with< Begin, End >::type
    sub(this_type& r, Begin const & b, End const & e)
    { return typename subrange_with< Begin, End >::type(r,b,e); }

    static T&
    at(this_type& r, sake::begin_tag)
    { return r[0]; }

    static T&
    at(this_type& r, sake::end_tag)
    { return r[N-1]; }

    static T&
    at(this_type& r, std::size_t const n)
    { return r[n]; }

    static bool
    empty(this_type& /*r*/)
    { return false; }

    typedef boost::true_type distance_enable_tag;
    static std::ptrdiff_t
    distance(this_type& /*r*/)
    { return N; }

    typedef boost::true_type size_enable_tag;
    static std::size_t
    size(this_type& /*r*/)
    { return N; }
};

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_TRAITS_HPP
