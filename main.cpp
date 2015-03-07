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

string base = "jamesimages/";


vector<string> images =
{
    "smile.jpg",
    "smile_super.jpg",
    "sunglasses.jpg",
    "tongue.jpg",
    "tongue_wink.jpg",
    "wink_smile.jpg",
    "wink_smile_super.jpg"
};

#define MOUTH_BOX_WIDTH 200
#define MOUTH_BOX_HEIGHT 160
#define MOUTH_BOX_X 400
#define MOUTH_BOX_Y 780

#define MOUTH_EYE_WIDTH 130
#define MOUTH_EYE_HEIGHT 90
#define MOUTH_EYE_LEFT_X 590
#define MOUTH_EYE_LEFT_Y 330
#define MOUTH_EYE_RIGHT_X 545
#define MOUTH_EYE_RIGHT_Y 580

void GetReccct(bounding booooooooom) 
{

}

int main(int argc, const char **argv)
{
    printf("helloworld");

    for(auto& i : images)
    {
        i = base + i;
    }

    Mat mat;
    mat = imread(images[3], CV_LOAD_IMAGE_COLOR);

    if(!mat.data)
    {
        cout << "whoops";
        exit(1);
    }

    //mouth bounding box:
    rectangle(mat, Point(MOUTH_BOX_X,MOUTH_BOX_Y), Point(MOUTH_BOX_X+MOUTH_BOX_WIDTH, MOUTH_BOX_Y+MOUTH_BOX_HEIGHT),Scalar(0,0,0) ,2,8,0);

    //eyes bounding box:
    rectangle(mat, Point(MOUTH_BOX_X,MOUTH_BOX_Y), Point(MOUTH_BOX_X+MOUTH_BOX_WIDTH, MOUTH_BOX_Y+MOUTH_BOX_HEIGHT),Scalar(0,0,0) ,2,8,0);
    rectangle(mat, Point(MOUTH_BOX_X,MOUTH_BOX_Y), Point(MOUTH_BOX_X+MOUTH_BOX_WIDTH, MOUTH_BOX_Y+MOUTH_BOX_HEIGHT),Scalar(0,0,0) ,2,8,0);


#ifdef RETARD
    imwrite( "I literally hate opencv.png", mat );
#else
    namedWindow( "Display window", WINDOW_AUTOSIZE );
    imshow( "Display window", mat );
    getchar();
#endif
}

