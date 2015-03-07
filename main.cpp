#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <cmath>
#include <map>
#include <utility>
#include <math.h>

using namespace cv;
using namespace std;

int main(int argc, const char **argv)
{
    printf("helloworld");

    Mat mat;
    mat = imread("img.png", CV_LOAD_IMAGE_COLOR);

    namedWindow( "Display window", WINDOW_AUTOSIZE );
    imshow( "Display window", image );
}

