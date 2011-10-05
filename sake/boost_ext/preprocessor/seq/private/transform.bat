@echo off

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: sake/boost_ext/preprocessor/seq/private/transform.bat
::
:: Copyright 2011, Jeffrey Hellrung.
:: Distributed under the Boost Software License, Version 1.0.  (See accompanying
:: file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
::
:: This Windows batch file generates transform.ipp.
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

if exist transform.ipp del transform.ipp
for /L %%i in ( 0, 1, 255 ) do @echo #define SAKE_BOOST_EXT_PP_SEQ_TRANSFORM_%%i_fold_left_op( s, state, elem ) SAKE_BOOST_EXT_PP_SEQ_TRANSFORM_%%i_fold_left_op_helper( s, BOOST_PP_TUPLE_ELEM( 3, 0, state ), BOOST_PP_TUPLE_ELEM( 3, 1, state ), BOOST_PP_TUPLE_ELEM( 3, 2, state ), elem ) >> transform.ipp
for /L %%i in ( 0, 1, 255 ) do @echo #define SAKE_BOOST_EXT_PP_SEQ_TRANSFORM_%%i_fold_left_op_helper( s, seq, op, data, elem ) ( seq ( op( s, data, elem ) ), op, data ) >> transform.ipp
for /L %%i in ( 0, 1, 255 ) do @echo #define SAKE_BOOST_EXT_PP_SEQ_TRANSFORM_%%i( op, data, seq ) BOOST_PP_TUPLE_ELEM( 3, 0, BOOST_PP_SEQ_FOLD_LEFT_%%i( SAKE_BOOST_EXT_PP_SEQ_TRANSFORM_fold_left_op, ( BOOST_PP_SEQ_NIL, op, data ), seq ) ) >> transform.ipp
