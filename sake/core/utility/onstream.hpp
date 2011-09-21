/*******************************************************************************
 * sake/core/utility/onstream.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct basic_onstream< Char, Traits = std::char_traits< Char > >
 * typedef basic_onstream<  char   >  onstream;
 * typedef basic_onstream< wchar_t > wonstream;
 * extern  onstream  nout;
 * extern wonstream wnout;
 *
 * This defines an output null-stream, which is a std::basic_ostream that
 * ignores any objects streamed to it.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_ONSTREAM_HPP
#define SAKE_CORE_UTILITY_ONSTREAM_HPP

#include <ostream>
#include <streambuf>

namespace sake
{

template< class Char, class Traits = std::char_traits< Char > >
struct basic_onstream
    : std::basic_ostream< Char, Traits >
{
    basic_onstream()
        : std::basic_ostream< Char, Traits >(&m_streambuf)
    { }
private:
    struct streambuf
        : std::basic_streambuf< Char, Traits >
    {
        typename Traits::int_type overflow(typename Traits::int_type c)
        { return Traits::not_eof(c); }
    } m_streambuf;
};

typedef basic_onstream<  char   >  onstream;
typedef basic_onstream< wchar_t > wonstream;

extern  onstream  nout;
extern wonstream wnout;

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_ONSTREAM_HPP
