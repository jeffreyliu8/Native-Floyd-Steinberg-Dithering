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
#include <zlib.h>
#include <android/log.h>
#include <android/bitmap.h>
#include <cstdio>
#include <string>
#include <cstring>
#include <cstdlib>
#include <string>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <vector>

using std::string;
using std::stringstream;

#define  LOG_TAG    "libimageprocessing"

#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


unsigned char *as_unsigned_char_array(jbyteArray array, JNIEnv *env) {
    int len = env->GetArrayLength(array);
    auto *buf = new unsigned char[len];
    env->GetByteArrayRegion(array, 0, len, reinterpret_cast<jbyte *>(buf));
    return buf;
}

jbyteArray as_byte_array(unsigned char* buf, int len, JNIEnv *env) {
    jbyteArray array = env->NewByteArray (len);
    env->SetByteArrayRegion (array, 0, len, reinterpret_cast<jbyte*>(buf));
    return array;
}

// Found these here http://mail-archives.apache.org/mod_mbox/trafficserver-dev/201110.mbox/%3CCACJPjhYf=+br1W39vyazP=ix
//eQZ-4Gh9-U6TtiEdReG3S4ZZng@mail.gmail.com%3E
#define MOD_GZIP_ZLIB_WINDOWSIZE (-15)
#define MOD_GZIP_ZLIB_CFACTOR    9
#define MOD_GZIP_ZLIB_BSIZE      8096

// Found this one here: http://panthema.net/2007/0328-ZLibString.html, author is Timo Bingmann
// edited version
/** Compress a STL string using zlib with given compression level and return
  * the binary data. */
std::vector<char> compress_gzip(const std::string& str,
                          int compressionLevel = 5)
{
    z_stream zs;                        // z_stream is zlib's control structure
    memset(&zs, 0, sizeof(zs));

    if (deflateInit2(&zs,
                     compressionLevel,
                     Z_DEFLATED,
                     MOD_GZIP_ZLIB_WINDOWSIZE ,
                     MOD_GZIP_ZLIB_CFACTOR,
                     Z_DEFAULT_STRATEGY) != Z_OK
            ) {
        throw(std::runtime_error("deflateInit2 failed while compressing."));
    }

    zs.next_in = (Bytef*)str.data();
    zs.avail_in = str.size();           // set the z_stream's input

    int ret;
    char outbuffer[32768];
//    std::string outstring;
    std::vector<char> outstring;

    // retrieve the compressed bytes blockwise
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = deflate(&zs, Z_FINISH);

        if (outstring.size() < zs.total_out) {
            // append the block to the output string
//            outstring.append(outbuffer,
//                             zs.total_out - outstring.size());

            for (int i = 0; i <   zs.total_out - outstring.size(); i++)
            {
                outstring.push_back(outbuffer[i]);
            }
        }
    } while (ret == Z_OK);

    deflateEnd(&zs);

    if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
        std::ostringstream oss;
        oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
        throw(std::runtime_error(oss.str()));
    }

    __android_log_print(ANDROID_LOG_DEBUG, "jeff_jni", "total out is %lu",zs.total_out);

    return outstring;
}
//
//// Found this one here: http://panthema.net/2007/0328-ZLibString.html, author is Timo Bingmann
///** Compress a STL string using zlib with given compression level and return
//  * the binary data. */
//std::string compress_deflate(const std::string& str,
//                             int compressionlevel = Z_BEST_COMPRESSION)
//{
//    z_stream zs;                        // z_stream is zlib's control structure
//    memset(&zs, 0, sizeof(zs));
//
//    if (deflateInit(&zs, compressionlevel) != Z_OK)
//        throw(std::runtime_error("deflateInit failed while compressing."));
//
//    zs.next_in = (Bytef*)str.data();
//    zs.avail_in = str.size();           // set the z_stream's input
//
//    int ret;
//    char outbuffer[32768];
//    std::string outstring;
//
//    // retrieve the compressed bytes blockwise
//    do {
//        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
//        zs.avail_out = sizeof(outbuffer);
//
//        ret = deflate(&zs, Z_FINISH);
//
//        if (outstring.size() < zs.total_out) {
//            // append the block to the output string
//            outstring.append(outbuffer,
//                             zs.total_out - outstring.size());
//        }
//    } while (ret == Z_OK);
//
//    deflateEnd(&zs);
//
//    if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
//        std::ostringstream oss;
//        oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
//        throw(std::runtime_error(oss.str()));
//    }
//
//    return outstring;
//}
//
///** Decompress an STL string using zlib and return the original data. */
//std::string decompress_deflate(const std::string& str)
//{
//    z_stream zs;                        // z_stream is zlib's control structure
//    memset(&zs, 0, sizeof(zs));
//
//    if (inflateInit(&zs) != Z_OK)
//        throw(std::runtime_error("inflateInit failed while decompressing."));
//
//    zs.next_in = (Bytef*)str.data();
//    zs.avail_in = str.size();
//
//    int ret;
//    char outbuffer[32768];
//    std::string outstring;
//
//    // get the decompressed bytes blockwise using repeated calls to inflate
//    do {
//        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
//        zs.avail_out = sizeof(outbuffer);
//
//        ret = inflate(&zs, 0);
//
//        if (outstring.size() < zs.total_out) {
//            outstring.append(outbuffer,
//                             zs.total_out - outstring.size());
//        }
//
//    } while (ret == Z_OK);
//
//    inflateEnd(&zs);
//
//    if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
//        std::ostringstream oss;
//        oss << "Exception during zlib decompression: (" << ret << ") "
//            << zs.msg;
//        throw(std::runtime_error(oss.str()));
//    }
//
//    return outstring;
//}

std::string decompress_gzip(const char* str)
{
    z_stream zs;                        // z_stream is zlib's control structure
    memset(&zs, 0, sizeof(zs));

    if (inflateInit2(&zs, MOD_GZIP_ZLIB_WINDOWSIZE ) != Z_OK)
        throw(std::runtime_error("inflateInit failed while decompressing."));

    zs.next_in = (Bytef*)str;
    zs.avail_in = sizeof(str);

    __android_log_print(ANDROID_LOG_DEBUG, "jeff_jni", "avail_in is %u",zs.avail_in);

    int ret;
    char outbuffer[32768];
    std::string outstring;

    // get the decompressed bytes blockwise using repeated calls to inflate
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = inflate(&zs, 0);

        if (outstring.size() < zs.total_out) {
            outstring.append(outbuffer,
                             zs.total_out - outstring.size());
        }

    } while (ret == Z_OK);

    inflateEnd(&zs);

    if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
        std::ostringstream oss;
        oss << "Exception during zlib decompression: (" << ret << ") "
            << zs.msg;
        throw(std::runtime_error(oss.str()));
    }

    return outstring;
}

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
                                                                                    jclass obj,
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
                                                                               jclass obj,
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

// hope this would help
int compressToGzip(const char* input, int inputSize, char* output, int outputSize)
{
    z_stream zs;
    zs.zalloc = Z_NULL;
    zs.zfree = Z_NULL;
    zs.opaque = Z_NULL;
    zs.avail_in = (uInt)inputSize;
    zs.next_in = (Bytef *)input;
    zs.avail_out = (uInt)outputSize;
    zs.next_out = (Bytef *)output;

    // hard to believe they don't have a macro for gzip encoding, "Add 16" is the best thing zlib can do:
    // "Add 16 to windowBits to write a simple gzip header and trailer around the compressed data instead of a zlib wrapper"
    deflateInit2(&zs, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY);
    deflate(&zs, Z_FINISH);
    deflateEnd(&zs);
    return zs.total_out;
}

bool gzipInflate( const std::string& compressedBytes, std::string& uncompressedBytes ) {
    if ( compressedBytes.empty() ) {
        uncompressedBytes = compressedBytes ;
        return true ;
    }

    uncompressedBytes.clear() ;

    unsigned full_length = compressedBytes.size() ;
    unsigned half_length = compressedBytes.size() / 2;

    unsigned uncompLength = full_length ;
    char* uncomp = (char*) calloc( sizeof(char), uncompLength );

    z_stream strm;
    strm.next_in = (Bytef *) compressedBytes.c_str();
    strm.avail_in = compressedBytes.size() ;
    strm.total_out = 0;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;

    bool done = false ;

    if (inflateInit2(&strm, (16+MAX_WBITS)) != Z_OK) {
        free( uncomp );
        return false;
    }

    while (!done) {
        // If our output buffer is too small
        if (strm.total_out >= uncompLength ) {
            // Increase size of output buffer
            char* uncomp2 = (char*) calloc( sizeof(char), uncompLength + half_length );
            memcpy( uncomp2, uncomp, uncompLength );
            uncompLength += half_length ;
            free( uncomp );
            uncomp = uncomp2 ;
        }

        strm.next_out = (Bytef *) (uncomp + strm.total_out);
        strm.avail_out = uncompLength - strm.total_out;

        // Inflate another chunk.
        int err = inflate (&strm, Z_SYNC_FLUSH);
        if (err == Z_STREAM_END) done = true;
        else if (err != Z_OK)  {
            break;
        }
    }

    if (inflateEnd (&strm) != Z_OK) {
        free( uncomp );
        return false;
    }

    for ( size_t i=0; i<strm.total_out; ++i ) {
        uncompressedBytes += uncomp[ i ];
    }
    free( uncomp );
    return true ;
}

JNIEXPORT jbyteArray JNICALL
Java_com_askjeffreyliu_floydsteinbergdithering_Utils_deflateInit2(JNIEnv *env,
                                                                  jclass obj,
                                                                  jbyteArray array) {

    int len = env->GetArrayLength(array);
    const char *buf = new char[len];
    env->GetByteArrayRegion(array, 0, len, (jbyte *) buf);

    char *output = new char[len];
    int outputSize = len;

    int totalOut = compressToGzip(buf, len, output, outputSize);
    std::string data ;
    std::string fileData = output ;


    if (!gzipInflate(fileData, data)) {
        printf("Error decompressing file.");
        return nullptr;
    }


    const char * outputPtr = fileData.c_str();


//    jbyteArray ret = env->NewByteArray(totalOut);
//    env->SetByteArrayRegion(ret, 0, totalOut, reinterpret_cast<const jbyte *>(output));
//    return ret;

    std::string s =  buf;


    __android_log_print(ANDROID_LOG_DEBUG, "jeff_jni", "input : %s", s.c_str());

    std::vector<char> compressed = compress_gzip(s);
   char* ptr = reinterpret_cast<char*> (&compressed[0]);

//    __android_log_print(ANDROID_LOG_DEBUG, "jeff_jni", "gzip compressed: %s",
//                        compressed.c_str());

    __android_log_print(ANDROID_LOG_DEBUG, "jeff_jni", "gzip decompressed: %s",
                        decompress_gzip(ptr).c_str());

//    __android_log_print(ANDROID_LOG_DEBUG, "jeff_jni", "deflate compressed: %s",
//                        compress_deflate(s).c_str());
//
//    __android_log_print(ANDROID_LOG_DEBUG, "jeff_jni", "deflate decompressed: %s",
//                        decompress_deflate(compress_deflate(s)).c_str());


//    char const *compressedPtr = compressed.c_str();

    jbyteArray ret = env->NewByteArray(fileData.size());
    env->SetByteArrayRegion(ret, 0, fileData.size(), reinterpret_cast<const jbyte *>(outputPtr));
    return ret;
}


JNIEXPORT jbyteArray JNICALL
Java_com_askjeffreyliu_floydsteinbergdithering_Utils_inflateInit2(JNIEnv *env,
                                                                  jclass obj,
                                                                  jbyteArray array) {

    int len = env->GetArrayLength(array);
    const char *buf = new char[len];
    env->GetByteArrayRegion(array, 0, len, (jbyte *) buf);

    char *output = new char[len];
    int outputSize = len;

    int totalOut = compressToGzip(buf, len, output, outputSize);
    std::string data ;
    std::string fileData = output ;


    if (!gzipInflate(fileData, data)) {
        printf("Error decompressing file.");
        return nullptr;
    }


    const char * outputPtr = fileData.c_str();


//    jbyteArray ret = env->NewByteArray(totalOut);
//    env->SetByteArrayRegion(ret, 0, totalOut, reinterpret_cast<const jbyte *>(output));
//    return ret;

//    std::string s =   "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";
//
//
//    __android_log_print(ANDROID_LOG_DEBUG, "jeff_jni", "input : %s", s.c_str());
//
//    __android_log_print(ANDROID_LOG_DEBUG, "jeff_jni", "gzip compressed: %s",
//                        compress_gzip(s).c_str());
//
//    __android_log_print(ANDROID_LOG_DEBUG, "jeff_jni", "gzip decompressed: %s",
//                        decompress_gzip(compress_gzip(s)).c_str());

//    __android_log_print(ANDROID_LOG_DEBUG, "jeff_jni", "deflate compressed: %s",
//                        compress_deflate(s).c_str());
//
//    __android_log_print(ANDROID_LOG_DEBUG, "jeff_jni", "deflate decompressed: %s",
//                        decompress_deflate(compress_deflate(s)).c_str());



    jbyteArray ret = env->NewByteArray(fileData.size());
    env->SetByteArrayRegion(ret, 0, fileData.size(), reinterpret_cast<const jbyte *>(outputPtr));
    return ret;
}

#ifdef __cplusplus
}
#endif

