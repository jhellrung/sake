@echo off

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: sake/boost_ext/preprocessor/seq/private/transpose_helper.bat
::
:: Copyright 2012, Jeffrey Hellrung.
:: Distributed under the Boost Software License, Version 1.0.  (See accompanying
:: file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

setlocal
set /a im1 = %1 - 1
@echo #define SAKE_BOOST_EXT_PP_SEQ_TRANSPOSE_impl_%1( seq, head, tail ) ( head seq ) SAKE_BOOST_EXT_PP_SEQ_TRANSPOSE_impl_%im1%( tail seq, head, tail ) >> transpose.ipp
@echo #define SAKE_BOOST_EXT_PP_SEQ_TRANSPOSE_head_%1( seq ) ( BOOST_PP_SEQ_HEAD( seq ) ) SAKE_BOOST_EXT_PP_SEQ_TRANSPOSE_head_%im1% >> transpose.ipp
@echo #define SAKE_BOOST_EXT_PP_SEQ_TRANSPOSE_tail_%1( seq ) ( BOOST_PP_SEQ_TAIL( seq ) ) SAKE_BOOST_EXT_PP_SEQ_TRANSPOSE_tail_%im1% >> transpose.ipp
endlocal
