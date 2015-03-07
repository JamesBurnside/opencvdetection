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

#define MOUTH_BOX_WIDTH 200
#define MOUTH_BOX_HEIGHT 160
#define MOUTH_BOX_X 400
#define MOUTH_BOX_Y 780

#define EYE_BOX_WIDTH 130
#define EYE_BOX_HEIGHT 90
#define EYE_BOX_LEFT_X 330
#define EYE_BOX_RIGHT_X 540
#define EYE_BOX_Y 565 

using namespace cv;
using namespace std;

string base = "jamesimages/";

struct bounding
{
    int x, y, width, height;
};

bounding eyeLeftRect = {EYE_BOX_LEFT_X, EYE_BOX_Y, EYE_BOX_WIDTH, EYE_BOX_HEIGHT}, 
         eyeRightRect = {EYE_BOX_RIGHT_X, EYE_BOX_Y, EYE_BOX_WIDTH, EYE_BOX_HEIGHT}, 
         mouthRect = {MOUTH_BOX_X, MOUTH_BOX_Y, MOUTH_BOX_WIDTH, MOUTH_BOX_HEIGHT};

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

void GetReccct(Mat img, bounding b) 
{
    rectangle(img, Point(b.x,b.y), Point(b.x+b.width,b.y+b.height), Scalar(0,0,0), 2, 8, 0);  
}

int main(int argc, const char **argv)
{
    printf("helloworld");

    for(auto& i : images)
    {
        i = base + i;
    }

    Mat mat;
    mat = imread(images[1], CV_LOAD_IMAGE_COLOR);

    if(!mat.data)
    {
        cout << "whoops";
        exit(1);
    }

    GetReccct(mat,eyeLeftRect);
    GetReccct(mat,eyeRightRect);
    GetReccct(mat,mouthRect);




#ifdef RETARD
    imwrite( "I literally hate opencv.png", mat );
#else
    namedWindow( "Display window", WINDOW_AUTOSIZE );
    imshow( "Display window", mat );
    getchar();
#endif
}

