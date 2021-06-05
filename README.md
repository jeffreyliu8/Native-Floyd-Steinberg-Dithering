[![License](https://img.shields.io/badge/license-Apache%202-blue.svg)](https://www.apache.org/licenses/LICENSE-2.0)
[![](https://jitpack.io/v/jeffreyliu8/Native-Floyd-Steinberg-Dithering.svg)](https://jitpack.io/#jeffreyliu8/Native-Floyd-Steinberg-Dithering)

[![](https://www.buymeacoffee.com/assets/img/custom_images/orange_img.png)](https://www.buymeacoffee.com/jeffliu)

# Native-Floyd-Steinberg-Dithering
Android library for floyd-steinberg dithering on bitmap using JNI/Cmake

Why FSD
----------------
For those who don't know the purpose of Floyd-Steinberg algorithm, it is a algorithm to turn color image to black and white(NOT GRAYSCALE). 

When would we use this? Well, for those e-ink displays like Amazon Kindle, they can't display color images, and just black and white would still look bad, this is when this algorithm is usful.

I have tried using just java to run the algorithm, but it is slow.
This library is small and easy to use. Ideally you should NOT run this not on the main thread.

Using FSD
----------------

### Setup
```groovy
	allprojects {
		repositories {
			...
			maven { url 'https://jitpack.io' }
		}
	}
```


##### Dependencies
```groovy
	dependencies {
	        compile 'com.github.jeffreyliu8:Native-Floyd-Steinberg-Dithering:1.0.1'
	}
```

A quick example is shown below:

```kotlin
val original = BitmapFactory.decodeResource(resources, R.drawable.lena)
val fsBitmap = Utils.floydSteinbergDithering(original)
```

And for just black and white image:
```kotlin
val bwBitmap = Utils.binaryBlackAndWhite(original)
```

And for gray-scale image(This is just a simple java function):
```kotlin
val grayBitmap = Utils.toGrayscale(original)
```

![Output sample](https://github.com/jeffreyliu8/Native-Floyd-Steinberg-Dithering/blob/master/screenshot.png)

Requirements
--------------
Requires a minimum SDK version of 14
