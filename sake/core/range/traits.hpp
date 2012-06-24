/*******************************************************************************
 * sake/core/range/traits.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct range_traits<R>
 *
 * struct range_iterator< R, Introversal = null_introversal_tag >
 * struct range_value<R>
 * struct range_reference<R>
 * struct range_difference<R>
 * struct range_size<R>
 * struct range_traversal<R>
 *
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
 *     template< class Introversal >
 *     struct iterator_with { typedef ... type; };
 *
 *     static iterator
 *     begin(R& r);
 *     template< class Introversal >
 *     static typenaem iterator_with< Introversal >::type
 *     begin(R& r, Introversal);
 *
 *     static iterator
 *     end(R& r);
 *     template< class Introversal >
 *     static typenaem iterator_with< Introversal >::type
 *     end(R& r, Introversal);
 *
 *     template< class T, class Introversal >
 *     static typename iterator_with< Introversal >::type
 *     iter_at(R& r, T x, Introversal);
 *
 *     template< class T >
 *     static reference
 *     at(R& r, T x);
 *
 *     static bool
 *     empty(R const & r);
 *
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

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/type_traits/make_unsigned.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/introspection/has_operator_minus.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/facade_fwd.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/range/default_impl/at.hpp>
#include <sake/core/range/default_impl/distance.hpp>
#include <sake/core/range/default_impl/empty.hpp>
#include <sake/core/range/default_impl/iterator.hpp>
#include <sake/core/range/default_impl/iter_at.hpp>
#include <sake/core/range/default_impl/iterator_with.hpp>
#include <sake/core/range/default_impl/size.hpp>
#include <sake/core/range/static_size.hpp>
#include <sake/core/range/traits_fwd.hpp>
#include <sake/core/utility/is_template_base_of.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

template< class R, class Introversal /*= sake::null_introversal_tag*/ >
struct range_iterator
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        Introversal, sake::null_introversal_tag >::value));
    typedef typename sake::range_traits<R>::template
        iterator_with< Introversal >::type type;
};

template< class R >
struct range_iterator< R, sake::null_introversal_tag >
{
private:
    typedef sake::range_traits<R> traits_;
public:
    typedef typename traits_::iterator type;
    BOOST_STATIC_ASSERT((boost::is_same<
        type,
        typename traits_::template
            iterator_with< sake::null_introversal_tag >::type
    >::value));
};

template< class R >
struct range_traits
    : sake::extension::range_traits<R>
{
    typedef sake::iterator_traits<
        typename sake::extension::range_traits<R>::iterator
    > iterator_traits;
    SAKE_USING_TYPEDEF( typename iterator_traits, iterator);
    SAKE_USING_TYPEDEF( typename iterator_traits, value_type );
    SAKE_USING_TYPEDEF( typename iterator_traits, reference );
    SAKE_USING_TYPEDEF( typename iterator_traits, difference_type );
    SAKE_USING_TYPEDEF( typename iterator_traits, traversal );

    template< class Introversal >
    struct iterator_with
        : sake::extension::range_traits<R>::template
              iterator_with< Introversal >
    { };

    template< class T, class Introversal >
    static typename iterator_with< Introversal >::type
    iter_at(R& r, T const & x, Introversal)
    { return sake::extension::range_traits<R>::iter_at(r, x, Introversal()); }

    static iterator
    begin(R& r)
    { return iter_at(r, sake::_begin, sake::null_introversal_tag()); }
    template< class Introversal >
    static typename iterator_with< Introversal >::type
    begin(R& r, Introversal)
    { return iter_at(r, sake::_begin, Introversal); }

    static iterator
    end(R& r)
    { return iter_at(r, sake::_end, sake::null_introversal_tag()); }
    template< class Introversal >
    static typename iterator_with< Introversal >::type
    end(R& r, Introversal)
    { return iter_at(r, sake::_end, Introversal); }
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

namespace range_traits_private
{

template< class R, class Iterator >
struct size_base_bool
{
    typedef sake::iterator_traits< Iterator > iterator_traits;
    SAKE_USING_TYPEDEF( typename iterator_traits, difference_type );
    SAKE_USING_TYPEDEF( typename iterator_traits, traversal );
    typedef typename boost::mpl::eval_if_c<
        boost::is_void< difference_type >::value,
        boost::mpl::identity< std::size_t >,
        boost::make_unsigned< difference_type >
    >::type size_type;
    static bool const value = boost_ext::mpl::or3<
        boost_ext::is_convertible<
            traversal, boost::random_access_traversal_tag >,
        sake::range_has_static_size<R>,
        sake::range::default_impl::has_intrinsic_size< R, size_type >,
        sake::is_template_base_of2<
            sake::iterator::facade, Iterator,
            sake::has_operator_minus<
                boost::mpl::_1, boost::mpl::_1, difference_type >
        >
    >::value;
};

template<
    class R, class Iterator,
    bool = size_base_bool< R, Iterator >::value
>
struct size_base;

template< class R, class Iterator >
struct size_base< R, Iterator, false >
{
    SAKE_USING_TYPEDEF(
        typename sake::iterator_traits< Iterator >,
        difference_type
    );
    typedef typename boost::mpl::eval_if_c<
        boost::is_void< difference_type >::value,
        boost::mpl::identity< void >,
        boost::make_unsigned< difference_type >
    >::type size_type;
    typedef boost::false_type size_enable_tag;
};

template< class R, class Iterator >
struct size_base< R, Iterator, true >
{
    SAKE_USING_TYPEDEF(
        typename sake::iterator_traits< Iterator >, difference_type );
    typedef typename boost::mpl::eval_if_c<
        boost::is_void< difference_type >::value,
        boost::mpl::identity< std::size_t >,
        boost::make_unsigned< difference_type >
    >::type size_type;
    typedef boost::true_type size_enable_tag;
    static size_type
    size(R const & r)
    { return sake::range::default_impl::size(r); }
};

} // namespace range_traits_private

template< class R, class Iterator /*= void*/ >
struct range_traits
    : range_traits_private::size_base< R, Iterator >
{
private:
    typedef range_traits_private::size_base< R, Iterator > size_base_;
public:
    typedef Iterator iterator;
    typedef sake::iterator_traits< Iterator > iterator_traits;
    //SAKE_USING_TYPEDEF( typename iterator_traits, value_type );
    SAKE_USING_TYPEDEF( typename iterator_traits, reference );
    SAKE_USING_TYPEDEF( typename iterator_traits, difference_type );
    SAKE_USING_TYPEDEF( typename iterator_traits, traversal );

    SAKE_USING_TYPEDEF( typename size_base_, size_type );

    template< class Introversal >
    struct iterator_with
    {
        BOOST_STATIC_ASSERT((boost_ext::is_convertible<
            Introversal, sake::null_introversal_tag >::value));
        typedef typename sake::range::default_impl::
            iterator_with< R, Iterator, Introversal >::type type;
        BOOST_STATIC_ASSERT((
            Introversal::value != 0
         || boost::is_same< type, iterator >::value
        ));
    };

    template< class T, class Introversal >
    static typename iterator_with< Introversal >::type
    iter_at(R& r, T const & x, Introversal)
    {
        BOOST_STATIC_ASSERT((boost_ext::mpl::or3<
            boost::is_same< T, sake::begin_tag >,
            boost::is_same< T, sake::end_tag >,
            boost_ext::is_convertible< T, iterator >
        >::value));
        BOOST_STATIC_ASSERT((boost_ext::is_convertible<
            Introversal, sake::null_introversal_tag >::value));
        return sake::range::default_impl::iter_at(r, x, Introversal());
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
        return sake::range::default_impl::at(r,x);
    }

    static bool
    empty(R const & r)
    { return sake::range::default_impl::empty(r); }

    static difference_type
    distance(R const & r)
    { return sake::range::default_impl::distance(r); }
};

template< class R >
struct range_traits< R, void >
    : sake::default_impl::range_traits<
          R, typename sake::range::default_impl::iterator<R>::type >
{ };

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_TRAITS_HPP
