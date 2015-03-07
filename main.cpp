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

int main(int argc, const char **argv)
{
    printf("helloworld");

    for(auto& i : images)
    {
        i = base + i;
    }

    Mat mat;
    mat = imread(images[0], CV_LOAD_IMAGE_COLOR);

    if(!mat.data)
    {
        cout << "whoops";
        exit(1);
    }

    //namedWindow( "Display window", WINDOW_AUTOSIZE );

    imwrite( "I literally hate opencv.png", mat );
}

