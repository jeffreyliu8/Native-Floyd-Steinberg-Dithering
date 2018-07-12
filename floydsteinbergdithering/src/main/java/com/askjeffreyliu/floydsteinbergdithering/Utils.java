package com.askjeffreyliu.floydsteinbergdithering;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.ColorMatrix;
import android.graphics.ColorMatrixColorFilter;
import android.graphics.Paint;

/**
 * Created by Jeffrey Liu on 3/6/17.
 */

public class Utils {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("fsdither");
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    private static native void floydSteinbergNative(Bitmap bmp);


    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    private static native void binaryBlackAndWhiteNative(Bitmap bmp);

    public static Bitmap floydSteinbergDithering(Bitmap originalColorBitmap) {
        Bitmap bmpGrayscaled = toGrayscale(originalColorBitmap);
        floydSteinbergNative(bmpGrayscaled);
        return bmpGrayscaled;
    }

    public static Bitmap binaryBlackAndWhite(Bitmap originalColorBitmap) {
        Bitmap bmpGrayscaled = toGrayscale(originalColorBitmap);
        binaryBlackAndWhiteNative(bmpGrayscaled);
        return bmpGrayscaled;
    }

    public static Bitmap toGrayscale(Bitmap bmpOriginal) {
        int width, height;
        height = bmpOriginal.getHeight();
        width = bmpOriginal.getWidth();

        Bitmap bmpGrayscale = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
        Canvas c = new Canvas(bmpGrayscale);
        Paint paint = new Paint();
        ColorMatrix cm = new ColorMatrix();
        cm.setSaturation(0);
        ColorMatrixColorFilter f = new ColorMatrixColorFilter(cm);
        paint.setColorFilter(f);
        c.drawBitmap(bmpOriginal, 0, 0, paint);
        return bmpGrayscale;
    }
}
