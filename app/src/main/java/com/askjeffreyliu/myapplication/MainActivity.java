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

        Bitmap original = BitmapFactory.decodeResource(getResources(), R.drawable.lena);
        Bitmap fsBitmap = Utils.floydSteinbergDithering(original);

        // Example of a call to a native method
        ImageView fsImageView = (ImageView) findViewById(R.id.imageViewDither);
        fsImageView.setImageBitmap(fsBitmap);


        Bitmap bwBitmap = Utils.binaryBlackAndWhite(original);

        // Example of a call to a native method
        ImageView monoImageView = (ImageView) findViewById(R.id.imageViewMono);
        monoImageView.setImageBitmap(bwBitmap);
    }
}