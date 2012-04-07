@echo off

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: sake/boost_ext/preprocessor/seq/private/join_helper.bat
::
:: Copyright 2012, Jeffrey Hellrung.
:: Distributed under the Boost Software License, Version 1.0.  (See accompanying
:: file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

setlocal
set /a im1 = %1 - 1
@echo #define SAKE_BOOST_EXT_PP_SEQ_JOIN_%1( seq ) SAKE_BOOST_EXT_PP_SEQ_EAT_NIL( seq ) SAKE_BOOST_EXT_PP_SEQ_JOIN_%im1% >> join.ipp
endlocal
