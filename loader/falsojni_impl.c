#pragma clang diagnostic ignored "-Wincompatible-function-pointer-types"
#pragma ide diagnostic ignored "UnusedParameter"
#pragma clang diagnostic ignored "-Wwritable-strings"

#include <FalsoJNI/FalsoJNI.h>
#include <FalsoJNI/FalsoJNI_Impl.h>

#include <pthread.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "android/java.io.InputStream.h"
#include "android/EAAudioCore.h"

NameToMethodID nameToMethodId[] = {
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

    // Class initializers
    { 401, "com/ea/blast/SystemAndroidDelegate/<init>", METHOD_TYPE_OBJECT },
    { 402, "com/ea/blast/PowerManagerAndroid/<init>", METHOD_TYPE_OBJECT },
    { 403, "com/ea/blast/DisplayAndroidDelegate/<init>", METHOD_TYPE_OBJECT },
    { 404, "com/ea/blast/DeviceOrientationHandlerAndroidDelegate/<init>", METHOD_TYPE_OBJECT },
    { 405, "com/ea/blast/GetAppDataDirectoryDelegate/<init>", METHOD_TYPE_OBJECT },
    { 406, "com/ea/EAMIO/StorageDirectory/<init>", METHOD_TYPE_OBJECT },
    { 407, "com/ea/blast/AccelerometerAndroidDelegate/<init>", METHOD_TYPE_OBJECT },
    { 408, "com/ea/blast/VirtualKeyboardAndroidDelegate/<init>", METHOD_TYPE_OBJECT },
    { 409, "com/ea/blast/PhysicalKeyboardAndroidDelegate/<init>", METHOD_TYPE_OBJECT },

    // AssetManager and java.io.InputStream
    { 600, "read", METHOD_TYPE_INT },
    { 601, "close", METHOD_TYPE_VOID },
    { 602, "skip", METHOD_TYPE_OBJECT },
    { 603, "open", METHOD_TYPE_OBJECT },
    { 604, "openFd", METHOD_TYPE_OBJECT },
    { 605, "list", METHOD_TYPE_OBJECT },
    { 606, "getLength", METHOD_TYPE_LONG },

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

    // EAAudioCore
    { 116, "play", METHOD_TYPE_VOID },
    { 117, "stop", METHOD_TYPE_VOID },
    { 118, "write", METHOD_TYPE_INT },

    { 119, "IsNavigationVisible", METHOD_TYPE_BOOLEAN },
    { 120, "IsVisible", METHOD_TYPE_BOOLEAN },

    { 700, "GetInternalStorageDirectory", METHOD_TYPE_OBJECT },
    { 701, "GetPrimaryExternalStorageDirectory", METHOD_TYPE_OBJECT },
    { 702, "GetPrimaryExternalStorageState", METHOD_TYPE_OBJECT },
    { 703, "GetDedicatedDirectory", METHOD_TYPE_OBJECT },
    { 704, "getPackage", METHOD_TYPE_OBJECT },
    { 705, "getContentPath", METHOD_TYPE_OBJECT },

    { 999, "finish", METHOD_TYPE_VOID }
};

extern void (*Java_com_ea_EAIO_EAIO_Startup)(JNIEnv*, void*, jobject);
// com/ea/EAIO/EAIO/Startup
void ea_EAIO_Startup(jmethodID id, va_list args) {
    void* assetManager = va_arg(args, void*);
    debugPrintf("JNI: Method Call: com/ea/EAIO/EAIO/Startup(AssetManager: 0x%x) / id: %i\n", (int)assetManager, id);

    Java_com_ea_EAIO_EAIO_Startup(&jni, NULL, assetManager);
}

// com/ea/blast/MainActivity/GetInstance
jobject ea_blast_MainActivity_GetInstance(jmethodID id, va_list args) {
    void* assetManager = va_arg(args, void*);
    debugPrintf("JNI: Method Call: com/ea/blast/MainActivity/GetInstance() / id: %i\n", id);
    return strdup("MainActivityInstance");
}

// 	com/android/content/Context/getAssets
jobject android_content_Context_getAssets(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/android/content/Context/getAssets() / id: %i\n", id);
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
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetAccelerometerCount() / id: %i\n", id);

    return 1;
}

// 	com/ea/blast/SystemAndroidDelegate/IsBatteryStateAvailable
jobject ea_blast_SystemAndroidDelegate_IsBatteryStateAvailable(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/IsBatteryStateAvailable() / id: %i\n", id);

    return (jobject)_string_0;
}

// 	com/ea/blast/SystemAndroidDelegate/GetCameraCount
jint ea_blast_SystemAndroidDelegate_GetCameraCount(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetCameraCount() / id: %i\n", id);

    return 0;
}

// 	com/ea/blast/SystemAndroidDelegate/GetChipset
jobject ea_blast_SystemAndroidDelegate_GetChipset(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetChipset() / id: %i\n", id);

    return (jobject)_string_0;
}

// 	com/ea/blast/SystemAndroidDelegate/GetCompassCount
jint ea_blast_SystemAndroidDelegate_GetCompassCount(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetCompassCount() / id: %i\n", id);

    return 0;
}

// 	com/ea/blast/SystemAndroidDelegate/GetManufacturer
jobject ea_blast_SystemAndroidDelegate_GetManufacturer(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetManufacturer() / id: %i\n", id);

    return (jobject)_string_manufacturer;
}

// 	com/ea/blast/SystemAndroidDelegate/GetDeviceModel
jobject ea_blast_SystemAndroidDelegate_GetDeviceModel(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetDeviceModel() / id: %i\n", id);

    return (jobject)_string_devicemodel;
}

// 	com/ea/blast/SystemAndroidDelegate/GetDeviceName
jobject ea_blast_SystemAndroidDelegate_GetDeviceName(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetDeviceName() / id: %i\n", id);

    return (jobject)_string_devicename;
}

// 	com/ea/blast/SystemAndroidDelegate/GetPhoneNumber
jobject ea_blast_SystemAndroidDelegate_GetPhoneNumber(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetPhoneNumber() / id: %i\n", id);

    return (jobject)_string_0;
}

// 	com/ea/blast/SystemAndroidDelegate/GetDeviceSubscriberID
jobject ea_blast_SystemAndroidDelegate_GetDeviceSubscriberID(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetDeviceSubscriberID() / id: %i\n", id);

    return (jobject)_string_0;
}

// 	com/ea/blast/SystemAndroidDelegate/GetDeviceUniqueId
jobject ea_blast_SystemAndroidDelegate_GetDeviceUniqueId(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetDeviceUniqueId() / id: %i\n", id);

    return (jobject)_string_minus1;
}

// 	com/ea/blast/SystemAndroidDelegate/GetDisplayCount
jobject ea_blast_SystemAndroidDelegate_GetDisplayCount(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetDisplayCount() / id: %i\n", id);

    return (jobject)_string_1;
}

// 	com/ea/blast/SystemAndroidDelegate/GetGyroscopeCount
jint ea_blast_SystemAndroidDelegate_GetGyroscopeCount(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetGyroscopeCount() / id: %i\n", id);

    return 1;
}

// 	com/ea/blast/SystemAndroidDelegate/GetLocationAvailable
jobject ea_blast_SystemAndroidDelegate_GetLocationAvailable(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetLocationAvailable() / id: %i\n", id);

    return (jobject)_string_true;
}

// 	com/ea/blast/SystemAndroidDelegate/GetMicrophoneCount
jint ea_blast_SystemAndroidDelegate_GetMicrophoneCount(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetMicrophoneCount() / id: %i\n", id);

    return 1;
}

// 	com/ea/blast/SystemAndroidDelegate/GetApiLevel
jobject ea_blast_SystemAndroidDelegate_GetApiLevel(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetApiLevel() / id: %i\n", id);

    return (jobject)_string_19;
}

// 	com/ea/blast/SystemAndroidDelegate/GetPlatformRawName
jobject ea_blast_SystemAndroidDelegate_GetPlatformRawName(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetPlatformRawName() / id: %i\n", id);

    return (jobject)_string_android;
}

// 	com/ea/blast/SystemAndroidDelegate/GetPlatformStdName
jobject ea_blast_SystemAndroidDelegate_GetPlatformStdName(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetPlatformStdName() / id: %i\n", id);

    return (jobject)_string_android;
}

// 	com/ea/blast/SystemAndroidDelegate/GetPlatformVersion
jobject ea_blast_SystemAndroidDelegate_GetPlatformVersion(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetPlatformVersion() / id: %i\n", id);

    return (jobject)_string_release;
}

// 	com/ea/blast/SystemAndroidDelegate/GetPhysicalKeyboardCount
jobject ea_blast_SystemAndroidDelegate_GetPhysicalKeyboardCount(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetPhysicalKeyboardCount() / id: %i\n", id);

    return (jobject)_string_1;
}

// 	com/ea/blast/SystemAndroidDelegate/GetProcessorArchitecture
jobject ea_blast_SystemAndroidDelegate_GetProcessorArchitecture(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetProcessorArchitecture() / id: %i\n", id);

    return (jobject)_string_cpuarch;
}

// 	com/ea/blast/SystemAndroidDelegate/GetLanguage
jobject ea_blast_SystemAndroidDelegate_GetLanguage(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetLanguage() / id: %i\n", id);
    // TODO: Check for system language
    return (jobject)_string_en;
}

// 	com/ea/blast/SystemAndroidDelegate/GetLocale
jobject ea_blast_SystemAndroidDelegate_GetLocale(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetLocale() / id: %i\n", id);
    // TODO: Check for system language
    return (jobject)_string_en;
}

// 	com/ea/blast/SystemAndroidDelegate/GetTotalRAM
jobject ea_blast_SystemAndroidDelegate_GetTotalRAM(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetTotalRAM() / id: %i\n", id);

    return (jobject)_string_minus1;
}

// 	com/ea/blast/SystemAndroidDelegate/GetTouchPadCount
jint ea_blast_SystemAndroidDelegate_GetTouchPadCount(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetTouchPadCount() / id: %i\n", id);

    return 1;
}

// 	com/ea/blast/SystemAndroidDelegate/GetTouchScreenCount
jint ea_blast_SystemAndroidDelegate_GetTouchScreenCount(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetTouchScreenCount() / id: %i\n", id);

    return 1;
}

// 	com/ea/blast/SystemAndroidDelegate/GetTrackBallCount
jint ea_blast_SystemAndroidDelegate_GetTrackBallCount(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetTrackBallCount() / id: %i\n", id);

    return 0;
}

// 	com/ea/blast/SystemAndroidDelegate/GetVibratorCount
jint ea_blast_SystemAndroidDelegate_GetVibratorCount(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetVibratorCount() / id: %i\n", id);
    // TODO: Support for DualShock vibrator?
    return 0;
}

// 	com/ea/blast/SystemAndroidDelegate/GetVibratorCount
jint ea_blast_SystemAndroidDelegate_GetApplicationVersionCode(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetApplicationVersionCode() / id: %i\n", id);
    return 42;
}

// 	com/ea/blast/SystemAndroidDelegate/GetVirtualKeyboardCount
jobject ea_blast_SystemAndroidDelegate_GetVirtualKeyboardCount(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetVirtualKeyboardCount() / id: %i\n", id);
    return (jobject)_string_1;
}

// 	com/ea/blast/SystemAndroidDelegate/GetApplicationVersionCode
jobject ea_blast_SystemAndroidDelegate_GetApplicationVersion(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetApplicationVersion() / id: %i\n", id);
    return (jobject)_string_1;
}
// 	com/ea/blast/SystemAndroidDelegate/GetHardwareFloatingPointSupport
jobject ea_blast_SystemAndroidDelegate_GetHardwareFloatingPointSupport(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetHardwareFloatingPointSupport() / id: %i\n", id);
    return (jobject)_string_true;
}

// 	com/ea/blast/SystemAndroidDelegate/GetFirmware
jobject ea_blast_SystemAndroidDelegate_GetFirmware(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: com/ea/blast/SystemAndroidDelegate/GetFirmware() / id: %i\n", id);
    return (jobject)_string_1;
}

// 	com/ea/blast/GetAppDataDirectoryDelegate/GetAppDataDirectory
jobject GetAppDataDirectory(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: GetAppDataDirectory() / id: %i\n", id);
    char * dir = DATA_PATH;
    return (jobject) strdup(dir);
}

// 	com/ea/blast/GetAppDataDirectoryDelegate/GetExternalStorageDirectory
jobject GetExternalStorageDirectory(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: GetExternalStorageDirectory() / id: %i\n", id);
    char * dir = DATA_PATH;
    return (jobject) strdup("nagaba");
}

jobject GetInternalStorageDirectory(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: GetInternalStorageDirectory() / id: %i\n", id);
    return (jobject) strdup("ux0:data/masseffect/assets/");
}

jobject GetPrimaryExternalStorageDirectory(jmethodID id, va_list args) {
    //debugPrintf("JNI: Method Call: GetPrimaryExternalStorageDirectory() / id: %i\n", id);
    return (jobject) strdup("ux0:data/masseffect/assets/");
}

jobject getContentPath(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: getContentPath() / id: %i\n", id);
    return (jobject) strdup("ux0:data/masseffect/assets/");
}

#define MEDIA_MOUNTED "mounted"

jobject GetPrimaryExternalStorageState(jmethodID id, va_list args) {
    //debugPrintf("JNI: Method Call: GetPrimaryExternalStorageState() / id: %i\n", id);
    return (jobject) strdup(MEDIA_MOUNTED);
}

jobject GetDedicatedDirectory(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: GetDedicatedDirectory() / id: %i\n", id);
    return (jobject) strdup("files");
}

jobject getPackage(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: getPackage() / id: %i\n", id);
    return (jobject) strdup("com.ea.games.meinfiltrator");
}


jobject dummyConstructor(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: <init>() / id: %i\n", id);
    // There might be an attempt to free this object; anyway, we can safely
    // leak a few bytes here.
    char * dummy = malloc(4);
    strcpy(dummy, "nop");
    return (jobject)dummy;
}

jboolean isContentReady(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: isContentReady() / id: %i\n", id);
    return JNI_TRUE;
}

jboolean IsTouchScreenMultiTouch(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: IsTouchScreenMultiTouch() / id: %i\n", id);
    return JNI_TRUE;
}

jboolean IsNavigationVisible(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: IsNavigationVisible() / id: %i\n", id);
    return JNI_FALSE;
}

jboolean IsVisible(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: IsVisible() / id: %i\n", id);
    return JNI_TRUE;
}

// com/eamobile/Query/getVersion
jobject getVersion(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: getVersion() / id: %i\n", id);
    return (jobject) strdup("1.0.1");
}

// com/ea/blast/PowerManagerAndroid/ApplyKeepAwake
void ApplyKeepAwake(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: ApplyKeepAwake() / id: %i\n", id);
    // TODO: Maybe it really is needed to keep the device awake here?
}

// com/ea/blast/DisplayAndroidDelegate.java
int GetStdOrientation(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: GetStdOrientation() / id: %i\n", id);
    // TODO: Maybe other values is needed? 0-3
    return 8;
    // 4 0 2 8 1
}

int GetDefaultWidth(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: GetDefaultWidth() / id: %i\n", id);
    return 960;
}

int GetDefaultHeight(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: GetDefaultHeight() / id: %i\n", id);

    return 544;
}

float GetDpiX(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: GetDpiX() / id: %i\n", id);
    return 200.0f;
}

float GetDpiY(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: GetDpiY() / id: %i\n", id);
    return 200.0f;
}

// com/ea/blast/DeviceOrientationHandlerAndroidDelegate/SetStdOrientation
void SetStdOrientation(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: SetStdOrientation() / id: %i\n", id);
    // We don't support changing orientation, ignore.
}

// com/ea/blast/DeviceOrientationHandlerAndroidDelegate/OnLifeCycleFocusGained
void OnLifeCycleFocusGained(jmethodID id, va_list args) {
    debugPrintf("JNI: Method Call: OnLifeCycleFocusGained() / id: %i\n", id);
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

//
void finish(jmethodID id, va_list args) {
    fprintf(stderr, "MainActivity.finish();");
    abort();
}

// com/eamobile/Query/getTotalMemory
jlong getTotalMemory(jmethodID id, va_list args) {
    return 256;
}

MethodsObject methodsObject[] = {
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
    // dummy constructor
    { 401, dummyConstructor },
    { 402, dummyConstructor },
    { 403, dummyConstructor },
    { 404, dummyConstructor },
    { 405, dummyConstructor },
    { 406, dummyConstructor },
    { 407, dummyConstructor },
    { 408, dummyConstructor },
    { 409, dummyConstructor },
    // fdf

    { 602, InputStream_skip },
    { 603, InputStream_open },
    { 604, InputStream_openFd },
    { 605, InputStream_list },
    // x
    { 107, GetAppDataDirectory },
    { 108, GetExternalStorageDirectory },
    { 113, getVersion },

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

    { 102, GetStdOrientation },
    { 103, GetDefaultWidth },
    { 104, GetDefaultHeight },
    { 600, InputStream_read },
    { 118, EAAudioCore_AudioTrack_write }
};

MethodsFloat methodsFloat[] = {
    { 105, GetDpiX },
    { 106, GetDpiY },
};

MethodsVoid methodsVoid[] = {
    { 219, ea_EAIO_Startup },
    { 101, ApplyKeepAwake },
    { 109, SetStdOrientation },
    { 110, OnLifeCycleFocusGained },
    { 111, SetEnabled },
    { 601, InputStream_close },
    { 115, SetUpdateFrequency },
    { 116, EAAudioCore_AudioTrack_play },
    { 117, EAAudioCore_AudioTrack_stop },
    { 999, finish }
};

MethodsBoolean methodsBoolean[] = {
    { 100, isContentReady },
    { 112, IsTouchScreenMultiTouch },
    { 119, IsNavigationVisible },
    { 120, IsVisible }
};

NameToFieldID nameToFieldId[] = {
    { 1, "WINDOW_SERVICE",         FIELD_TYPE_STRING },
    { 2, "gamepadAxisIndices",     FIELD_TYPE_INT_ARRAY },
    { 3, "gamepadAxisMinVals",     FIELD_TYPE_INT_ARRAY },
    { 4, "gamepadAxisMaxVals",     FIELD_TYPE_INT_ARRAY },
    { 5, "gamepadButtonIndices",   FIELD_TYPE_INT_ARRAY },
    { 6, "main_obb_mounted_path",  FIELD_TYPE_STRING },
    { 7, "patch_obb_mounted_path", FIELD_TYPE_STRING },
    { 8, "screenWidth", FIELD_TYPE_INT },
    { 9, "screenHeight", FIELD_TYPE_INT },
    { 10, "is_licensed", FIELD_TYPE_BOOLEAN }
};

FieldsBoolean fieldsBoolean[] = {
    { 10, JNI_TRUE }
};

FieldsInt fieldsInt[] = {
    { 8, 960 },
    { 9, 544 },
};

FieldsString fieldsString[] = {
    { 1, "window_service_field_val" },
    { 6, "ux0:/data/warband/main_unpacked" }, // no trailing slash!
    { 7, "ux0:/data/warband/patch_unpacked" }, // no trailing slash!
};

int _fieldIntArray2_value[] = {0, 1, 2, 3, 4, 5 };
int _fieldIntArray3_value[] = {0, 0, 0, 0, 0, 0 };
int _fieldIntArray4_value[] = {127, 127, 127, 127, 127, 127 };
int _fieldIntArray5_value[] = {188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 96, 97, 98, 99, 100, 101, 102, 104, 103, 105, 108, 109, 110, 3, 19, 20, 21, 22, 23};
FieldsIntArray fieldsIntArray[] = {
    { 2, _fieldIntArray2_value, sizeof(_fieldIntArray2_value) / sizeof (int) },
    { 3, _fieldIntArray3_value, sizeof(_fieldIntArray3_value) / sizeof (int) },
    { 4, _fieldIntArray4_value, sizeof(_fieldIntArray4_value) / sizeof (int) },
    { 5, _fieldIntArray5_value, sizeof(_fieldIntArray5_value) / sizeof (int) },
};

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