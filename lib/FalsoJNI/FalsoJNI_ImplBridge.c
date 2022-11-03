#include "FalsoJNI_Impl.h"
#include "FalsoJNI_Logger.h"

#include "FalsoJNI_ImplBridge.h"

#include <string.h>
#include <malloc.h>
#include <pthread.h>

const char* fieldStringGet(jfieldID id) {
    for (int i = 0; i < fieldsString_size() / sizeof(FieldsString); i++) {
        if (fieldsString[i].id == (int)id) {
            return fieldsString[i].value;
        }
    }
    return NULL;
}

const int* fieldIntArrayGet(jfieldID id) {
    for (int i = 0; i < fieldsIntArray_size() / sizeof(FieldsIntArray); i++) {
        if (fieldsIntArray[i].id == (int)id) {
            return fieldsIntArray[i].value;
        }
    }
    return NULL;
}

jsize* fieldIntArrayGetLengthByPtr(const int * arr) {
    for (int i = 0; i < fieldsIntArray_size() / sizeof(FieldsIntArray); i++) {
        if (fieldsIntArray[i].value == arr) {
            fprintf(stderr, "found array: id #%i\n", fieldsIntArray[i].id);
            return &fieldsIntArray[i].length;
        }
    }
    //fprintf(stderr, "not found int array\n");
    return NULL;
}


jfieldID getFieldIdByName(const char* name) {
    for (int i = 0; i < nameToFieldId_size() / sizeof(NameToFieldID); i++) {
        if (strcmp(name, nameToFieldId[i].name) == 0) {
            return (jfieldID) nameToFieldId[i].id;
        }
    }

    fjni_logv_warn("Unknown field name \"%s\"", name);
    return NULL;
}

// Field type Object can represent different types, so no macro here.
jobject getObjectFieldValueById(jfieldID id) {
    for (int i = 0; i < nameToFieldId_size() / sizeof(NameToFieldID); i++) {
        if (nameToFieldId[i].id == (int)id) {
            switch (nameToFieldId[i].f) {
                case FIELD_TYPE_STRING: {
                    const char *ret = fieldStringGet(id);
                    if (ret) return (jobject)ret;
                    value_fail:
                    fjni_logv_warn("Field ID %i (\"%s\") is known but has no associated value defined", id, nameToFieldId[i].name);
                    return NULL;
                }
                case FIELD_TYPE_INT_ARRAY: {
                    const int *ret = fieldIntArrayGet(id);
                    if (ret) return (jobject)ret;
                    goto value_fail;
                }
                    // Add other types represented by type Object here when needed
                case FIELD_TYPE_UNKNOWN:
                default:
                    fjni_logv_warn("Unknown field type for \"%s\"!\n", nameToFieldId[i].name);
                    return NULL;
            }
        }
    }

    fjni_logv_warn("Unknown field id %i\n", (int)id);
    return NULL;
}

const char* fieldTypeToStr(FIELD_TYPE t) {
    switch (t) {
        case FIELD_TYPE_INT:
            return "FIELD_TYPE_INT";
        case FIELD_TYPE_STRING:
            return "FIELD_TYPE_STRING";
        case FIELD_TYPE_BOOLEAN:
            return "FIELD_TYPE_BOOLEAN";
        case FIELD_TYPE_INT_ARRAY:
            return "FIELD_TYPE_INT_ARRAY";
        default:
            return "FIELD_TYPE_UNKNOWN";
    }
}

jint getIntFieldValueById(jfieldID id) {
    getFieldValueById(jint, FIELD_TYPE_INT, FieldsInt, fieldsInt, fieldsInt_size, id, 0);
}

jboolean getBooleanFieldValueById(jfieldID id) {
    getFieldValueById(jboolean, FIELD_TYPE_BOOLEAN, FieldsBoolean, fieldsBoolean, fieldsBoolean_size, id, JNI_FALSE);
}

jbyte getByteFieldValueById(jfieldID id) {
    getFieldValueById(jbyte, FIELD_TYPE_BYTE, FieldsByte, fieldsByte, fieldsByte_size, id, JNI_FALSE);
}

jchar getCharFieldValueById(jfieldID id) {
    getFieldValueById(jchar, FIELD_TYPE_CHAR, FieldsChar, fieldsChar, fieldsChar_size, id, JNI_FALSE);
}

jshort getShortFieldValueById(jfieldID id) {
    getFieldValueById(jshort, FIELD_TYPE_SHORT, FieldsShort, fieldsShort, fieldsShort_size, id, JNI_FALSE);
}

jlong getLongFieldValueById(jfieldID id) {
    getFieldValueById(jlong, FIELD_TYPE_LONG, FieldsLong, fieldsLong, fieldsLong_size, id, JNI_FALSE);
}

jfloat getFloatFieldValueById(jfieldID id) {
    getFieldValueById(jfloat, FIELD_TYPE_FLOAT, FieldsFloat, fieldsFloat, fieldsFloat_size, id, JNI_FALSE);
}

jdouble getDoubleFieldValueById(jfieldID id) {
    getFieldValueById(jdouble, FIELD_TYPE_DOUBLE, FieldsDouble, fieldsDouble, fieldsDouble_size, id, JNI_FALSE);
}

jmethodID getMethodIdByName(const char* name) {
    for (int i = 0; i < nameToMethodId_size() / sizeof(NameToMethodID); i++) {
        if (strcmp(name, nameToMethodId[i].name) == 0) {
            return (jmethodID) nameToMethodId[i].id;
        }
    }

    fjni_logv_warn("Unknown method name \"%s\"", name);
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

int dynamicallyAllocatedArrays_length = 0;
DynamicallyAllocatedArrays* dynamicallyAllocatedArrays = NULL;
pthread_mutex_t* dynamicallyAllocatedArrays_mutex = NULL;

void dynarr_check_init_mutex() {
    if (dynamicallyAllocatedArrays_mutex == NULL) {
        pthread_mutex_t initTmpNormal;
        dynamicallyAllocatedArrays_mutex = malloc(sizeof(pthread_mutex_t));
        memcpy(dynamicallyAllocatedArrays_mutex, &initTmpNormal, sizeof(pthread_mutex_t));

        if (pthread_mutex_init(dynamicallyAllocatedArrays_mutex, NULL) != 0) {
            fjni_log_err("Failed to allocate dynarr mutex");
        }
    }
}

jsize* findDynamicallyAllocatedArrayLength(const void* arr) {
    dynarr_check_init_mutex();

    jsize* ret = NULL;
    pthread_mutex_lock(dynamicallyAllocatedArrays_mutex);

    if (dynamicallyAllocatedArrays_length > 0) {
        for (int i = 0; i < dynamicallyAllocatedArrays_length; i++) {
            if (dynamicallyAllocatedArrays[i].arr == arr) {
                ret = &dynamicallyAllocatedArrays[i].length;
                break;
            }
        }
    }
    pthread_mutex_unlock(dynamicallyAllocatedArrays_mutex);
    if (!ret) { fjni_logv_warn("not found dynalloc array 0x%x", (int)arr); }
    return ret;
}

void saveDynamicallyAllocatedArrayPointer(const void * arr, jsize sz) {
    dynarr_check_init_mutex();

    pthread_mutex_lock(dynamicallyAllocatedArrays_mutex);
    if (dynamicallyAllocatedArrays_length == 0) {
        dynamicallyAllocatedArrays = malloc(sizeof(DynamicallyAllocatedArrays));
    } else {
        dynamicallyAllocatedArrays = realloc(dynamicallyAllocatedArrays, (sizeof(DynamicallyAllocatedArrays) * (dynamicallyAllocatedArrays_length+1)));
    }
    dynamicallyAllocatedArrays[dynamicallyAllocatedArrays_length].length = sz;
    dynamicallyAllocatedArrays[dynamicallyAllocatedArrays_length].arr = arr;
    dynamicallyAllocatedArrays[dynamicallyAllocatedArrays_length].id = dynamicallyAllocatedArrays_length;
    dynamicallyAllocatedArrays[dynamicallyAllocatedArrays_length].freed = 0;
    dynamicallyAllocatedArrays_length++;
    pthread_mutex_unlock(dynamicallyAllocatedArrays_mutex);
}

jboolean tryFreeDynamicallyAllocatedArray(const void * arr) {
    dynarr_check_init_mutex();

    pthread_mutex_lock(dynamicallyAllocatedArrays_mutex);

    if (dynamicallyAllocatedArrays_length <= 0) {
        pthread_mutex_unlock(dynamicallyAllocatedArrays_mutex);
        return JNI_FALSE;
    }

    for (int i = 0; i < dynamicallyAllocatedArrays_length; i++) {
        if (dynamicallyAllocatedArrays[i].arr == arr) {
            if (dynamicallyAllocatedArrays[i].freed == 1 || dynamicallyAllocatedArrays[i].arr == NULL) {
                pthread_mutex_unlock(dynamicallyAllocatedArrays_mutex);
                return JNI_TRUE;
            }
            free((void*)dynamicallyAllocatedArrays[i].arr);
            dynamicallyAllocatedArrays[i].freed = 1;
            pthread_mutex_unlock(dynamicallyAllocatedArrays_mutex);
            return JNI_TRUE;
        }
    }

    pthread_mutex_unlock(dynamicallyAllocatedArrays_mutex);
    return JNI_FALSE;
}
