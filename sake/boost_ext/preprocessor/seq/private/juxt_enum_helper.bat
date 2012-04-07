@echo off

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: sake/boost_ext/preprocessor/seq/private/juxt_enum_helper.bat
::
:: Copyright 2012, Jeffrey Hellrung.
:: Distributed under the Boost Software License, Version 1.0.  (See accompanying
:: file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

setlocal
set /a im1 = %1 - 1
@echo #define SAKE_BOOST_EXT_PP_SEQ_JUXT_ENUM_%1( seq ) BOOST_PP_SEQ_ENUM( seq ) SAKE_BOOST_EXT_PP_SEQ_JUXT_ENUM_%im1% >> juxt_enum.ipp
endlocal
