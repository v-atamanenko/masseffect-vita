#ifndef FALSOJNI_H
#define FALSOJNI_H

#define FALSOJNI_DEBUG_NO    4
#define FALSOJNI_DEBUG_ERROR 3
#define FALSOJNI_DEBUG_WARN  2
#define FALSOJNI_DEBUG_INFO  1
#define FALSOJNI_DEBUG_ALL   0

#ifndef FALSOJNI_DEBUGLEVEL
#define FALSOJNI_DEBUGLEVEL FALSOJNI_DEBUG_WARN
#endif

#include "jni.h"

#ifdef FALSOJNI_IMPLEMENTATION_SAMPLE
#include "FalsoJNI_ImplSample.h"
#endif

extern JavaVM jvm;
extern JNIEnv jni;

void jni_init();

#endif // FALSOJNI_H
