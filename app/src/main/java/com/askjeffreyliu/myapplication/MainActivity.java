package com.askjeffreyliu.myapplication;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.widget.ImageView;

import com.askjeffreyliu.floydsteinbergdithering.Utils;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        ImageView fsImageView = (ImageView) findViewById(R.id.imageViewDither);
        ImageView monoImageView = (ImageView) findViewById(R.id.imageViewMono);
        ImageView grayImageView = (ImageView) findViewById(R.id.imageViewGray);

        // get the bitmap of the girl
        Bitmap original = BitmapFactory.decodeResource(getResources(), R.drawable.lena);

        // Example of a call to the library
        Bitmap fsBitmap = Utils.floydSteinbergDithering(original);
        fsImageView.setImageBitmap(fsBitmap);

        // Example of a call to the library
        Bitmap bwBitmap = Utils.binaryBlackAndWhite(original);
        monoImageView.setImageBitmap(bwBitmap);

        // Example of a call to the library, this is just a simple android gray-scale function
        Bitmap grayBitmap = Utils.toGrayscale(original);
        grayImageView.setImageBitmap(grayBitmap);
    }
}