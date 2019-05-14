#include <jni.h>
#include <string>
#include <opencv2/opencv.hpp>
#include "opencv2/core/mat.hpp"
#include "utils.cpp"

using namespace std;
using namespace cv;

extern "C" JNIEXPORT jstring JNICALL
Java_com_wangtao_opencv_1idcard_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_wangtao_opencv_1idcard_MainActivity_findIdNumber(JNIEnv *env, jobject instance,
                                                          jobject bitmap, jobject argb8888) {
    //原图
    Mat src_img;
    Mat dst;
    Mat dst_img;
    //结果图
    jobject result;

    //Bitmap转Mat
    BitmapToMat(env, bitmap, &src_img);
    //归一化
//    resize(src_img, src_img, FIX_IDCARD_SIZE);
    //灰度化
    cvtColor(src_img, dst, COLOR_RGB2GRAY);
    //二值化
    threshold(src_img, dst, 100, 255, THRESH_BINARY);
    //膨胀
    Mat erodeElement = getStructuringElement(MORPH_RECT, Size(20, 10));
    erode(dst, dst, erodeElement);
    //轮廓检测
    vector<vector<Point>> contours;
    vector<Rect> rects;
    findContours(dst, contours, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
    for (int i = 0; i < contours.size(); i++) {
        //获取到矩形区域
        Rect rect = boundingRect(contours.at(i));
        //绘制
        rectangle(dst, rect, Scalar(0, 0, 255));
        if (rect.width > rect.height * 8 && rect.width < rect.height * 16) {
            rectangle(dst, rect, Scalar(0, 0, 255));
            rects.push_back(rect);
        }
    }
    //查找坐标最低 矩形区域
    int lowPoint = 0;
    Rect finalRect;
    for (int i = 0; i < rects.size(); i++) {
        Rect rect = rects.at(i);
        Point point = rect.tl();
        if (point.y > lowPoint) {
            lowPoint = point.y;
            finalRect = rect;
        }
    }
    //图像切割
    dst_img = src_img(finalRect);
//    rectangle(dst, finalRect, Scalar(0, 0, 255));

    //Mat转Bitmap
    return createBitmap(env, dst_img, argb8888);
}