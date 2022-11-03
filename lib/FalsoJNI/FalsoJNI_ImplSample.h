#ifndef FALSOJNI_IMPL_SAMPLE
#define FALSOJNI_IMPL_SAMPLE

#include "FalsoJNI_Impl.h"

#ifdef __cplusplus
extern "C" {
#endif

NameToMethodID nameToMethodId[] = {};

MethodsBoolean methodsBoolean[] = {};
MethodsByte methodsByte[] = {};
MethodsChar methodsChar[] = {};
MethodsDouble methodsDouble[] = {};
MethodsFloat methodsFloat[] = {};
MethodsInt methodsInt[] = {};
MethodsLong methodsLong[] = {};
MethodsObject methodsObject[] = {};
MethodsShort methodsShort[] = {};
MethodsVoid methodsVoid[] = {};

NameToFieldID nameToFieldId[] = {};

FieldsBoolean fieldsBoolean[] = {};
FieldsByte fieldsByte[] = {};
FieldsChar fieldsChar[] = {};
FieldsDouble fieldsDouble[] = {};
FieldsFloat fieldsFloat[] = {};
FieldsInt fieldsInt[] = {};
FieldsIntArray fieldsIntArray[] = {};
FieldsLong fieldsLong[] = {};
FieldsShort fieldsShort[] = {};
FieldsString fieldsString[] = {};

size_t nameToMethodId_size() { return sizeof nameToMethodId; }

size_t methodsBoolean_size() { return sizeof methodsBoolean; }
size_t methodsByte_size() { return sizeof methodsByte; }
size_t methodsChar_size() { return sizeof methodsChar; }
size_t methodsDouble_size() { return sizeof methodsDouble; }
size_t methodsFloat_size() { return sizeof methodsFloat; }
size_t methodsInt_size() { return sizeof methodsInt; }
size_t methodsLong_size() { return sizeof methodsLong; }
size_t methodsObject_size() { return sizeof methodsObject; }
size_t methodsShort_size() { return sizeof methodsShort; }
size_t methodsVoid_size() { return sizeof methodsVoid; }

size_t nameToFieldId_size() { return sizeof nameToFieldId; }

size_t fieldsBoolean_size() { return sizeof fieldsBoolean; }
size_t fieldsByte_size() { return sizeof fieldsByte; }
size_t fieldsChar_size() { return sizeof fieldsChar; }
size_t fieldsDouble_size() { return sizeof fieldsDouble; }
size_t fieldsFloat_size() { return sizeof fieldsFloat; }
size_t fieldsInt_size() { return sizeof fieldsInt; }
size_t fieldsIntArray_size() { return sizeof fieldsIntArray; }
size_t fieldsLong_size() { return sizeof fieldsLong; }
size_t fieldsShort_size() { return sizeof fieldsShort; }
size_t fieldsString_size() { return sizeof fieldsString; }

#ifdef __cplusplus
};
#endif

#endif // FALSOJNI_IMPL_SAMPLE
