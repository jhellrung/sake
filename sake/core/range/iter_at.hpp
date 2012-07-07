/*******************************************************************************
 * sake/core/range/iter_at.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * range::iter_at(R&& r, T const & x, Introterminal)
 *     -> range_forward_iterator< R, Introterminal >::type
 * range::citer_at(R const & r, T const & x, Introterminal)
 *     -> range_iterator< R const, Introterminal >::type
 * struct range::functional::iter_at
 * struct range::functional::citer_at
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_ITER_AT_HPP
#define SAKE_CORE_RANGE_ITER_AT_HPP

#include <boost/config.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/add_const_remove_reference.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/config.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/range/forward_traits.hpp>
#include <sake/core/range/private/iter_at_static_assert_cond.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace range
{

namespace result_of
{

template< class R, class T, class Introterminal >
class iter_at
{
    typedef typename boost_ext::
        remove_qualifiers< Introterminal >::type introterminal;
    BOOST_STATIC_ASSERT((
        sake::range::private_::iter_at_static_assert_cond<R,T>::value));
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        introterminal, sake::null_introterminal_tag >::value));
public:
    typedef typename sake::range_forward_iterator< R, introterminal >::type type;
};

template< class R, class T, class Introterminal >
class citer_at
{
    typedef typename boost_ext::add_const_remove_reference<R>::type range_;
    typedef typename boost_ext::
        remove_qualifiers< Introterminal >::type introterminal;
    BOOST_STATIC_ASSERT((sake::range::private_::
        iter_at_static_assert_cond< range_&, T >::value));
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        introterminal, sake::null_introterminal_tag >::value));
public:
    typedef typename sake::range_iterator< range_, introterminal >::type type;
};

} // namespace result_of

namespace functional
{

struct iter_at
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::range::result_of::iter_at, 3 )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R, class T, class Introterminal >
    typename result< iter_at ( R, T, Introterminal ) >::type
    operator()(R&& r, T const & x, Introterminal) const
    {
        return sake::range_forward_traits<R>::iter_at(
            sake::forward<R>(r), x, Introterminal());
    }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R, class T, class Introterminal >
    typename result< iter_at ( R&, T, Introterminal ) >::type
    operator()(R& r, T const & x, Introterminal) const
    {
        return sake::range_forward_traits< R& >::iter_at(
            r, x, Introterminal());
    }

    template< class R, class T, class Introterminal >
    typename result< iter_at ( R const &, T, Introterminal ) >::type
    operator()(R const & r, T const & x, Introterminal) const
    {
        return sake::range_forward_traits< R const & >::iter_at(
            r, x, Introterminal());
    }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

struct citer_at
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::range::result_of::citer_at, 3 )

    template< class R, class T, class Introterminal >
    typename result< citer_at ( R, T, Introterminal ) >::type
    operator()(R const & r, T const & x, Introterminal) const
    { return sake::range_traits< R const >::iter_at(r, x, Introterminal()); }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace iter_at_adl_barrier
{
    sake::range::functional::iter_at const iter_at = { };
    sake::range::functional::citer_at const citer_at = { };
}
using namespace iter_at_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::range::functional::iter_at const iter_at = { };
sake::range::functional::citer_at const citer_at = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_ITER_AT_HPP
