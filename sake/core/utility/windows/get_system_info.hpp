/*******************************************************************************
 * sake/core/utility/win/get_system_info.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * win::get_system_info() -> SYSTEM_INFO const &
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_WIN_GET_SYSTEM_INFO_HPP
#define SAKE_CORE_UTILITY_WIN_GET_SYSTEM_INFO_HPP

#include <boost/config.hpp>

#ifdef BOOST_WINDOWS

#include <windows.h>

namespace sake
{

namespace windows
{

namespace get_system_info_private
{

inline SYSTEM_INFO
helper()
{
    SYSTEM_INFO result;
    GetSystemInfo(&result);
    return result;
}

} // namespace get_system_info_private

inline SYSTEM_INFO const &
get_system_info()
{
    static SYSTEM_INFO const result = get_system_info_private::helper();
    return result;
}

} // namespace windows

} // namespace sake

#endif // #ifdef BOOST_WINDOWS

#endif // #ifndef SAKE_CORE_UTILITY_WIN_GET_SYSTEM_INFO_HPP
