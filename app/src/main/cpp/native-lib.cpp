#include <jni.h>
#include <string>
#include <fstream>
#include "libleaktracer/include/MemoryTrace.hpp"

#ifdef ANDROID

#include <android/log.h>

#define TAG "NDKLeak"

#define ALOGE(fmt, ...) __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##__VA_ARGS__)
#define ALOGI(fmt, ...) __android_log_print(ANDROID_LOG_INFO, TAG, fmt, ##__VA_ARGS__)
#define ALOGD(fmt, ...) __android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, ##__VA_ARGS__)
#define ALOGW(fmt, ...) __android_log_print(ANDROID_LOG_WARN, TAG, fmt, ##__VA_ARGS__)
#else
#define ALOGE printf
#define ALOGI printf
#define ALOGD printf
#define ALOGW printf
#endif
#ifdef __cplusplus
extern "C"{
#endif
void mem_leak(void);
#ifdef __cplusplus
};
#endif

class MemoryTest {

};

extern "C" JNIEXPORT jstring
JNICALL
Java_com_jarvis_leaktracer_sample_MainActivity_NativeLeak(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    leaktracer::MemoryTrace::GetInstance().startMonitoringAllThreads();
    //mem_leak();
    MemoryTest *memoryTest = new MemoryTest;
    //delete memoryTest;
    leaktracer::MemoryTrace::GetInstance().stopAllMonitoring();


    //leaktracer::MemoryTrace::GetInstance().writeLeaksToFile("/sdcard/leak.out");
    std::ofstream out;
    out.open("/sdcard/leaks.out", std::ios_base::out);
    if (out.is_open()) {
        leaktracer::MemoryTrace::GetInstance().writeLeaks(out);
        ALOGI("Success to write to \"leaks.out\"\n");
    } else {
        ALOGI("Failed to write to \"leaks.out\"\n");
    }



    return env->NewStringUTF(hello.c_str());
}
