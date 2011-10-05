/*******************************************************************************
 * sake/core/utility/private/implicitly_defined/any_reference.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_PRIVATE_IMPLICITLY_DEFINED_ANY_REFERENCE_HPP
#define SAKE_CORE_UTILITY_PRIVATE_IMPLICITLY_DEFINED_ANY_REFERENCE_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/preprocessor/keyword/class.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>

#define SAKE_IMPLICITLY_DEFINED_ASSIGN_any_reference( r, member_seq ) \
    BOOST_PP_CAT( ::sake::boost_ext::mpl::or, BOOST_PP_SEQ_SIZE( member_seq ) )< \
        BOOST_PP_SEQ_FOR_EACH_I_R( r, SAKE_IMPLICITLY_DEFINED_ASSIGN_comma_is_reference, ~, member_seq ) \
    >

#define SAKE_IMPLICITLY_DEFINED_ASSIGN_comma_is_reference( r, data, i, elem ) \
    BOOST_PP_COMMA_IF( i ) \
    ::sake::boost_ext::is_reference< \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_CLASS( \
            BOOST_PP_TUPLE_ELEM( 2, 0, elem ) \
        ) \
    >

#endif // #ifndef SAKE_CORE_UTILITY_PRIVATE_IMPLICITLY_DEFINED_ANY_REFERENCE_HPP
