/*******************************************************************************
 * sake/core/data_structures/pair/private/value_constructor.ipp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifdef SAKE_PAIR_INCLUDE_HEADERS

#include <boost/config.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/curry_quote.hpp>

#include <sake/core/data_structures/private/value_constructor_enable.hpp>
#include <sake/core/functional/construct.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/utility/emplacer/constructible.hpp>
#include <sake/core/utility/emplacer/emplacer.hpp>

#endif // #ifdef SAKE_PAIR_INCLUDE_HEADERS

#ifdef SAKE_PAIR_DEFINE_MEMBERS

private:
    template< class U0, class U1 >
    struct value_constructor_enable
        : boost_ext::mpl::and2<
              sake::data_structures_private::value_constructor_enable< nocv0_type, U0 >,
              sake::data_structures_private::value_constructor_enable< nocv1_type, U1 >
          >
    { };
    template< class U0, class U1 >
    struct value_constructor_enabler
        : boost::enable_if_c< value_constructor_enable< U0, U1 >::value >
    { };
public:

    template< class V0, class V1 >
    pair(sake::emplacer< V0 ( ) >, sake::emplacer< V1 ( ) >,
        typename value_constructor_enabler<
            sake::emplacer< V0 ( ) >, sake::emplacer< V1 ( ) > >::type* = 0)
    { }

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U0, class V1 >
    pair(U0&& x0, sake::emplacer< V1 ( ) >,
        typename value_constructor_enabler< U0, sake::emplacer< V1 ( ) > >::type* = 0)
        : first(sake::emplacer_constructible< nocv0_type >(sake::forward< U0 >(x0)))
    { }

    template< class V0, class U1 >
    pair(sake::emplacer< void ( ) >, U1&& x1,
        typename value_constructor_enabler< sake::emplacer< V0 ( ) >, U1 >::type* = 0)
        : second(sake::emplacer_constructible< nocv1_type >(sake::forward< U1 >(x1)))
    { }

    template< class U0, class U1 >
    pair(U0&& x0, U1&& x1,
        typename value_constructor_enabler< U0, U1 >::type* = 0)
        : first(sake::emplacer_constructible< nocv0_type >(sake::forward< U0 >(x0))),
          second(sake::emplacer_constructible< nocv1_type >(sake::forward< U1 >(x1)))
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    typedef sake::rv_sink_traits1<
        nocv0_type,
        typename boost_ext::mpl::curry_quote2<
            sake::data_structures_private::value_constructor_enable
        >::apply< nocv0_type >::type
    > value0_constructor_rv_sink_traits;
    typedef typename value0_constructor_rv_sink_traits::template
        default_< sake::functional::construct< nocv0_type > >
        value0_constructor_rv_sink_default_type;
    typedef sake::rv_sink_traits1<
        nocv1_type,
        typename boost_ext::mpl::curry_quote2<
            sake::data_structures_private::value_constructor_enable
        >::apply< nocv1_type >::type
    > value1_constructor_rv_sink_traits;
    typedef typename value1_constructor_rv_sink_traits::template
        default_< sake::functional::construct< nocv1_type > >
        value1_constructor_rv_sink_default_type;
public:

    // lvalues + explicit movable rvalues
    template< class U0, class V1 >
    pair(U0& x0, sake::emplacer< V1 ( ) >,
        typename boost::enable_if_c< boost_ext::mpl::and2<
            typename value0_constructor_rv_sink_traits::template ref_enable< U0 >,
            sake::data_structures_private::value_constructor_enable<
                nocv1_type, sake::emplacer< V1 ( ) > >
        >::value >::type* = 0)
        : first(sake::emplacer_constructible< nocv0_type >(x0))
    { }
    template< class V0, class U1 >
    pair(sake::emplacer< V0 ( ) >, U1& x1,
        typename boost::enable_if_c< boost_ext::mpl::and2<
            sake::data_structures_private::value_constructor_enable<
                nocv0_type, sake::emplacer< V0 ( ) > >,
            typename value1_constructor_rv_sink_traits::template ref_enable< U1 >
        >::value >::type* = 0)
        : second(sake::emplacer_constructible< nocv1_type >(x1))
    { }

    // T0/T1 rvalues
    template< class V1 >
    pair(
        typename value0_constructor_rv_sink_traits::primary_type x0,
        sake::emplacer< V1 ( ) >,
        typename sake::data_structures_private::value_constructor_enabler<
            nocv1_type, sake::emplacer< V1 ( ) > >::type* = 0)
        : first(sake::move(x0.value))
    { }
    template< class V0 >
    pair(
        sake::emplacer< V0 ( ) >,
        typename value1_constructor_rv_sink_traits::primary_type x1,
        typename sake::data_structures_private::value_constructor_enabler<
            nocv0_type, sake::emplacer< V0 ( ) > >::type* = 0)
        : second(sake::move(x1.value))
    { }

    // movable implicit rvalues
    template< class V1 >
    pair(
        value0_constructor_rv_sink_default_type x0,
        sake::emplacer< V1 ( ) >,
        typename sake::data_structures_private::value_constructor_enabler<
            nocv1_type, sake::emplacer< V1 ( ) > >::type* = 0)
        : first(x0())
    { }
    template< class V0 >
    pair(
        sake::emplacer< V0 ( ) >,
        value1_constructor_rv_sink_default_type x1,
        typename sake::data_structures_private::value_constructor_enabler<
            nocv0_type, sake::emplacer< V0 ( ) > >::type* = 0)
        : second(x1())
    { }

    // const lvalues + non-movable rvalues
    template< class U0, class V1 >
    pair(U0 const & x0, sake::emplacer< V1 ( ) >,
        typename boost::enable_if_c< boost_ext::mpl::and2<
            typename value0_constructor_rv_sink_traits::template cref_enable< U0 >,
            sake::data_structures_private::value_constructor_enable<
                nocv1_type, sake::emplacer< V1 ( ) > >
        >::value >::type* = 0)
        : first(sake::emplacer_constructible< nocv0_type >(x0))
    { }
    template< class V0, class U1 >
    pair(sake::emplacer< V0 ( ) >, U1 const & x1,
        typename boost::enable_if_c< boost_ext::mpl::and2<
            sake::data_structures_private::value_constructor_enable<
                nocv0_type, sake::emplacer< V0 ( ) > >,
            typename value1_constructor_rv_sink_traits::template cref_enable< U1 >
        >::value >::type* = 0)
        : second(sake::emplacer_constructible< nocv1_type >(x1))
    { }

    // Okay, let's do all permutations of the above...
    template< class U0, class U1 >
    pair(
        U0& x0,
        U1& x1,
        typename boost::enable_if_c< boost_ext::mpl::and2<
            typename value0_constructor_rv_sink_traits::template ref_enable< U0 >,
            typename value1_constructor_rv_sink_traits::template ref_enable< U1 >
        >::value >::type* = 0)
        : first(sake::emplacer_constructible< nocv0_type >(x0)),
          second(sake::emplacer_constructible< nocv1_type >(x1))
    { }
    template< class U0 >
    pair(
        U0& x0,
        typename value1_constructor_rv_sink_traits::primary_type x1,
        typename value0_constructor_rv_sink_traits::template ref_enabler< U0 >::type* = 0)
        : first(sake::emplacer_constructible< nocv0_type >(x0)),
          second(sake::move(x1.value))
    { }
    template< class U0 >
    pair(
        U0& x0,
        value1_constructor_rv_sink_default_type x1,
        typename value0_constructor_rv_sink_traits::template ref_enabler< U0 >::type* = 0)
        : first(sake::emplacer_constructible< nocv0_type >(x0)),
          second(x1())
    { }
    template< class U0, class U1 >
    pair(
        U0& x0,
        U1 const & x1,
        typename boost::enable_if_c< boost_ext::mpl::and2<
            typename value0_constructor_rv_sink_traits::template ref_enable< U0 >,
            typename value1_constructor_rv_sink_traits::template cref_enable< U1 >
        >::value >::type* = 0)
        : first(sake::emplacer_constructible< nocv0_type >(x0)),
          second(sake::emplacer_constructible< nocv1_type >(x1))
    { }

    template< class U1 >
    pair(
        typename value0_constructor_rv_sink_traits::primary_type x0,
        U1& x1,
        typename value1_constructor_rv_sink_traits::template ref_enabler< U1 >::type* = 0)
        : first(sake::move(x0.value)),
          second(sake::emplacer_constructible< nocv1_type >(x1))
    { }
    pair(
        typename value0_constructor_rv_sink_traits::primary_type x0,
        typename value1_constructor_rv_sink_traits::primary_type x1)
        : first(sake::move(x0.value)),
          second(sake::move(x1.value))
    { }
    pair(
        typename value0_constructor_rv_sink_traits::primary_type x0,
        value1_constructor_rv_sink_default_type x1)
        : first(sake::move(x0.value)),
          second(x1())
    { }
    template< class U1 >
    pair(
        typename value0_constructor_rv_sink_traits::primary_type x0,
        U1 const & x1,
        typename value1_constructor_rv_sink_traits::template cref_enabler< U1 >::type* = 0)
        : first(sake::move(x0.value)),
          second(sake::emplacer_constructible< nocv1_type >(x1))
    { }

    template< class U1 >
    pair(
        value0_constructor_rv_sink_default_type x0,
        U1& x1,
        typename value1_constructor_rv_sink_traits::template ref_enabler< U1 >::type* = 0)
        : first(x0()),
          second(sake::emplacer_constructible< nocv1_type >(x1))
    { }
    pair(
        value0_constructor_rv_sink_default_type x0,
        typename value1_constructor_rv_sink_traits::primary_type x1)
        : first(x0()),
          second(sake::move(x1.value))
    { }
    pair(
        value0_constructor_rv_sink_default_type x0,
        value1_constructor_rv_sink_default_type x1)
        : first(x0()),
          second(x1())
    { }
    template< class U1 >
    pair(
        value0_constructor_rv_sink_default_type x0,
        U1 const & x1,
        typename value1_constructor_rv_sink_traits::template cref_enabler< U1 >::type* = 0)
        : first(x0()),
          second(sake::emplacer_constructible< nocv1_type >(x1))
    { }

    template< class U0, class U1 >
    pair(
        U0 const & x0,
        U1& x1,
        typename boost::enable_if_c< boost_ext::mpl::and2<
            typename value0_constructor_rv_sink_traits::template cref_enable< U0 >,
            typename value1_constructor_rv_sink_traits::template ref_enable< U1 >
        >::value >::type* = 0)
        : first(sake::emplacer_constructible< nocv0_type >(x0)),
          second(sake::emplacer_constructible< nocv1_type >(x1))
    { }
    template< class U0 >
    pair(
        U0 const & x0,
        typename value1_constructor_rv_sink_traits::primary_type x1,
        typename value0_constructor_rv_sink_traits::template cref_enabler< U0 >::type* = 0)
        : first(sake::emplacer_constructible< nocv0_type >(x0)),
          second(sake::move(x1.value))
    { }
    template< class U0 >
    pair(
        U0 const & x0,
        value1_constructor_rv_sink_default_type x1,
        typename value0_constructor_rv_sink_traits::template cref_enabler< U0 >::type* = 0)
        : first(sake::emplacer_constructible< nocv0_type >(x0)),
          second(x1())
    { }
    template< class U0, class U1 >
    pair(
        U0 const & x0,
        U1 const & x1,
        typename boost::enable_if_c< boost_ext::mpl::and2<
            typename value0_constructor_rv_sink_traits::template cref_enable< U0 >,
            typename value1_constructor_rv_sink_traits::template cref_enable< U1 >
        >::value >::type* = 0)
        : first(sake::emplacer_constructible< nocv0_type >(x0)),
          second(sake::emplacer_constructible< nocv1_type >(x1))
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#endif // #ifdef SAKE_PAIR_DEFINE_MEMBERS
