#include "Blobs.hpp"

Blobs::Blobs() : NUMCORES(4)
{
    canny_th1 = 50;
    canny_th2 = 100;
    size_slider = 5;
}

Mat Blobs::addBlobToImg(Mat im, CBlob t2){

    Scalar mean, stddev;
        
    Rect bbox = t2.GetBoundingBox();
    rectangle(im,bbox,Scalar(0,220,0),1);

    return im;
}

CBlobResult Blobs::findBlobs(Mat im){
	Mat img, imgc, imgd;

	cvtColor(im,img,CV_BGR2GRAY);
    Mat st_elem = getStructuringElement(MORPH_RECT, Size(size_slider, size_slider));
    dilate(img, imgd, st_elem);

    Canny(imgd, imgc, canny_th1, canny_th2);

	return CBlobResult(imgc,Mat(),NUMCORES);
}

