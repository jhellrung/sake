/*******************************************************************************
 * sake/core/utility/bisfo.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_BISFO( Name )
 *
 * Expands to
 *     const functional:: Name Name = { };
 * It is intended solely to reduce boilerplate when defining BISFOs
 * (Brace-Initialized Static Function Objects).
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_BISFO_HPP
#define SAKE_CORE_UTILITY_BISFO_HPP

#define SAKE_BISFO( Name ) functional:: Name const Name = { };

#endif // #ifndef SAKE_CORE_UTILITY_BISFO_HPP
