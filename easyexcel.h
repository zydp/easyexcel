#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"

ZEND_BEGIN_MODULE_GLOBALS(easyexcel)
//globals

ZEND_END_MODULE_GLOBALS(easyexcel)

extern zend_module_entry easyexcel_module_entry;
#define phpext_easyexcel_ptr &easyexcel_module_entry

#define PHP_EASYEXCEL_VERSION "1.0.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_EASYEXCEL_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_EASYEXCEL_API __attribute__ ((visibility("default")))
#else
#	define PHP_EASYEXCEL_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#define EASYEXCEL_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(easyexcel, v)

#if defined(ZTS) && defined(COMPILE_DL_EASYEXCEL)
	ZEND_TSRMLS_CACHE_EXTERN()
#endif



