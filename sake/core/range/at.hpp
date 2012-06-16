/*******************************************************************************
 * sake/core/range/at.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * range::at(R&& r, I i) -> sake::range_forward_reference<R>::type
 * range::cat(R const & r, I i) -> sake::range_reference< R const >::type
 * struct range::functional::at
 * struct range::functional::cat
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_AT_HPP
#define SAKE_CORE_RANGE_AT_HPP

#include <boost/config.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/config.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/range/forward_traits.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace range
{

namespace result_of
{

template< class R, class I >
class at
{
    typedef typename boost_ext::remove_qualifiers<R>::type range_;
    typedef typename boost_ext::remove_qualifiers<I>::type index;
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        index, typename sake::range_difference< range_ >::type >::value));
public:
    typedef typename sake::range_forward_reference<R>::type type;
};

template< class R >
class cat
{
    typedef typename boost_ext::remove_qualifiers<R>::type range_;
    typedef typename boost_ext::remove_qualifiers<I>::type index;
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        index, typename sake::range_difference< range_ >::type >::value));
public:
    typedef typename sake::range_reference< range_ const >::type type;
};

} // namespace result_of

namespace functional
{

struct at
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::range::result_of::at, 2 )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R, class I >
    typename sake::range::result_of::at<R,I>::type
    operator()(R&& r, I const i) const
    { return sake::range_forward_traits<R>::at(sake::forward<R>(r), i); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R, class I >
    typename sake::range::result_of::at< R&, I >::type
    operator()(R& r, I const i) const
    { return sake::range_forward_traits< R& >::at(r); }

    template< class R, class I >
    typename sake::range::result_of::at< R const &, I >::type
    operator()(R const & r, I const i) const
    { return sake::range_forward_traits< R const & >::at(r); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

struct cat
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::range::result_of::cat, 2 )

    template< class R, class I >
    typename sake::range::result_of::cat< R const &, I >::type
    operator()(R const & r, I const i) const
    { return sake::range_traits< R const >::at(r,i); }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace at_adl_barrier
{
    sake::range::functional::at const at = { };
    sake::range::functional::cat const cat = { };
}
using namespace at_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::range::functional::at const at = { };
sake::range::functional::cat const cat = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_AT_HPP
