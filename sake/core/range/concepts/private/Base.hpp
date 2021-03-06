/*******************************************************************************
 * sake/core/range/concepts/private/Base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_CONCEPTS_PRIVATE_BASE_HPP
#define SAKE_CORE_RANGE_CONCEPTS_PRIVATE_BASE_HPP

#include <boost/concept/assert.hpp>
#include <boost/concept/usage.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/concepts/Void.hpp>
#include <sake/core/config.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/concepts/fwd.hpp>
#include <sake/core/iterator/concepts/Iterator.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/iterator/traits_fwd.hpp>
#include <sake/core/range/concepts/fwd.hpp>
#include <sake/core/range/concepts/private/DistanceBase.hpp>
#include <sake/core/range/concepts/private/SizeBase.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/range/traits_fwd.hpp>
#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace range
{

namespace concepts
{

namespace private_
{

template< class R >
struct Base
    : sake::range::concepts::private_::DistanceBase<R>,
      sake::range::concepts::private_::SizeBase<R>
{
private:
    typedef sake::range_traits<R> traits_;
public:
    SAKE_USING_TYPEDEF( typename traits_, iterator );
    SAKE_USING_TYPEDEF( typename traits_, value_type );
    SAKE_USING_TYPEDEF( typename traits_, reference );
    SAKE_USING_TYPEDEF( typename traits_, difference_type );
    SAKE_USING_TYPEDEF( typename traits_, traversal );
    BOOST_MPL_ASSERT((boost_ext::is_convertible<
        traversal, boost::single_pass_traversal_tag >));

    template< class Introterminal >
    struct iterator_with
        : traits_::template iterator_with< Introterminal >
    { };
    template< class Begin, class End >
    struct subrange_with
        : traits_::template subrange_with< Begin, End >
    { };

    // Typedef each introterminal-iterator.
#define assert_introterminal( tag ) \
    typedef typename iterator_with< \
        sake::tag ## _introterminal_tag >::type tag ## _iterator; \
    typedef sake::iterator_traits< tag ## _iterator > tag ## _iter_traits;
    assert_introterminal( null )
    assert_introterminal( begin_detect )
    assert_introterminal( begin_access )
    assert_introterminal( end_detect )
    assert_introterminal( end_access )
    assert_introterminal( begin_detect_end_detect )
    assert_introterminal( begin_access_end_detect )
    assert_introterminal( begin_detect_end_access )
    assert_introterminal( begin_access_end_access )
#undef assert_introterminal
    BOOST_MPL_ASSERT((boost::is_same< null_iterator, iterator >));

    BOOST_CONCEPT_ASSERT((sake::concepts::Iterator< iterator >));
    typedef sake::iterator_traits< iterator > iterator_traits_;
    BOOST_MPL_ASSERT((boost::is_same< typename iterator_traits_::value_type, value_type >));
    BOOST_MPL_ASSERT((boost::is_same< typename iterator_traits_::reference, reference >));
    BOOST_MPL_ASSERT((boost::is_same< typename iterator_traits_::difference_type, difference_type >));
    BOOST_MPL_ASSERT((boost::is_same< typename iterator_traits_::traversal, traversal >));

#ifdef SAKE_RANGE_CONCEPT_COMPLETE

    // Assert each introterminal-iterator satisfies the Iterator concept.
#define assert_introterminal( tag ) \
    BOOST_CONCEPT_ASSERT((sake::concepts::Iterator< tag ## _iterator >));
    assert_introterminal( begin_detect )
    assert_introterminal( begin_access )
    assert_introterminal( end_detect )
    assert_introterminal( end_access )
    assert_introterminal( begin_detect_end_detect )
    assert_introterminal( begin_access_end_detect )
    assert_introterminal( begin_detect_end_access )
    assert_introterminal( begin_access_end_access )
#undef assert_introterminal

    // Assert each introterminal-iterator's associated types are identical to
    // the range's associated types.
#define assert_introterminal( tag ) \
    BOOST_MPL_ASSERT((boost_ext::mpl::and4< \
        boost::is_same< typename tag ## _iter_traits::value_type, value_type >, \
        boost::is_same< typename tag ## _iter_traits::reference, reference >, \
        boost::is_same< typename tag ## _iter_traits::difference_type, difference_type >, \
        boost::is_same< typename tag ## _iter_traits::traversal, traversal > \
    >));
    assert_introterminal( begin_detect )
    assert_introterminal( begin_access )
    assert_introterminal( end_detect )
    assert_introterminal( end_access )
    assert_introterminal( begin_detect_end_detect )
    assert_introterminal( begin_access_end_detect )
    assert_introterminal( begin_detect_end_access )
    assert_introterminal( begin_access_end_access )
#undef assert_introterminal

    // Assert each introterminal-iterator's introterminal is as requested from
    // iterator_with.
#define assert_introterminal( tag ) \
    BOOST_MPL_ASSERT((boost_ext::is_convertible< \
        typename tag ## _iter_traits::introterminal, \
        sake::tag ## _introterminal_tag \
    >));
    assert_introterminal( begin_detect )
    assert_introterminal( begin_access )
    assert_introterminal( end_detect )
    assert_introterminal( end_access )
    assert_introterminal( begin_detect_end_detect )
    assert_introterminal( begin_access_end_detect )
    assert_introterminal( begin_detect_end_access )
    assert_introterminal( begin_access_end_access )
#undef assert_introterminal

    // Assert convertibility among the various introterminal-iterators.
#define assert_introterminal( from_tag, to_tag ) \
    BOOST_MPL_ASSERT((boost_ext::is_convertible< \
        from_tag ## _iterator, to_tag ## _iterator >));
    assert_introterminal( begin_detect, null )
    assert_introterminal( begin_access, null )
    assert_introterminal( begin_access, begin_detect )
    assert_introterminal( end_detect, null )
    assert_introterminal( end_access, null )
    assert_introterminal( end_access, end_detect )
    assert_introterminal( begin_detect_end_detect, null )
    assert_introterminal( begin_detect_end_detect, begin_detect )
    assert_introterminal( begin_detect_end_detect, end_detect )
    assert_introterminal( begin_access_end_detect, null )
    assert_introterminal( begin_access_end_detect, begin_detect )
    assert_introterminal( begin_access_end_detect, begin_access )
    assert_introterminal( begin_access_end_detect, end_detect )
    assert_introterminal( begin_access_end_detect, begin_detect_end_detect )
    assert_introterminal( begin_detect_end_access, null )
    assert_introterminal( begin_detect_end_access, begin_detect )
    assert_introterminal( begin_detect_end_access, end_detect )
    assert_introterminal( begin_detect_end_access, end_access )
    assert_introterminal( begin_detect_end_access, begin_detect_end_detect )
    assert_introterminal( begin_access_end_access, null )
    assert_introterminal( begin_access_end_access, begin_detect )
    assert_introterminal( begin_access_end_access, begin_access )
    assert_introterminal( begin_access_end_access, end_detect )
    assert_introterminal( begin_access_end_access, end_access )
    assert_introterminal( begin_access_end_access, begin_detect_end_detect )
    assert_introterminal( begin_access_end_access, begin_access_end_detect )
    assert_introterminal( begin_access_end_access, begin_detect_end_access )
#undef assert_introterminal

    // Assert introterminal relaxation agrees with the range's iterator_with.
#define assert_introterminal( from_tag, to_tag ) \
    BOOST_MPL_ASSERT((boost::is_same< \
        typename sake::iterator_relax< \
            from_tag ## _iterator, \
            sake::to_tag ## _introterminal_tag \
        >::type, \
        to_tag ## _iterator \
    >));
    assert_introterminal( begin_detect, null )
    assert_introterminal( begin_access, null )
    assert_introterminal( begin_access, begin_detect )
    assert_introterminal( end_detect, null )
    assert_introterminal( end_access, null )
    assert_introterminal( end_access, end_detect )
    assert_introterminal( begin_detect_end_detect, null )
    assert_introterminal( begin_detect_end_detect, begin_detect )
    assert_introterminal( begin_detect_end_detect, end_detect )
    assert_introterminal( begin_access_end_detect, null )
    assert_introterminal( begin_access_end_detect, begin_detect )
    assert_introterminal( begin_access_end_detect, begin_access )
    assert_introterminal( begin_access_end_detect, end_detect )
    assert_introterminal( begin_access_end_detect, begin_detect_end_detect )
    assert_introterminal( begin_detect_end_access, null )
    assert_introterminal( begin_detect_end_access, begin_detect )
    assert_introterminal( begin_detect_end_access, end_detect )
    assert_introterminal( begin_detect_end_access, end_access )
    assert_introterminal( begin_detect_end_access, begin_detect_end_detect )
    assert_introterminal( begin_access_end_access, null )
    assert_introterminal( begin_access_end_access, begin_detect )
    assert_introterminal( begin_access_end_access, begin_access )
    assert_introterminal( begin_access_end_access, end_detect )
    assert_introterminal( begin_access_end_access, end_access )
    assert_introterminal( begin_access_end_access, begin_detect_end_detect )
    assert_introterminal( begin_access_end_access, begin_access_end_detect )
    assert_introterminal( begin_access_end_access, begin_detect_end_access )
#undef assert_introterminal

    // Typedef each subrange.
#define assert_subrange( prefix, begin_tag_, end_tag_ ) \
    typedef typename subrange_with< begin_tag_, end_tag_ >::type prefix ## _subrange;
    assert_subrange( void_void, void, void )
    assert_subrange( begin_void, sake::begin_tag, void )
    assert_subrange( void_end, void, sake::end_tag )
    assert_subrange( begin_end, sake::begin_tag, sake::end_tag )
#undef assert_subrange

    // Assert each subrange satisfies the Range concept.
#define assert_subrange( prefix ) \
    typedef typename boost::mpl::if_c< \
        boost::is_same< R, prefix ## _subrange >::value, \
        sake::concepts::Void, \
        sake::concepts::Range< prefix ## _subrange > \
    >::type prefix ## _subrange_concept; \
    BOOST_CONCEPT_ASSERT((prefix ## _subrange_concept));
    assert_subrange( void_void )
    assert_subrange( begin_void )
    assert_subrange( void_end )
    assert_subrange( begin_end )
#undef assert_subrange

    // Assert each subrange's iterators are convertible to the base range's
    // iterators.
#define assert_subrange( prefix ) \
    BOOST_MPL_ASSERT((boost_ext::is_convertible< \
        typename sake::range_iterator< prefix ## _subrange >::type, \
        iterator \
    >));
    assert_subrange( void_void )
    assert_subrange( begin_void )
    assert_subrange( void_end )
    assert_subrange( begin_end )
#undef assert_subrange

#endif // #ifdef SAKE_RANGE_CONCEPT_COMPLETE

    BOOST_CONCEPT_USAGE( Base )
    {

        assert_result< iterator >(traits_::begin(r));
        assert_result< iterator >(traits_::end(r));

#ifdef SAKE_RANGE_CONCEPT_COMPLETE

#define assert_introterminal( tag ) \
        assert_result< tag ## _iterator >(traits_::begin( \
            r, sake::tag ## _introterminal_tag())); \
        assert_result< tag ## _iterator >(traits_::end( \
            r, sake::tag ## _introterminal_tag()));
        assert_introterminal( null )
        assert_introterminal( begin_detect )
        assert_introterminal( begin_access )
        assert_introterminal( end_detect )
        assert_introterminal( end_access )
        assert_introterminal( begin_detect_end_detect )
        assert_introterminal( begin_access_end_detect )
        assert_introterminal( begin_detect_end_access )
        assert_introterminal( begin_access_end_access )
#undef assert_introterminal

#define assert_introterminal( from_tag, to_tag ) \
        assert_result< to_tag ## _iterator >(traits_::iter_at( \
            r, sake::declval< from_tag ## _iterator >(), \
            sake::to_tag ## _introterminal_tag()));
        assert_introterminal( null, null );
        assert_introterminal( null, begin_detect );
        assert_introterminal( null, end_detect );
        assert_introterminal( null, begin_access );
        assert_introterminal( null, end_access );
        assert_introterminal( null, begin_detect_end_detect );
        assert_introterminal( null, begin_access_end_detect );
        assert_introterminal( null, begin_detect_end_access );
        assert_introterminal( null, begin_access_end_access );
        assert_introterminal( begin_detect, null );
        assert_introterminal( begin_detect, begin_detect );
        assert_introterminal( begin_detect, begin_access );
        assert_introterminal( begin_detect, end_detect );
        assert_introterminal( begin_detect, end_access );
        assert_introterminal( begin_detect, begin_detect_end_detect );
        assert_introterminal( begin_detect, begin_access_end_detect );
        assert_introterminal( begin_detect, begin_detect_end_access );
        assert_introterminal( begin_detect, begin_access_end_access );
        assert_introterminal( begin_access, null );
        assert_introterminal( begin_access, begin_detect );
        assert_introterminal( begin_access, begin_access );
        assert_introterminal( begin_access, end_detect );
        assert_introterminal( begin_access, end_access );
        assert_introterminal( begin_access, begin_detect_end_detect );
        assert_introterminal( begin_access, begin_access_end_detect );
        assert_introterminal( begin_access, begin_detect_end_access );
        assert_introterminal( begin_access, begin_access_end_access );
        assert_introterminal( end_detect, null );
        assert_introterminal( end_detect, begin_detect );
        assert_introterminal( end_detect, begin_access );
        assert_introterminal( end_detect, end_detect );
        assert_introterminal( end_detect, end_access );
        assert_introterminal( end_detect, begin_detect_end_detect );
        assert_introterminal( end_detect, begin_access_end_detect );
        assert_introterminal( end_detect, begin_detect_end_access );
        assert_introterminal( end_detect, begin_access_end_access );
        assert_introterminal( end_access, null );
        assert_introterminal( end_access, begin_detect );
        assert_introterminal( end_access, begin_access );
        assert_introterminal( end_access, end_detect );
        assert_introterminal( end_access, end_access );
        assert_introterminal( end_access, begin_detect_end_detect );
        assert_introterminal( end_access, begin_access_end_detect );
        assert_introterminal( end_access, begin_detect_end_access );
        assert_introterminal( end_access, begin_access_end_access );
        assert_introterminal( begin_detect_end_detect, null );
        assert_introterminal( begin_detect_end_detect, begin_detect );
        assert_introterminal( begin_detect_end_detect, begin_access );
        assert_introterminal( begin_detect_end_detect, end_detect );
        assert_introterminal( begin_detect_end_detect, end_access );
        assert_introterminal( begin_detect_end_detect, begin_access_end_access );
        assert_introterminal( begin_detect_end_detect, begin_detect_end_detect );
        assert_introterminal( begin_detect_end_detect, begin_access_end_detect );
        assert_introterminal( begin_detect_end_detect, begin_detect_end_access );
        assert_introterminal( begin_access_end_detect, null );
        assert_introterminal( begin_access_end_detect, begin_detect );
        assert_introterminal( begin_access_end_detect, begin_access );
        assert_introterminal( begin_access_end_detect, end_detect );
        assert_introterminal( begin_access_end_detect, end_access );
        assert_introterminal( begin_access_end_detect, begin_detect_end_detect );
        assert_introterminal( begin_access_end_detect, begin_detect_end_access );
        assert_introterminal( begin_access_end_detect, begin_access_end_detect );
        assert_introterminal( begin_access_end_detect, begin_access_end_access );
        assert_introterminal( begin_detect_end_access, null );
        assert_introterminal( begin_detect_end_access, begin_detect );
        assert_introterminal( begin_detect_end_access, begin_access );
        assert_introterminal( begin_detect_end_access, end_detect );
        assert_introterminal( begin_detect_end_access, end_access );
        assert_introterminal( begin_detect_end_access, begin_detect_end_detect );
        assert_introterminal( begin_detect_end_access, begin_access_end_detect );
        assert_introterminal( begin_detect_end_access, begin_detect_end_access );
        assert_introterminal( begin_detect_end_access, begin_access_end_access );
        assert_introterminal( begin_access_end_access, null );
        assert_introterminal( begin_access_end_access, begin_detect );
        assert_introterminal( begin_access_end_access, begin_access );
        assert_introterminal( begin_access_end_access, end_detect );
        assert_introterminal( begin_access_end_access, end_access );
        assert_introterminal( begin_access_end_access, begin_detect_end_detect );
        assert_introterminal( begin_access_end_access, begin_access_end_detect );
        assert_introterminal( begin_access_end_access, begin_detect_end_access );
        assert_introterminal( begin_access_end_access, begin_access_end_access );
#undef assert_introterminal

        assert_result< void_void_subrange >(traits_::sub(r,i,i));
        assert_result< begin_void_subrange >(traits_::sub(r, sake::_begin, i));
        assert_result< void_end_subrange >(traits_::sub(r, i, sake::_end));
        assert_result< begin_end_subrange >(traits_::sub(r, sake::_begin, sake::_end));

#endif // #ifdef SAKE_RANGE_CONCEPT_COMPLETE

        assert_result< bool >(traits_::empty(r));
    }

private:
    R r;
    iterator const i;

    template< class T >
    void assert_result(T);

    SAKE_SUPPRESS_WARNING_UNINIITIALIZED_IN_CONCEPT_CHECKING_CLASS( Base )
};

} // namespace private_

} // namespace concepts

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_CONCEPTS_PRIVATE_BASE_HPP
