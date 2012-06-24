/*******************************************************************************
 * sake/core/iterator/at_ip.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * iterator::at_ip(I& i, T const & x) -> I&
 * struct iterator::functional::at_ip
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_AT_IP_HPP
#define SAKE_CORE_ITERATOR_AT_IP_HPP

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/is_lvalue_reference_to_nonconst.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/config.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/private/is_interoperable.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace iterator
{

namespace result_of
{

template< class I, class T >
class at_ip
{
    BOOST_STATIC_ASSERT((boost_ext::is_lvalue_reference_to_nonconst<I>::value));
    typedef typename boost_ext::remove_qualifiers<I>::type iterator_;
    typedef typename boost_ext::remove_qualifiers<T>::type noqual_type;
    typedef typename sake::iterator_introversal< iterator_ >::type introversal;
    BOOST_STATIC_ASSERT((boost_ext::mpl::or3<
        boost_ext::mpl::and2<
            boost::is_same< noqual_type, sake::begin_tag >,
            boost_ext::is_convertible<
                introversal, sake::begin_access_introversal_tag >
        >,
        boost_ext::mpl::and2<
            boost::is_same< noqual_type, sake::end_tag >,
            boost_ext::is_convertible<
                introversal, sake::end_access_introversal_tag >
        >,
        sake::iterator::private_::is_interoperable< iterator_, noqual_type >
    >::value));
public:
    typedef I type;
};

} // namespace result_of

namespace functional
{

struct at_ip
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::iterator::result_of::at_ip, 2 )

    template< class I, class T >
    typename sake::iterator::result_of::at_ip< I&, T const & >::type
    operator()(I& i, T const & x) const
    {
        sake::iterator_traits<I>::at_ip(i,x);
        return i;
    }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace at_ip_adl_barrier
{ sake::iterator::functional::at_ip const at_ip = { }; }
using namespace at_ip_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::iterator::functional::at_ip const at_ip = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_AT_IP_HPP
