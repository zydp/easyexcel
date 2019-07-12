/*easyexcel.cpp*/
#include "easyexcel.h"
#include "zend_exceptions.h"
/*--------------------------*/
#include "libxl.h"
#include "strings.h" //strcasecmp

static zend_class_entry *easyexcel_easy_ce = NULL;
static zend_object_handlers easyexcel_easy_handlers;

ZEND_DECLARE_MODULE_GLOBALS(easyexcel);

//interim
typedef struct _easyexcel_easy_object {
	libxl::Book *handle;
	libxl::Sheet *cursheet;
	zend_object std;
}easyexcel_easy_object;

static zend_object* easyexcel_easy_to_zend_object(easyexcel_easy_object *objval) {
	return ((zend_object*)(objval + 1)) - 1;
}

static easyexcel_easy_object* easyexcel_easy_from_zend_object(zend_object *objval) {
	return ((easyexcel_easy_object*)(objval + 1)) - 1;
}

/*function start*/
PHP_METHOD(easyexcel, LoadFile) {
	/*
	if (zend_parse_parameters_none_throw() == FAILURE) {
		return;
	}
	*/
	char *file_name = NULL;
	size_t name_len;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "s", &file_name, &name_len) == FAILURE) {
		RETURN_FALSE;
	}
	easyexcel_easy_object *objval = easyexcel_easy_from_zend_object(Z_OBJ_P(getThis()));
	RETURN_BOOL(objval->handle->load(file_name));
}

PHP_METHOD(easyexcel, SheetCount) {
	easyexcel_easy_object *objval = easyexcel_easy_from_zend_object(Z_OBJ_P(getThis()));
	RETURN_LONG(objval->handle->sheetCount());
}

PHP_METHOD(easyexcel, SheetSwitch) {
	long sheet_index;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "l", &sheet_index) == FAILURE) {
		RETURN_FALSE
	}
	easyexcel_easy_object *objval = easyexcel_easy_from_zend_object(Z_OBJ_P(getThis()));
	long sheet_count = objval->handle->sheetCount();
	libxl::Sheet *pSheet = objval->handle->getSheet(sheet_index<0?0:sheet_index>sheet_count?sheet_count:sheet_index);
	if(NULL!=pSheet){
		objval->cursheet = pSheet;
		RETURN_TRUE;
	}
	RETURN_FALSE;
}

PHP_METHOD(easyexcel, AddSheetAndSwitchTo) {
	char *sheet_name = NULL;
	long sheet_name_len;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "s", &sheet_name, &sheet_name_len) == FAILURE) {
		RETURN_FALSE;
	}
	easyexcel_easy_object *objval = easyexcel_easy_from_zend_object(Z_OBJ_P(getThis()));
	objval->cursheet = objval->handle->addSheet(sheet_name);
	RETURN_BOOL(objval->cursheet!=NULL);
}

PHP_METHOD(easyexcel, GetSheetNameByIndex) {
	long sheet_index;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "l", &sheet_index) == FAILURE) {
		RETURN_STRING("");
	}
	easyexcel_easy_object *objval = easyexcel_easy_from_zend_object(Z_OBJ_P(getThis()));
	long sheet_count = objval->handle->sheetCount();
	libxl::Sheet *pSheet = objval->handle->getSheet(sheet_index<0?0:sheet_index>sheet_count?sheet_count:sheet_index);
	if(NULL!=pSheet){
		const char *sheet_name = pSheet->name();
		RETURN_STRINGL(sheet_name, strlen(sheet_name));
	}
	RETURN_STRING("");
}

PHP_METHOD(easyexcel, SetSheetNameByIndex) {
	long sheet_index;
	char *sheet_name = NULL;
	size_t name_len;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &sheet_index, &sheet_name, &name_len) == FAILURE) {
		RETURN_FALSE;
	}
	easyexcel_easy_object *objval = easyexcel_easy_from_zend_object(Z_OBJ_P(getThis()));
	long sheet_count = objval->handle->sheetCount();
	libxl::Sheet *pSheet = objval->handle->getSheet(sheet_index<0?0:sheet_index>sheet_count?sheet_count:sheet_index);
	if(NULL!=pSheet){
		pSheet->setName(sheet_name);
		RETURN_TRUE
	}
	RETURN_FALSE
}

PHP_METHOD(easyexcel, GetTotalRow) {
	easyexcel_easy_object *objval = easyexcel_easy_from_zend_object(Z_OBJ_P(getThis()));
	if(objval->cursheet!=NULL){
		RETURN_LONG(objval->cursheet->lastRow());
	}
	RETURN_LONG(0)
}

PHP_METHOD(easyexcel, GetTotalCol) {
	easyexcel_easy_object *objval = easyexcel_easy_from_zend_object(Z_OBJ_P(getThis()));
	if(objval->cursheet!=NULL){
		RETURN_LONG(objval->cursheet->lastCol());
	}
	RETURN_LONG(0)
}

PHP_METHOD(easyexcel, GetCellType) {
	long row, col;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &row, &col) == FAILURE) {
		RETURN_LONG(5);//TYPE_ERROR
	}
	easyexcel_easy_object *objval = easyexcel_easy_from_zend_object(Z_OBJ_P(getThis()));
	if(objval->cursheet!=NULL){
		RETURN_LONG(objval->cursheet->cellType(row, col));
	}
	RETURN_LONG(5);//TYPE_ERROR
}

PHP_METHOD(easyexcel, ReadNum) {
	long row, col;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &row, &col) == FAILURE) {
		RETURN_DOUBLE(0.0)
	}
	easyexcel_easy_object *objval = easyexcel_easy_from_zend_object(Z_OBJ_P(getThis()));
	if(objval->cursheet!=NULL){
		//const double val = objval->cursheet->readNum(row, col);
		//RETURN_DOUBLE(val);
		RETURN_DOUBLE(objval->cursheet->readNum(row, col));
	}
	RETURN_DOUBLE(0.0);
	
}

PHP_METHOD(easyexcel, ReadStr) {
	long row, col;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &row, &col) == FAILURE) {
		RETURN_STRING("");
	}
	easyexcel_easy_object *objval = easyexcel_easy_from_zend_object(Z_OBJ_P(getThis()));
	if(objval->cursheet!=NULL){
		const char *str = objval->cursheet->readStr(row, col);
		RETURN_STRINGL(str, strlen(str));
	}
	RETURN_STRING("");
}

PHP_METHOD(easyexcel, WriteNum) {
	long row, col;
	double num;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "lld", &row, &col, &num) == FAILURE) {
		RETURN_FALSE
	}
	easyexcel_easy_object *objval = easyexcel_easy_from_zend_object(Z_OBJ_P(getThis()));
	if(objval->cursheet!=NULL){
		RETURN_BOOL(objval->cursheet->writeNum(row, col, num));
	}
	RETURN_FALSE
}

PHP_METHOD(easyexcel, WriteStr) {
	long row, col;
	char *str = NULL;
	long str_len;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "lls", &row, &col, &str, &str_len) == FAILURE) {
		RETURN_FALSE
	}
	easyexcel_easy_object *objval = easyexcel_easy_from_zend_object(Z_OBJ_P(getThis()));
	if(objval->cursheet!=NULL){
		RETURN_BOOL(objval->cursheet->writeStr(row, col, str));
	}
	RETURN_FALSE
}

PHP_METHOD(easyexcel, IsFormula) {
	long row, col;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &row, &col) == FAILURE) {
		RETURN_FALSE;
	}
	easyexcel_easy_object *objval = easyexcel_easy_from_zend_object(Z_OBJ_P(getThis()));
	if(objval->cursheet!=NULL){
		RETURN_BOOL(objval->cursheet->isFormula(row, col));
	}
	RETURN_FALSE
}

PHP_METHOD(easyexcel, ReadFormula) {
	long row, col;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &row, &col) == FAILURE) {
		RETURN_STRING("")
	}
	easyexcel_easy_object *objval = easyexcel_easy_from_zend_object(Z_OBJ_P(getThis()));
	if(objval->cursheet!=NULL){
		const char *str = objval->cursheet->readFormula(row, col);
		RETURN_STRINGL(str, strlen(str));
	}
	RETURN_STRING("")
}

PHP_METHOD(easyexcel, WriteFormula) {
	long row, col, formula_len;
	char *formula = NULL;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "lls", &row, &col, &formula, &formula_len) == FAILURE) {
		RETURN_FALSE
	}
	easyexcel_easy_object *objval = easyexcel_easy_from_zend_object(Z_OBJ_P(getThis()));
	if(objval->cursheet!=NULL){
		RETURN_BOOL(objval->cursheet->writeFormula(row, col, formula));
	}
	RETURN_FALSE
}

PHP_METHOD(easyexcel, IsDate) {
	long row, col;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &row, &col) == FAILURE) {
		RETURN_FALSE;
	}
	easyexcel_easy_object *objval = easyexcel_easy_from_zend_object(Z_OBJ_P(getThis()));
	if(objval->cursheet!=NULL){
		RETURN_BOOL(objval->cursheet->isDate(row, col));
	}
	RETURN_FALSE
}

PHP_METHOD(easyexcel, SetMerge) {
	long rowFirst, colFirst, rowLast, colLast;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "llll", &rowFirst, &colFirst, &rowLast, &colLast) == FAILURE) {
		RETURN_FALSE;
	}
	easyexcel_easy_object *objval = easyexcel_easy_from_zend_object(Z_OBJ_P(getThis()));
	if(objval->cursheet!=NULL){
		RETURN_BOOL(objval->cursheet->setMerge(rowFirst, rowLast, colFirst, colLast));
	}
	RETURN_FALSE
}

PHP_METHOD(easyexcel, DelMerge) {
	long row, col;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &row, &col) == FAILURE) {
		RETURN_FALSE;
	}
	easyexcel_easy_object *objval = easyexcel_easy_from_zend_object(Z_OBJ_P(getThis()));
	if(objval->cursheet!=NULL){
		RETURN_BOOL(objval->cursheet->delMerge(row, col));
	}
	RETURN_FALSE
}

PHP_METHOD(easyexcel, InsertRow) {
	long rowFirst, rowLast;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &rowFirst, &rowLast) == FAILURE) {
		RETURN_FALSE;
	}
	easyexcel_easy_object *objval = easyexcel_easy_from_zend_object(Z_OBJ_P(getThis()));
	if(objval->cursheet!=NULL){
		RETURN_BOOL(objval->cursheet->insertRow(rowFirst, rowLast));
	}
	RETURN_FALSE
}

PHP_METHOD(easyexcel, DelRow) {
	long rowFirst, rowLast;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &rowFirst, &rowLast) == FAILURE) {
		RETURN_FALSE;
	}
	easyexcel_easy_object *objval = easyexcel_easy_from_zend_object(Z_OBJ_P(getThis()));
	if(objval->cursheet!=NULL){
		RETURN_BOOL(objval->cursheet->removeRow(rowFirst, rowLast));
	}
	RETURN_FALSE
}

PHP_METHOD(easyexcel, InsertCol) {
	long colFirst, colLast;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &colFirst, &colLast) == FAILURE) {
		RETURN_FALSE;
	}
	easyexcel_easy_object *objval = easyexcel_easy_from_zend_object(Z_OBJ_P(getThis()));
	if(objval->cursheet!=NULL){
		RETURN_BOOL(objval->cursheet->insertCol(colFirst, colLast));
	}
	RETURN_FALSE
}

PHP_METHOD(easyexcel, DelCol) {
	long colFirst, colLast;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &colFirst, &colLast) == FAILURE) {
		RETURN_FALSE;
	}
	easyexcel_easy_object *objval = easyexcel_easy_from_zend_object(Z_OBJ_P(getThis()));
	if(objval->cursheet!=NULL){
		RETURN_BOOL(objval->cursheet->removeCol(colFirst, colLast));
	}
	RETURN_FALSE
}

PHP_METHOD(easyexcel, CopyCell) {
	long rowSrc, colSrc, rowDst, colDst;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "llll", &rowSrc, &colSrc, &rowDst, &colDst) == FAILURE) {
		RETURN_FALSE;
	}
	easyexcel_easy_object *objval = easyexcel_easy_from_zend_object(Z_OBJ_P(getThis()));
	if(objval->cursheet!=NULL){
		RETURN_BOOL(objval->cursheet->copyCell(rowSrc, colSrc, rowDst, colDst));
	}
	RETURN_FALSE
}

PHP_METHOD(easyexcel, ClearCell) {
	long rowFirst, colFirst, rowLast, colLast;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "llll", &rowFirst, &colFirst, &rowLast, &colLast) == FAILURE) {
		RETURN_FALSE;
	}
	easyexcel_easy_object *objval = easyexcel_easy_from_zend_object(Z_OBJ_P(getThis()));
	if(objval->cursheet!=NULL){
		RETURN_BOOL(objval->cursheet->clear(rowFirst, rowLast, colFirst, colLast));
	}
	RETURN_FALSE
}

PHP_METHOD(easyexcel, SetRowHeigh) {
	long row;
	double heigh;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "ld", &row, &heigh) == FAILURE) {
		RETURN_FALSE;
	}
	easyexcel_easy_object *objval = easyexcel_easy_from_zend_object(Z_OBJ_P(getThis()));
	if(objval->cursheet!=NULL){
		RETURN_BOOL(objval->cursheet->setRow(row, heigh));
	}
	RETURN_FALSE
}

PHP_METHOD(easyexcel, SetColWidth) {
	long colFirst, colLast;
	double width;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "lld", &colFirst, &colLast, &width) == FAILURE) {
		RETURN_FALSE;
	}
	easyexcel_easy_object *objval = easyexcel_easy_from_zend_object(Z_OBJ_P(getThis()));
	if(objval->cursheet!=NULL){
		RETURN_BOOL(objval->cursheet->setCol(colFirst, colLast, width));
	}
	RETURN_FALSE
}

PHP_METHOD(easyexcel, RowHide) {
	long row;
	bool hide;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "lb", &row, &hide) == FAILURE) {
		RETURN_FALSE;
	}
	easyexcel_easy_object *objval = easyexcel_easy_from_zend_object(Z_OBJ_P(getThis()));
	if(objval->cursheet!=NULL){
		RETURN_BOOL(objval->cursheet->setRowHidden(row, hide));
	}
	RETURN_FALSE
}

PHP_METHOD(easyexcel, ColHide) {
	long col;
	bool hide;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "lb", &col, &hide) == FAILURE) {
		RETURN_FALSE;
	}
	easyexcel_easy_object *objval = easyexcel_easy_from_zend_object(Z_OBJ_P(getThis()));
	if(objval->cursheet!=NULL){
		RETURN_BOOL(objval->cursheet->setColHidden(col, hide));
	}
	RETURN_FALSE
}

PHP_METHOD(easyexcel, SaveToFile) {
	char *sheet_name = NULL;
	long sheet_name_len;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "s", &sheet_name, &sheet_name_len) == FAILURE) {
		RETURN_FALSE;
	}
	easyexcel_easy_object *objval = easyexcel_easy_from_zend_object(Z_OBJ_P(getThis()));
	RETURN_BOOL(objval->handle->save(sheet_name));
}

static PHP_METHOD(easyexcel, __construct) {
	char* name = NULL;
	size_t name_len;
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE){
		return;
	}
	easyexcel_easy_object *objval = easyexcel_easy_from_zend_object(Z_OBJ_P(getThis()));
	if( strcasecmp("xlsx", name) == 0) {
		objval->handle = xlCreateXMLBook();
	}else{
		objval->handle = xlCreateBook();
	}
	if(NULL!=objval->handle){
		objval->handle->setKey("lucas.D", "linux-2927280408c8e1006bb26867a5t2u3h5");
		objval->cursheet = NULL;
	}
}

//apis 
static zend_function_entry easyexcel_easy_methods[] = {
	PHP_ME(easyexcel, __construct, NULL, ZEND_ACC_CTOR | ZEND_ACC_PUBLIC)
	PHP_ME(easyexcel, LoadFile, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(easyexcel, SheetCount, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(easyexcel, SheetSwitch, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(easyexcel, AddSheetAndSwitchTo, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(easyexcel, GetSheetNameByIndex, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(easyexcel, SetSheetNameByIndex, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(easyexcel, GetTotalRow, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(easyexcel, GetTotalCol, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(easyexcel, GetCellType, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(easyexcel, ReadNum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(easyexcel, ReadStr, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(easyexcel, WriteNum, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(easyexcel, WriteStr, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(easyexcel, IsFormula, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(easyexcel, ReadFormula, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(easyexcel, WriteFormula, NULL, ZEND_ACC_PUBLIC)
	//PHP_ME(easyexcel, IsDate, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(easyexcel, SetMerge, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(easyexcel, DelMerge, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(easyexcel, InsertRow, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(easyexcel, DelRow, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(easyexcel, InsertCol, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(easyexcel, DelCol, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(easyexcel, CopyCell, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(easyexcel, ClearCell, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(easyexcel, SetRowHeigh, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(easyexcel, SetColWidth, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(easyexcel, RowHide, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(easyexcel, ColHide, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(easyexcel, SaveToFile, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

//ctor
static zend_object* easyexcel_easy_ctor(zend_class_entry *ce) {
	easyexcel_easy_object *objval = (easyexcel_easy_object*)ecalloc(1, sizeof(easyexcel_easy_object) + zend_object_properties_size(ce));
	//objval->handle = new Person(0,"");
	objval->handle = NULL;
	zend_object* ret = easyexcel_easy_to_zend_object(objval);
	zend_object_std_init(ret, ce);
	object_properties_init(ret, ce);
	ret->handlers = &easyexcel_easy_handlers;
	return ret;
}


//clone	--not registered
static zend_object* easyexcel_easy_clone(zval *srcval) {
	zend_object *zsrc = Z_OBJ_P(srcval);
	zend_object *zdst = easyexcel_easy_ctor(zsrc->ce);
	zend_objects_clone_members(zdst, zsrc);
	return zdst;
	/*need deep copy*/
	easyexcel_easy_object *src = easyexcel_easy_from_zend_object(zsrc);
	easyexcel_easy_object *dst = easyexcel_easy_from_zend_object(zdst);
	*dst->handle = *src->handle;   
	return zdst;
}

//delete
static void easyexcel_easy_free(zend_object *zobj) {
	easyexcel_easy_object *obj = easyexcel_easy_from_zend_object(zobj);
	if (obj->handle!=NULL){
		obj->cursheet = NULL;
		obj->handle->release();
	}
	zend_object_std_dtor(zobj);
}


/*ready function*/
PHP_INI_BEGIN()
	//no globals
	//STD_PHP_INI_ENTRY("tutorial.default_url", "", PHP_INI_ALL,OnUpdateString, default_url, zend_tutorial_globals, tutorial_globals)
PHP_INI_END()


static PHP_MINIT_FUNCTION(easyexcel) {
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "easyexcel", easyexcel_easy_methods);
	easyexcel_easy_ce = zend_register_internal_class(&ce);
	easyexcel_easy_ce->create_object = easyexcel_easy_ctor;
	memcpy(&easyexcel_easy_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	easyexcel_easy_handlers.offset = XtOffsetOf(easyexcel_easy_object, std);
	//easyexcel_easy_handlers.clone_obj = easyexcel_easy_clone;	//not supported
	easyexcel_easy_handlers.free_obj = easyexcel_easy_free;
	REGISTER_INI_ENTRIES();
	return SUCCESS;
}
static PHP_MSHUTDOWN_FUNCTION(easyexcel) {
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}

static PHP_GINIT_FUNCTION(easyexcel) {
#if defined(COMPILE_DL_ASTKIT) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	//init global??
}

zend_module_entry easyexcel_module_entry = {
	STANDARD_MODULE_HEADER,
	"easyexcel",
	NULL, /* functions */
	PHP_MINIT(easyexcel),
	PHP_MSHUTDOWN(easyexcel),
	NULL,/*print*/
	NULL,/*rshutdown*/
	NULL,/*minfo*/
	NO_VERSION_YET,
	PHP_MODULE_GLOBALS(easyexcel),
	PHP_GINIT(easyexcel),
	NULL,/*gshutdown*/
	NULL,/*rpostshutdown*/
	STANDARD_MODULE_PROPERTIES_EX


};


#ifdef COMPILE_DL_EASYEXCEL
	ZEND_GET_MODULE(easyexcel)
#endif
