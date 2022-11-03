#ifndef FALSOJNI_IMPL
#define FALSOJNI_IMPL

#include "FalsoJNI_ImplBridge.h"

extern NameToMethodID nameToMethodId[];

extern MethodsBoolean methodsBoolean[];
extern MethodsByte methodsByte[];
extern MethodsChar methodsChar[];
extern MethodsDouble methodsDouble[];
extern MethodsFloat methodsFloat[];
extern MethodsInt methodsInt[];
extern MethodsLong methodsLong[];
extern MethodsObject methodsObject[];
extern MethodsShort methodsShort[];
extern MethodsVoid methodsVoid[];

extern NameToFieldID nameToFieldId[];

extern FieldsBoolean fieldsBoolean[];
extern FieldsByte fieldsByte[];
extern FieldsChar fieldsChar[];
extern FieldsDouble fieldsDouble[];
extern FieldsFloat fieldsFloat[];
extern FieldsInt fieldsInt[];
extern FieldsIntArray fieldsIntArray[];
extern FieldsLong fieldsLong[];
extern FieldsShort fieldsShort[];
extern FieldsString fieldsString[];

extern size_t nameToMethodId_size();

extern size_t methodsBoolean_size();
extern size_t methodsByte_size();
extern size_t methodsChar_size();
extern size_t methodsDouble_size();
extern size_t methodsFloat_size();
extern size_t methodsInt_size();
extern size_t methodsLong_size();
extern size_t methodsObject_size();
extern size_t methodsShort_size();
extern size_t methodsVoid_size();

extern size_t nameToFieldId_size();

extern size_t fieldsBoolean_size();
extern size_t fieldsByte_size();
extern size_t fieldsChar_size();
extern size_t fieldsDouble_size();
extern size_t fieldsFloat_size();
extern size_t fieldsInt_size();
extern size_t fieldsIntArray_size();
extern size_t fieldsLong_size();
extern size_t fieldsShort_size();
extern size_t fieldsString_size();

#endif // FALSOJNI_IMPL
