package com.askjeffreyliu.myapplication

import android.graphics.BitmapFactory
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.askjeffreyliu.floydsteinbergdithering.Utils
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)


        // get the bitmap of the girl
        val original = BitmapFactory.decodeResource(resources, R.drawable.lena)

        // Example of a call to the library

        // Example of a call to the library
        val fsBitmap = Utils.floydSteinbergDithering(original)
        imageViewDither.setImageBitmap(fsBitmap)

        // Example of a call to the library

        // Example of a call to the library
        val bwBitmap = Utils.binaryBlackAndWhite(original)
        imageViewMono.setImageBitmap(bwBitmap)

        // Example of a call to the library, this is just a simple android gray-scale function

        // Example of a call to the library, this is just a simple android gray-scale function
        val grayBitmap = Utils.toGrayscale(original)
        imageViewGray.setImageBitmap(grayBitmap)
    }
}