/*
 * FalsoJNI_ImplBridge.c
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

#include "FalsoJNI_Impl.h"
#include "FalsoJNI_Logger.h"

#include "FalsoJNI_ImplBridge.h"

#include <string.h>
#include <malloc.h>
#include <pthread.h>

JavaDynArray* javaDynArrays = NULL;
pthread_mutex_t* javaDynArrays_mutex = NULL;
size_t javaDynArrays_free = 0;
size_t javaDynArrays_taken = 0;

jfieldID getFieldIdByName(const char* name) {
    for (int i = 0; i < nameToFieldId_size() / sizeof(NameToFieldID); i++) {
        if (strcmp(name, nameToFieldId[i].name) == 0) {
            return (jfieldID) nameToFieldId[i].id;
        }
    }

    fjni_logv_warn("Unknown field name \"%s\"", name);
    return NULL;
}

const char* fieldTypeToStr(FIELD_TYPE t) {
    switch (t) {
        case FIELD_TYPE_INT:
            return "FIELD_TYPE_INT";
        case FIELD_TYPE_OBJECT:
            return "FIELD_TYPE_OBJECT";
        case FIELD_TYPE_BOOLEAN:
            return "FIELD_TYPE_BOOLEAN";
        case FIELD_TYPE_BYTE:
            return "FIELD_TYPE_BYTE";
        case FIELD_TYPE_CHAR:
            return "FIELD_TYPE_CHAR";
        case FIELD_TYPE_SHORT:
            return "FIELD_TYPE_SHORT";
        case FIELD_TYPE_LONG:
            return "FIELD_TYPE_LONG";
        case FIELD_TYPE_FLOAT:
            return "FIELD_TYPE_FLOAT";
        case FIELD_TYPE_DOUBLE:
            return "FIELD_TYPE_DOUBLE";
        default:
            return "FIELD_TYPE_UNKNOWN";
    }
}

jsize getFieldTypeSize(FIELD_TYPE fieldType) {
    switch (fieldType) {
        case FIELD_TYPE_OBJECT:
            return sizeof(jobject);
        case FIELD_TYPE_BOOLEAN:
            return sizeof(jboolean);
        case FIELD_TYPE_BYTE:
            return sizeof(jbyte);
        case FIELD_TYPE_CHAR:
            return sizeof(jchar);
        case FIELD_TYPE_SHORT:
            return sizeof(jshort);
        case FIELD_TYPE_INT:
            return sizeof(jint);
        case FIELD_TYPE_LONG:
            return sizeof(jlong);
        case FIELD_TYPE_FLOAT:
            return sizeof(jfloat);
        case FIELD_TYPE_DOUBLE:
            return sizeof(jdouble);
        default:
            return sizeof(void *);
    }
}

jobject getObjectFieldValueById(jfieldID id) {
    getFieldValueById(jobject, FIELD_TYPE_OBJECT, FieldsObject, fieldsObject, fieldsObject_size, id, (jobject)0x42424242);
}

jint getIntFieldValueById(jfieldID id) {
    getFieldValueById(jint, FIELD_TYPE_INT, FieldsInt, fieldsInt, fieldsInt_size, id, 1);
}

jboolean getBooleanFieldValueById(jfieldID id) {
    getFieldValueById(jboolean, FIELD_TYPE_BOOLEAN, FieldsBoolean, fieldsBoolean, fieldsBoolean_size, id, JNI_FALSE);
}

jbyte getByteFieldValueById(jfieldID id) {
    getFieldValueById(jbyte, FIELD_TYPE_BYTE, FieldsByte, fieldsByte, fieldsByte_size, id, 'a');
}

jchar getCharFieldValueById(jfieldID id) {
    getFieldValueById(jchar, FIELD_TYPE_CHAR, FieldsChar, fieldsChar, fieldsChar_size, id, 'b');
}

jshort getShortFieldValueById(jfieldID id) {
    getFieldValueById(jshort, FIELD_TYPE_SHORT, FieldsShort, fieldsShort, fieldsShort_size, id, 1);
}

jlong getLongFieldValueById(jfieldID id) {
    getFieldValueById(jlong, FIELD_TYPE_LONG, FieldsLong, fieldsLong, fieldsLong_size, id, 1);
}

jfloat getFloatFieldValueById(jfieldID id) {
    getFieldValueById(jfloat, FIELD_TYPE_FLOAT, FieldsFloat, fieldsFloat, fieldsFloat_size, id, 1.0f);
}

jdouble getDoubleFieldValueById(jfieldID id) {
    getFieldValueById(jdouble, FIELD_TYPE_DOUBLE, FieldsDouble, fieldsDouble, fieldsDouble_size, id, 1);
}

void setObjectFieldValueById(jfieldID id, jobject value) {
    setFieldValueById(jobject, FIELD_TYPE_OBJECT, FieldsObject, fieldsObject, fieldsObject_size, id, value);
}

void setIntFieldValueById(jfieldID id, jint value) {
    setFieldValueById(jint, FIELD_TYPE_INT, FieldsInt, fieldsInt, fieldsInt_size, id, value);
}

void setBooleanFieldValueById(jfieldID id, jboolean value) {
    setFieldValueById(jboolean, FIELD_TYPE_BOOLEAN, FieldsBoolean, fieldsBoolean, fieldsBoolean_size, id, value);
}

void setByteFieldValueById(jfieldID id, jbyte value) {
    setFieldValueById(jbyte, FIELD_TYPE_BYTE, FieldsByte, fieldsByte, fieldsByte_size, id, value);
}

void setCharFieldValueById(jfieldID id, jchar value) {
    setFieldValueById(jchar, FIELD_TYPE_CHAR, FieldsChar, fieldsChar, fieldsChar_size, id, value);
}

void setShortFieldValueById(jfieldID id, jshort value) {
    setFieldValueById(jshort, FIELD_TYPE_SHORT, FieldsShort, fieldsShort, fieldsShort_size, id, value);
}

void setLongFieldValueById(jfieldID id, jlong value) {
    setFieldValueById(jlong, FIELD_TYPE_LONG, FieldsLong, fieldsLong, fieldsLong_size, id, value);
}

void setFloatFieldValueById(jfieldID id, jfloat value) {
    setFieldValueById(jfloat, FIELD_TYPE_FLOAT, FieldsFloat, fieldsFloat, fieldsFloat_size, id, value);
}

void setDoubleFieldValueById(jfieldID id, jdouble value) {
    setFieldValueById(jdouble, FIELD_TYPE_DOUBLE, FieldsDouble, fieldsDouble, fieldsDouble_size, id, value);
}

jmethodID getMethodIdByName(const char* name) {
    for (int i = 0; i < nameToMethodId_size() / sizeof(NameToMethodID); i++) {
        if (strcmp(name, nameToMethodId[i].name) == 0) {
            return (jmethodID) nameToMethodId[i].id;
        }
    }
    return NULL;
}

jobject methodObjectCall(jmethodID id, va_list args) {
    for (int i = 0; i < methodsObject_size() / sizeof(MethodsObject); i++) {
        if (methodsObject[i].id == (int)id) {
            return methodsObject[i].Method(id, args);
        }
    }

    fjni_logv_warn("method ID %i not found!", (int)id);
    return NULL;
}

void methodVoidCall(jmethodID id, va_list args) {
    for (int i = 0; i < methodsVoid_size() / sizeof(MethodsVoid); i++) {
        if (methodsVoid[i].id == (int)id) {
            return methodsVoid[i].Method(id, args);
        }
    }

    fjni_logv_warn("method ID %i not found!", (int)id);
}

jboolean methodBooleanCall(jmethodID id, va_list args) {
    for (int i = 0; i < methodsBoolean_size() / sizeof(MethodsBoolean); i++) {
        if (methodsBoolean[i].id == (int)id) {
            return methodsBoolean[i].Method(id, args);
        }
    }

    fjni_logv_warn("method ID %i not found!", (int)id);
    return JNI_FALSE;
}

jbyte methodByteCall(jmethodID id, va_list args) {
    for (int i = 0; i < methodsByte_size() / sizeof(MethodsByte); i++) {
        if (methodsByte[i].id == (int)id) {
            return methodsByte[i].Method(id, args);
        }
    }

    fjni_logv_warn("method ID %i not found!", (int)id);
    return 0;
}

jshort methodShortCall(jmethodID id, va_list args) {
    for (int i = 0; i < methodsShort_size() / sizeof(MethodsShort); i++) {
        if (methodsShort[i].id == (int)id) {
            return methodsShort[i].Method(id, args);
        }
    }

    fjni_logv_warn("method ID %i not found!", (int)id);
    return 0;
}

jdouble methodDoubleCall(jmethodID id, va_list args) {
    for (int i = 0; i < methodsDouble_size() / sizeof(MethodsDouble); i++) {
        if (methodsDouble[i].id == (int)id) {
            return methodsDouble[i].Method(id, args);
        }
    }

    fjni_logv_warn("method ID %i not found!", (int)id);
    return 0;
}

jchar methodCharCall(jmethodID id, va_list args) {
    for (int i = 0; i < methodsChar_size() / sizeof(MethodsChar); i++) {
        if (methodsChar[i].id == (int)id) {
            return methodsChar[i].Method(id, args);
        }
    }

    fjni_logv_warn("method ID %i not found!", (int)id);
    return 0;
}

jlong methodLongCall(jmethodID id, va_list args) {
    for (int i = 0; i < methodsLong_size() / sizeof(MethodsLong); i++) {
        if (methodsLong[i].id == (int)id) {
            return methodsLong[i].Method(id, args);
        }
    }

    fjni_logv_warn("method ID %i not found!", (int)id);
    return -1;
}

jint methodIntCall(jmethodID id, va_list args) {
    for (int i = 0; i < methodsInt_size() / sizeof(MethodsInt); i++) {
        if (methodsInt[i].id == (int)id) {
            return methodsInt[i].Method(id, args);
        }
    }

    fjni_logv_warn("method ID %i not found!", (int)id);
    return -1;
}

jfloat methodFloatCall(jmethodID id, va_list args) {
    for (int i = 0; i < methodsFloat_size() / sizeof(MethodsFloat); i++) {
        if (methodsFloat[i].id == (int)id) {
            return methodsFloat[i].Method(id, args);
        }
    }

    fjni_logv_warn("method ID %i not found!", (int)id);
    return -1;
}

void jda_lock() {
    if (javaDynArrays_mutex == NULL) {
        pthread_mutex_t initTmpNormal;
        javaDynArrays_mutex = malloc(sizeof(pthread_mutex_t));
        memcpy(javaDynArrays_mutex, &initTmpNormal, sizeof(pthread_mutex_t));

        if (pthread_mutex_init(javaDynArrays_mutex, NULL) != 0) {
            fjni_log_err("Failed to allocate dynarr mutex");
            javaDynArrays_mutex = NULL;
            return;
        }
    }

    pthread_mutex_lock(javaDynArrays_mutex);
}

void jda_unlock() {
    if (javaDynArrays_mutex != NULL) {
        pthread_mutex_unlock(javaDynArrays_mutex);
    }
}

jboolean jda_tryinit() {
    if (javaDynArrays == NULL) {
        javaDynArrays = malloc(16 * sizeof(JavaDynArray));
        if (!javaDynArrays)
            return JNI_FALSE;

        for (int i = 0; i < 16; ++i) {
            javaDynArrays[i].array = NULL;
            javaDynArrays[i].len = -1;
            javaDynArrays[i].type = FIELD_TYPE_UNKNOWN;
        }

        javaDynArrays_free = 16;
        javaDynArrays_taken = 0;
    }

    return JNI_TRUE;
}

jboolean jda_extend() {
    if (jda_tryinit() == JNI_FALSE)
        return JNI_FALSE;

    if (javaDynArrays_free == 0) {
        if (javaDynArrays == NULL)
            return JNI_FALSE;

        javaDynArrays = realloc(javaDynArrays, (sizeof(JavaDynArray) * (javaDynArrays_taken+16)));

        if (javaDynArrays == NULL)
            return JNI_FALSE;

        for (int i = javaDynArrays_taken; i < javaDynArrays_taken+16; ++i) {
            javaDynArrays[i].array = NULL;
            javaDynArrays[i].len = -1;
            javaDynArrays[i].type = FIELD_TYPE_UNKNOWN;
        }

        javaDynArrays_free = 16;
    }
    return JNI_TRUE;
}

JavaDynArray * jda_alloc(jsize len, FIELD_TYPE type) {
    jda_lock();

    void * array = malloc(len * getFieldTypeSize(type));
    if (!array) {
        jda_unlock();
        return NULL;
    }

    if (jda_extend() == JNI_FALSE) {
        jda_unlock();
        free(array);
        return NULL;
    }

    int index = -1;
    for (int i = 0; i < (javaDynArrays_taken+javaDynArrays_free); ++i) {
        if (javaDynArrays[i].array == NULL) {
            index = i;
        }
    }

    if (index == -1) {
        jda_unlock();
        free(array);
        return NULL;
    }

    javaDynArrays[index].array = array;
    javaDynArrays[index].len = len;
    javaDynArrays[index].type = type;

    javaDynArrays_taken++;
    javaDynArrays_free--;

    JavaDynArray * ret = &javaDynArrays[index];
    jda_unlock();
    return ret;
}

jsize jda_sizeof(JavaDynArray * jda) {
    if (!jda) return -1;
    if (!javaDynArrays) return -1;

    jda_lock();
    if (javaDynArrays_taken == 0) {
        jda_unlock();
        return -1;
    }

    for (int i = 0; i < (javaDynArrays_taken+javaDynArrays_free); i++) {
        if (jda == &javaDynArrays[i]) {
            if (javaDynArrays[i].array == NULL) {
                jda_unlock();
                return NULL;
            }
            jsize ret = javaDynArrays[i].len;
            jda_unlock();
            return ret;
        }
    }

    jda_unlock();
    return -1;
}

jboolean jda_free(JavaDynArray * jda) {
    if (!jda) return JNI_FALSE;
    if (!javaDynArrays) return JNI_FALSE;

    jda_lock();
    if (javaDynArrays_taken == 0) {
        jda_unlock();
        return JNI_FALSE;
    }

    int index = -1;
    for (int i = 0; i < (javaDynArrays_taken+javaDynArrays_free); ++i) {
        if (jda == &javaDynArrays[i]) {
            index = i;
        }
    }

    if (index == -1) {
        jda_unlock();
        return JNI_FALSE;
    }

    free(javaDynArrays[index].array);
    javaDynArrays[index].array = NULL;
    javaDynArrays[index].type = FIELD_TYPE_UNKNOWN;
    javaDynArrays[index].len = 0;

    javaDynArrays_taken--;
    javaDynArrays_free++;
    jda_unlock();
    return JNI_TRUE;
}

JavaDynArray * jda_find(void * arr) {
    if (!arr) return NULL;
    if (!javaDynArrays) return NULL;

    jda_lock();
    if (javaDynArrays_taken == 0) {
        jda_unlock();
        return NULL;
    }

    for (int i = 0; i < (javaDynArrays_taken+javaDynArrays_free); ++i) {
        if (arr == &javaDynArrays[i]) {
            if (javaDynArrays[i].array == NULL) {
                jda_unlock();
                return NULL;
            }
            JavaDynArray * ret = &javaDynArrays[i];
            jda_unlock();
            return ret;
        }
    }

    jda_unlock();
    return NULL;
}

va_list _AtoV(int dummy, ...) {
    va_list args1;
    va_start(args1, dummy);
    va_list args2;
    va_copy(args2, args1);
    va_end(args1);
    return args2;
}
