/*******************************************************************************
 * sake/core/move/rv.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost::rv<T>
 * #define SAKE_BOOST_EXT_RV_REF( T )
 * #define SAKE_BOOST_EXT_RV_REF_TEMPLATE( class_template, type_param_seq )
 ******************************************************************************/

#ifndef SAKE_CORE_MOVE_RV_HPP
#define SAKE_CORE_MOVE_RV_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/seq/enum.hpp>

#include <sake/core/move/boost_move.hpp>

#ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_BOOST_EXT_RV_REF( T ) T &&
#define SAKE_BOOST_EXT_RV_REF_TEMPLATE( class_template, type_param_seq ) \
    class_template < BOOST_PP_SEQ_ENUM( type_param_seq ) > &&

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_BOOST_EXT_RV_REF( T ) ::boost::rv< T > &
#define SAKE_BOOST_EXT_RV_REF_TEMPLATE( class_template, type_param_seq ) \
    ::boost::rv< class_template < BOOST_PP_SEQ_ENUM( type_param_seq ) > > &

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#endif // #ifndef SAKE_CORE_MOVE_RV_HPP
