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

enum state
{
    OPEN = 0,
    CLOSED = 1,
    TONGUE = 2,
    SUNGLASSES = 3
};

enum emoji
{
    E_SMILE,E_SUPER_SMILE,E_TONGUE,E_WINK,E_SUPER_WINK,E_TONGUE_WINK,E_SUNGLASSES
}

string base = "jamesimages/";

struct bounding
{
    int x, y, width, height;
};

#define FEATURES 3

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

vector<Mat> image_mat;

vector<Vec3f> feature_vals =
{
    ///Eyes open
    0,
    ///Eyes closed
    0,
    ///Mouth closed
    0,
    ///Tongue out
    0,
    ///Mouth open
    0,
    ///Sunglasses
    0
};



vector<int> types =
{
    0,
    0,
    1,
    1,
    1,
    0
};

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

emoji mapEmoji(state eye1, state eye2, state mouth)
{
    switch(mouth)
    {
        case OPEN:
        case CLOSED:
        case TONGUE:
        default:
    }
}

void GetReccct(Mat img, bounding b)
{
    rectangle(img, Point(b.x,b.y), Point(b.x+b.width,b.y+b.height), Scalar(0,0,0), 2, 8, 0);
}

vector<int> inverse_feature_vals
{
    3,
    5,
    5,
    3,
    1
};

int main(int argc, const char **argv)
{
    for(auto& i : images)
    {
        i = base + i;
    }

    for(int i=0; i<images.size(); i++)
    {
        image_mat.push_back(get_matrix(i));
    }


    feature_vals[0] = (get_sum_val(image_mat[3], 0) + get_sum_val(image_mat[3], 1)) / 2.0f;
    feature_vals[1] = get_sum_val(image_mat[5], 1);
    feature_vals[2] = get_sum_val(image_mat[5], 2);
    feature_vals[3] = get_sum_val(image_mat[3], 2);
    feature_vals[4] = get_sum_val(image_mat[1], 2);
    //feature_vals[5] =

    Mat mat = get_matrix(4);

    GetReccct(mat,boundings[0]);
    GetReccct(mat,boundings[1]);
    GetReccct(mat,boundings[2]);

    for(int i=0; i<FEATURES; i++)
    {
        Vec3f feat = get_sum_val(mat, i);

        printf("%f %f %f\n", feat[0], feat[1], feat[2]);

        int type = 0;

        if(i == 2)
            type = 1;

        float minimum_difference = 999999999999999;
        int minimum_num = -1;

        for(int j=0; j<feature_vals.size(); j++)
        {
            if(types[j] != type)
                continue;

            Vec3f my_val = feat;

            Vec3f their_val = feature_vals[j];

            Vec3f diff = their_val - my_val;

            float difference = diff[0]*diff[0] + diff[1]*diff[1] + diff[2]*diff[2];

            difference = sqrtf(difference);

            if(difference < minimum_difference)
            {
                minimum_difference = difference;
                minimum_num = j;
            }
        }

        assert(minimum_num != -1);

        printf("Feature %i classified as image %s\n", i, images[inverse_feature_vals[minimum_num]].c_str());
    }



#ifdef RETARD
    imwrite( "I literally hate opencv.png", mat );
#else
    namedWindow( "Display window", WINDOW_AUTOSIZE );
    imshow( "Display window", mat );
    getchar();
#endif
}
