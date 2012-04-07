@echo off

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: sake/boost_ext/preprocessor/seq/private/join.bat
::
:: Copyright 2012, Jeffrey Hellrung.
:: Distributed under the Boost Software License, Version 1.0.  (See accompanying
:: file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
::
:: This Windows batch file generates join.ipp.
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

if exist join.ipp del join.ipp
for /L %%i in ( 2, 1, 256 ) do @join_helper %%i
