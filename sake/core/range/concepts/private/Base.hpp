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
#include <boost/statiC_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/config.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/concepts/Iterator.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/range/concepts/private/DistanceBase.hpp>
#include <sake/core/range/concepts/private/SizeBase.hpp>
#include <sake/core/range/traits.hpp>
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
    BOOST_CONCEPT_ASSERT((sake::concepts::Iterator< iterator >));

    SAKE_USING_TYPEDEF( typename traits_, reference );
    SAKE_USING_TYPEDEF( typename traits_, traversal );
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        traversal, boost::single_pass_traversal_tag >::value));

    template< class Introversal >
    struct iterator_with
        : traits_::template iterator_with< Introversal >
    { };

#define assert_introversal( tag ) \
    typedef typename iterator_with< \
        sake::tag ## _introversal_tag >::type tag ## _iterator; \
    BOOST_STATIC_ASSERT((boost_ext::is_convertible< \
        typename sake::iterator_introversal< tag ## _iterator >::type, \
        sake::tag ## _introversal_tag \
    >::value)); \
    BOOST_CONCEPT_ASSERT((sake::concepts::Iterator< tag ## _iterator >));
    assert_introversal( null )
    assert_introversal( begin_detect )
    assert_introversal( begin_access )
    assert_introversal( end_detect )
    assert_introversal( end_access )
    assert_introversal( begin_detect_end_detect )
    assert_introversal( begin_access_end_detect )
    assert_introversal( begin_detect_end_access )
    assert_introversal( begin_access_end_access )
#undef assert_introversal

    BOOST_STATIC_ASSERT((boost::is_same< null_iterator, iterator >::value));

#define assert_introversal( from_tag, to_tag ) \
    BOOST_STATIC_ASSERT((boost_ext::is_convertible< \
        from_tag ## _iterator, to_tag ## _iterator >::value));
    assert_introversal( begin_detect, null )
    assert_introversal( begin_access, null )
    assert_introversal( begin_access, begin_detect )
    assert_introversal( end_detect, null )
    assert_introversal( end_access, null )
    assert_introversal( end_access, end_detect )
    assert_introversal( begin_detect_end_detect, null )
    assert_introversal( begin_detect_end_detect, begin_detect )
    assert_introversal( begin_detect_end_detect, end_detect )
    assert_introversal( begin_access_end_detect, null )
    assert_introversal( begin_access_end_detect, begin_detect )
    assert_introversal( begin_access_end_detect, begin_access )
    assert_introversal( begin_access_end_detect, end_detect )
    assert_introversal( begin_access_end_detect, begin_detect_end_detect )
    assert_introversal( begin_detect_end_access, null )
    assert_introversal( begin_detect_end_access, begin_detect )
    assert_introversal( begin_detect_end_access, end_detect )
    assert_introversal( begin_detect_end_access, end_access )
    assert_introversal( begin_detect_end_access, begin_detect_end_detect )
    assert_introversal( begin_access_end_access, null )
    assert_introversal( begin_access_end_access, begin_detect )
    assert_introversal( begin_access_end_access, begin_access )
    assert_introversal( begin_access_end_access, end_detect )
    assert_introversal( begin_access_end_access, end_access )
    assert_introversal( begin_access_end_access, begin_detect_end_detect )
    assert_introversal( begin_access_end_access, begin_access_end_detect )
    assert_introversal( begin_access_end_access, begin_detect_end_access )
#undef assert_introversal

#define assert_introversal( from_tag, to_tag ) \
    BOOST_STATIC_ASSERT((boost::is_same< \
        typename sake::iterator_relax< \
            from_tag ## _iterator, \
            sake::to_tag ## _introversal_tag \
        >::type, \
        to_tag ## _iterator \
    >::value));
    assert_introversal( begin_detect, null )
    assert_introversal( begin_access, null )
    assert_introversal( begin_access, begin_detect )
    assert_introversal( end_detect, null )
    assert_introversal( end_access, null )
    assert_introversal( end_access, end_detect )
    assert_introversal( begin_detect_end_detect, null )
    assert_introversal( begin_detect_end_detect, begin_detect )
    assert_introversal( begin_detect_end_detect, end_detect )
    assert_introversal( begin_access_end_detect, null )
    assert_introversal( begin_access_end_detect, begin_detect )
    assert_introversal( begin_access_end_detect, begin_access )
    assert_introversal( begin_access_end_detect, end_detect )
    assert_introversal( begin_access_end_detect, begin_detect_end_detect )
    assert_introversal( begin_detect_end_access, null )
    assert_introversal( begin_detect_end_access, begin_detect )
    assert_introversal( begin_detect_end_access, end_detect )
    assert_introversal( begin_detect_end_access, end_access )
    assert_introversal( begin_detect_end_access, begin_detect_end_detect )
    assert_introversal( begin_access_end_access, null )
    assert_introversal( begin_access_end_access, begin_detect )
    assert_introversal( begin_access_end_access, begin_access )
    assert_introversal( begin_access_end_access, end_detect )
    assert_introversal( begin_access_end_access, end_access )
    assert_introversal( begin_access_end_access, begin_detect_end_detect )
    assert_introversal( begin_access_end_access, begin_access_end_detect )
    assert_introversal( begin_access_end_access, begin_detect_end_access )
#undef assert_introversal

    BOOST_CONCEPT_USAGE( Base )
    {

#define assert_introversal( tag ) \
        assert_result< tag ## _iterator >(traits_::iter_at( \
            r, sake::_begin, sake::tag ## _introversal_tag())); \
        assert_result< tag ## _iterator >(traits_::iter_at( \
            r, sake::_end, sake::tag ## _introversal_tag()));
        assert_introversal( null )
        assert_introversal( begin_detect )
        assert_introversal( begin_access )
        assert_introversal( end_detect )
        assert_introversal( end_access )
        assert_introversal( begin_detect_end_detect )
        assert_introversal( begin_access_end_detect )
        assert_introversal( begin_detect_end_access )
        assert_introversal( begin_access_end_access )
#undef assert_introversal

#define assert_introversal( from_tag, to_tag ) \
        assert_result< to_tag ## _iterator >(traits_::iter_at( \
            r, sake::declval< from_tag ## _iterator >(), \
            sake::to_tag ## _introversal_tag()));
        assert_introversal( null, null );
        assert_introversal( null, begin_detect );
        assert_introversal( null, begin_access );
        assert_introversal( null, end_detect );
        assert_introversal( null, end_access );
        assert_introversal( null, begin_detect_end_detect );
        assert_introversal( null, begin_access_end_detect );
        assert_introversal( null, begin_detect_end_access );
        assert_introversal( null, begin_access_end_access );
        assert_introversal( begin_detect, null );
        assert_introversal( begin_detect, begin_detect );
        assert_introversal( begin_detect, begin_access );
        assert_introversal( begin_detect, end_detect );
        assert_introversal( begin_detect, end_access );
        assert_introversal( begin_detect, begin_detect_end_detect );
        assert_introversal( begin_detect, begin_access_end_detect );
        assert_introversal( begin_detect, begin_detect_end_access );
        assert_introversal( begin_detect, begin_access_end_access );
        assert_introversal( begin_access, null );
        assert_introversal( begin_access, begin_detect );
        assert_introversal( begin_access, begin_access );
        assert_introversal( begin_access, end_detect );
        assert_introversal( begin_access, end_access );
        assert_introversal( begin_access, begin_detect_end_detect );
        assert_introversal( begin_access, begin_access_end_detect );
        assert_introversal( begin_access, begin_detect_end_access );
        assert_introversal( begin_access, begin_access_end_access );
        assert_introversal( end_detect, null );
        assert_introversal( end_detect, begin_detect );
        assert_introversal( end_detect, begin_access );
        assert_introversal( end_detect, end_detect );
        assert_introversal( end_detect, end_access );
        assert_introversal( end_detect, begin_detect_end_detect );
        assert_introversal( end_detect, begin_access_end_detect );
        assert_introversal( end_detect, begin_detect_end_access );
        assert_introversal( end_detect, begin_access_end_access );
        assert_introversal( end_access, null );
        assert_introversal( end_access, begin_detect );
        assert_introversal( end_access, begin_access );
        assert_introversal( end_access, end_detect );
        assert_introversal( end_access, end_access );
        assert_introversal( end_access, begin_detect_end_detect );
        assert_introversal( end_access, begin_access_end_detect );
        assert_introversal( end_access, begin_detect_end_access );
        assert_introversal( end_access, begin_access_end_access );
        assert_introversal( begin_detect_end_detect, null );
        assert_introversal( begin_detect_end_detect, begin_detect );
        assert_introversal( begin_detect_end_detect, begin_access );
        assert_introversal( begin_detect_end_detect, end_detect );
        assert_introversal( begin_detect_end_detect, end_access );
        assert_introversal( begin_detect_end_detect, begin_detect_end_detect );
        assert_introversal( begin_detect_end_detect, begin_access_end_detect );
        assert_introversal( begin_detect_end_detect, begin_detect_end_access );
        assert_introversal( begin_detect_end_detect, begin_access_end_access );
        assert_introversal( begin_access_end_detect, null );
        assert_introversal( begin_access_end_detect, begin_detect );
        assert_introversal( begin_access_end_detect, begin_access );
        assert_introversal( begin_access_end_detect, end_detect );
        assert_introversal( begin_access_end_detect, end_access );
        assert_introversal( begin_access_end_detect, begin_detect_end_detect );
        assert_introversal( begin_access_end_detect, begin_access_end_detect );
        assert_introversal( begin_access_end_detect, begin_detect_end_access );
        assert_introversal( begin_access_end_detect, begin_access_end_access );
        assert_introversal( begin_detect_end_access, null );
        assert_introversal( begin_detect_end_access, begin_detect );
        assert_introversal( begin_detect_end_access, begin_access );
        assert_introversal( begin_detect_end_access, end_detect );
        assert_introversal( begin_detect_end_access, end_access );
        assert_introversal( begin_detect_end_access, begin_detect_end_detect );
        assert_introversal( begin_detect_end_access, begin_access_end_detect );
        assert_introversal( begin_detect_end_access, begin_detect_end_access );
        assert_introversal( begin_detect_end_access, begin_access_end_access );
        assert_introversal( begin_access_end_access, null );
        assert_introversal( begin_access_end_access, begin_detect );
        assert_introversal( begin_access_end_access, begin_access );
        assert_introversal( begin_access_end_access, end_detect );
        assert_introversal( begin_access_end_access, end_access );
        assert_introversal( begin_access_end_access, begin_detect_end_detect );
        assert_introversal( begin_access_end_access, begin_access_end_detect );
        assert_introversal( begin_access_end_access, begin_detect_end_access );
        assert_introversal( begin_access_end_access, begin_access_end_access );
#undef assert_introversal

        assert_result< bool >(traits_::empty(r));
    }

private:
    R r;

    template< class T >
    void assert_result(T);

    SAKE_SUPPRESS_WARNING_UNINIITIALIZED_IN_CONCEPT_CHECKING_CLASS( SizeBase )
};

} // namespace private_

} // namespace concepts

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_CONCEPTS_PRIVATE_BASE_HPP
