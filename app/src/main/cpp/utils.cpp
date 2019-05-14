//
// Created by 王涛 on 2019-05-14.
//

#include "common.h"
#include <android/bitmap.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/core/mat.hpp"
#include "common.h"
#include "opencv2/core/hal/interface.h"
#include "opencv2/imgproc.hpp"

using namespace cv;

void BitmapToMat2(JNIEnv *env, jobject &bitmap, Mat *mat, jboolean needUnPremultiplyAlpha) {
    AndroidBitmapInfo info;
    void *pixels = 0;
    Mat &dst = *mat;

    LOGD("nBitmapToMat");
    CV_Assert(AndroidBitmap_getInfo(env, bitmap, &info) >= 0);
    CV_Assert(info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ||
              info.format == ANDROID_BITMAP_FORMAT_RGB_565);
    CV_Assert(AndroidBitmap_lockPixels(env, bitmap, &pixels) >= 0);
    CV_Assert(pixels);
    dst.create(info.height, info.width, CV_8UC4);
    if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGD("nBitmapToMat: RGBA_8888 -> CV_8UC4");
        Mat tmp(info.height, info.width, CV_8UC4, pixels);
        if (needUnPremultiplyAlpha) cvtColor(tmp, dst, COLOR_mRGBA2RGBA);
        else tmp.copyTo(dst);
    } else {
        // info.format == ANDROID_BITMAP_FORMAT_RGB_565
        LOGD("nBitmapToMat: RGB_565 -> CV_8UC4");
        Mat tmp(info.height, info.width, CV_8UC2, pixels);
        cvtColor(tmp, dst, COLOR_BGR5652RGBA);
    }
    AndroidBitmap_unlockPixels(env, bitmap);
}

void BitmapToMat(JNIEnv *env, jobject &bitmap, Mat *mat) {
    BitmapToMat2(env, bitmap, mat, false);
}

void MatToBitmap2
        (JNIEnv *env, Mat *mat, jobject &bitmap, jboolean needPremultiplyAlpha) {
    AndroidBitmapInfo info;
    void *pixels = 0;
    Mat &src = *mat;

    LOGD("nMatToBitmap");
    CV_Assert(AndroidBitmap_getInfo(env, bitmap, &info) >= 0);
    CV_Assert(info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ||
              info.format == ANDROID_BITMAP_FORMAT_RGB_565);
    CV_Assert(src.dims == 2 && info.height == (uint32_t) src.rows &&
              info.width == (uint32_t) src.cols);
    CV_Assert(src.type() == CV_8UC1 || src.type() == CV_8UC3 || src.type() == CV_8UC4);
    CV_Assert(AndroidBitmap_lockPixels(env, bitmap, &pixels) >= 0);
    CV_Assert(pixels);
    if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        Mat tmp(info.height, info.width, CV_8UC4, pixels);
        if (src.type() == CV_8UC1) {
            LOGD("nMatToBitmap: CV_8UC1 -> RGBA_8888");
            cvtColor(src, tmp, COLOR_GRAY2RGBA);
        } else if (src.type() == CV_8UC3) {
            LOGD("nMatToBitmap: CV_8UC3 -> RGBA_8888");
            cvtColor(src, tmp, COLOR_RGB2RGBA);
        } else if (src.type() == CV_8UC4) {
            LOGD("nMatToBitmap: CV_8UC4 -> RGBA_8888");
            if (needPremultiplyAlpha)
                cvtColor(src, tmp, COLOR_RGBA2mRGBA);
            else
                src.copyTo(tmp);
        }
    } else {
        // info.format == ANDROID_BITMAP_FORMAT_RGB_565
        Mat tmp(info.height, info.width, CV_8UC2, pixels);
        if (src.type() == CV_8UC1) {
            LOGD("nMatToBitmap: CV_8UC1 -> RGB_565");
            cvtColor(src, tmp, COLOR_GRAY2BGR565);
        } else if (src.type() == CV_8UC3) {
            LOGD("nMatToBitmap: CV_8UC3 -> RGB_565");
            cvtColor(src, tmp, COLOR_RGB2BGR565);
        } else if (src.type() == CV_8UC4) {
            LOGD("nMatToBitmap: CV_8UC4 -> RGB_565");
            cvtColor(src, tmp, COLOR_RGBA2BGR565);
        }
    }
    AndroidBitmap_unlockPixels(env, bitmap);
}

void MatToBitmap(JNIEnv *env, Mat *mat, jobject &bitmap) {
    MatToBitmap2(env, mat, bitmap, false);
}

jobject createBitmap(JNIEnv *env, Mat mat, jobject config) {

}