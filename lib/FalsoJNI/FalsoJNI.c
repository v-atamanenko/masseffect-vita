/*
 * jni.c
 *
 * Fake Java Native Interface, providing JavaVM and JNIEnv objects.
 *
 * Copyright (C) 2021 Andy Nguyen
 * Copyright (C) 2021 Rinnegatamante
 * Copyright (C) 2022 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 *
 * Contains parts of Dalvik implementation of JNI interfaces,
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#pragma ide diagnostic ignored "UnusedParameter"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <psp2/apputil.h>

#include "FalsoJNI_ImplBridge.h"
#include "FalsoJNI_Logger.h"

// Objects to be passed to client applications:
JavaVM jvm;
JNIEnv jni;

// Private structs backing the above objects:
struct JNIInvokeInterface * _jvm; // NOLINT(bugprone-reserved-identifier)
struct JNINativeInterface * _jni; // NOLINT(bugprone-reserved-identifier)

/*
 * JavaVM Methods
 */

jint DestroyJavaVM(JavaVM* vm) {
    fjni_log_dbg("[JVM] DestroyJavaVM(): ignored");
    return 0;
}

jint AttachCurrentThread(JavaVM* vm, JNIEnv **p_env, void *thr_args) {
    fjni_log_dbg("[JVM] AttachCurrentThread(vm, *p_env, thr_args)");
    *p_env = &jni;
    return 0;
}

jint DetachCurrentThread(JavaVM* vm) {
    fjni_log_dbg("[JVM] DetachCurrentThread(): ignored");
    // Since we don't operate the actual Java VM, we don't need to care about
    // it being available only in one thread. Anyway, due to this restriction,
    // client applications are guaranteed to use JNI in a safe way.
    return 0;
}

jint GetEnv(JavaVM *vm, void **env, jint version) {
    fjni_logv_dbg("[JVM] GetEnv(vm, **env, version:%i)", version);
    *env = &jni;
    return JNI_OK;
}

jint AttachCurrentThreadAsDaemon(JavaVM* vm, JNIEnv** penv, void* thr_args) {
    fjni_log_dbg("[JVM] AttachCurrentThreadAsDaemon(vm, *p_env, thr_args)");
    *penv = &jni;
    return 0;
}

/*
 * JNIEnv Methods
 */

jint GetVersion(JNIEnv * env) {
    fjni_log_dbg("[JNI] GetVersion(env)");
    return JNI_VERSION_1_6;
}

jclass DefineClass(JNIEnv* env, const char* name, jobject obj, const jbyte* byt, jsize sz) {
    fjni_logv_warn("[JNI] DefineClass(env, \"%s\", 0x%x, 0x%x, %i): not implemented", name, obj, byt, sz);
    return 0;
}

jclass FindClass(JNIEnv* env, const char* name) {
    // While we don't manipulate any actual classes here, the following code
    // serves two purposes:
    //   1) Being able to uniquely identify constructor methods for classes
    //      ("<init>"), since in GetMethodID we only receive class pointer.
    //   2) Providing a valid pointer to a valid object so that it behaves
    //      normally in memory and can be freed.

    jclass * clazz = (jclass *) strdup(name);
    fjni_logv_dbg("[JNI] FindClass(%s): 0x%x", name, (int)clazz);
    return (jclass)clazz;
}

jmethodID FromReflectedMethod(JNIEnv* env, jobject method) {
    fjni_logv_warn("[JNI] FromReflectedMethod(env, 0x%x): not implemented", (int)method);
    return NULL;
}

jfieldID FromReflectedField(JNIEnv* env, jobject field) {
    fjni_logv_warn("[JNI] FromReflectedField(env, 0x%x): not implemented", (int)field);
    return NULL;
}

jobject ToReflectedMethod(JNIEnv* env, jclass cls, jmethodID methodID, jboolean isStatic) {
    fjni_logv_warn("[JNI] ToReflectedMethod(env, 0x%x, %i, %i): not implemented", (int)cls, methodID, isStatic);
    return NULL;
}

jclass GetSuperclass(JNIEnv* env, jclass clazz) {
    fjni_logv_warn("[JNI] GetSuperclass(env, 0x%x): not implemented", (int)clazz);
    return NULL;
}

jboolean IsAssignableFrom(JNIEnv* env, jclass clazz1, jclass clazz2) {
    fjni_logv_warn("[JNI] IsAssignableFrom(env, 0x%x, 0x%x): not implemented", (int)clazz1, (int)clazz2);
    return 0;
}

jobject ToReflectedField(JNIEnv* env, jclass cls, jfieldID fieldID, jboolean isStatic) {
    fjni_logv_warn("[JNI] ToReflectedField(env, 0x%x, %i, %i): not implemented", (int)cls, fieldID, isStatic);
    return 0;
}

// TODO: Handle exceptions

jint Throw(JNIEnv* env, jthrowable obj) {
    fjni_logv_warn("[JNI] Throw(env, 0x%x): not implemented", (int)obj);
    return 0;
}

jint ThrowNew(JNIEnv* env, jclass clazz, const char* message) {
    fjni_logv_warn("[JNI] ThrowNew(env, 0x%x, \"%s\"): not implemented", (int)clazz, message);
    return 0;
}

jthrowable ExceptionOccurred(JNIEnv* env) {
    fjni_log_dbg("[JNI] ExceptionOccurred(env): ignored");
    // We never have exceptions.
    return NULL;
}

void ExceptionDescribe(JNIEnv* env) {
    fjni_log_dbg("[JNI] ExceptionDescribe(env): ignored");
    // We never have exceptions.
}

void ExceptionClear(JNIEnv* env) {
    fjni_log_dbg("[JNI] ExceptionClear(env): ignored");
    // We never have exceptions.
}

void FatalError(JNIEnv* env, const char* msg) {
    fjni_log_err(msg);
    abort();
}

jint PushLocalFrame(JNIEnv* env, jint capacity) {
    fjni_logv_dbg("[JNI] PushLocalFrame(env, %i): ignored", capacity);
    // Since we operate with global pointers everywhere, no need to scope them.
    return 0;
}

jobject PopLocalFrame(JNIEnv* env, jobject result) {
    fjni_logv_dbg("[JNI] PopLocalFrame(env, 0x%x): ignored", (int)result);
    // Since we operate with global pointers everywhere, no need to scope them.
    return NULL;
}

jobject NewGlobalRef(JNIEnv* env, jobject obj) {
    fjni_logv_dbg("[JNI] NewGlobalRef(env, 0x%x)", (int)obj);

    // The concept of global/local references really makes sense only with
    // a real JVM. Here, since we basically operate with shared global pointers
    // everywhere, it should be safe to just return `obj` back

    return obj;
}

void DeleteGlobalRef(JNIEnv* env, jobject obj) {
    fjni_logv_dbg("[JNI] DeleteGlobalRef(env, 0x%x)", (int)obj);

    // Empirically, DeleteGlobalRef() is called on dynamically allocated things
    // returned from other JNI functions. For most of them it's safe to just
    // call free(), except for dynamically allocated arrays. We provide
    // a separate "destructor" for those.

    if (tryFreeDynamicallyAllocatedArray(obj) == JNI_FALSE) {
        // Reserved fake identifiers
        if ((int)obj != 0x42424242 && (int)obj != 0x69696969) {
            if (obj) free(obj);
        }
    }
}

void DeleteLocalRef(JNIEnv* env, jobject obj) {
    fjni_logv_dbg("[JNI] DeleteLocalRef(env, 0x%x): ignored", (int)obj);
    // Local references are useless in the setting of our fake JVM.
}

jboolean IsSameObject(JNIEnv* env, jobject ref1, jobject ref2) {
    fjni_logv_dbg("[JNI] IsSameObject(env, 0x%x, 0x%x)", (int)ref1, (int)ref2);
    return (ref1 == ref2) ? JNI_TRUE : JNI_FALSE;
}

jobject NewLocalRef(JNIEnv* env, jobject obj) {
    fjni_logv_dbg("[JNI] NewLocalRef(env, 0x%x): ignored", (int)obj);
    // Local references are useless in the setting of our fake JVM.
    // Just return `obj` back.
    return obj;
}

jint EnsureLocalCapacity(JNIEnv* env, jint capacity) {
    fjni_logv_dbg("[JNI] EnsureLocalCapacity(env, %i)", capacity);
    return JNI_OK;
}

jobject AllocObject(JNIEnv* env, jclass clazz) {
    fjni_logv_warn("[JNI] AllocObject(env, 0x%x): not implemented", (int)clazz);
    return 0;
}

jobject NewObject(JNIEnv* env, jclass clazz, jmethodID methodID, ...) {
    fjni_logv_dbg("[JNI] NewObject(env, 0x%x, %i)", clazz, methodID);

    jobject ret;
    va_list args;
    va_start(args, methodID);
    ret = methodObjectCall(methodID, args);
    va_end(args);

    return ret;
}

jobject NewObjectV(JNIEnv* env, jclass clazz, jmethodID methodID, va_list args) {
    fjni_logv_dbg("[JNI] NewObjectV(env, 0x%x, %i)", clazz, methodID);
    return methodObjectCall(methodID, args);
}

jobject NewObjectA(JNIEnv* env, jclass clazz, jmethodID methodID, const jvalue *args) {
    fjni_logv_warn("[JNI] NewObjectA(env, 0x%x, %i): not implemented", (int)clazz, methodID);
    return NULL;
}

jclass GetObjectClass(JNIEnv* env, jobject obj) {
    fjni_logv_dbg("[JNI] GetObjectClass(0x%x)", (int)obj);
    // Due to the way we implement class methods, it's safe to not waste
    // resources on mapping objects to classess and return just a constant val.
    return (jclass)0x44444444;
}

jboolean IsInstanceOf(JNIEnv* env, jobject obj, jclass clazz) {
    fjni_logv_warn("[JNI] IsInstanceOf(env, 0x%x, 0x%x): not implemented", (int)obj, (int)clazz);
    return 0;
}

jmethodID GetMethodID(JNIEnv* env, jclass clazz, const char* name, const char* sig) {
    jmethodID ret;

    if (strcmp("<init>", name) == 0) {
        if (!clazz) {
            fjni_log_err("Cannot find method ID for class NULL");
            return NULL;
        }

        // In FindClass we return a char ptr of class name as `clazz`, so we
        // can use it here for distinguishing different constructors
        char name_new[512];
        snprintf(name_new, 511, "%s/%s", (char*)clazz, name);

        ret = getMethodIdByName(name_new);
    } else {
        ret = getMethodIdByName(name);
    }

    fjni_logv_dbg("[JNI] GetMethodID(env, 0x%x, \"%s\", \"%s\"): %i", (int)clazz, name, sig, (int)ret);
    return ret;
}

jobject CallObjectMethod(JNIEnv* env, jobject obj, jmethodID methodID, ...) {
    fjni_logv_dbg("[JNI] CallObjectMethod(env, 0x%x, %i)", (int)obj, methodID);

    jobject ret;
    va_list args;
    va_start(args, methodID);
    ret = methodObjectCall(methodID, args);
    va_end(args);

    return ret;
}

jobject CallObjectMethodV(JNIEnv* env, jobject obj, jmethodID methodID, va_list args) {
    fjni_logv_dbg("[JNI] CallObjectMethodV(env, 0x%x, %i, args)", (int)obj, (int)methodID);
    return methodObjectCall(methodID, args);
}

jobject CallObjectMethodA(JNIEnv* env, jobject obj, jmethodID methodID, const jvalue* args) {
    fjni_logv_warn("[JNI] CallObjectMethodA(env, 0x%x, %i, args): not implemented", (int)obj, (int)methodID);
    return NULL;
}

jboolean CallBooleanMethod(JNIEnv* env, jobject obj, jmethodID methodID, ...) {
    fjni_logv_dbg("[JNI] CallBooleanMethod(env, 0x%x, %i)", (int)obj, methodID);

    jboolean ret;
    va_list args;
    va_start(args, methodID);
    ret = methodBooleanCall(methodID, args);
    va_end(args);

    return ret;
}

jboolean CallBooleanMethodV(JNIEnv* env, jobject obj, jmethodID methodID, va_list args) {
    fjni_logv_dbg("[JNI] CallBooleanMethodV(env, 0x%x, %i, args)", (int)obj, (int)methodID);
    return methodBooleanCall(methodID, args);
}

jboolean CallBooleanMethodA(JNIEnv* env, jobject obj, jmethodID methodID, const jvalue* args) {
    fjni_logv_warn("[JNI] CallBooleanMethodA(env, 0x%x, %i, args): not implemented", (int)obj, (int)methodID);
    return JNI_FALSE;
}

jbyte CallByteMethod(JNIEnv* env, jobject obj, jmethodID methodID, ...) {
    fjni_logv_dbg("[JNI] CallByteMethod(env, 0x%x, %i)", (int)obj, methodID);

    jbyte ret;
    va_list args;
    va_start(args, methodID);
    ret = methodByteCall(methodID, args);
    va_end(args);

    return ret;
}

jbyte CallByteMethodV(JNIEnv* env, jobject obj, jmethodID methodID, va_list args) {
    fjni_logv_dbg("[JNI] CallByteMethodV(env, 0x%x, %i, args)", (int)obj, (int)methodID);
    return methodByteCall(methodID, args);
}

jbyte CallByteMethodA(JNIEnv* env, jobject obj, jmethodID methodID, const jvalue* args) {
    fjni_logv_warn("[JNI] CallByteMethodA(env, 0x%x, %i, args): not implemented", (int)obj, (int)methodID);
    return 0;
}

jchar CallCharMethod(JNIEnv* env, jobject obj, jmethodID methodID, ...) {
    fjni_logv_dbg("[JNI] CallCharMethod(env, 0x%x, %i)", (int)obj, methodID);

    jchar ret;
    va_list args;
    va_start(args, methodID);
    ret = methodCharCall(methodID, args);
    va_end(args);

    return ret;
}

jchar CallCharMethodV(JNIEnv* env, jobject obj, jmethodID methodID, va_list args) {
    fjni_logv_dbg("[JNI] CallCharMethodV(env, 0x%x, %i, args)", (int)obj, (int)methodID);
    return methodCharCall(methodID, args);
}

jchar CallCharMethodA(JNIEnv* env, jobject obj, jmethodID methodID, const jvalue* args) {
    fjni_logv_warn("[JNI] CallCharMethodA(env, 0x%x, %i, args): not implemented", (int)obj, (int)methodID);
    return 0;
}

jshort CallShortMethod(JNIEnv* env, jobject obj, jmethodID methodID, ...) {
    fjni_logv_dbg("[JNI] CallShortMethod(env, 0x%x, %i)", (int)obj, methodID);

    jshort ret;
    va_list args;
    va_start(args, methodID);
    ret = methodShortCall(methodID, args);
    va_end(args);

    return ret;
}

jshort CallShortMethodV(JNIEnv* env, jobject obj, jmethodID methodID, va_list args) {
    fjni_logv_dbg("[JNI] CallShortMethodV(env, 0x%x, %i, args)", (int)obj, (int)methodID);
    return methodShortCall(methodID, args);
}

jshort CallShortMethodA(JNIEnv* env, jobject obj, jmethodID methodID, const jvalue* args) {
    fjni_logv_warn("[JNI] CallShortMethodA(env, 0x%x, %i, args): not implemented", (int)obj, (int)methodID);
    return 0;
}

jint CallIntMethod(JNIEnv* env, jobject obj, jmethodID methodID, ...) {
    fjni_logv_dbg("[JNI] CallIntMethod(env, 0x%x, %i, ...)", (int)obj, methodID);

    jint ret;
    va_list args;
    va_start(args, methodID);
    ret = methodIntCall(methodID, args);
    va_end(args);

    return ret;
}

jint CallIntMethodV(JNIEnv* env, jobject obj, jmethodID methodID, va_list args) {
    fjni_logv_dbg("[JNI] CallIntMethodV(env, 0x%x, %i, args)", (int)obj, methodID);
    return methodIntCall(methodID, args);
}

jint CallIntMethodA(JNIEnv* env, jobject obj, jmethodID methodID, const jvalue* args) {
    fjni_logv_warn("[JNI] CallIntMethodA(env, 0x%x, %i, args): not implemented", (int)obj, (int)methodID);
    return 0;
}

jlong CallLongMethod(JNIEnv* env, jobject obj, jmethodID methodID, ...) {
    fjni_logv_dbg("[JNI] CallLongMethod(env, 0x%x, %i, ...)", (int)obj, methodID);

    jlong ret;
    va_list args;
    va_start(args, methodID);
    ret = methodLongCall(methodID, args);
    va_end(args);

    return ret;
}

jlong CallLongMethodV(JNIEnv* env, jobject obj, jmethodID methodID, va_list args) {
    fjni_logv_dbg("[JNI] CallLongMethodV(env, 0x%x, %i, args)", (int)obj, methodID);
    return methodLongCall(methodID, args);
}

jlong CallLongMethodA(JNIEnv* env, jobject obj, jmethodID methodID, const jvalue* args) {
    fjni_logv_warn("[JNI] CallLongMethodA(env, 0x%x, %i, args): not implemented", (int)obj, (int)methodID);
    return 0;
}

jfloat CallFloatMethod(JNIEnv* env, jobject obj, jmethodID methodID, ...) {
    fjni_logv_dbg("[JNI] CallFloatMethod(env, 0x%x, %i, ...)", (int)obj, methodID);

    jfloat ret;
    va_list args;
    va_start(args, methodID);
    ret = methodFloatCall(methodID, args);
    va_end(args);

    return ret;
}

jfloat CallFloatMethodV(JNIEnv* env, jobject obj, jmethodID methodID, va_list args) {
    fjni_logv_dbg("[JNI] CallFloatMethodV(env, 0x%x, %i, args)", (int)obj, methodID);
    return methodFloatCall(methodID, args);
}

jfloat CallFloatMethodA(JNIEnv* env, jobject obj, jmethodID methodID, const jvalue* args) {
    fjni_logv_warn("[JNI] CallFloatMethodA(env, 0x%x, %i, args): not implemented", (int)obj, (int)methodID);
    return 0;
}

jdouble CallDoubleMethod(JNIEnv* env, jobject obj, jmethodID methodID, ...) {
    fjni_logv_dbg("[JNI] CallDoubleMethod(env, 0x%x, %i, ...)", (int)obj, methodID);

    jdouble ret;
    va_list args;
    va_start(args, methodID);
    ret = methodDoubleCall(methodID, args);
    va_end(args);

    return ret;
}

jdouble CallDoubleMethodV(JNIEnv* env, jobject obj, jmethodID methodID, va_list args) {
    fjni_logv_dbg("[JNI] CallDoubleMethodV(env, 0x%x, %i, args)", (int)obj, methodID);
    return methodDoubleCall(methodID, args);
}

jdouble CallDoubleMethodA(JNIEnv* env, jobject obj, jmethodID methodID, const jvalue* args) {
    fjni_logv_warn("[JNI] CallDoubleMethodA(env, 0x%x, %i, args): not implemented", (int)obj, (int)methodID);
    return 0;
}

void CallVoidMethod(JNIEnv* env, jobject obj, jmethodID methodID, ...) {
    fjni_logv_dbg("[JNI] CallVoidMethod(env, 0x%x, %i, ...)", (int)obj, methodID);

    va_list args;
    va_start(args, methodID);
    methodVoidCall(methodID, args);
    va_end(args);
}

void CallVoidMethodV(JNIEnv* env, jobject clazz, jmethodID methodID, va_list args) {
    fjni_logv_dbg("[JNI] CallVoidMethodV(env, 0x%x, %i, args)", (int)clazz, methodID);
    methodVoidCall(methodID, args);
}

void CallVoidMethodA(JNIEnv* env, jobject obj, jmethodID methodID, const jvalue* args) {
    fjni_logv_warn("[JNI] CallVoidMethodA(env, 0x%x, %i, args): not implemented", (int)obj, (int)methodID);
}

jobject CallNonvirtualObjectMethod(JNIEnv* env, jobject obj, jclass clazz, jmethodID methodID, ...) {
    fjni_logv_dbg("[JNI] CallNonvirtualObjectMethod(env, 0x%x, 0x%x, %i, ...)", (int)obj, (int)clazz, methodID);

    jobject ret;
    va_list args;
    va_start(args, methodID);
    ret = methodObjectCall(methodID, args);
    va_end(args);

    return ret;
}

jobject CallNonvirtualObjectMethodV(JNIEnv* env, jobject obj, jclass clazz, jmethodID methodID, va_list args) {
    fjni_logv_dbg("[JNI] CallNonvirtualObjectMethodV(env, 0x%x, 0x%x, %i, args)", (int)obj, (int)clazz, methodID);
    return methodObjectCall(methodID, args);
}

jobject CallNonvirtualObjectMethodA(JNIEnv* env, jobject obj, jclass clazz, jmethodID methodID, const jvalue* args) {
    fjni_logv_warn("[JNI] CallNonvirtualObjectMethodA(env, 0x%x, 0x%x, %i, args): not implemented", (int)obj, (int)clazz, methodID);
    return NULL;
}

jboolean CallNonvirtualBooleanMethod(JNIEnv* env, jobject obj, jclass clazz, jmethodID methodID, ...) {
    fjni_logv_dbg("[JNI] CallNonvirtualBooleanMethod(env, 0x%x, 0x%x, %i, ...)", (int)obj, (int)clazz, methodID);

    jboolean ret;
    va_list args;
    va_start(args, methodID);
    ret = methodBooleanCall(methodID, args);
    va_end(args);

    return ret;
}

jboolean CallNonvirtualBooleanMethodV(JNIEnv* env, jobject obj, jclass clazz, jmethodID methodID, va_list args) {
    fjni_logv_dbg("[JNI] CallNonvirtualBooleanMethodV(env, 0x%x, 0x%x, %i, args)", (int)obj, (int)clazz, methodID);
    return methodBooleanCall(methodID, args);
}

jboolean CallNonvirtualBooleanMethodA(JNIEnv* env, jobject obj, jclass clazz, jmethodID methodID, const jvalue* args) {
    fjni_logv_warn("[JNI] CallNonvirtualBooleanMethodA(env, 0x%x, 0x%x, %i, args): not implemented", (int)obj, (int)clazz, methodID);
    return JNI_FALSE;
}

jbyte CallNonvirtualByteMethod(JNIEnv* env, jobject obj, jclass clazz, jmethodID methodID, ...) {
    fjni_logv_dbg("[JNI] CallNonvirtualByteMethod(env, 0x%x, 0x%x, %i, ...)", (int)obj, (int)clazz, methodID);

    jbyte ret;
    va_list args;
    va_start(args, methodID);
    ret = methodByteCall(methodID, args);
    va_end(args);

    return ret;
}

jbyte CallNonvirtualByteMethodV(JNIEnv* env, jobject obj, jclass clazz, jmethodID methodID, va_list args) {
    fjni_logv_dbg("[JNI] CallNonvirtualByteMethodV(env, 0x%x, 0x%x, %i, args)", (int)obj, (int)clazz, methodID);
    return methodByteCall(methodID, args);
}

jbyte CallNonvirtualByteMethodA(JNIEnv* env, jobject obj, jclass clazz, jmethodID methodID, const jvalue* args) {
    fjni_logv_warn("[JNI] CallNonvirtualByteMethodA(env, 0x%x, 0x%x, %i, args): not implemented", (int)obj, (int)clazz, methodID);
    return 0;
}

jchar CallNonvirtualCharMethod(JNIEnv* env, jobject obj, jclass clazz, jmethodID methodID, ...) {
    fjni_logv_dbg("[JNI] CallNonvirtualCharMethod(env, 0x%x, 0x%x, %i, ...)", (int)obj, (int)clazz, methodID);

    jchar ret;
    va_list args;
    va_start(args, methodID);
    ret = methodCharCall(methodID, args);
    va_end(args);

    return ret;
}

jchar CallNonvirtualCharMethodV(JNIEnv* env, jobject obj, jclass clazz, jmethodID methodID, va_list args) {
    fjni_logv_dbg("[JNI] CallNonvirtualCharMethodV(env, 0x%x, 0x%x, %i, args)", (int)obj, (int)clazz, methodID);
    return methodCharCall(methodID, args);
}

jchar CallNonvirtualCharMethodA(JNIEnv* env, jobject obj, jclass clazz, jmethodID methodID, const jvalue* args) {
    fjni_logv_warn("[JNI] CallNonvirtualCharMethodA(env, 0x%x, 0x%x, %i, args): not implemented", (int)obj, (int)clazz, methodID);
    return 0;
}

jshort CallNonvirtualShortMethod(JNIEnv* env, jobject obj, jclass clazz, jmethodID methodID, ...) {
    fjni_logv_dbg("[JNI] CallNonvirtualShortMethod(env, 0x%x, 0x%x, %i, ...)", (int)obj, (int)clazz, methodID);

    jshort ret;
    va_list args;
    va_start(args, methodID);
    ret = methodShortCall(methodID, args);
    va_end(args);

    return ret;
}

jshort CallNonvirtualShortMethodV(JNIEnv* env, jobject obj, jclass clazz, jmethodID methodID, va_list args) {
    fjni_logv_dbg("[JNI] CallNonvirtualShortMethodV(env, 0x%x, 0x%x, %i, args)", (int)obj, (int)clazz, methodID);
    return methodShortCall(methodID, args);
}

jshort CallNonvirtualShortMethodA(JNIEnv* env, jobject obj, jclass clazz, jmethodID methodID, const jvalue* args) {
    fjni_logv_warn("[JNI] CallNonvirtualShortMethodA(env, 0x%x, 0x%x, %i, args): not implemented", (int)obj, (int)clazz, methodID);
    return 0;
}

jint CallNonvirtualIntMethod(JNIEnv* env, jobject obj, jclass clazz, jmethodID methodID, ...) {
    fjni_logv_dbg("[JNI] CallNonvirtualIntMethod(env, 0x%x, 0x%x, %i, ...)", (int)obj, (int)clazz, methodID);

    jint ret;
    va_list args;
    va_start(args, methodID);
    ret = methodIntCall(methodID, args);
    va_end(args);

    return ret;
}

jint CallNonvirtualIntMethodV(JNIEnv* env, jobject obj, jclass clazz, jmethodID methodID, va_list args) {
    fjni_logv_dbg("[JNI] CallNonvirtualIntMethodV(env, 0x%x, 0x%x, %i, args)", (int)obj, (int)clazz, methodID);
    return methodIntCall(methodID, args);
}

jint CallNonvirtualIntMethodA(JNIEnv* env, jobject obj, jclass clazz, jmethodID methodID, const jvalue* args) {
    fjni_logv_warn("[JNI] CallNonvirtualIntMethodA(env, 0x%x, 0x%x, %i, args): not implemented", (int)obj, (int)clazz, methodID);
    return 0;
}

jlong CallNonvirtualLongMethod(JNIEnv* env, jobject obj, jclass clazz, jmethodID methodID, ...) {
    fjni_logv_dbg("[JNI] CallNonvirtualLongMethod(env, 0x%x, 0x%x, %i, ...)", (int)obj, (int)clazz, methodID);

    jlong ret;
    va_list args;
    va_start(args, methodID);
    ret = methodLongCall(methodID, args);
    va_end(args);

    return ret;
}

jlong CallNonvirtualLongMethodV(JNIEnv* env, jobject obj, jclass clazz, jmethodID methodID, va_list args) {
    fjni_logv_dbg("[JNI] CallNonvirtualLongMethodV(env, 0x%x, 0x%x, %i, args)", (int)obj, (int)clazz, methodID);
    return methodLongCall(methodID, args);
}

jlong CallNonvirtualLongMethodA(JNIEnv* env, jobject obj, jclass clazz, jmethodID methodID, const jvalue* args) {
    fjni_logv_warn("[JNI] CallNonvirtualLongMethodA(env, 0x%x, 0x%x, %i, args): not implemented", (int)obj, (int)clazz, methodID);
    return 0;
}

jfloat CallNonvirtualFloatMethod(JNIEnv* env, jobject obj, jclass clazz, jmethodID methodID, ...) {
    fjni_logv_dbg("[JNI] CallNonvirtualFloatMethod(env, 0x%x, 0x%x, %i, ...)", (int)obj, (int)clazz, methodID);

    jfloat ret;
    va_list args;
    va_start(args, methodID);
    ret = methodFloatCall(methodID, args);
    va_end(args);

    return ret;
}

jfloat CallNonvirtualFloatMethodV(JNIEnv* env, jobject obj, jclass clazz, jmethodID methodID, va_list args) {
    fjni_logv_dbg("[JNI] CallNonvirtualFloatMethodV(env, 0x%x, 0x%x, %i, args)", (int)obj, (int)clazz, methodID);
    return methodFloatCall(methodID, args);
}

jfloat CallNonvirtualFloatMethodA(JNIEnv* env, jobject obj, jclass clazz, jmethodID methodID, const jvalue* args) {
    fjni_logv_warn("[JNI] CallNonvirtualFloatMethodA(env, 0x%x, 0x%x, %i, args): not implemented", (int)obj, (int)clazz, methodID);
    return 0;
}

jdouble CallNonvirtualDoubleMethod(JNIEnv* env, jobject obj, jclass clazz, jmethodID methodID, ...) {
    fjni_logv_dbg("[JNI] CallNonvirtualDoubleMethod(env, 0x%x, 0x%x, %i, ...)", (int)obj, (int)clazz, methodID);

    jdouble ret;
    va_list args;
    va_start(args, methodID);
    ret = methodDoubleCall(methodID, args);
    va_end(args);

    return ret;
}

jdouble CallNonvirtualDoubleMethodV(JNIEnv* env, jobject obj, jclass clazz, jmethodID methodID, va_list args) {
    fjni_logv_dbg("[JNI] CallNonvirtualDoubleMethodV(env, 0x%x, 0x%x, %i, args)", (int)obj, (int)clazz, methodID);
    return methodDoubleCall(methodID, args);
}

jdouble CallNonvirtualDoubleMethodA(JNIEnv* env, jobject obj, jclass clazz, jmethodID methodID, const jvalue* args) {
    fjni_logv_warn("[JNI] CallNonvirtualDoubleMethodA(env, 0x%x, 0x%x, %i, args): not implemented", (int)obj, (int)clazz, methodID);
    return 0;
}

void CallNonvirtualVoidMethod(JNIEnv* env, jobject obj, jclass clazz, jmethodID methodID, ...) {
    fjni_logv_dbg("[JNI] CallNonvirtualVoidMethod(env, 0x%x, 0x%x, %i, ...)", (int)obj, (int)clazz, methodID);

    va_list args;
    va_start(args, methodID);
    methodVoidCall(methodID, args);
    va_end(args);
}

void CallNonvirtualVoidMethodV(JNIEnv* env, jobject obj, jclass clazz, jmethodID methodID, va_list args) {
    fjni_logv_dbg("[JNI] CallNonvirtualVoidMethodV(env, 0x%x, 0x%x, %i, args)", (int)obj, (int)clazz, methodID);
    methodVoidCall(methodID, args);
}

void CallNonvirtualVoidMethodA(JNIEnv* env, jobject obj, jclass clazz, jmethodID methodID, const jvalue* args) {
    fjni_logv_warn("[JNI] CallNonvirtualVoidMethodA(env, 0x%x, 0x%x, %i, args): not implemented", (int)obj, (int)clazz, methodID);
}

jfieldID GetFieldID(JNIEnv * env, jclass clazz, const char* name, const char* t) {
    fjni_logv_dbg("[JNI] GetFieldID(env, 0x%x, \"%s\", \"%s\")", (int)clazz, name, t);
    return getFieldIdByName(name);
}

jobject GetObjectField(JNIEnv* env, jobject obj, jfieldID fieldID) {
    fjni_logv_dbg("[JNI] GetObjectField(env, 0x%x, %i)", (int)obj, fieldID);
    return getObjectFieldValueById(fieldID);
}

jboolean GetBooleanField(JNIEnv* env, jobject obj, jfieldID fieldID) {
    fjni_logv_dbg("[JNI] GetBooleanField(env, 0x%x, %i)", (int)obj, fieldID);
    return getBooleanFieldValueById(fieldID);
}

jbyte GetByteField(JNIEnv* env, jobject obj, jfieldID fieldID) {
    fjni_logv_dbg("[JNI] GetByteField(env, 0x%x, %i)", (int)obj, fieldID);
    return getByteFieldValueById(fieldID);
}

jchar GetCharField(JNIEnv* env, jobject obj, jfieldID fieldID) {
    fjni_logv_dbg("[JNI] GetCharField(env, 0x%x, %i)", (int)obj, fieldID);
    return getCharFieldValueById(fieldID);
}

jshort GetShortField(JNIEnv* env, jobject obj, jfieldID fieldID) {
    fjni_logv_dbg("[JNI] GetShortField(env, 0x%x, %i)", (int)obj, fieldID);
    return getShortFieldValueById(fieldID);
}

jint GetIntField(JNIEnv* env, jobject obj, jfieldID fieldID) {
    fjni_logv_dbg("[JNI] GetIntField(env, 0x%x, %i)", (int)obj, fieldID);
    return getIntFieldValueById(fieldID);
}

jlong GetLongField(JNIEnv* env, jobject obj, jfieldID fieldID) {
    fjni_logv_dbg("[JNI] GetLongField(env, 0x%x, %i)", (int)obj, fieldID);
    return getLongFieldValueById(fieldID);
}

jfloat GetFloatField(JNIEnv* env, jobject obj, jfieldID fieldID) {
    fjni_logv_dbg("[JNI] GetFloatField(env, 0x%x, %i)", (int)obj, fieldID);
    return getFloatFieldValueById(fieldID);
}

jdouble GetDoubleField(JNIEnv* env, jobject obj, jfieldID fieldID) {
    fjni_logv_dbg("[JNI] GetFloatField(env, 0x%x, %i)", (int)obj, fieldID);
    return getDoubleFieldValueById(fieldID);
}

// TODO: Implement Set<type>Field routines

void SetObjectField(JNIEnv* env, jobject obj, jfieldID fieldID, jobject value) { fjni_log_warn("[JNI] SetObjectField(): not implemented"); }
void SetBooleanField(JNIEnv* env, jobject obj, jfieldID fieldID, jboolean value) { fjni_log_warn("[JNI] SetBooleanField(): not implemented"); }
void SetByteField(JNIEnv* env, jobject obj, jfieldID fieldID, jbyte value) { fjni_log_warn("[JNI] SetByteField(): not implemented"); }
void SetCharField(JNIEnv* env, jobject obj, jfieldID fieldID, jchar value) { fjni_log_warn("[JNI] SetCharField(): not implemented"); }
void SetShortField(JNIEnv* env, jobject obj, jfieldID fieldID, jshort value) { fjni_log_warn("[JNI] SetShortField(): not implemented"); }
void SetIntField(JNIEnv* env, jobject obj, jfieldID fieldID, jint value) { fjni_log_warn("[JNI] SetIntField(): not implemented"); }
void SetLongField(JNIEnv* env, jobject obj, jfieldID fieldID, jlong value) { fjni_log_warn("[JNI] SetLongField(): not implemented"); }
void SetFloatField(JNIEnv* env, jobject obj, jfieldID fieldID, jfloat value) { fjni_log_warn("[JNI] SetFloatField(): not implemented"); }
void SetDoubleField(JNIEnv* env, jobject obj, jfieldID fieldID, jdouble value) { fjni_log_warn("[JNI] SetDoubleField(): not implemented"); }

jmethodID GetStaticMethodID(JNIEnv* env, jclass clazz, const char* name, const char* sig) {
    jmethodID ret = getMethodIdByName(name);
    fjni_logv_dbg("[JNI] GetStaticMethodID(env, 0x%x, \"%s\", \"%s\"): %i", (int)clazz, name, sig, ret);
    return ret;
}

jobject CallStaticObjectMethod(JNIEnv* env, jclass clazz, jmethodID methodID, ...) {
    fjni_logv_dbg("[JNI] CallStaticObjectMethod(env, 0x%x, %i)", (int)clazz, methodID);

    jobject ret;
    va_list args;
    va_start(args, methodID);
    ret = methodObjectCall(methodID, args);
    va_end(args);

    return ret;
}

jobject CallStaticObjectMethodV(JNIEnv* env, jclass clazz, jmethodID methodID, va_list args) {
    fjni_logv_dbg("[JNI] CallStaticObjectMethodV(env, 0x%x, %i, args)", (int)clazz, (int)methodID);
    return methodObjectCall(methodID, args);
}

jobject CallStaticObjectMethodA(JNIEnv* env, jclass clazz, jmethodID methodID, const jvalue* args) {
    fjni_logv_warn("[JNI] CallStaticObjectMethodA(env, 0x%x, %i, args): not implemented", (int)clazz, (int)methodID);
    return NULL;
}

jboolean CallStaticBooleanMethod(JNIEnv* env, jclass clazz, jmethodID methodID, ...) {
    fjni_logv_dbg("[JNI] CallStaticBooleanMethod(env, 0x%x, %i)", (int)clazz, methodID);

    jboolean ret;
    va_list args;
    va_start(args, methodID);
    ret = methodBooleanCall(methodID, args);
    va_end(args);

    return ret;
}

jboolean CallStaticBooleanMethodV(JNIEnv* env, jclass clazz, jmethodID methodID, va_list args) {
    fjni_logv_dbg("[JNI] CallStaticBooleanMethodV(env, 0x%x, %i, args)", (int)clazz, (int)methodID);
    return methodBooleanCall(methodID, args);
}

jboolean CallStaticBooleanMethodA(JNIEnv* env, jclass clazz, jmethodID methodID, const jvalue* args) {
    fjni_logv_warn("[JNI] CallStaticBooleanMethodA(env, 0x%x, %i, args): not implemented", (int)clazz, (int)methodID);
    return JNI_FALSE;
}

jbyte CallStaticByteMethod(JNIEnv* env, jclass clazz, jmethodID methodID, ...) {
    fjni_logv_dbg("[JNI] CallStaticByteMethod(env, 0x%x, %i)", (int)clazz, methodID);

    jbyte ret;
    va_list args;
    va_start(args, methodID);
    ret = methodByteCall(methodID, args);
    va_end(args);

    return ret;
}

jbyte CallStaticByteMethodV(JNIEnv* env, jclass clazz, jmethodID methodID, va_list args) {
    fjni_logv_dbg("[JNI] CallStaticByteMethodV(env, 0x%x, %i, args)", (int)clazz, (int)methodID);
    return methodByteCall(methodID, args);
}

jbyte CallStaticByteMethodA(JNIEnv* env, jclass clazz, jmethodID methodID, const jvalue* args) {
    fjni_logv_warn("[JNI] CallStaticByteMethodA(env, 0x%x, %i, args): not implemented", (int)clazz, (int)methodID);
    return 0;
}

jchar CallStaticCharMethod(JNIEnv* env, jclass clazz, jmethodID methodID, ...) {
    fjni_logv_dbg("[JNI] CallStaticCharMethod(env, 0x%x, %i)", (int)clazz, methodID);

    jchar ret;
    va_list args;
    va_start(args, methodID);
    ret = methodCharCall(methodID, args);
    va_end(args);

    return ret;
}

jchar CallStaticCharMethodV(JNIEnv* env, jclass clazz, jmethodID methodID, va_list args) {
    fjni_logv_dbg("[JNI] CallStaticCharMethodV(env, 0x%x, %i, args)", (int)clazz, (int)methodID);
    return methodCharCall(methodID, args);
}

jchar CallStaticCharMethodA(JNIEnv* env, jclass clazz, jmethodID methodID, const jvalue* args) {
    fjni_logv_warn("[JNI] CallStaticCharMethodA(env, 0x%x, %i, args): not implemented", (int)clazz, (int)methodID);
    return 0;
}

jshort CallStaticShortMethod(JNIEnv* env, jclass clazz, jmethodID methodID, ...) {
    fjni_logv_dbg("[JNI] CallStaticShortMethod(env, 0x%x, %i)", (int)clazz, methodID);

    jshort ret;
    va_list args;
    va_start(args, methodID);
    ret = methodShortCall(methodID, args);
    va_end(args);

    return ret;
}

jshort CallStaticShortMethodV(JNIEnv* env, jclass clazz, jmethodID methodID, va_list args) {
    fjni_logv_dbg("[JNI] CallStaticShortMethodV(env, 0x%x, %i, args)", (int)clazz, (int)methodID);
    return methodShortCall(methodID, args);
}

jshort CallStaticShortMethodA(JNIEnv* env, jclass clazz, jmethodID methodID, const jvalue* args) {
    fjni_logv_warn("[JNI] CallStaticShortMethodA(env, 0x%x, %i, args): not implemented", (int)clazz, (int)methodID);
    return 0;
}

jint CallStaticIntMethod(JNIEnv* env, jclass clazz, jmethodID methodID, ...) {
    fjni_logv_dbg("[JNI] CallStaticIntMethod(env, 0x%x, %i, ...)", (int)clazz, methodID);

    jint ret;
    va_list args;
    va_start(args, methodID);
    ret = methodIntCall(methodID, args);
    va_end(args);

    return ret;
}

jint CallStaticIntMethodV(JNIEnv* env, jclass clazz, jmethodID methodID, va_list args) {
    fjni_logv_dbg("[JNI] CallStaticIntMethodV(env, 0x%x, %i, args)", (int)clazz, methodID);
    return methodIntCall(methodID, args);
}

jint CallStaticIntMethodA(JNIEnv* env, jclass clazz, jmethodID methodID, const jvalue* args) {
    fjni_logv_warn("[JNI] CallStaticIntMethodA(env, 0x%x, %i, args): not implemented", (int)clazz, (int)methodID);
    return 0;
}

jlong CallStaticLongMethod(JNIEnv* env, jclass clazz, jmethodID methodID, ...) {
    fjni_logv_dbg("[JNI] CallStaticLongMethod(env, 0x%x, %i, ...)", (int)clazz, methodID);

    jlong ret;
    va_list args;
    va_start(args, methodID);
    ret = methodLongCall(methodID, args);
    va_end(args);

    return ret;
}

jlong CallStaticLongMethodV(JNIEnv* env, jclass clazz, jmethodID methodID, va_list args) {
    fjni_logv_dbg("[JNI] CallStaticLongMethodV(env, 0x%x, %i, args)", (int)clazz, methodID);
    return methodLongCall(methodID, args);
}

jlong CallStaticLongMethodA(JNIEnv* env, jclass clazz, jmethodID methodID, const jvalue* args) {
    fjni_logv_warn("[JNI] CallStaticLongMethodA(env, 0x%x, %i, args): not implemented", (int)clazz, (int)methodID);
    return 0;
}

jfloat CallStaticFloatMethod(JNIEnv* env, jclass clazz, jmethodID methodID, ...) {
    fjni_logv_dbg("[JNI] CallStaticFloatMethod(env, 0x%x, %i, ...)", (int)clazz, methodID);

    jfloat ret;
    va_list args;
    va_start(args, methodID);
    ret = methodFloatCall(methodID, args);
    va_end(args);

    return ret;
}

jfloat CallStaticFloatMethodV(JNIEnv* env, jclass clazz, jmethodID methodID, va_list args) {
    fjni_logv_dbg("[JNI] CallStaticFloatMethodV(env, 0x%x, %i, args)", (int)clazz, methodID);
    return methodFloatCall(methodID, args);
}

jfloat CallStaticFloatMethodA(JNIEnv* env, jclass clazz, jmethodID methodID, const jvalue* args) {
    fjni_logv_warn("[JNI] CallStaticFloatMethodA(env, 0x%x, %i, args): not implemented", (int)clazz, (int)methodID);
    return 0;
}

jdouble CallStaticDoubleMethod(JNIEnv* env, jclass clazz, jmethodID methodID, ...) {
    fjni_logv_dbg("[JNI] CallStaticDoubleMethod(env, 0x%x, %i, ...)", (int)clazz, methodID);

    jdouble ret;
    va_list args;
    va_start(args, methodID);
    ret = methodDoubleCall(methodID, args);
    va_end(args);

    return ret;
}

jdouble CallStaticDoubleMethodV(JNIEnv* env, jclass clazz, jmethodID methodID, va_list args) {
    fjni_logv_dbg("[JNI] CallStaticDoubleMethodV(env, 0x%x, %i, args)", (int)clazz, methodID);
    return methodDoubleCall(methodID, args);
}

jdouble CallStaticDoubleMethodA(JNIEnv* env, jclass clazz, jmethodID methodID, const jvalue* args) {
    fjni_logv_warn("[JNI] CallStaticDoubleMethodA(env, 0x%x, %i, args): not implemented", (int)clazz, (int)methodID);
    return 0;
}

void CallStaticVoidMethod(JNIEnv* env, jclass clazz, jmethodID methodID, ...) {
    fjni_logv_dbg("[JNI] CallStaticVoidMethod(env, 0x%x, %i, ...)", (int)clazz, methodID);

    va_list args;
    va_start(args, methodID);
    methodVoidCall(methodID, args);
    va_end(args);
}

void CallStaticVoidMethodV(JNIEnv* env, jclass clazz, jmethodID methodID, va_list args) {
    fjni_logv_dbg("[JNI] CallStaticVoidMethodV(env, 0x%x, %i, args)", (int)clazz, methodID);
    methodVoidCall(methodID, args);
}

void CallStaticVoidMethodA(JNIEnv* env, jclass clazz, jmethodID methodID, const jvalue* args) {
    fjni_logv_warn("[JNI] CallStaticVoidMethodA(env, 0x%x, %i, args): not implemented", (int)clazz, (int)methodID);
}

jfieldID GetStaticFieldID(JNIEnv* env, jclass clazz, const char* name, const char* t) {
    fjni_logv_dbg("[JNI] GetStaticFieldID(env, 0x%x, \"%s\", \"%s\")", (int)clazz, name, t);
    return getFieldIdByName(name);
}

jobject GetStaticObjectField(JNIEnv* env, jclass clazz, jfieldID fieldID) {
    fjni_logv_dbg("[JNI] GetStaticObjectField(env, 0x%x, %i)", (int)clazz, fieldID);
    return getObjectFieldValueById(fieldID);
}

jboolean GetStaticBooleanField(JNIEnv* env, jclass clazz, jfieldID fieldID) {
    fjni_logv_dbg("[JNI] GetStaticBooleanField(env, 0x%x, %i)", (int)clazz, fieldID);
    return getBooleanFieldValueById(fieldID);
}

jbyte GetStaticByteField(JNIEnv* env, jclass clazz, jfieldID fieldID) {
    fjni_logv_dbg("[JNI] GetStaticByteField(env, 0x%x, %i)", (int)clazz, fieldID);
    return getByteFieldValueById(fieldID);
}

jchar GetStaticCharField(JNIEnv* env, jclass clazz, jfieldID fieldID) {
    fjni_logv_dbg("[JNI] GetStaticCharField(env, 0x%x, %i)", (int)clazz, fieldID);
    return getCharFieldValueById(fieldID);
}

jshort GetStaticShortField(JNIEnv* env, jclass clazz, jfieldID fieldID) {
    fjni_logv_dbg("[JNI] GetStaticShortField(env, 0x%x, %i)", (int)clazz, fieldID);
    return getShortFieldValueById(fieldID);
}

jint GetStaticIntField(JNIEnv* env, jclass clazz, jfieldID fieldID) {
    fjni_logv_dbg("[JNI] GetStaticIntField(env, 0x%x, %i): ", (int)clazz, fieldID);
    return getIntFieldValueById(fieldID);
}

jlong GetStaticLongField(JNIEnv* env, jclass clazz, jfieldID fieldID) {
    fjni_logv_dbg("[JNI] GetStaticLongField(env, 0x%x, %i)", (int)clazz, fieldID);
    return getLongFieldValueById(fieldID);
}

jfloat GetStaticFloatField(JNIEnv* env, jclass clazz, jfieldID fieldID) {
    fjni_logv_dbg("[JNI] GetStaticFloatField(env, 0x%x, %i)", (int)clazz, fieldID);
    return getFloatFieldValueById(fieldID);
}

jdouble GetStaticDoubleField(JNIEnv* env, jclass clazz, jfieldID fieldID) {
    fjni_logv_dbg("[JNI] GetStaticDoubleField(env, 0x%x, %i)", (int)clazz, fieldID);
    return getDoubleFieldValueById(fieldID);
}

// TODO: Implement SetStatic<type>Field routines

void SetStaticObjectField(JNIEnv* env, jclass clazz, jfieldID fieldID, jobject value) { fjni_log_warn("[JNI] SetStaticObjectField(): not implemented"); }
void SetStaticBooleanField(JNIEnv* env, jclass clazz, jfieldID fieldID, jboolean value) { fjni_log_warn("[JNI] SetStaticBooleanField(): not implemented"); }
void SetStaticByteField(JNIEnv* env, jclass clazz, jfieldID fieldID, jbyte value) { fjni_log_warn("[JNI] SetStaticByteField(): not implemented"); }
void SetStaticCharField(JNIEnv* env, jclass clazz, jfieldID fieldID, jchar value) { fjni_log_warn("[JNI] SetStaticCharField(): not implemented"); }
void SetStaticShortField(JNIEnv* env, jclass clazz, jfieldID fieldID, jshort value) { fjni_log_warn("[JNI] SetStaticShortField(): not implemented"); }
void SetStaticIntField(JNIEnv* env, jclass clazz, jfieldID fieldID, jint value) { fjni_log_warn("[JNI] SetStaticIntField(): not implemented"); }
void SetStaticLongField(JNIEnv* env, jclass clazz, jfieldID fieldID, jlong value) { fjni_log_warn("[JNI] SetStaticLongField(): not implemented"); }
void SetStaticFloatField(JNIEnv* env, jclass clazz, jfieldID fieldID, jfloat value) { fjni_log_warn("[JNI] SetStaticFloatField(): not implemented"); }
void SetStaticDoubleField(JNIEnv* env, jclass clazz, jfieldID fieldID, jdouble value) { fjni_log_warn("[JNI] SetStaticDoubleField(): not implemented"); }

jstring NewString(JNIEnv* env, const jchar* chars, jsize char_count) {
    fjni_logv_dbg("[JNI] NewString(env, \"%s\", %i)", (char*)chars, char_count);

    // abort()s here replicate Dalvik behavior.
    if (char_count < 0) {
        fjni_logv_err("Fatal: char_count < 0: %d! Aborting.", char_count);
        abort();
    }

    if (chars == NULL && char_count > 0) {
        fjni_log_err("Fatal: chars == null && char_count > 0");
        abort();
    }

    char* newStr = malloc(char_count+1);
    strncpy(newStr, (const char*)chars, char_count+1);
    return newStr;
}

jsize GetStringLength(JNIEnv* env, jstring string) {
    fjni_logv_dbg("[JNI] GetStringLength(env, 0x%x/\"%s\")", (int)string, (char*)string);
    return (jsize)strlen(string);
}

const jchar * GetStringChars(JNIEnv* env, jstring string, jboolean *isCopy) {
    fjni_logv_dbg("[JNI] GetStringChars(env, 0x%x/\"%s\", *isCopy)", string, string);

    if (!string) {
        fjni_log_err("String is null.");
        return NULL;
    }

    if (isCopy != NULL) {
        *isCopy = JNI_TRUE;
    }

    return (const jchar *)strdup((const char*)string);
}

void ReleaseStringChars(JNIEnv* env, jstring string, const jchar *chars) {
    fjni_logv_dbg("[JNI] ReleaseStringChars(env, 0x%x/\"%s\", 0x%x)", string, string, chars);

    if (!chars) {
        fjni_log_err("Chars is null");
        return;
    }

    free((char*)chars);
}

jstring NewStringUTF(JNIEnv* env, const char* bytes) {
    fjni_logv_dbg("[JNI] NewStringUTF(env, \"%s\")", bytes);

    char* newStr;
    if (bytes == NULL) {
        /* this shouldn't happen; throw NPE? */
        newStr = NULL;
    } else {
        newStr = strdup(bytes);
        if (newStr == NULL) {
            /* assume memory failure */
            fjni_log_err("native heap string alloc failed! aborting.");
            abort();
        }
    }

    return newStr;
}

jsize GetStringUTFLength(JNIEnv* env, jstring string) {
    fjni_logv_dbg("[JNI] GetStringUTFLength(env, \"%s\")", string);

    if (string != NULL) {
        return (jsize)strlen(string);
    }

    return 0;
}

const char* GetStringUTFChars(JNIEnv* env, jstring string, jboolean* isCopy) {
    fjni_logv_dbg("[JNI] GetStringUTFChars(env, \"%s\", *isCopy)", string);

    char* newStr;
    if (string == NULL) {
        /* this shouldn't happen; throw NPE? */
        newStr = NULL;
    } else {
        if (isCopy != NULL)
            *isCopy = JNI_TRUE;
        newStr = strdup(string);
        if (newStr == NULL) {
            /* assume memory failure */
            fjni_log_err("native heap string alloc failed! aborting.");
            abort();
        }
    }

    return newStr;
}

void ReleaseStringUTFChars(JNIEnv* env, jstring string, char* chars) {
    fjni_logv_dbg("[JNI] ReleaseStringUTFChars(env, 0x%x, \"%s\")", (int)string, chars);
    if (chars) {
        free(chars);
    }
}

jsize GetArrayLength(JNIEnv* env, jarray array) {
    fjni_logv_dbg("[JNI] GetArrayLength(env, 0x%x)", (int)array);
    jsize* ret;

    ret = fieldIntArrayGetLengthByPtr(array);
    if (ret) return *ret;

    //TODO: Other array types

    ret = findDynamicallyAllocatedArrayLength(array);
    if (ret) return * ret;

    fjni_logv_warn("Array not found. Unknown array type?", (int)array);
    return 0;
}

jobjectArray NewObjectArray(JNIEnv* env, jsize length, jclass elementClass, jobject initialElement) {
    fjni_logv_warn("[JNI] NewObjectArray(env, %i, 0x%x, 0x%x): not implemented", length, elementClass, initialElement);
    return 0;
}

jobject GetObjectArrayElement(JNIEnv* env, jobjectArray array, jsize index) {
    fjni_logv_dbg("[JNI] GetObjectArrayElement(env, 0x%x, idx:%i)", array, index);
    jobject* arr = (jobject*)array;
    return arr[index];
}

void SetObjectArrayElement(JNIEnv* env, jobjectArray array, jsize index, jobject value) {
    fjni_logv_dbg("[JNI] SetObjectArrayElement(env, 0x%x, idx:%i, val:0x%x)", array, index, value);
    jobject* arr = (jobject*)array;
    arr[index] = value;
}

jbooleanArray NewBooleanArray(JNIEnv* env, jsize length) {
    fjni_logv_dbg("[JNI] NewBooleanArray(env, size:%i)", length);
    return (jbooleanArray) malloc(length * sizeof(jboolean));
}

jbyteArray NewByteArray(JNIEnv* env, jsize length) {
    fjni_logv_dbg("[JNI] NewByteArray(env, size:%i)", length);
    return (jbyteArray) malloc(length * sizeof(jbyte));
}

jcharArray NewCharArray(JNIEnv* env, jsize length) {
    fjni_logv_dbg("[JNI] NewCharArray(env, size:%i)", length);
    return (jcharArray) malloc(length * sizeof(jchar));
}

jshortArray NewShortArray(JNIEnv* env, jsize length) {
    fjni_logv_dbg("[JNI] NewShortArray(env, size:%i)", length);
    return (jshortArray) malloc(length * sizeof(jshort));
}

jintArray NewIntArray(JNIEnv* env, jsize length) {
    fjni_logv_dbg("[JNI] NewIntArray(env, size:%i)", length);
    return (jintArray) malloc(length * sizeof(jint));
}

jlongArray NewLongArray(JNIEnv* env, jsize length) {
    fjni_logv_dbg("[JNI] NewLongArray(env, size:%i)", length);
    return (jlongArray) malloc(length * sizeof(jlong));
}

jfloatArray NewFloatArray(JNIEnv* env, jsize length) {
    fjni_logv_dbg("[JNI] NewFloatArray(env, size:%i)", length);
    return (jfloatArray) malloc(length * sizeof(jfloat));
}

jdoubleArray NewDoubleArray(JNIEnv* env, jsize length) {
    fjni_logv_dbg("[JNI] NewDoubleArray(env, size:%i)", length);
    return (jdoubleArray) malloc(length * sizeof(jdouble));
}

jboolean* GetBooleanArrayElements(JNIEnv* env, jbooleanArray array, jboolean* isCopy) {
    fjni_logv_dbg("[JNI] GetBooleanArrayElements(env, 0x%x, 0x%x)", (int)array, (int)isCopy);
    if (isCopy != NULL) *isCopy = JNI_FALSE;
    return (jboolean*) array;
}

jbyte* GetByteArrayElements(JNIEnv* env, jbyteArray array, jboolean* isCopy) {
    fjni_logv_dbg("[JNI] GetByteArrayElements(env, 0x%x, 0x%x)", (int)array, (int)isCopy);
    if (isCopy != NULL) *isCopy = JNI_FALSE;
    return (jbyte*) array;
}

jchar* GetCharArrayElements(JNIEnv* env, jcharArray array, jboolean* isCopy) {
    fjni_logv_dbg("[JNI] GetCharArrayElements(env, 0x%x, 0x%x)", (int)array, (int)isCopy);
    if (isCopy != NULL) *isCopy = JNI_FALSE;
    return (jchar*) array;
}

jshort* GetShortArrayElements(JNIEnv* env, jshortArray array, jboolean* isCopy) {
    fjni_logv_dbg("[JNI] GetShortArrayElements(env, 0x%x, 0x%x)", (int)array, (int)isCopy);
    if (isCopy != NULL) *isCopy = JNI_FALSE;
    return (jshort*) array;
}

jint* GetIntArrayElements(JNIEnv* env, jintArray array, jboolean* isCopy) {
    fjni_logv_dbg("[JNI] GetIntArrayElements(env, 0x%x, 0x%x)", (int)array, (int)isCopy);
    if (isCopy != NULL) *isCopy = JNI_FALSE;
    return (jint*) array;
}

jlong* GetLongArrayElements(JNIEnv* env, jlongArray array, jboolean* isCopy) {
    fjni_logv_dbg("[JNI] GetLongArrayElements(env, 0x%x, 0x%x)", (int)array, (int)isCopy);
    if (isCopy != NULL) *isCopy = JNI_FALSE;
    return (jlong*) array;
}

jfloat* GetFloatArrayElements(JNIEnv* env, jfloatArray array, jboolean* isCopy) {
    fjni_logv_dbg("[JNI] GetFloatArrayElements(env, 0x%x, 0x%x)", (int)array, (int)isCopy);
    if (isCopy != NULL) *isCopy = JNI_FALSE;
    return (jfloat*) array;
}

jdouble* GetDoubleArrayElements(JNIEnv* env, jdoubleArray array, jboolean* isCopy) {
    fjni_logv_dbg("[JNI] GetDoubleArrayElements(env, 0x%x, 0x%x)", (int)array, (int)isCopy);
    if (isCopy != NULL) *isCopy = JNI_FALSE;
    return (jdouble*) array;
}

// In Get<type>ArrayElements we never make copies, so Release<type>ArrayElements can be ignored

void ReleaseBooleanArrayElements(JNIEnv* env, jbooleanArray array, jboolean* elems, jint mode) { fjni_log_dbg("[JNI] ReleaseBooleanArrayElements(): ignored"); }
void ReleaseByteArrayElements(JNIEnv* env, jbyteArray array, jbyte* elems, jint mode) { fjni_log_dbg("[JNI] ReleaseByteArrayElements(): ignored"); }
void ReleaseCharArrayElements(JNIEnv* env, jcharArray array, jchar* elems, jint mode) { fjni_log_dbg("[JNI] ReleaseCharArrayElements(): ignored"); }
void ReleaseShortArrayElements(JNIEnv* env, jshortArray array, jshort* elems, jint mode) { fjni_log_dbg("[JNI] ReleaseShortArrayElements(): ignored"); }
void ReleaseIntArrayElements(JNIEnv* env, jintArray array, jint* elems, jint mode) { fjni_log_dbg("[JNI] ReleaseIntArrayElements(): ignored"); }
void ReleaseLongArrayElements(JNIEnv* env, jlongArray array, jlong* elems, jint mode) { fjni_log_dbg("[JNI] ReleaseLongArrayElements(): ignored"); }
void ReleaseFloatArrayElements(JNIEnv* env, jfloatArray array, jfloat* elems, jint mode) { fjni_log_dbg("[JNI] ReleaseFloatArrayElements(): ignored"); }
void ReleaseDoubleArrayElements(JNIEnv* env, jdoubleArray array, jdouble* elems, jint mode) { fjni_log_dbg("[JNI] ReleaseDoubleArrayElements(): ignored"); }

void GetBooleanArrayRegion(JNIEnv* env, jbooleanArray array, jsize start, jsize length, jboolean* buffer) {
    fjni_logv_dbg("[JNI] GetBooleanArrayRegion(env, 0x%x, %i, %i, buffer)", (int)array, start, length);

    if (!buffer)
        buffer = (jboolean*) malloc(length);

    memcpy(buffer, (jboolean*)array + start, length);
}

void GetByteArrayRegion(JNIEnv* env, jbyteArray array, jsize start, jsize length, jbyte* buffer) {
    fjni_logv_dbg("[JNI] GetByteArrayRegion(env, 0x%x, %i, %i, buffer)", (int)array, start, length);

    if (!buffer)
        buffer = (jbyte*) malloc(length);

    memcpy(buffer, (jbyte*)array + start, length);
}

void GetCharArrayRegion(JNIEnv* env, jcharArray array, jsize start, jsize length, jchar* buffer) {
    fjni_logv_dbg("[JNI] GetCharArrayRegion(env, 0x%x, %i, %i, buffer)", (int)array, start, length);

    if (!buffer)
        buffer = (jchar*) malloc(length);

    memcpy(buffer, (jchar*)array + start, length);
}

void GetShortArrayRegion(JNIEnv* env, jshortArray array, jsize start, jsize length, jshort* buffer) {
    fjni_logv_dbg("[JNI] GetShortArrayRegion(env, 0x%x, %i, %i, buffer)", (int)array, start, length);

    if (!buffer)
        buffer = (jshort*) malloc(length);

    memcpy(buffer, (jshort*)array + start, length);
}

void GetIntArrayRegion(JNIEnv* env, jintArray array, jsize start, jsize length, jint* buffer) {
    fjni_logv_dbg("[JNI] GetIntArrayRegion(env, 0x%x, %i, %i, buffer)", (int)array, start, length);

    if (!buffer)
        buffer = (jint*) malloc(length);

    memcpy(buffer, (jint*)array + start, length);
}

void GetLongArrayRegion(JNIEnv* env, jlongArray array, jsize start, jsize length, jlong* buffer) {
    fjni_logv_dbg("[JNI] GetLongArrayRegion(env, 0x%x, %i, %i, buffer)", (int)array, start, length);

    if (!buffer)
        buffer = (jlong*) malloc(length);

    memcpy(buffer, (jlong*)array + start, length);
}

void GetFloatArrayRegion(JNIEnv* env, jfloatArray array, jsize start, jsize length, jfloat* buffer) {
    fjni_logv_dbg("[JNI] GetFloatArrayRegion(env, 0x%x, %i, %i, buffer)", (int)array, start, length);

    if (!buffer)
        buffer = (jfloat*) malloc(length);

    memcpy(buffer, (jfloat*)array + start, length);
}

void GetDoubleArrayRegion(JNIEnv* env, jdoubleArray array, jsize start, jsize length, jdouble* buffer) {
    fjni_logv_dbg("[JNI] GetDoubleArrayRegion(env, 0x%x, %i, %i, buffer)", (int)array, start, length);

    if (!buffer)
        buffer = (jdouble*) malloc(length);

    memcpy(buffer, (jdouble*)array + start, length);
}

void SetBooleanArrayRegion(JNIEnv* env, jbooleanArray array, jsize start, jsize len, const jboolean* buf) {
    fjni_logv_dbg("[JNI] SetBooleanArrayRegion(env, 0x%x, %i, %i, buffer)", (int)array, start, len);
    SetPrimitiveArrayRegion(array, "jbooleanArray", start, len, buf, jboolean);
}

void SetByteArrayRegion(JNIEnv* env, jbyteArray array, jsize start, jsize len, const jbyte* buf) {
    fjni_logv_dbg("[JNI] SetByteArrayRegion(env, 0x%x, %i, %i, buffer)", (int)array, start, len);
    SetPrimitiveArrayRegion(array, "jbyteArray", start, len, buf, jbyte);
}

void SetCharArrayRegion(JNIEnv* env, jcharArray array, jsize start, jsize len, const jchar* buf) {
    fjni_logv_dbg("[JNI] SetCharArrayRegion(env, 0x%x, %i, %i, buffer)", (int)array, start, len);
    SetPrimitiveArrayRegion(array, "jcharArray", start, len, buf, jchar);
}

void SetShortArrayRegion(JNIEnv* env, jshortArray array, jsize start, jsize len, const jshort* buf) {
    fjni_logv_dbg("[JNI] SetShortArrayRegion(env, 0x%x, %i, %i, buffer)", (int)array, start, len);
    SetPrimitiveArrayRegion(array, "jshortArray", start, len, buf, jshort);
}

void SetIntArrayRegion(JNIEnv* env, jintArray array, jsize start, jsize len, const jint* buf) {
    fjni_logv_dbg("[JNI] SetIntArrayRegion(env, 0x%x, %i, %i, buffer)", (int)array, start, len);
    SetPrimitiveArrayRegion(array, "jintArray", start, len, buf, jint);
}

void SetLongArrayRegion(JNIEnv* env, jlongArray array, jsize start, jsize len, const jlong* buf) {
    fjni_logv_dbg("[JNI] SetLongArrayRegion(env, 0x%x, %i, %i, buffer)", (int)array, start, len);
    SetPrimitiveArrayRegion(array, "jlongArray", start, len, buf, jlong);
}

void SetFloatArrayRegion(JNIEnv* env, jfloatArray array, jsize start, jsize len, const jfloat* buf) {
    fjni_logv_dbg("[JNI] SetFloatArrayRegion(env, 0x%x, %i, %i, buffer)", (int)array, start, len);
    SetPrimitiveArrayRegion(array, "jfloatArray", start, len, buf, jfloat);
}

void SetDoubleArrayRegion(JNIEnv* env, jdoubleArray array, jsize start, jsize len, const jdouble* buf) {
    fjni_logv_dbg("[JNI] SetDoubleArrayRegion(env, 0x%x, %i, %i, buffer)", (int)array, start, len);
    SetPrimitiveArrayRegion(array, "jdoubleArray", start, len, buf, jdouble);
}

// Due to the way we define and execute functions, Register/UnregisterNatives are redundant

jint RegisterNatives(JNIEnv* env, jclass clazz, const JNINativeMethod* methods, jint nMethods) {
    fjni_logv_dbg("[JNI] RegisterNatives(env, 0x%x, 0x%x, n:%i): ignored", (int)clazz, (int)methods, nMethods);
    return 0;
}

jint UnregisterNatives(JNIEnv* env, jclass clazz) {
    fjni_logv_dbg("[JNI] UnregisterNatives(env, 0x%x): ignored", (int)clazz);
    return 0;
}

// TODO: Implement MonitorEnter/MonitorExit with mutexes

jint MonitorEnter(JNIEnv* env, jobject obj) {
    //fjni_logv_warn("[JNI] MonitorEnter(env, 0x%x): not implemented", (int)obj);
    return 0;
}

jint MonitorExit(JNIEnv* env, jobject obj) {
    //fjni_logv_warn("[JNI] MonitorExit(env, 0x%x): not implemented", (int)obj);
    return 0;
}

jint GetJavaVM(JNIEnv* env, JavaVM** vm) {
    fjni_log_dbg("[JNI] GetJavaVM(env, *vm)");
    *vm = &jvm;
    return 0;
}

void GetStringRegion(JNIEnv* env, jstring str, jsize start, jsize len, jchar* buf) {
    fjni_logv_dbg("[JNI] GetStringRegion(env, 0x%x, start:%i, len:%i, 0x%x)", (int)str, start, len, (int)buf);

    if (str == NULL) {
        fjni_log_err("str is NULL");
        return;
    }

    if ((start + len) > strlen(str)) {
        fjni_log_err("StringIndexOutOfBoundsException");
        return;
    }

    if (buf == NULL) {
        buf = malloc(len+1);
    }

    strncpy((char*)buf, (const char*)str + start, len);
}

void GetStringUTFRegion(JNIEnv* env, jstring str, jsize start, jsize len, char* buf) {
    fjni_logv_dbg("[JNI] GetStringUTFRegion(env, 0x%x, start:%i, len:%i, 0x%x)", (int)str, start, len, (int)buf);

    if (str == NULL) {
        fjni_log_err("str is NULL");
        return;
    }

    if ((start + len) > strlen(str)) {
        fjni_log_err("StringIndexOutOfBoundsException");
        return;
    }

    if (buf == NULL) {
        buf = malloc(len+1);
    }

    strncpy(buf, (const char*)str + start, len);
}

// TODO: Implement GetPrimitiveArrayCritical/ReleasePrimitiveArrayCritical

void* GetPrimitiveArrayCritical(JNIEnv* env, jarray array, jboolean* isCopy) {
    fjni_logv_warn("[JNI] GetPrimitiveArrayCritical(env, 0x%x, 0x%x): not implemented", (int)array, (int)isCopy);
    return NULL;
}

void ReleasePrimitiveArrayCritical(JNIEnv* env, jarray array, void* carray, jint mode) {
    fjni_log_warn("[JNI] ReleasePrimitiveArrayCritical(): not implemented");
}

const jchar* GetStringCritical(JNIEnv* env, jstring string, jboolean* isCopy) {
    fjni_logv_dbg("[JNI] GetStringCritical(env, 0x%x/\"%s\", *isCopy)", string, string);

    if (!string) {
        fjni_log_err("String is null.");
        return NULL;
    }

    if (isCopy != NULL) {
        *isCopy = JNI_TRUE;
    }

    return (const jchar *)strdup((const char*)string);
}

void ReleaseStringCritical(JNIEnv* env, jstring string, const jchar* carray) {
    fjni_logv_dbg("[JNI] ReleaseStringCritical(env, 0x%x/\"%s\", 0x%x)", string, string, carray);

    if (!carray) {
        fjni_log_err("carray is null");
        return;
    }

    free((char*)carray);
}

jweak NewWeakGlobalRef(JNIEnv* env, jobject obj) {
    fjni_logv_dbg("[JNI] NewWeakGlobalRef(env, 0x%x): ignored", (int)obj);
    return (jweak)obj;
}

void DeleteWeakGlobalRef(JNIEnv* env, jweak obj) {
    fjni_logv_dbg("[JNI] DeleteWeakGlobalRef(env, 0x%x)", (int)obj);
    // Do not free, since we have not created any additional references above.
}

jboolean ExceptionCheck(JNIEnv* env) {
    fjni_log_dbg("[JNI] ExceptionCheck(env): ignored");
    return JNI_FALSE;
}

jobject NewDirectByteBuffer(JNIEnv* env, void* address, jlong capacity) {
    fjni_logv_warn("[JNI] NewDirectByteBuffer(env, 0x%x, %i): not implemented", (int)address, capacity);
    return NULL;
}

void* GetDirectBufferAddress(JNIEnv* env, jobject buf) {
    fjni_logv_warn("[JNI] GetDirectBufferAddress(env, 0x%x)", (int)buf);
    return NULL;
}

jlong GetDirectBufferCapacity(JNIEnv* env, jobject buf) {
    fjni_logv_warn("[JNI] GetDirectBufferCapacity(env, 0x%x)", (int)buf);
    return 0;
}

jobjectRefType GetObjectRefType(JNIEnv* env, jobject obj) {
    fjni_logv_warn("[JNI] GetObjectRefType(env, 0x%x): not implemented", (int)obj);
    return JNIInvalidRefType;
}

void jni_init() {
    _jvm = (struct JNIInvokeInterface *) malloc(sizeof(struct JNIInvokeInterface));
    _jvm->DestroyJavaVM = DestroyJavaVM;
    _jvm->AttachCurrentThread = AttachCurrentThread;
    _jvm->DetachCurrentThread = DetachCurrentThread;
    _jvm->GetEnv = GetEnv;
    _jvm->AttachCurrentThreadAsDaemon = AttachCurrentThreadAsDaemon;

    _jni = (struct JNINativeInterface *) malloc(sizeof(struct JNINativeInterface));
    _jni->GetVersion = GetVersion;
    _jni->DefineClass = DefineClass;
    _jni->FindClass = FindClass;
    _jni->FromReflectedMethod = FromReflectedMethod;
    _jni->FromReflectedField = FromReflectedField;
    _jni->ToReflectedMethod = ToReflectedMethod;
    _jni->GetSuperclass = GetSuperclass;
    _jni->IsAssignableFrom = IsAssignableFrom;
    _jni->ToReflectedField = ToReflectedField;
    _jni->Throw = Throw;
    _jni->ThrowNew = ThrowNew;
    _jni->ExceptionOccurred = ExceptionOccurred;
    _jni->ExceptionDescribe = ExceptionDescribe;
    _jni->ExceptionClear = ExceptionClear;
    _jni->FatalError = FatalError;
    _jni->PushLocalFrame = PushLocalFrame;
    _jni->PopLocalFrame = PopLocalFrame;
    _jni->NewGlobalRef = NewGlobalRef;
    _jni->DeleteGlobalRef = DeleteGlobalRef;
    _jni->DeleteLocalRef = DeleteLocalRef;
    _jni->IsSameObject = IsSameObject;
    _jni->NewLocalRef = NewLocalRef;
    _jni->EnsureLocalCapacity = EnsureLocalCapacity;
    _jni->AllocObject = AllocObject;
    _jni->NewObject = NewObject;
    _jni->NewObjectV = NewObjectV;
    _jni->NewObjectA = NewObjectA;
    _jni->GetObjectClass = GetObjectClass;
    _jni->IsInstanceOf = IsInstanceOf;
    _jni->GetMethodID = GetMethodID;
    _jni->CallObjectMethod = CallObjectMethod;
    _jni->CallObjectMethodV = CallObjectMethodV;
    _jni->CallObjectMethodA = CallObjectMethodA;
    _jni->CallBooleanMethod = CallBooleanMethod;
    _jni->CallBooleanMethodV = CallBooleanMethodV;
    _jni->CallBooleanMethodA = CallBooleanMethodA;
    _jni->CallByteMethod = CallByteMethod;
    _jni->CallByteMethodV = CallByteMethodV;
    _jni->CallByteMethodA = CallByteMethodA;
    _jni->CallCharMethod = CallCharMethod;
    _jni->CallCharMethodV = CallCharMethodV;
    _jni->CallCharMethodA = CallCharMethodA;
    _jni->CallShortMethod = CallShortMethod;
    _jni->CallShortMethodV = CallShortMethodV;
    _jni->CallShortMethodA = CallShortMethodA;
    _jni->CallIntMethod = CallIntMethod;
    _jni->CallIntMethodV = CallIntMethodV;
    _jni->CallIntMethodA = CallIntMethodA;
    _jni->CallLongMethod = CallLongMethod;
    _jni->CallLongMethodV = CallLongMethodV;
    _jni->CallLongMethodA = CallLongMethodA;
    _jni->CallFloatMethod = CallFloatMethod;
    _jni->CallFloatMethodV = CallFloatMethodV;
    _jni->CallFloatMethodA = CallFloatMethodA;
    _jni->CallDoubleMethod = CallDoubleMethod;
    _jni->CallDoubleMethodV = CallDoubleMethodV;
    _jni->CallDoubleMethodA = CallDoubleMethodA;
    _jni->CallVoidMethod = CallVoidMethod;
    _jni->CallVoidMethodV = CallVoidMethodV;
    _jni->CallVoidMethodA = CallVoidMethodA;
    _jni->CallNonvirtualObjectMethod = CallNonvirtualObjectMethod;
    _jni->CallNonvirtualObjectMethodV = CallNonvirtualObjectMethodV;
    _jni->CallNonvirtualObjectMethodA = CallNonvirtualObjectMethodA;
    _jni->CallNonvirtualBooleanMethod = CallNonvirtualBooleanMethod;
    _jni->CallNonvirtualBooleanMethodV = CallNonvirtualBooleanMethodV;
    _jni->CallNonvirtualBooleanMethodA = CallNonvirtualBooleanMethodA;
    _jni->CallNonvirtualByteMethod = CallNonvirtualByteMethod;
    _jni->CallNonvirtualByteMethodV = CallNonvirtualByteMethodV;
    _jni->CallNonvirtualByteMethodA = CallNonvirtualByteMethodA;
    _jni->CallNonvirtualCharMethod = CallNonvirtualCharMethod;
    _jni->CallNonvirtualCharMethodV = CallNonvirtualCharMethodV;
    _jni->CallNonvirtualCharMethodA = CallNonvirtualCharMethodA;
    _jni->CallNonvirtualShortMethod = CallNonvirtualShortMethod;
    _jni->CallNonvirtualShortMethodV = CallNonvirtualShortMethodV;
    _jni->CallNonvirtualShortMethodA = CallNonvirtualShortMethodA;
    _jni->CallNonvirtualIntMethod = CallNonvirtualIntMethod;
    _jni->CallNonvirtualIntMethodV = CallNonvirtualIntMethodV;
    _jni->CallNonvirtualIntMethodA = CallNonvirtualIntMethodA;
    _jni->CallNonvirtualLongMethod = CallNonvirtualLongMethod;
    _jni->CallNonvirtualLongMethodV = CallNonvirtualLongMethodV;
    _jni->CallNonvirtualLongMethodA = CallNonvirtualLongMethodA;
    _jni->CallNonvirtualFloatMethod = CallNonvirtualFloatMethod;
    _jni->CallNonvirtualFloatMethodV = CallNonvirtualFloatMethodV;
    _jni->CallNonvirtualFloatMethodA = CallNonvirtualFloatMethodA;
    _jni->CallNonvirtualDoubleMethod = CallNonvirtualDoubleMethod;
    _jni->CallNonvirtualDoubleMethodV = CallNonvirtualDoubleMethodV;
    _jni->CallNonvirtualDoubleMethodA = CallNonvirtualDoubleMethodA;
    _jni->CallNonvirtualVoidMethod = CallNonvirtualVoidMethod;
    _jni->CallNonvirtualVoidMethodV = CallNonvirtualVoidMethodV;
    _jni->CallNonvirtualVoidMethodA = CallNonvirtualVoidMethodA;
    _jni->GetFieldID = GetFieldID;
    _jni->GetObjectField = GetObjectField;
    _jni->GetBooleanField = GetBooleanField;
    _jni->GetByteField = GetByteField;
    _jni->GetCharField = GetCharField;
    _jni->GetShortField = GetShortField;
    _jni->GetIntField = GetIntField;
    _jni->GetLongField = GetLongField;
    _jni->GetFloatField = GetFloatField;
    _jni->GetDoubleField = GetDoubleField;
    _jni->SetObjectField = SetObjectField;
    _jni->SetBooleanField = SetBooleanField;
    _jni->SetByteField = SetByteField;
    _jni->SetCharField = SetCharField;
    _jni->SetShortField = SetShortField;
    _jni->SetIntField = SetIntField;
    _jni->SetLongField = SetLongField;
    _jni->SetFloatField = SetFloatField;
    _jni->SetDoubleField = SetDoubleField;
    _jni->GetStaticMethodID = GetStaticMethodID;
    _jni->CallStaticObjectMethod = CallStaticObjectMethod;
    _jni->CallStaticObjectMethodV = CallStaticObjectMethodV;
    _jni->CallStaticObjectMethodA = CallStaticObjectMethodA;
    _jni->CallStaticBooleanMethod = CallStaticBooleanMethod;
    _jni->CallStaticBooleanMethodV = CallStaticBooleanMethodV;
    _jni->CallStaticBooleanMethodA = CallStaticBooleanMethodA;
    _jni->CallStaticByteMethod = CallStaticByteMethod;
    _jni->CallStaticByteMethodV = CallStaticByteMethodV;
    _jni->CallStaticByteMethodA = CallStaticByteMethodA;
    _jni->CallStaticCharMethod = CallStaticCharMethod;
    _jni->CallStaticCharMethodV = CallStaticCharMethodV;
    _jni->CallStaticCharMethodA = CallStaticCharMethodA;
    _jni->CallStaticShortMethod = CallStaticShortMethod;
    _jni->CallStaticShortMethodV = CallStaticShortMethodV;
    _jni->CallStaticShortMethodA = CallStaticShortMethodA;
    _jni->CallStaticIntMethod = CallStaticIntMethod;
    _jni->CallStaticIntMethodV = CallStaticIntMethodV;
    _jni->CallStaticIntMethodA = CallStaticIntMethodA;
    _jni->CallStaticLongMethod = CallStaticLongMethod;
    _jni->CallStaticLongMethodV = CallStaticLongMethodV;
    _jni->CallStaticLongMethodA = CallStaticLongMethodA;
    _jni->CallStaticFloatMethod = CallStaticFloatMethod;
    _jni->CallStaticFloatMethodV = CallStaticFloatMethodV;
    _jni->CallStaticFloatMethodA = CallStaticFloatMethodA;
    _jni->CallStaticDoubleMethod = CallStaticDoubleMethod;
    _jni->CallStaticDoubleMethodV = CallStaticDoubleMethodV;
    _jni->CallStaticDoubleMethodA = CallStaticDoubleMethodA;
    _jni->CallStaticVoidMethod = CallStaticVoidMethod;
    _jni->CallStaticVoidMethodV = CallStaticVoidMethodV;
    _jni->CallStaticVoidMethodA = CallStaticVoidMethodA;
    _jni->GetStaticFieldID = GetStaticFieldID;
    _jni->GetStaticObjectField = GetStaticObjectField;
    _jni->GetStaticBooleanField = GetStaticBooleanField;
    _jni->GetStaticByteField = GetStaticByteField;
    _jni->GetStaticCharField = GetStaticCharField;
    _jni->GetStaticShortField = GetStaticShortField;
    _jni->GetStaticIntField = GetStaticIntField;
    _jni->GetStaticLongField = GetStaticLongField;
    _jni->GetStaticFloatField = GetStaticFloatField;
    _jni->GetStaticDoubleField = GetStaticDoubleField;
    _jni->SetStaticObjectField = SetStaticObjectField;
    _jni->SetStaticBooleanField = SetStaticBooleanField;
    _jni->SetStaticByteField = SetStaticByteField;
    _jni->SetStaticCharField = SetStaticCharField;
    _jni->SetStaticShortField = SetStaticShortField;
    _jni->SetStaticIntField = SetStaticIntField;
    _jni->SetStaticLongField = SetStaticLongField;
    _jni->SetStaticFloatField = SetStaticFloatField;
    _jni->SetStaticDoubleField = SetStaticDoubleField;
    _jni->NewString = NewString;
    _jni->GetStringLength = GetStringLength;
    _jni->GetStringChars = GetStringChars;
    _jni->ReleaseStringChars = ReleaseStringChars;
    _jni->NewStringUTF = NewStringUTF;
    _jni->GetStringUTFLength = GetStringUTFLength;
    _jni->GetStringUTFChars = GetStringUTFChars;
    _jni->ReleaseStringUTFChars = ReleaseStringUTFChars;
    _jni->GetArrayLength = GetArrayLength;
    _jni->NewObjectArray = NewObjectArray;
    _jni->GetObjectArrayElement = GetObjectArrayElement;
    _jni->SetObjectArrayElement = SetObjectArrayElement;
    _jni->NewBooleanArray = NewBooleanArray;
    _jni->NewByteArray = NewByteArray;
    _jni->NewCharArray = NewCharArray;
    _jni->NewShortArray = NewShortArray;
    _jni->NewIntArray = NewIntArray;
    _jni->NewLongArray = NewLongArray;
    _jni->NewFloatArray = NewFloatArray;
    _jni->NewDoubleArray = NewDoubleArray;
    _jni->GetBooleanArrayElements = GetBooleanArrayElements;
    _jni->GetByteArrayElements = GetByteArrayElements;
    _jni->GetCharArrayElements = GetCharArrayElements;
    _jni->GetShortArrayElements = GetShortArrayElements;
    _jni->GetIntArrayElements = GetIntArrayElements;
    _jni->GetLongArrayElements = GetLongArrayElements;
    _jni->GetFloatArrayElements = GetFloatArrayElements;
    _jni->GetDoubleArrayElements = GetDoubleArrayElements;
    _jni->ReleaseBooleanArrayElements = ReleaseBooleanArrayElements;
    _jni->ReleaseByteArrayElements = ReleaseByteArrayElements;
    _jni->ReleaseCharArrayElements = ReleaseCharArrayElements;
    _jni->ReleaseShortArrayElements = ReleaseShortArrayElements;
    _jni->ReleaseIntArrayElements = ReleaseIntArrayElements;
    _jni->ReleaseLongArrayElements = ReleaseLongArrayElements;
    _jni->ReleaseFloatArrayElements = ReleaseFloatArrayElements;
    _jni->ReleaseDoubleArrayElements = ReleaseDoubleArrayElements;
    _jni->GetBooleanArrayRegion = GetBooleanArrayRegion;
    _jni->GetByteArrayRegion = GetByteArrayRegion;
    _jni->GetCharArrayRegion = GetCharArrayRegion;
    _jni->GetShortArrayRegion = GetShortArrayRegion;
    _jni->GetIntArrayRegion = GetIntArrayRegion;
    _jni->GetLongArrayRegion = GetLongArrayRegion;
    _jni->GetFloatArrayRegion = GetFloatArrayRegion;
    _jni->GetDoubleArrayRegion = GetDoubleArrayRegion;
    _jni->SetBooleanArrayRegion = SetBooleanArrayRegion;
    _jni->SetByteArrayRegion = SetByteArrayRegion;
    _jni->SetCharArrayRegion = SetCharArrayRegion;
    _jni->SetShortArrayRegion = SetShortArrayRegion;
    _jni->SetIntArrayRegion = SetIntArrayRegion;
    _jni->SetLongArrayRegion = SetLongArrayRegion;
    _jni->SetFloatArrayRegion = SetFloatArrayRegion;
    _jni->SetDoubleArrayRegion = SetDoubleArrayRegion;
    _jni->RegisterNatives = RegisterNatives;
    _jni->UnregisterNatives = UnregisterNatives;
    _jni->MonitorEnter = MonitorEnter;
    _jni->MonitorExit = MonitorExit;
    _jni->GetJavaVM = GetJavaVM;
    _jni->GetStringRegion = GetStringRegion;
    _jni->GetStringUTFRegion = GetStringUTFRegion;
    _jni->GetPrimitiveArrayCritical = GetPrimitiveArrayCritical;
    _jni->ReleasePrimitiveArrayCritical = ReleasePrimitiveArrayCritical;
    _jni->GetStringCritical = GetStringCritical;
    _jni->ReleaseStringCritical = ReleaseStringCritical;
    _jni->NewWeakGlobalRef = NewWeakGlobalRef;
    _jni->DeleteWeakGlobalRef = DeleteWeakGlobalRef;
    _jni->ExceptionCheck = ExceptionCheck;
    _jni->NewDirectByteBuffer = NewDirectByteBuffer;
    _jni->GetDirectBufferAddress = GetDirectBufferAddress;
    _jni->GetDirectBufferCapacity = GetDirectBufferCapacity;
    _jni->GetObjectRefType = GetObjectRefType;

    jvm = _jvm;
    jni = _jni;
}
