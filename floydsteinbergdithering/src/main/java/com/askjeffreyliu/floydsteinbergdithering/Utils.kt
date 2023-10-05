package com.askjeffreyliu.floydsteinbergdithering

import android.graphics.Bitmap
import android.graphics.Canvas
import android.graphics.ColorMatrix
import android.graphics.ColorMatrixColorFilter
import android.graphics.Paint

class Utils {
    init {
        System.loadLibrary("fsdither")
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    private external fun floydSteinbergNative(bmp: Bitmap)


    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    private external fun binaryBlackAndWhiteNative(bmp: Bitmap)

    fun floydSteinbergDithering(originalColorBitmap: Bitmap): Bitmap {
        val bmpGrayScaled = toGrayscale(originalColorBitmap)
        floydSteinbergNative(bmpGrayScaled)
        return bmpGrayScaled
    }

    fun binaryBlackAndWhite(originalColorBitmap: Bitmap): Bitmap {
        val bmpGrayScaled = toGrayscale(originalColorBitmap)
        binaryBlackAndWhiteNative(bmpGrayScaled)
        return bmpGrayScaled
    }

    fun toGrayscale(bmpOriginal: Bitmap): Bitmap {
        val bmpGrayscale =
            Bitmap.createBitmap(bmpOriginal.width, bmpOriginal.height, Bitmap.Config.ARGB_8888)
        val c = Canvas(bmpGrayscale)
        val paint = Paint()
        val cm = ColorMatrix()
        cm.setSaturation(0f)
        val f = ColorMatrixColorFilter(cm)
        paint.colorFilter = f
        c.drawBitmap(bmpOriginal, 0f, 0f, paint)
        return bmpGrayscale
    }

    external fun addListener(jniCallback: JniCallback)
    external fun changeValue(stringData: String, intData: Int, byteArray: ByteArray)
}

interface JniCallback {
    fun callback(data: String, data2: Int, data3: ByteArray)
}