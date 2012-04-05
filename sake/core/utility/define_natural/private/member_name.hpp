/*******************************************************************************
 * sake/core/utility/define_natural/private/member_name.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_PRIVATE_MEMBER_NAME_HPP
#define SAKE_CORE_UTILITY_DEFINE_NATURAL_PRIVATE_MEMBER_NAME_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/detail/is_binary.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#define SAKE_DEFINE_NATURAL_member_name( member ) \
    BOOST_PP_CAT( \
        SAKE_DEFINE_NATURAL_member_name_, \
        BOOST_PP_IS_BINARY( member ) \
    ) ( member )

#define SAKE_DEFINE_NATURAL_member_name_0( member ) member
#define SAKE_DEFINE_NATURAL_member_name_1( member ) BOOST_PP_TUPLE_ELEM( 2, 1, member )

#endif // #ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_PRIVATE_MEMBER_NAME_HPP
