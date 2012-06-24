/*******************************************************************************
 * sake/core/utility/value_constructor.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_VALUE_CONSTRUCTOR_GENERATE()
 *
 * This provides machinery to generate forwarding constructor overloads, with
 * support for capturing rvalues and perfect forwarding of lvalues in C++03.
 * One defines certain named parameter macros to define the constructor
 * overload set, then #include's SAKE_VALUE_CONSTRUCTOR_GENERATE().
 * 
 * The macros to define to specify the desired constructor overload set are as
 * follows:
 *
 * SAKE_VALUE_CONSTRUCTOR_T
 *     [ defaults to "T" ]
 *     Expands to the template parameter prefix.
 * SAKE_VALUE_CONSTRUCTOR_X
 *     [ defaults to "x" ]
 *     Expands to the function argument prefix.
 *
 * SAKE_VALUE_CONSTRUCTOR_CLASS_NAME
 *     Expands to the name of the enclosing class.
 * SAKE_VALUE_CONSTRUCTOR_FORWARD
 *     Expands to the name of the forwarding constructor (typically a base
 *     class)
 *
 * SAKE_VALUE_CONSTRUCTOR_ARITY
 *     Expands to an integer giving the arity of the constructor.
 * SAKE_VALUE_CONSTRUCTOR_PERFECT_MAX_ARITY
 *     [ optional ]
 *     Expands to an integer giving the maximum arity of the constructor with
 *     "perfect" forwarding. See the documentation for overload.hpp.
 * SAKE_VALUE_CONSTRUCTOR_TYPE0
 *     Expands to the value type of the first parameter.
 * SAKE_VALUE_CONSTRUCTOR_TYPE1
 *     Expands to the value type of the second parameter.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_VALUE_CONSTRUCTOR_HPP
#define SAKE_CORE_UTILITY_VALUE_CONSTRUCTOR_HPP

#include <boost/config.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>

#include <sake/core/emplacer/constructible.hpp>
#include <sake/core/emplacer/emplacer.hpp>
#include <sake/core/functional/construct.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/move/rv_cast.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/utility/overload.hpp>
#include <sake/core/utility/value_constructor_enable.hpp>

#define SAKE_VALUE_CONSTRUCTOR_GENERATE() \
    <sake/core/utility/private/value_constructor.ipp>

#endif // #ifndef SAKE_CORE_UTILITY_VALUE_CONSTRUCTOR_HPP
