/*******************************************************************************
 * sake/core/range/begin.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * range::begin(R&& r, Introterminal = null_introterminal_tag())
 *     -> range_forward_iterator< R, Introterminal >::type
 * range::cbegin(R const & r, Introterminal = null_introterminal_tag())
 *     -> range_iterator< R const, Introterminal >::type
 * struct range::functional::begin
 * struct range::functional::cbegin
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_BEGIN_HPP
#define SAKE_CORE_RANGE_BEGIN_HPP

#include <boost/config.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/add_const_remove_reference.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/config.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/range/forward_traits.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace range
{

namespace result_of
{

template< class R, class Introterminal = sake::null_introterminal_tag >
class begin
{
    typedef typename boost_ext::
        remove_qualifiers< Introterminal >::type introterminal;
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        introterminal, sake::null_introterminal_tag >::value));
public:
    typedef typename sake::range_forward_iterator<
        R, introterminal >::type type;
};

template< class R, class Introterminal = sake::null_introterminal_tag >
class cbegin
{
    typedef typename boost_ext::add_const_remove_reference<R>::type range_;
    typedef typename boost_ext::
        remove_qualifiers< Introterminal >::type introterminal;
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        introterminal, sake::null_introterminal_tag >::value));
public:
    typedef typename sake::range_iterator< range_, introterminal >::type type;
};

} // namespace result_of

namespace functional
{

struct begin
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::range::result_of::begin, (1,2) )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    typename result< begin ( R ) >::type
    operator()(R&& r) const
    { return sake::range_forward_traits<R>::begin(sake::forward<R>(r)); }

    template< class R, class Introterminal >
    typename result< begin ( R, Introterminal ) >::type
    operator()(R&& r, Introterminal) const
    {
        return sake::range_forward_traits<R>::begin(
            sake::forward<R>(r), Introterminal());
    }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    typename result< begin ( R& ) >::type
    operator()(R& r) const
    { return sake::range_forward_traits< R& >::begin(r); }

    template< class R, class Introterminal >
    typename result< begin ( R&, Introterminal ) >::type
    operator()(R& r, Introterminal) const
    { return sake::range_forward_traits< R& >::begin(r, Introterminal()); }

    template< class R >
    typename result< begin ( R const & ) >::type
    operator()(R const & r) const
    { return sake::range_forward_traits< R const & >::begin(r); }

    template< class R, class Introterminal >
    typename result< begin ( R const &, Introterminal ) >::type
    operator()(R const & r, Introterminal) const
    { return sake::range_forward_traits< R const & >::begin(r, Introterminal()); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

struct cbegin
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::range::result_of::cbegin, (1,2) )

    template< class R >
    typename result< cbegin ( R ) >::type
    operator()(R const & r) const
    { return sake::range_traits< R const >::begin(r); }

    template< class R, class Introterminal >
    typename result< cbegin ( R, Introterminal ) >::type
    operator()(R const & r, Introterminal) const
    { return sake::range_traits< R const >::begin(r, Introterminal()); }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace begin_adl_barrier
{
    sake::range::functional::begin const begin = { };
    sake::range::functional::cbegin const cbegin = { };
}
using namespace begin_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::range::functional::begin const begin = { };
sake::range::functional::cbegin const cbegin = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_BEGIN_HPP
