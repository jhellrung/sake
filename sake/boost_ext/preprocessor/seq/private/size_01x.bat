@echo off

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: sake/boost_ext/preprocessor/seq/private/size_01x.bat
::
:: Copyright 2012, Jeffrey Hellrung.
:: Distributed under the Boost Software License, Version 1.0.  (See accompanying
:: file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
::
:: This Windows batch file generates size_01x.ipp.
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

if exist size_01x.ipp del size_01x.ipp
for /L %%i in ( 3, 1, 256 ) do @echo #define SAKE_BOOST_EXT_PP_SEQ_SIZE_01X_%%i( x ) x >> size_01x.ipp
