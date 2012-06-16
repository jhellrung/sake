/*******************************************************************************
 * sake/core/range/end.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * range::end(R&& r, Introversal = null_introversal_tag())
 *     -> range_forward_iterator< R, Introversal >::type
 * range::cend(R const & r, Introversal = null_introversal_tag())
 *     -> range_iterator< R const, Introversal >::type
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

template< class R, class Introversal = sake::null_introversal_tag >
class end
{
    typedef typename boost_ext::remove_qualifiers< Introversal >::type introversal;
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        introversal, sake::null_introversal_tag >::value));
public:
    typedef typename sake::range_forward_iterator< R, introversal >::type type;
};

template< class R, class Introversal = sake::null_introversal_tag >
class cend
{
    typedef typename boost_ext::add_const_remove_reference<R>::type range_;
    typedef typename boost_ext::remove_qualifiers< Introversal >::type introversal;
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        introversal, sake::null_introversal_tag >::value));
public:
    typedef typename sake::range_iterator< range_, introversal >::type type;
};

} // namespace result_of

namespace functional
{

struct end
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::range::result_of::end, (1,2) )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    typename sake::range::result_of::end<R>::type
    operator()(R&& r) const
    { return operator()(sake::forward<R>(r), sake::null_introversal_tag()); }

    template< class R, class Introversal >
    typename sake::range::result_of::end< R, Introversal >::type
    operator()(R&& r, Introversal) const
    {
        return sake::range_forward_traits<R>::iter_at(
            sake::forward<R>(r), sake::_end, Introversal());
    }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    typename sake::range::result_of::end< R& >::type
    operator()(R& r) const
    { return operator()(r, sake::null_introversal_tag()); }

    template< class R, class Introversal >
    typename sake::range::result_of::end< R&, Introversal >::type
    operator()(R& r, Introversal) const
    {
        return sake::range_forward_traits< R& >::iter_at(
            r, sake::_end, Introversal());
    }

    template< class R >
    typename sake::range::result_of::end< R const & >::type
    operator()(R const & r) const
    { return operator()(r, sake::null_introversal_tag()); }

    template< class R, class Introversal >
    typename sake::range::result_of::end< R const &, Introversal >::type
    operator()(R const & r, Introversal) const
    {
        return sake::range_forward_traits< R const & >::iter_at(
            r, sake::_end, Introversal());
    }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

struct cend
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::range::result_of::cend, (1,2) )

    template< class R >
    typename sake::range::result_of::cend< R const & >::type
    operator()(R const & r) const
    { return operator()(r, sake::null_introversal_tag()); }

    template< class R, class Introversal >
    typename sake::range::result_of::cend< R const &, Introversal >::type
    operator()(R const & r, Introversal) const
    {
        return sake::range_traits< R const >::iter_at(
            r, sake::_end, Introversal());
    }
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
