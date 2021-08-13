package com.askjeffreyliu.myapplication

import android.graphics.BitmapFactory
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.askjeffreyliu.floydsteinbergdithering.Utils
import com.askjeffreyliu.myapplication.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        val view = binding.root
        setContentView(view)

        // get the bitmap of the girl
        val original = BitmapFactory.decodeResource(resources, R.drawable.lena)

        // Example of a call to the library
        val fsBitmap = Utils().floydSteinbergDithering(original)
        binding.imageViewDither.setImageBitmap(fsBitmap)

        // Example of a call to the library
        val bwBitmap = Utils().binaryBlackAndWhite(original)
        binding.imageViewMono.setImageBitmap(bwBitmap)

        // Example of a call to the library, this is just a simple android gray-scale function
        val grayBitmap = Utils().toGrayscale(original)
        binding.imageViewGray.setImageBitmap(grayBitmap)
    }
}