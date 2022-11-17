/*
 * FalsoJNI_Impl.h
 *
 * Fake Java Native Interface, providing JavaVM and JNIEnv objects.
 *
 * Copyright (C) 2022 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

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
extern FieldsObject fieldsObject[];
extern FieldsLong fieldsLong[];
extern FieldsShort fieldsShort[];

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
extern size_t fieldsObject_size();
extern size_t fieldsLong_size();
extern size_t fieldsShort_size();

#define __FALSOJNI_IMPL_CONTAINER_SIZES \
size_t nameToMethodId_size() { return sizeof nameToMethodId; } \
size_t methodsBoolean_size() { return sizeof methodsBoolean; } \
size_t methodsByte_size() { return sizeof methodsByte; } \
size_t methodsChar_size() { return sizeof methodsChar; } \
size_t methodsDouble_size() { return sizeof methodsDouble; } \
size_t methodsFloat_size() { return sizeof methodsFloat; } \
size_t methodsInt_size() { return sizeof methodsInt; } \
size_t methodsLong_size() { return sizeof methodsLong; } \
size_t methodsObject_size() { return sizeof methodsObject; } \
size_t methodsShort_size() { return sizeof methodsShort; } \
size_t methodsVoid_size() { return sizeof methodsVoid; } \
size_t nameToFieldId_size() { return sizeof nameToFieldId; } \
size_t fieldsBoolean_size() { return sizeof fieldsBoolean; } \
size_t fieldsByte_size() { return sizeof fieldsByte; } \
size_t fieldsChar_size() { return sizeof fieldsChar; } \
size_t fieldsDouble_size() { return sizeof fieldsDouble; } \
size_t fieldsFloat_size() { return sizeof fieldsFloat; } \
size_t fieldsInt_size() { return sizeof fieldsInt; } \
size_t fieldsObject_size() { return sizeof fieldsObject; } \
size_t fieldsLong_size() { return sizeof fieldsLong; } \
size_t fieldsShort_size() { return sizeof fieldsShort; }

#endif // FALSOJNI_IMPL
