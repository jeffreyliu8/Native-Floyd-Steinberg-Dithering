[![License](https://img.shields.io/badge/license-Apache%202-blue.svg)](https://www.apache.org/licenses/LICENSE-2.0)

# Native-Floyd-Steinberg-Dithering
Android library for native floyd steinberg dithering on bitmap using JNI

Why NFSD
----------------
I have tried using just java to run the algorithm, but it is slow. Not a good idea to run on main thread. 
With NFSD, you can use Android Studio with instand run and still run these image filters with C++. It is small and easy to use.

Using NFSD
----------------

### Setup
In your app module build.gradle(before jcenter is ready)
```groovy
repositories {
    maven {
        url  "http://dl.bintray.com/jeffreyliu8/JeffLiuFloydSteinbergDithering"
    }
}
```


##### Dependencies
```groovy
dependencies {
    compile 'com.github.jeffreyliu8.nfsd:floydsteinbergdithering:0.0.2@aar'
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

![Output sample](https://github.com/jeffreyliu8/Native-Floyd-Steinberg-Dithering/blob/master/screenshot.png)

Requirements
--------------
Android 2.3, requires a minimum SDK version of 9

Developed By
-------
Jeffrey Liu - <jeffreyliu8@gmail.com>

<a href="https://www.linkedin.com/in/jeffrey-liu-08a0b936">

License
-------

    Copyright 2017 Jeffrey Liu

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
