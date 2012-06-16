/*******************************************************************************
 * sake/core/cursor/adaptor_fwd.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_CURSOR_ADAPTOR_FWD_HPP
#define SAKE_CORE_CURSOR_ADAPTOR_FWD_HPP

namespace sake
{

template<
    class I,
    class Introversal,
    class IntroversalMask = sake::null_introversal_tag
>
class cursor_adaptor;

} // namespace sake

#endif // #ifndef SAKE_CORE_CURSOR_ADAPTOR_FWD_HPP
