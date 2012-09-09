/*******************************************************************************
 * unit_test/test/core/keyword/keyword_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/mpl/assert.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/vector/vector10.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/utility/result_of.hpp>

#include <sake/core/keyword/arg_packer.hpp>
#include <sake/core/keyword/keyword.hpp>
#include <sake/core/keyword/param_spec.hpp>
#include <sake/core/keyword/tagged_value.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

namespace sake_unit_test
{

namespace
{

struct A { };
struct B { };
struct C { };
struct D { };

namespace keyword
{

SAKE_KEYWORD( a )
SAKE_KEYWORD( b )
SAKE_KEYWORD( c )
SAKE_KEYWORD( d )

} // namespace keyword

typedef boost::mpl::vector4<
    sake::keyword::required<
        keyword::tag::a,
        sake::boost_ext::is_convertible< boost::mpl::_1, A >
    >,
    sake::keyword::optional<
        keyword::tag::b,
        sake::boost_ext::is_convertible< boost::mpl::_1, B >
    >,
    sake::keyword::required<
        sake::keyword::deduced< keyword::tag::c >,
        sake::boost_ext::is_convertible< boost::mpl::_1, C >
    >,
    sake::keyword::optional<
        sake::keyword::deduced< keyword::tag::d >,
        sake::boost_ext::is_convertible< boost::mpl::_1, D >
    >
> param_spec_types;

typedef sake::keyword::arg_packer< param_spec_types > arg_packer_type;

/*******************************************************************************
 * sake::keyword::satisfies test
 ******************************************************************************/

BOOST_MPL_ASSERT((sake::keyword::satisfies<
    boost::mpl::vector2<
        sake::keyword::tagged_value< keyword::tag::a, A const & >,
        sake::keyword::tagged_value< keyword::tag::c, C const & >
    >,
    param_spec_types
>));
BOOST_MPL_ASSERT((sake::keyword::satisfies<
    boost::mpl::vector3<
        sake::keyword::tagged_value< keyword::tag::a, A const & >,
        sake::keyword::tagged_value< keyword::tag::b, B const & >,
        sake::keyword::tagged_value< keyword::tag::c, C const & >
    >,
    param_spec_types
>));
BOOST_MPL_ASSERT((sake::keyword::satisfies<
    boost::mpl::vector3<
        sake::keyword::tagged_value< keyword::tag::a, A const & >,
        sake::keyword::tagged_value< keyword::tag::c, C const & >,
        sake::keyword::tagged_value< keyword::tag::d, D const & >
    >,
    param_spec_types
>));
BOOST_MPL_ASSERT((sake::keyword::satisfies<
    boost::mpl::vector4<
        sake::keyword::tagged_value< keyword::tag::a, A const & >,
        sake::keyword::tagged_value< keyword::tag::b, B const & >,
        sake::keyword::tagged_value< keyword::tag::c, C const & >,
        sake::keyword::tagged_value< keyword::tag::d, D const & >
    >,
    param_spec_types
>));

BOOST_MPL_ASSERT_NOT((sake::keyword::satisfies<
    boost::mpl::vector0<>,
    param_spec_types
>));
BOOST_MPL_ASSERT_NOT((sake::keyword::satisfies<
    boost::mpl::vector2<
        sake::keyword::tagged_value< keyword::tag::a, A const & >,
        sake::keyword::tagged_value< keyword::tag::b, B const & >
    >,
    param_spec_types
>));
BOOST_MPL_ASSERT_NOT((sake::keyword::satisfies<
    boost::mpl::vector2<
        sake::keyword::tagged_value< keyword::tag::a, A const & >,
        sake::keyword::tagged_value< keyword::tag::c, B const & >
    >,
    param_spec_types
>));

/*******************************************************************************
 * sake::keyword::arg_packer<P>::enable test
 ******************************************************************************/

BOOST_MPL_ASSERT((arg_packer_type::enable< A, B, C >));
BOOST_MPL_ASSERT((arg_packer_type::enable< A, B, C, D >));
BOOST_MPL_ASSERT((arg_packer_type::enable< A, B, D, C >));

BOOST_MPL_ASSERT_NOT((arg_packer_type::enable< A, B >));
BOOST_MPL_ASSERT_NOT((arg_packer_type::enable< A, C >));
BOOST_MPL_ASSERT_NOT((arg_packer_type::enable< B, A, C >));

BOOST_MPL_ASSERT((arg_packer_type::enable<
    A,
    sake::keyword::tagged_value< keyword::tag::c, C const & >
>));
BOOST_MPL_ASSERT((arg_packer_type::enable<
    sake::keyword::tagged_value< keyword::tag::c, C const & >,
    sake::keyword::tagged_value< keyword::tag::a, A const & >
>));
BOOST_MPL_ASSERT((arg_packer_type::enable<
    sake::keyword::tagged_value< keyword::tag::c, C const & >,
    sake::keyword::tagged_value< keyword::tag::a, A const & >,
    D
>));

} // namespace

void keyword_test(sake::test::environment& env)
{
    A const a = { }, a2 = { };
    B const b = { }, b2 = { };
    C const c = { }, c2 = { };
    D const d = { }, d2 = { };
    sake::keyword::tagged_value< keyword::tag::a, A const & > const ka = (keyword::_a = a);
    sake::keyword::tagged_value< keyword::tag::c, C const & > const kc = (keyword::_c = c);
    {
        typedef sake::boost_ext::result_of< arg_packer_type (
            A const &, B const &, C const &
        ) >::type arg_pack_type;
        arg_pack_type arg_pack = arg_packer_type()(a, b, c);
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_a], ==, &a );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_b], ==, &b );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_c], ==, &c );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_a | a2], ==, &a  );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_b | b2], ==, &b  );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_c | c2], ==, &c  );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_d | d2], ==, &d2 );
    }
    {
        typedef sake::boost_ext::result_of< arg_packer_type (
            A const &, B const &, C const &, D const &
        ) >::type arg_pack_type;
        arg_pack_type arg_pack = arg_packer_type()(a, b, c, d);
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_a], ==, &a );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_b], ==, &b );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_c], ==, &c );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_d], ==, &d );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_a | a2], ==, &a  );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_b | b2], ==, &b  );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_c | c2], ==, &c  );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_d | d2], ==, &d  );
    }
    {
        typedef sake::boost_ext::result_of< arg_packer_type (
            A const &, B const &, D const &, C const &
        ) >::type arg_pack_type;
        arg_pack_type arg_pack = arg_packer_type()(a, b, d, c);
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_a], ==, &a );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_b], ==, &b );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_c], ==, &c );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_d], ==, &d );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_a | a2], ==, &a  );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_b | b2], ==, &b  );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_c | c2], ==, &c  );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_d | d2], ==, &d  );
    }
    {
        typedef sake::boost_ext::result_of< arg_packer_type (
            A const &,
            sake::keyword::tagged_value< keyword::tag::c, C const & > const &
        ) >::type arg_pack_type;
        arg_pack_type arg_pack = arg_packer_type()(a, kc);
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_a], ==, &a );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_c], ==, &c );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_a | a2], ==, &a  );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_b | b2], ==, &b2 );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_c | c2], ==, &c  );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_d | d2], ==, &d2 );
    }
    {
        typedef sake::boost_ext::result_of< arg_packer_type (
            sake::keyword::tagged_value< keyword::tag::c, C const & > const &,
            sake::keyword::tagged_value< keyword::tag::a, A const & > const &
        ) >::type arg_pack_type;
        arg_pack_type arg_pack = arg_packer_type()(kc, ka);
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_a], ==, &a );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_c], ==, &c );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_a | a2], ==, &a  );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_b | b2], ==, &b2 );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_c | c2], ==, &c  );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_d | d2], ==, &d2 );
    }
    {
        typedef sake::boost_ext::result_of< arg_packer_type (
            sake::keyword::tagged_value< keyword::tag::c, C const & > const &,
            sake::keyword::tagged_value< keyword::tag::a, A const & > const &,
            D const &
        ) >::type arg_pack_type;
        arg_pack_type arg_pack = arg_packer_type()(kc, ka, d);
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_a], ==, &a );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_c], ==, &c );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_d], ==, &d );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_a | a2], ==, &a  );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_b | b2], ==, &b2 );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_c | c2], ==, &c  );
        SAKE_TEST_CHECK_RELATION( env, &arg_pack[keyword::_d | d2], ==, &d  );
    }
}

} // namespace sake_unit_test
