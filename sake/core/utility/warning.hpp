/*******************************************************************************
 * core/utility/warning.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * SAKE_WARNING()
 *
 * This attempts to facilitate compiler warning messages in a portable way,
 * although it is currently quite limited.  The motivating application is to
 * issue a warning when including deprecated headers.
 *
 * To issue a compiler warning message, first #define SAKE_WARNING_MESSAGE (as
 * a double-quoted string literal), then #include SAKE_WARNING().  For example:
 *
 * #define SAKE_WARNING_MESSAGE \
 *     "WARNING:  abc in def.hpp is deprecated!  Use ghi in jkl.hpp instead."
 * #include SAKE_WARNING()
 *
 * #define SAKE_NO_WARNING to suppress warning messages.
 ******************************************************************************/

#ifndef SAKE_WARNING_MESSAGE

#ifndef SAKE_CORE_UTILITY_WARNING_HPP
#define SAKE_CORE_UTILITY_WARNING_HPP

#define SAKE_WARNING() <sake/core/utility/warning.hpp>

#endif // #ifndef SAKE_CORE_UTILITY_WARNING_HPP

#else // #ifndef SAKE_WARNING_MESSAGE

#ifndef SAKE_NO_WARNING

#ifdef _MSC_VER
#pragma message( SAKE_WARNING_MESSAGE )
#else // #ifdef _MSC_VER
#warning SAKE_WARNING_MESSAGE
#endif // #ifdef _MSC_VER

#endif // #ifndef SAKE_NO_WARNING

#undef SAKE_WARNING_MESSAGE

#endif // #ifndef SAKE_WARNING_MESSAGE
