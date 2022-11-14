/*
 * FalsoJNI_ImplBridge.h
 *
 * Fake Java Native Interface, providing JavaVM and JNIEnv objects.
 *
 * Copyright (C) 2021 Andy Nguyen
 * Copyright (C) 2021 Rinnegatamante
 * Copyright (C) 2022 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef FALSOJNI_IMPL_BRIDGE
#define FALSOJNI_IMPL_BRIDGE

#include "config.h"
#include "utils/utils.h"
#include "jni.h"

/// DYNAMICALLY ALLOCATED ARRAYS

typedef struct {
    int id;
    const void* arr;
    jsize length;
    uint8_t freed;
} DynamicallyAllocatedArrays;

jboolean tryFreeDynamicallyAllocatedArray(const void * arr);
void saveDynamicallyAllocatedArrayPointer(const void * arr, jsize sz);
jsize* findDynamicallyAllocatedArrayLength(const void* arr);

typedef enum FIELD_TYPE {
    FIELD_TYPE_UNKNOWN   = 0,
    FIELD_TYPE_STRING    = 1,
    FIELD_TYPE_BOOLEAN   = 2,
    FIELD_TYPE_INT       = 3,
    FIELD_TYPE_INT_ARRAY = 4,
    FIELD_TYPE_BYTE      = 5,
    FIELD_TYPE_CHAR      = 6,
    FIELD_TYPE_SHORT     = 7,
    FIELD_TYPE_LONG      = 8,
    FIELD_TYPE_FLOAT     = 9,
    FIELD_TYPE_DOUBLE    = 10
} FIELD_TYPE;

typedef struct {
    int id;
    char *name;
    FIELD_TYPE f;
} NameToFieldID;

typedef struct {
    int id;
    jboolean value;
} FieldsBoolean;

typedef struct {
    int id;
    int value;
} FieldsInt;

typedef struct {
    int id;
    char *value;
} FieldsString;

typedef struct {
    int id;
    const int* value;
    jsize length;
} FieldsIntArray;

typedef struct {
    int id;
    jbyte value;
} FieldsByte;

typedef struct {
    int id;
    jchar value;
} FieldsChar;

typedef struct {
    int id;
    jshort value;
} FieldsShort;

typedef struct {
    int id;
    jlong value;
} FieldsLong;

typedef struct {
    int id;
    jfloat value;
} FieldsFloat;

typedef struct {
    int id;
    jdouble value;
} FieldsDouble;

typedef enum METHOD_TYPE {
    METHOD_TYPE_UNKNOWN   = 0,
    METHOD_TYPE_VOID      = 1,
    METHOD_TYPE_INT       = 2,
    METHOD_TYPE_FLOAT     = 3,
    METHOD_TYPE_LONG      = 4,
    METHOD_TYPE_BOOLEAN   = 5,
    METHOD_TYPE_OBJECT    = 6,
    METHOD_TYPE_INT_ARRAY = 7,
} METHOD_TYPE;

typedef struct {
    int id;
    char *name;
    METHOD_TYPE f;
} NameToMethodID;

typedef struct {
    int id;
    jobject (*Method)(jmethodID id, va_list args);
} MethodsObject;

typedef struct {
    int id;
    jint (*Method)(jmethodID id, va_list args);
} MethodsInt;

typedef struct {
    int id;
    jfloat (*Method)(jmethodID id, va_list args);
} MethodsFloat;

typedef struct {
    int id;
    void (*Method)(jmethodID id, va_list args);
} MethodsVoid;

typedef struct {
    int id;
    jboolean (*Method)(jmethodID id, va_list args);
} MethodsBoolean;

typedef struct {
    int id;
    jbyte (*Method)(jmethodID id, va_list args);
} MethodsByte;

typedef struct {
    int id;
    jshort (*Method)(jmethodID id, va_list args);
} MethodsShort;

typedef struct {
    int id;
    jchar (*Method)(jmethodID id, va_list args);
} MethodsChar;

typedef struct {
    int id;
    jlong (*Method)(jmethodID id, va_list args);
} MethodsLong;

typedef struct {
    int id;
    jdouble (*Method)(jmethodID id, va_list args);
} MethodsDouble;

#define getFieldValueById(jtype, fieldtype, containertype, container, containersize, id, defaultval) ({ \
  for (int i = 0; i < nameToFieldId_size() / sizeof(NameToFieldID); i++) { \
    if (nameToFieldId[i].id == (int)id) { \
        if (nameToFieldId[i].f != FIELD_TYPE_INT) { \
            fjni_logv_err("Field type mismatch for field #%i: expected %s, found %s", (int)id, fieldTypeToStr(fieldtype), fieldTypeToStr(nameToFieldId[i].f)); \
            return defaultval; \
        } \
         \
        const jtype * x = NULL; \
        for (int u = 0; u < containersize() / sizeof(containertype); u++) { \
            if (container[u].id == (int)id) { \
                x = &container[u].value; \
            } \
        } \
        \
        if (!x) { \
            fjni_logv_err("Field #%i is defined in NameToFieldID table but has no value set", (int)id); \
            return defaultval; \
        } \
         \
        return *x; \
    } \
  } \
  fjni_logv_err("Undefined fieldID #%i", (int)id); \
  return defaultval; \
})

#define SetPrimitiveArrayRegion(array, arrayTypeStr, start, length, buf, elementType) ({ \
    if ((array) == NULL) { \
        fjni_logv_err("SetPrimitiveArrayRegion(%s): array can not be NULL.", (arrayTypeStr)); \
        return; \
    } \
    if ((start) < 0 || (length) < 0) { \
        fjni_logv_err("SetPrimitiveArrayRegion(%s): out of bounds.", (arrayTypeStr)); \
        return; \
    } \
    \
    if ((length) != 0 && (buf) == NULL) { \
        fjni_logv_err("SetPrimitiveArrayRegion(%s): buf is null.", (arrayTypeStr));\
    } \
    \
    memcpy((array) + (start), (buf), (length) * sizeof(elementType));\
})

const char* fieldStringGet(jfieldID id);
const int* fieldIntGet(jfieldID id);
const jboolean * fieldBoolGet(jfieldID id);
const int* fieldIntArrayGet(jfieldID id);
jsize* fieldIntArrayGetLengthByPtr(const int * arr);
jfieldID getFieldIdByName(const char* name);
jobject getObjectFieldValueById(jfieldID id);
jint getIntFieldValueById(jfieldID id);
jboolean getBooleanFieldValueById(jfieldID id);
jmethodID getMethodIdByName(const char* name);

jbyte getByteFieldValueById(jfieldID id);
jchar getCharFieldValueById(jfieldID id);
jshort getShortFieldValueById(jfieldID id);
jlong getLongFieldValueById(jfieldID id);
jfloat getFloatFieldValueById(jfieldID id);
jdouble getDoubleFieldValueById(jfieldID id);

jobject methodObjectCall(jmethodID id, va_list args);
jboolean methodBooleanCall(jmethodID id, va_list args);
jbyte methodByteCall(jmethodID id, va_list args);
jchar methodCharCall(jmethodID id, va_list args);
jshort methodShortCall(jmethodID id, va_list args);
jint methodIntCall(jmethodID id, va_list args);
jlong methodLongCall(jmethodID id, va_list args);
jfloat methodFloatCall(jmethodID id, va_list args);
jdouble methodDoubleCall(jmethodID id, va_list args);
void methodVoidCall(jmethodID id, va_list args);

#endif // FALSOJNI_IMPL_BRIDGE
