/*******************************************************************************
 * sake/core/functional/forwarding/deduced_params.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * class forwarding::deduced_params< T, ChainedBase = void >
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_FORWARDING_DEDUCED_PARAMS_HPP
#define SAKE_CORE_FUNCTIONAL_FORWARDING_DEDUCED_PARAMS_HPP

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/map/map10.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/utility/result_of.hpp>

#include <sake/boost_ext/mpl/insert_if.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/mpl/result_type.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>

#include <sake/core/functional/forwarding/keyword.hpp>
#include <sake/core/introspection/has_type_result_type.hpp>
#include <sake/core/introspection/is_callable.hpp>
#include <sake/core/utility/template_keyword.hpp>

namespace sake
{

namespace forwarding
{

template< class T, class ChainedBase = void >
class deduced_params
    : public boost::mpl::insert<
          typename deduced_params< T, void >::type,
          sake::forwarding::keyword::chained_base< ChainedBase >
      >
{ };

template< class T >
class deduced_params< T, void >
{
    typedef sake::is_callable<
        typename boost_ext::add_reference<T>::type,
        void ( )
    > is_nullary_callable;

    typedef sake::is_callable<
        typename boost_ext::add_reference_add_const<T>::type,
        void ( )
    > is_nullary_const_callable;

    typedef typename boost_ext::remove_reference<
        typename boost::mpl::if_c<
            is_nullary_const_callable::value,
            typename boost::add_const<T>::type,
            T
        >::type
    >::type nullary_result_test_type;

    typedef sake::has_type_result_type<
        typename boost_ext::remove_reference<T>::type
    > has_type_result_type_;

public:
    typedef typename sake::lazy_insert_keyword_value_if_c<
        boost_ext::mpl::or2< has_type_result_type_, is_nullary_callable >::value,
        typename boost_ext::mpl::insert_if_c<
            is_nullary_const_callable::value,
            typename boost_ext::mpl::insert_if_c<
                is_nullary_callable::value,
                sake::forwarding::keyword::default_params,
                sake::forwarding::keyword::nullary_callable
            >::type,
            sake::forwarding::keyword::nullary_const_callable
        >::type,
        sake::forwarding::keyword::result,
        boost::mpl::eval_if_c<
            has_type_result_type_::value,
            boost_ext::mpl::result_type< typename boost_ext::remove_reference<T>::type >,
            boost::result_of< nullary_result_test_type ( ) >
        >
    >::type type;
};

} // namespace forwarding

} // namespace sake

#endif // #ifndef SAKE_CORE_FUNCTIONAL_FORWARDING_DEDUCED_PARAMS_HPP
