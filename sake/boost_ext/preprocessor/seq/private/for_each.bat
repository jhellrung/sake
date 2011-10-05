@echo off

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: sake/boost_ext/preprocessor/seq/private/for_each.bat
::
:: Copyright 2011, Jeffrey Hellrung.
:: Distributed under the Boost Software License, Version 1.0.  (See accompanying
:: file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
::
:: This Windows batch file generates for_each.ipp.
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

if exist for_each.ipp del for_each.ipp
for /L %%i in ( 0, 1, 255 ) do @echo #define SAKE_BOOST_EXT_PP_SEQ_FOR_EACH_%%i_for_macro( r, state ) BOOST_PP_TUPLE_ELEM( 4, 1, state ) ( r, BOOST_PP_TUPLE_ELEM( 4, 2, state ), BOOST_PP_SEQ_HEAD( BOOST_PP_TUPLE_ELEM( 4, 3, state ) ) ) >> for_each.ipp
for /L %%i in ( 0, 1, 255 ) do @echo #define SAKE_BOOST_EXT_PP_SEQ_FOR_EACH_%%i( macro, data, seq ) BOOST_PP_FOR_%%i( ( BOOST_PP_SEQ_SIZE( seq ), macro, data, seq ), SAKE_BOOST_EXT_PP_SEQ_FOR_EACH_for_pred, SAKE_BOOST_EXT_PP_SEQ_FOR_EACH_for_op, SAKE_BOOST_EXT_PP_SEQ_FOR_EACH_%%i_for_macro ) >> for_each.ipp
