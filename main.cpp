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

vector<bounding> boundings =
{
    {EYE_BOX_LEFT_X, EYE_BOX_Y, EYE_BOX_WIDTH, EYE_BOX_HEIGHT},
    {EYE_BOX_RIGHT_X, EYE_BOX_Y, EYE_BOX_WIDTH, EYE_BOX_HEIGHT},
    {MOUTH_BOX_X, MOUTH_BOX_Y, MOUTH_BOX_WIDTH, MOUTH_BOX_HEIGHT}
};

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
vector<Vec3f> region_vals;

Mat get_matrix(int image)
{
    Mat mat;
    mat = imread(images[image], CV_LOAD_IMAGE_COLOR);

    if(!mat.data)
    {
        cout << "whoops";
        exit(1);
    }

    return mat;
}

Vec3f get_sum_val(Mat& mat, int type)
{
    Vec3f sum = 0;
    int num = 0;

    assert(type >= 0 && type < boundings.size());

    for(int j=boundings[type].y; j<boundings[type].y + boundings[type].height; j++)
    {
        for(int i=boundings[type].x; i<boundings[type].x + boundings[type].width; i++)
        {
            Vec3b val = mat.at<Vec3b>(j, i);

            //mat.at<Vec3b>(j, i) = 0;

            sum += val;
            num++;
        }
    }

    assert(num != 0);

    return sum / num;
}
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

    Mat mat = get_matrix(3);

    GetReccct(mat,boundings[0]);
    GetReccct(mat,boundings[1]);
    GetReccct(mat,boundings[2]);

    Vec3f sum_val = get_sum_val(mat, 0);

#ifdef RETARD
    imwrite( "I literally hate opencv.png", mat );
#else
    namedWindow( "Display window", WINDOW_AUTOSIZE );
    imshow( "Display window", mat );
    getchar();
#endif
}
