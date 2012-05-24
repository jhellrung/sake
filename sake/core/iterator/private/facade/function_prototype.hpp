/*******************************************************************************
 * sake/core/iterator/private/facade/function_prototype.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_FUNCTION_PROTOTYPE_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_FACADE_FUNCTION_PROTOTYPE_HPP

#define SAKE_ITERATOR_FACADE_function_prototype( prefix, enabler, name ) \
template< class D0, class P0, class D1, class P1 > \
prefix typename ::sake::iterator_facade_adl::private_::enabler< D0, P0, D1, P1 >::type \
name(::sake::iterator_facade_adl::iterator_facade< D0, P0 > const & i0, \
     ::sake::iterator_facade_adl::iterator_facade< D1, P1 > const & i1)
 
#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_FUNCTION_PROTOTYPE_HPP
