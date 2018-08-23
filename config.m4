dnl config.m4
PHP_ARG_WITH(easyexcel)
PHP_ARG_ENABLE(easyexcel, whether to enable easyexcel support,
dnl Make sure that the comment is aligned:
[  --enable-easyexcel           Enable easyexcel support])

if test "$PHP_EASYEXCEL" != "no"; then
	dnl EXTRA_CXXFLAGS = "-std=c++11" 
	PHP_REQUIRE_CXX()
	PHP_ADD_INCLUDE(./include)
	PHP_ADD_LIBRARY_WITH_PATH(xl, ./ext_lib, EASYEXCEL_SHARED_LIBADD)

	PHP_SUBST(EASYEXCEL_SHARED_LIBADD)
	dnl PHP_NEW_EXTENSIDDON(easyexcel, easyexcel.cpp, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
	PHP_NEW_EXTENSION(easyexcel, easyexcel.cpp, $ext_shared)
fi

