#ifndef BLOBS_HPP
#define BLOBS_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "blob.h"
#include "BlobResult.h"
#include "Utils.hpp"

using namespace std;
using namespace cv;

class Blobs{
    private:
        const int NUMCORES; 
        CBlobResult res;
    public:
        int canny_th1;
        int canny_th2;
        int size_slider;
        int grados;

        Blobs(); 
        Mat addBlobToImg(Mat im, CBlob t2);
        CBlobResult findBlobs(Mat im);
};

#endif
