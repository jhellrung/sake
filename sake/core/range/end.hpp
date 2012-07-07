/*******************************************************************************
 * sake/core/range/end.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * range::end(R&& r, Introterminal = null_introterminal_tag())
 *     -> range_forward_iterator< R, Introterminal >::type
 * range::cend(R const & r, Introterminal = null_introterminal_tag())
 *     -> range_iterator< R const, Introterminal >::type
 * struct range::functional::end
 * struct range::functional::cend
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_END_HPP
#define SAKE_CORE_RANGE_END_HPP

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
class end
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
class cend
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

struct end
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::range::result_of::end, (1,2) )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    typename result< end ( R ) >::type
    operator()(R&& r) const
    { return sake::range_forward_traits<R>::end(sake::forward<R>(r)); }

    template< class R, class Introterminal >
    typename result< end ( R, Introterminal ) >::type
    operator()(R&& r, Introterminal) const
    {
        return sake::range_forward_traits<R>::end(
            sake::forward<R>(r), Introterminal());
    }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    typename result< end ( R& ) >::type
    operator()(R& r) const
    { return sake::range_forward_traits< R& >::end(r); }

    template< class R, class Introterminal >
    typename result< end ( R&, Introterminal ) >::type
    operator()(R& r, Introterminal) const
    { return sake::range_forward_traits< R& >::end(r, Introterminal()); }

    template< class R >
    typename result< end ( R const & ) >::type
    operator()(R const & r) const
    { return sake::range_forward_traits< R const & >::end(r); }

    template< class R, class Introterminal >
    typename result< end ( R const &, Introterminal ) >::type
    operator()(R const & r, Introterminal) const
    { return sake::range_forward_traits< R const & >::end(r, Introterminal()); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

struct cend
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::range::result_of::cend, (1,2) )

    template< class R >
    typename result< cend ( R ) >::type
    operator()(R const & r) const
    { return sake::range_traits< R const >::end(r); }

    template< class R, class Introterminal >
    typename result< cend ( R, Introterminal ) >::type
    operator()(R const & r, Introterminal) const
    { return sake::range_traits< R const >::end(r, Introterminal()); }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace end_adl_barrier
{
    sake::range::functional::end const end = { };
    sake::range::functional::cend const cend = { };
}
using namespace end_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::range::functional::end const end = { };
sake::range::functional::cend const cend = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_END_HPP
