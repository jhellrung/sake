/*******************************************************************************
 * unit_test/test/core/utility/overload_test.cpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/mpl/always.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/tuple/rem.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/result_of.hpp>

#include <sake/boost_ext/mpl/all.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/utility/overload.hpp>

#include <sake/test/environment.hpp>

namespace sake_unit_test
{

namespace
{

template< int N >
struct R
{ };

R<0> f() { return R<0>(); }
R<1> f(int) { return R<1>(); }
R<2> f(int,int) { return R<2>(); }
R<3> f(int,int,int) { return R<3>(); }

struct X
{

    // template< class T0, ... >
    // typename boost::lazy_enable_if<
    //     sake::boost_ext::mpl::all<
    //         boost::mpl::vector ## n < T0, ... >,
    //         boost::is_convertible< boost::mpl::_1, int >
    //     >,
    //     boost::mpl::identity< R< n > >
    // >::type
    // operator()(T0&& x0, ... ) const
    // { return f(sake::forward< T0 >(x0), ... ); }
#define SAKE_OVERLOAD_DEFINE_RESULT
#define SAKE_OVERLOAD_IS_CALLABLE( n, T_tuple ) \
    sake::boost_ext::mpl::all< \
        BOOST_PP_CAT( boost::mpl::vector, n ) < BOOST_PP_TUPLE_REM_CTOR( n, T_tuple ) >, \
        boost::is_convertible< boost::mpl::_1, int > \
    >
#define SAKE_OVERLOAD_RESULT( n, T_tuple ) \
    boost::mpl::always< R< n > >::apply< void T_tuple >
#define SAKE_OVERLOAD_CONST_MEMBER_FUNCTION_NAME \
    operator()
#define SAKE_OVERLOAD_BODY( n, T_tuple, x_tuple, forward_x_tuple ) \
    return f forward_x_tuple ;
#define SAKE_OVERLOAD_PERFECT_MAX_ARITY 3
#include SAKE_OVERLOAD_GENERATE()

};

template< class T >
int
test_same(T const &);

BOOST_MPL_ASSERT((boost::is_same< boost::result_of< X ( int ) >::type, R<1> >));
BOOST_MPL_ASSERT((boost::is_same< boost::result_of< X ( int, int ) >::type, R<2> >));
BOOST_MPL_ASSERT((boost::is_same< boost::result_of< X ( int, int, int ) >::type, R<3> >));

} // namespace

void overload_test(sake::test::environment& /*env*/)
{
    X x;
    int a = 0, b = 0, c = 0;
    static_cast< void >(sizeof(test_same< R<1> >(x(a))));
    static_cast< void >(sizeof(test_same< R<2> >(x(a, b))));
    static_cast< void >(sizeof(test_same< R<3> >(x(a, b, c))));
}

} // namespace sake_unit_test
