[![License](https://img.shields.io/badge/license-Apache%202-blue.svg)](https://www.apache.org/licenses/LICENSE-2.0)
[![](https://jitpack.io/v/jeffreyliu8/Native-Floyd-Steinberg-Dithering.svg)](https://jitpack.io/#jeffreyliu8/Native-Floyd-Steinberg-Dithering)

# Native-Floyd-Steinberg-Dithering
Android library for floyd-steinberg dithering on bitmap using JNI/Cmake

Why FSD
----------------
For those who don't know the purpose of Floyd-Steinberg algorithm, it is a algorithm to turn color image to black and white(NOT GRAYSCALE). 

When would we use this? Well, for those e-ink displays like Amazon Kindle, they can't display color images, and just black and white would still look bad, this is when this algorithm is usful.

I have tried using just java to run the algorithm, but it is slow. Not a good idea to run on main thread. 
This library is small and easy to use.

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
	        compile 'com.github.jeffreyliu8:Native-Floyd-Steinberg-Dithering:0.0.5'
	}
```

A quick example is shown below:

```java
Bitmap original = BitmapFactory.decodeResource(getResources(), R.drawable.lena);
Bitmap fsBitmap = Utils.floydSteinbergDithering(original);
```

And for just black and white image:
```java
Bitmap bwBitmap = com.askjeffreyliu.floydsteinbergdithering.Utils.binaryBlackAndWhite(original);
```

And for gray-scale image(This is just a simple java function):
```java
Bitmap grayBitmap = com.askjeffreyliu.floydsteinbergdithering.Utils.toGrayscale(original);
```

![Output sample](https://github.com/jeffreyliu8/Native-Floyd-Steinberg-Dithering/blob/master/screenshot.png)

Requirements
--------------
Requires a minimum SDK version of 14

Developed By
-------
Jeffrey Liu - <jeffreyliu8@gmail.com>

<a href="https://www.linkedin.com/in/jeffrey-liu-08a0b936">

License
-------

    Copyright 2018 Jeffrey Liu

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
