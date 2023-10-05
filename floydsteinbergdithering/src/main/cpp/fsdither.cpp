//#include <jni.h>
//#include <string>
//
//extern "C"
//jstring
//Java_com_example_jeffreyliu_testcpp_MainActivity_stringFromJNI(
//        JNIEnv* env,
//        jobject /* this */) {
//    std::string hello = "Hello from C++";
//    return env->NewStringUTF(hello.c_str());
//}




#include <jni.h>

#include <android/log.h>
#include <android/bitmap.h>


#define  LOG_TAG    "libimageprocessing"

#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static uint32_t find_closest_palette_color(int value) {
    if (value < 128) {
        return 0xFF000000;
    }
    return 0xFFFFFFFF;
}

static void floydSteinberg(AndroidBitmapInfo *info, void *pixels) {
    int x, y, w = info->width, h = info->height;
    uint32_t *line;

    void *pixels2 = pixels;

    auto **d = new uint32_t *[info->height];
    for (int i = 0; i < info->height; ++i) {
        d[i] = new uint32_t[info->width];
    }

    for (y = 0; y < info->height; y++) {
        line = (uint32_t *) pixels2;
        for (x = 0; x < info->width; x++) {

            //extract the RGB values from the pixel
            d[y][x] = 0xFF000000 | (line[x] & 0x00FFFFFFu);
        }

        pixels2 = (char *) pixels2 + info->stride;
    }


    for (y = 0; y < info->height; y++) {
        line = (uint32_t *) pixels;
        for (x = 0; x < info->width; x++) {

            uint32_t oldpixel = d[y][x];
            uint32_t newpixel = find_closest_palette_color((int) oldpixel);

            // set the new pixel back in
            line[x] = newpixel;

            int err = (int) (oldpixel - newpixel);

            if (x + 1 < w)
                d[y][x + 1] = d[y][x + 1] + (int) (err * (7. / 16));
            if (x - 1 >= 0 && y + 1 < h)
                d[y + 1][x - 1] = d[y + 1][x - 1] + (int) (err * (3. / 16));
            if (y + 1 < h)
                d[y + 1][x] = d[y + 1][x] + (int) (err * (5. / 16));
            if (x + 1 < w && y + 1 < h)
                d[y + 1][x + 1] = d[y + 1][x + 1] + (int) (err * (1. / 16));
        }

        pixels = (char *) pixels + info->stride;
    }

    //Free each sub-array
    for (int i = 0; i < info->height; ++i) {
        delete[] d[i];
    }
    //Free the array of pointers
    delete[] d;
}

static void global_mono(AndroidBitmapInfo *info, void *pixels) {
    int x, y;
    uint32_t *line;
    for (y = 0; y < info->height; y++) {
        line = (uint32_t *) pixels;
        for (x = 0; x < info->width; x++) {
            line[x] = find_closest_palette_color((int) (line[x] & 0xFFu));
        }

        pixels = (char *) pixels + info->stride;
    }
}


#ifdef __cplusplus
extern "C" {
#endif

void Java_com_askjeffreyliu_floydsteinbergdithering_Utils_binaryBlackAndWhiteNative(JNIEnv *env,
                                                                                    jobject obj,
                                                                                    jobject bitmap) {
    AndroidBitmapInfo info;
    int ret;
    void *pixels;

    if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }
    if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap format is not RGBA_8888 !");
        return;
    }

    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }

    global_mono(&info, pixels);

    AndroidBitmap_unlockPixels(env, bitmap);
}

void Java_com_askjeffreyliu_floydsteinbergdithering_Utils_floydSteinbergNative(JNIEnv *env,
                                                                               jobject obj,
                                                                               jobject bitmap) {
    AndroidBitmapInfo info;
    int ret;
    void *pixels;

    if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }
    if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap format is not RGBA_8888 !");
        return;
    }

    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }

    floydSteinberg(&info, pixels);

    AndroidBitmap_unlockPixels(env, bitmap);
}

#ifdef __cplusplus
}
#endif


static JavaVM *jvm = nullptr;
jclass store_listener = nullptr;

extern "C"
JNIEXPORT void JNICALL
Java_com_askjeffreyliu_floydsteinbergdithering_Utils_addListener(JNIEnv *env, jobject thiz,
                                                                 jobject jnicallback) {

    env->GetJavaVM(&jvm);
    store_listener = static_cast<jclass>(env->NewGlobalRef(jnicallback));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_askjeffreyliu_floydsteinbergdithering_Utils_changeValue(JNIEnv *env, jobject thiz,
                                                                 jstring value, jint int_data,
                                                                 jbyteArray byte_array) {
    //https://www.youtube.com/watch?v=jEQSaCvW8eo&ab_channel=Uptechniques
    if (store_listener != nullptr) {
        jclass clazz = env->GetObjectClass(store_listener);
        jmethodID store_method = env->GetMethodID(clazz, "callback",
                                                  "(Ljava/lang/String;I[B)V");
//        jstring jstr = env->NewStringUTF("Some callback");
        env->CallVoidMethod(store_listener, store_method, value, int_data, byte_array);
    }
}