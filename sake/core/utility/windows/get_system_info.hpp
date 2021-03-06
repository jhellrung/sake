/*******************************************************************************
 * sake/core/utility/windows/get_system_info.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_WINDOWS_GET_SYSTEM_INFO_HPP
#define SAKE_CORE_UTILITY_WINDOWS_GET_SYSTEM_INFO_HPP

#include <boost/config.hpp>

#ifdef BOOST_WINDOWS

#include <windows.h>

namespace sake
{

namespace windows
{

SYSTEM_INFO const &
get_system_info();

} // namespace windows

} // namespace sake

#endif // #ifdef BOOST_WINDOWS

#endif // #ifndef SAKE_CORE_UTILITY_WINDOWS_GET_SYSTEM_INFO_HPP
