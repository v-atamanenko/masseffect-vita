/*
 * falojni_impl.c
 *
 * Implementations for JNI functions and fields to be used by FalsoJNI.
 *
 * Copyright (C) 2022 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#pragma clang diagnostic ignored "-Wincompatible-function-pointer-types"
#pragma clang diagnostic ignored "-Wwritable-strings"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "UnusedParameter"

#include <FalsoJNI/FalsoJNI.h>
#include <FalsoJNI/FalsoJNI_Impl.h>

#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "android/java.io.InputStream.h"
#include "android/EAAudioCore.h"

#include "utils/logger.h"

//extern void (*Java_com_ea_EAIO_EAIO_Startup)(JNIEnv*, void*, jobject);
// com/ea/EAIO/EAIO/Startup
void ea_EAIO_Startup(jmethodID id, va_list args) {
    void* assetManager = va_arg(args, void*);
    logv_debug("JNI: Method Call: com/ea/EAIO/EAIO/Startup(AssetManager: 0x%x) / id: %i", (int)assetManager, id);

    //Java_com_ea_EAIO_EAIO_Startup(&jni, NULL, assetManager);
}

// com/ea/blast/MainActivity/GetInstance
jobject ea_blast_MainActivity_GetInstance(jmethodID id, va_list args) {
    //void* assetManager = va_arg(args, void*);
    logv_debug("JNI: Method Call: com/ea/blast/MainActivity/GetInstance() / id: %i", id);
    return strdup("MainActivityInstance");
}

// 	com/android/content/Context/getAssets
jobject android_content_Context_getAssets(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/android/content/Context/getAssets() / id: %i", id);
    return strdup("getAssetsInstance");
}

const char* _string_empty = "";
const char* _string_0 = "0";
const char* _string_1 = "1";
const char* _string_2 = "2";
const char* _string_19 = "19";
const char* _string_minus1 = "-1";
const char* _string_manufacturer = "sony";
const char* _string_devicename = "R800";
const char* _string_devicemodel = "Vita";
const char* _string_true = "true";
const char* _string_android = "Android";
const char* _string_release = "4.4.4";
const char* _string_cpuarch = "armeabi-v7a";
const char* _string_en = "en";

// 	com/ea/blast/SystemAndroidDelegate/GetAccelerometerCount
jint ea_blast_SystemAndroidDelegate_GetAccelerometerCount(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetAccelerometerCount() / id: %i", id);
    return 1;
}

// 	com/ea/blast/SystemAndroidDelegate/IsBatteryStateAvailable
jobject ea_blast_SystemAndroidDelegate_IsBatteryStateAvailable(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/IsBatteryStateAvailable() / id: %i", id);
    return (jobject)_string_0;
}

// 	com/ea/blast/SystemAndroidDelegate/GetCameraCount
jint ea_blast_SystemAndroidDelegate_GetCameraCount(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetCameraCount() / id: %i", id);
    return 0;
}

// 	com/ea/blast/SystemAndroidDelegate/GetChipset
jobject ea_blast_SystemAndroidDelegate_GetChipset(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetChipset() / id: %i", id);
    return (jobject)_string_0;
}

// 	com/ea/blast/SystemAndroidDelegate/GetCompassCount
jint ea_blast_SystemAndroidDelegate_GetCompassCount(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetCompassCount() / id: %i", id);
    return 0;
}

// 	com/ea/blast/SystemAndroidDelegate/GetManufacturer
jobject ea_blast_SystemAndroidDelegate_GetManufacturer(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetManufacturer() / id: %i", id);
    return (jobject)_string_manufacturer;
}

// 	com/ea/blast/SystemAndroidDelegate/GetDeviceModel
jobject ea_blast_SystemAndroidDelegate_GetDeviceModel(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetDeviceModel() / id: %i", id);
    return (jobject)_string_devicemodel;
}

// 	com/ea/blast/SystemAndroidDelegate/GetDeviceName
jobject ea_blast_SystemAndroidDelegate_GetDeviceName(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetDeviceName() / id: %i", id);
    return (jobject)_string_devicename;
}

// 	com/ea/blast/SystemAndroidDelegate/GetPhoneNumber
jobject ea_blast_SystemAndroidDelegate_GetPhoneNumber(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetPhoneNumber() / id: %i", id);
    return (jobject)_string_0;
}

// 	com/ea/blast/SystemAndroidDelegate/GetDeviceSubscriberID
jobject ea_blast_SystemAndroidDelegate_GetDeviceSubscriberID(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetDeviceSubscriberID() / id: %i", id);
    return (jobject)_string_0;
}

// 	com/ea/blast/SystemAndroidDelegate/GetDeviceUniqueId
jobject ea_blast_SystemAndroidDelegate_GetDeviceUniqueId(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetDeviceUniqueId() / id: %i", id);
    return (jobject)_string_minus1;
}

// 	com/ea/blast/SystemAndroidDelegate/GetDisplayCount
jobject ea_blast_SystemAndroidDelegate_GetDisplayCount(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetDisplayCount() / id: %i", id);
    return (jobject)_string_1;
}

// 	com/ea/blast/SystemAndroidDelegate/GetGyroscopeCount
jint ea_blast_SystemAndroidDelegate_GetGyroscopeCount(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetGyroscopeCount() / id: %i", id);
    return 1;
}

// 	com/ea/blast/SystemAndroidDelegate/GetLocationAvailable
jobject ea_blast_SystemAndroidDelegate_GetLocationAvailable(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetLocationAvailable() / id: %i", id);
    return (jobject)_string_true;
}

// 	com/ea/blast/SystemAndroidDelegate/GetMicrophoneCount
jint ea_blast_SystemAndroidDelegate_GetMicrophoneCount(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetMicrophoneCount() / id: %i", id);
    return 1;
}

// 	com/ea/blast/SystemAndroidDelegate/GetApiLevel
jobject ea_blast_SystemAndroidDelegate_GetApiLevel(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetApiLevel() / id: %i", id);
    return (jobject)_string_19;
}

// 	com/ea/blast/SystemAndroidDelegate/GetPlatformRawName
jobject ea_blast_SystemAndroidDelegate_GetPlatformRawName(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetPlatformRawName() / id: %i", id);

    return (jobject)_string_android;
}

// 	com/ea/blast/SystemAndroidDelegate/GetPlatformStdName
jobject ea_blast_SystemAndroidDelegate_GetPlatformStdName(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetPlatformStdName() / id: %i", id);
    return (jobject)_string_android;
}

// 	com/ea/blast/SystemAndroidDelegate/GetPlatformVersion
jobject ea_blast_SystemAndroidDelegate_GetPlatformVersion(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetPlatformVersion() / id: %i", id);
    return (jobject)_string_release;
}

// 	com/ea/blast/SystemAndroidDelegate/GetPhysicalKeyboardCount
jobject ea_blast_SystemAndroidDelegate_GetPhysicalKeyboardCount(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetPhysicalKeyboardCount() / id: %i", id);
    return (jobject)_string_1;
}

// 	com/ea/blast/SystemAndroidDelegate/GetProcessorArchitecture
jobject ea_blast_SystemAndroidDelegate_GetProcessorArchitecture(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetProcessorArchitecture() / id: %i", id);
    return (jobject)_string_cpuarch;
}

// 	com/ea/blast/SystemAndroidDelegate/GetLanguage
jobject ea_blast_SystemAndroidDelegate_GetLanguage(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetLanguage() / id: %i", id);
    // TODO: Check for system language
    return (jobject)_string_en;
}

// 	com/ea/blast/SystemAndroidDelegate/GetLocale
jobject ea_blast_SystemAndroidDelegate_GetLocale(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetLocale() / id: %i", id);
    // TODO: Check for system language
    return (jobject)_string_en;
}

// 	com/ea/blast/SystemAndroidDelegate/GetTotalRAM
jobject ea_blast_SystemAndroidDelegate_GetTotalRAM(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetTotalRAM() / id: %i", id);
    return (jobject)_string_minus1;
}

// 	com/ea/blast/SystemAndroidDelegate/GetTouchPadCount
jint ea_blast_SystemAndroidDelegate_GetTouchPadCount(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetTouchPadCount() / id: %i", id);
    return 1;
}

// 	com/ea/blast/SystemAndroidDelegate/GetTouchScreenCount
jint ea_blast_SystemAndroidDelegate_GetTouchScreenCount(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetTouchScreenCount() / id: %i", id);
    return 1;
}

// 	com/ea/blast/SystemAndroidDelegate/GetTrackBallCount
jint ea_blast_SystemAndroidDelegate_GetTrackBallCount(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetTrackBallCount() / id: %i", id);
    return 0;
}

// 	com/ea/blast/SystemAndroidDelegate/GetVibratorCount
jint ea_blast_SystemAndroidDelegate_GetVibratorCount(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetVibratorCount() / id: %i", id);
    // TODO: Support for DualShock vibrator?
    return 0;
}

// 	com/ea/blast/SystemAndroidDelegate/GetVibratorCount
jint ea_blast_SystemAndroidDelegate_GetApplicationVersionCode(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetApplicationVersionCode() / id: %i", id);
    return 42;
}

// 	com/ea/blast/SystemAndroidDelegate/GetVirtualKeyboardCount
jobject ea_blast_SystemAndroidDelegate_GetVirtualKeyboardCount(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetVirtualKeyboardCount() / id: %i", id);
    return (jobject)_string_1;
}

// 	com/ea/blast/SystemAndroidDelegate/GetApplicationVersionCode
jobject ea_blast_SystemAndroidDelegate_GetApplicationVersion(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetApplicationVersion() / id: %i", id);
    return (jobject)_string_1;
}
// 	com/ea/blast/SystemAndroidDelegate/GetHardwareFloatingPointSupport
jobject ea_blast_SystemAndroidDelegate_GetHardwareFloatingPointSupport(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetHardwareFloatingPointSupport() / id: %i", id);
    return (jobject)_string_true;
}

// 	com/ea/blast/SystemAndroidDelegate/GetFirmware
jobject ea_blast_SystemAndroidDelegate_GetFirmware(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetFirmware() / id: %i", id);
    return (jobject)_string_1;
}

// 	com/ea/blast/GetAppDataDirectoryDelegate/GetAppDataDirectory
jobject GetAppDataDirectory(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: GetAppDataDirectory() / id: %i", id);
    return (jobject) strdup(DATA_PATH);
}

// 	com/ea/blast/GetAppDataDirectoryDelegate/GetExternalStorageDirectory
jobject GetExternalStorageDirectory(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: GetExternalStorageDirectory() / id: %i", id);
    return (jobject) strdup(DATA_PATH);
}

jobject GetInternalStorageDirectory(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: GetInternalStorageDirectory() / id: %i", id);
    return (jobject) strdup(DATA_PATH_INT);
}

jobject GetPrimaryExternalStorageDirectory(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: GetPrimaryExternalStorageDirectory() / id: %i", id);
    return (jobject) strdup(DATA_PATH_INT);
}

jobject getContentPath(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: getContentPath() / id: %i", id);
    return (jobject) strdup(DATA_PATH_INT);
}

#define MEDIA_MOUNTED "mounted"

jobject GetPrimaryExternalStorageState(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: GetPrimaryExternalStorageState() / id: %i", id);
    return (jobject) strdup(MEDIA_MOUNTED);
}

jobject GetDedicatedDirectory(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: GetDedicatedDirectory() / id: %i", id);
    return (jobject) strdup("files");
}

jobject getPackage(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: getPackage() / id: %i", id);
    return (jobject) strdup("com.ea.games.meinfiltrator");
}

jobject dummyConstructor(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: <init>() / id: %i", id);
    // There might be an attempt to free this object; anyway, we can afford to
    // leak a few bytes here.
    return (jobject)strdup("nop");
}

jboolean isContentReady(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: isContentReady() / id: %i", id);
    return JNI_TRUE;
}

jboolean IsTouchScreenMultiTouch(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: IsTouchScreenMultiTouch() / id: %i", id);
    return JNI_TRUE;
}

jboolean IsNavigationVisible(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: IsNavigationVisible() / id: %i", id);
    return JNI_FALSE;
}

jboolean IsVisible(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: IsVisible() / id: %i", id);
    return JNI_TRUE;
}

// com/eamobile/Query/getVersion
jobject getVersion(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: getVersion() / id: %i", id);
    return (jobject) strdup("1.0.1");
}

// com/ea/blast/PowerManagerAndroid/ApplyKeepAwake
void ApplyKeepAwake(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: ApplyKeepAwake() / id: %i", id);
}

// com/ea/blast/DisplayAndroidDelegate.java
int GetStdOrientation(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: GetStdOrientation() / id: %i", id);
    return 8; // possible vals: 4 0 2 8 1
}

int GetDefaultWidth(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: GetDefaultWidth() / id: %i", id);
    return 960;
}

int GetDefaultHeight(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: GetDefaultHeight() / id: %i", id);
    return 544;
}

float GetDpiX(jmethodID id, va_list args) {
    //logv_debug("JNI: Method Call: GetDpiX() / id: %i", id);
    return 200.0f;
}

float GetDpiY(jmethodID id, va_list args) {
    //logv_debug("JNI: Method Call: GetDpiY() / id: %n", id);
    return 200.0f;
}

// com/ea/blast/DeviceOrientationHandlerAndroidDelegate/SetStdOrientation
void SetStdOrientation(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: SetStdOrientation() / id: %i", id);
    // We don't support changing orientation, ignore.
}

// com/ea/blast/DeviceOrientationHandlerAndroidDelegate/OnLifeCycleFocusGained
void OnLifeCycleFocusGained(jmethodID id, va_list args) {
    logv_debug("JNI: Method Call: OnLifeCycleFocusGained() / id: %i", id);
    // We don't support changing orientation, ignore.
}

// com/ea/blast/AccelerometerAndroidDelegate/SetEnabled
// com/ea/blast/DeviceOrientationHandlerAndroidDelegate/SetEnabled
void SetEnabled(jmethodID id, va_list args) {
    // We deal with acceletometer stuff other way, ignore
    // We don't support changing orientation, ignore.
}

// com/ea/blast/AccelerometerAndroidDelegate/SetUpdateFrequency
void SetUpdateFrequency(jmethodID id, va_list args) {
    // We deal with acceletometer stuff other way, ignore
}

void finish(jmethodID id, va_list args) {
    fprintf(stderr, "MainActivity.finish();\n");
    abort();
}

// com/eamobile/Query/getTotalMemory
jlong getTotalMemory(jmethodID id, va_list args) {
    return MEMORY_NEWLIB_MB;
}

NameToMethodID nameToMethodId[] = {
    { 100, "isContentReady", METHOD_TYPE_BOOLEAN },
    { 101, "ApplyKeepAwake", METHOD_TYPE_VOID },
    { 102, "GetStdOrientation", METHOD_TYPE_INT },
    { 103, "GetDefaultWidth", METHOD_TYPE_INT },
    { 104, "GetDefaultHeight", METHOD_TYPE_INT },
    { 105, "GetDpiX", METHOD_TYPE_FLOAT },
    { 106, "GetDpiY", METHOD_TYPE_FLOAT },
    { 107, "GetAppDataDirectory", METHOD_TYPE_OBJECT},
    { 108, "GetExternalStorageDirectory", METHOD_TYPE_OBJECT},
    { 109, "SetStdOrientation", METHOD_TYPE_VOID },
    { 110, "OnLifeCycleFocusGained", METHOD_TYPE_VOID },
    { 111, "SetEnabled", METHOD_TYPE_VOID },
    { 112, "IsTouchScreenMultiTouch", METHOD_TYPE_BOOLEAN },
    { 113, "getVersion", METHOD_TYPE_OBJECT },
    { 114, "getTotalMemory", METHOD_TYPE_LONG },
    { 115, "SetUpdateFrequency", METHOD_TYPE_VOID },
    { 116, "play", METHOD_TYPE_VOID },
    { 117, "stop", METHOD_TYPE_VOID },
    { 118, "write", METHOD_TYPE_INT },
    { 119, "IsNavigationVisible", METHOD_TYPE_BOOLEAN },
    { 120, "IsVisible", METHOD_TYPE_BOOLEAN },
    { 219, "Startup", METHOD_TYPE_VOID },
    { 220, "GetInstance", METHOD_TYPE_OBJECT },
    { 221, "getAssets", METHOD_TYPE_OBJECT },
    { 300, "GetAccelerometerCount", METHOD_TYPE_INT },
    { 301, "IsBatteryStateAvailable", METHOD_TYPE_OBJECT },
    { 302, "GetCameraCount", METHOD_TYPE_INT },
    { 303, "GetChipset", METHOD_TYPE_OBJECT },
    { 304, "GetCompassCount", METHOD_TYPE_INT },
    { 305, "GetManufacturer", METHOD_TYPE_OBJECT },
    { 306, "GetDeviceModel", METHOD_TYPE_OBJECT },
    { 307, "GetDeviceName", METHOD_TYPE_OBJECT },
    { 308, "GetPhoneNumber", METHOD_TYPE_OBJECT },
    { 309, "GetDeviceSubscriberID", METHOD_TYPE_OBJECT },
    { 310, "GetDeviceUniqueId", METHOD_TYPE_OBJECT },
    { 311, "GetDisplayCount", METHOD_TYPE_OBJECT },
    { 312, "GetGyroscopeCount", METHOD_TYPE_INT },
    { 313, "GetLocationAvailable", METHOD_TYPE_OBJECT },
    { 314, "GetMicrophoneCount", METHOD_TYPE_INT },
    { 315, "GetApiLevel", METHOD_TYPE_OBJECT },
    { 316, "GetPlatformRawName", METHOD_TYPE_OBJECT },
    { 317, "GetPlatformStdName", METHOD_TYPE_OBJECT },
    { 318, "GetPlatformVersion", METHOD_TYPE_OBJECT },
    { 319, "GetPhysicalKeyboardCount", METHOD_TYPE_OBJECT },
    { 320, "GetProcessorArchitecture", METHOD_TYPE_OBJECT },
    { 321, "GetLanguage", METHOD_TYPE_OBJECT },
    { 322, "GetLocale", METHOD_TYPE_OBJECT },
    { 323, "GetTotalRAM", METHOD_TYPE_OBJECT },
    { 324, "GetTouchPadCount", METHOD_TYPE_INT },
    { 325, "GetTouchScreenCount", METHOD_TYPE_INT },
    { 326, "GetTrackBallCount", METHOD_TYPE_INT },
    { 327, "GetVibratorCount", METHOD_TYPE_INT },
    { 328, "GetVirtualKeyboardCount", METHOD_TYPE_OBJECT },
    { 329, "GetApplicationVersionCode", METHOD_TYPE_INT },
    { 330, "GetApplicationVersion", METHOD_TYPE_OBJECT },
    { 331, "GetFirmware", METHOD_TYPE_OBJECT },
    { 332, "GetHardwareFloatingPointSupport", METHOD_TYPE_OBJECT },
    { 401, "com/ea/blast/SystemAndroidDelegate/<init>", METHOD_TYPE_OBJECT },
    { 402, "com/ea/blast/PowerManagerAndroid/<init>", METHOD_TYPE_OBJECT },
    { 403, "com/ea/blast/DisplayAndroidDelegate/<init>", METHOD_TYPE_OBJECT },
    { 404, "com/ea/blast/DeviceOrientationHandlerAndroidDelegate/<init>", METHOD_TYPE_OBJECT },
    { 405, "com/ea/blast/GetAppDataDirectoryDelegate/<init>", METHOD_TYPE_OBJECT },
    { 406, "com/ea/EAMIO/StorageDirectory/<init>", METHOD_TYPE_OBJECT },
    { 407, "com/ea/blast/AccelerometerAndroidDelegate/<init>", METHOD_TYPE_OBJECT },
    { 408, "com/ea/blast/VirtualKeyboardAndroidDelegate/<init>", METHOD_TYPE_OBJECT },
    { 409, "com/ea/blast/PhysicalKeyboardAndroidDelegate/<init>", METHOD_TYPE_OBJECT },
    { 600, "read", METHOD_TYPE_INT },
    { 601, "close", METHOD_TYPE_VOID },
    { 602, "skip", METHOD_TYPE_OBJECT },
    { 603, "open", METHOD_TYPE_OBJECT },
    { 604, "openFd", METHOD_TYPE_OBJECT },
    { 605, "list", METHOD_TYPE_OBJECT },
    { 606, "getLength", METHOD_TYPE_LONG },
    { 700, "GetInternalStorageDirectory", METHOD_TYPE_OBJECT },
    { 701, "GetPrimaryExternalStorageDirectory", METHOD_TYPE_OBJECT },
    { 702, "GetPrimaryExternalStorageState", METHOD_TYPE_OBJECT },
    { 703, "GetDedicatedDirectory", METHOD_TYPE_OBJECT },
    { 704, "getPackage", METHOD_TYPE_OBJECT },
    { 705, "getContentPath", METHOD_TYPE_OBJECT },
    { 999, "finish", METHOD_TYPE_VOID }
};

MethodsObject methodsObject[] = {
    { 107, GetAppDataDirectory },
    { 108, GetExternalStorageDirectory },
    { 113, getVersion },
    { 220, ea_blast_MainActivity_GetInstance },
    { 221, android_content_Context_getAssets },
    { 301, ea_blast_SystemAndroidDelegate_IsBatteryStateAvailable },
    { 303, ea_blast_SystemAndroidDelegate_GetChipset },
    { 305, ea_blast_SystemAndroidDelegate_GetManufacturer },
    { 306, ea_blast_SystemAndroidDelegate_GetDeviceModel },
    { 307, ea_blast_SystemAndroidDelegate_GetDeviceName },
    { 308, ea_blast_SystemAndroidDelegate_GetPhoneNumber },
    { 309, ea_blast_SystemAndroidDelegate_GetDeviceSubscriberID },
    { 310, ea_blast_SystemAndroidDelegate_GetDeviceUniqueId },
    { 311, ea_blast_SystemAndroidDelegate_GetDisplayCount },
    { 313, ea_blast_SystemAndroidDelegate_GetLocationAvailable },
    { 315, ea_blast_SystemAndroidDelegate_GetApiLevel },
    { 316, ea_blast_SystemAndroidDelegate_GetPlatformRawName },
    { 317, ea_blast_SystemAndroidDelegate_GetPlatformStdName },
    { 318, ea_blast_SystemAndroidDelegate_GetPlatformVersion },
    { 319, ea_blast_SystemAndroidDelegate_GetPhysicalKeyboardCount },
    { 320, ea_blast_SystemAndroidDelegate_GetProcessorArchitecture },
    { 321, ea_blast_SystemAndroidDelegate_GetLanguage },
    { 322, ea_blast_SystemAndroidDelegate_GetLocale },
    { 323, ea_blast_SystemAndroidDelegate_GetTotalRAM },
    { 328, ea_blast_SystemAndroidDelegate_GetVirtualKeyboardCount },
    { 330, ea_blast_SystemAndroidDelegate_GetApplicationVersion },
    { 331, ea_blast_SystemAndroidDelegate_GetFirmware },
    { 332, ea_blast_SystemAndroidDelegate_GetHardwareFloatingPointSupport },
    { 401, dummyConstructor },
    { 402, dummyConstructor },
    { 403, dummyConstructor },
    { 404, dummyConstructor },
    { 405, dummyConstructor },
    { 406, dummyConstructor },
    { 407, dummyConstructor },
    { 408, dummyConstructor },
    { 409, dummyConstructor },
    { 602, InputStream_skip },
    { 603, InputStream_open },
    { 604, InputStream_openFd },
    { 605, InputStream_list },
    { 700, GetInternalStorageDirectory },
    { 701, GetPrimaryExternalStorageDirectory },
    { 702, GetPrimaryExternalStorageState },
    { 703, GetDedicatedDirectory },
    { 704, getPackage },
    { 705, getContentPath },
};

MethodsLong methodsLong[] = {
    { 606, InputStream_getLength },
    { 114, getTotalMemory }
};

MethodsInt methodsInt[] = {
    { 102, GetStdOrientation },
    { 103, GetDefaultWidth },
    { 104, GetDefaultHeight },
    { 118, EAAudioCore_AudioTrack_write },
    { 300, ea_blast_SystemAndroidDelegate_GetAccelerometerCount },
    { 302, ea_blast_SystemAndroidDelegate_GetCameraCount },
    { 304, ea_blast_SystemAndroidDelegate_GetCompassCount },
    { 312, ea_blast_SystemAndroidDelegate_GetGyroscopeCount },
    { 314, ea_blast_SystemAndroidDelegate_GetMicrophoneCount },
    { 324, ea_blast_SystemAndroidDelegate_GetTouchPadCount },
    { 325, ea_blast_SystemAndroidDelegate_GetTouchScreenCount },
    { 326, ea_blast_SystemAndroidDelegate_GetTrackBallCount },
    { 327, ea_blast_SystemAndroidDelegate_GetVibratorCount },
    { 329, ea_blast_SystemAndroidDelegate_GetApplicationVersionCode },
    { 600, InputStream_read },
};

MethodsFloat methodsFloat[] = {
    { 105, GetDpiX },
    { 106, GetDpiY },
};

MethodsVoid methodsVoid[] = {
    { 101, ApplyKeepAwake },
    { 109, SetStdOrientation },
    { 110, OnLifeCycleFocusGained },
    { 111, SetEnabled },
    { 115, SetUpdateFrequency },
    { 116, EAAudioCore_AudioTrack_play },
    { 117, EAAudioCore_AudioTrack_stop },
    { 219, ea_EAIO_Startup },
    { 601, InputStream_close },
    { 999, finish }
};

MethodsBoolean methodsBoolean[] = {
    { 100, isContentReady },
    { 112, IsTouchScreenMultiTouch },
    { 119, IsNavigationVisible },
    { 120, IsVisible }
};

NameToFieldID nameToFieldId[] = {
    { 8, "screenWidth",            FIELD_TYPE_INT },
    { 9, "screenHeight",           FIELD_TYPE_INT },
    { 10, "is_licensed",           FIELD_TYPE_BOOLEAN }
};

FieldsBoolean fieldsBoolean[] = {
    { 10, JNI_TRUE }
};

FieldsInt fieldsInt[] = {
    { 8, 960 },
    { 9, 544 },
};

FieldsObject fieldsObject[] = {};

MethodsByte methodsByte[] = {};
MethodsChar methodsChar[] = {};
MethodsDouble methodsDouble[] = {};
MethodsShort methodsShort[] = {};

FieldsByte fieldsByte[] = {};
FieldsChar fieldsChar[] = {};
FieldsDouble fieldsDouble[] = {};
FieldsFloat fieldsFloat[] = {};

FieldsLong fieldsLong[] = {};
FieldsShort fieldsShort[] = {};

__FALSOJNI_IMPL_CONTAINER_SIZES
