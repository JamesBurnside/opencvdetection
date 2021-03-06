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

enum emoji
{
    E_NONE,
    E_SMILE,
    E_SUPER_SMILE,
    E_TONGUE,
    E_WINK,
    E_SUPER_WINK,
    E_TONGUE_WINK,
    E_SUNGLASSES
};

enum state
{
    OPEN = 0,
    CLOSED = 1,
    TONGUE = 2,
    SUNGLASSES = 3
};

enum feature_type
{
    EYES,
    MOUTH
};

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

vector<state> feature_states =
{
    OPEN,
    CLOSED,
    CLOSED,
    TONGUE,
    OPEN,
    SUNGLASSES
};

vector<feature_type> types =
{
    EYES,
    EYES,
    MOUTH,
    MOUTH,
    MOUTH,
    EYES
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

emoji mapEmoji(int features[3])
{
    state eye1_state, eye2_state, mouth_state;

    eye1_state=feature_states[features[0]];
    eye2_state=feature_states[features[1]];
    mouth_state=feature_states[features[2]];

    //if one is sunglasses and one isn't, assign both to be the opposite one
    if(eye1_state != eye2_state)
    {
        if(eye1_state == SUNGLASSES)
            eye1_state = eye2_state;
        else if(eye2_state == SUNGLASSES)
            eye2_state = eye1_state;
    }

    if(mouth_state == OPEN)
    {
        //wink - if both different or both closed
        if(eye1_state != eye2_state || (eye1_state == CLOSED && eye2_state == CLOSED))
        {
            return E_SUPER_WINK;
        }
        //both open
        else if(eye1_state == OPEN && eye2_state == OPEN)
        {
            return E_SUPER_SMILE;
        }
        //both sunglasses
        else if(eye1_state == SUNGLASSES && eye2_state == SUNGLASSES)
        {
            return E_SUNGLASSES;
        }
    }
    else if(mouth_state == CLOSED)
    {
        //wink - if both different or both closed
        if(eye1_state != eye2_state || (eye1_state == CLOSED && eye2_state == CLOSED))
        {
            return E_WINK;
        }
        //both open
        else if(eye1_state == OPEN && eye2_state == OPEN)
        {
            return E_SMILE;
        }
        //both sunglasses
        else if(eye1_state == SUNGLASSES && eye2_state == SUNGLASSES)
        {
            return E_SUNGLASSES;
        }
    }
    else if(mouth_state == TONGUE)
    {
        //wink - if both different or both closed
        if(eye1_state != eye2_state || (eye1_state == CLOSED && eye2_state == CLOSED))
        {
            return E_TONGUE_WINK;
        }
        //both open
        else if(eye1_state == OPEN && eye2_state == OPEN)
        {
            return E_TONGUE;
        }
        //both sunglasses
        else if(eye1_state == SUNGLASSES && eye2_state == SUNGLASSES)
        {
            return E_SUNGLASSES;
        }
    }

    assert(false); //how'd we even get here??
    return E_NONE;
}

void GetReccct(Mat img, bounding b)
{
    rectangle(img, Point(b.x,b.y), Point(b.x+b.width,b.y+b.height), Scalar(0,0,0), 2, 8, 0);
}

vector<int> inverse_feature_vals =
{
    3, 5, 5, 3, 1
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

    for(int i=0; i<feature_vals.size(); i++)
    {
        printf("%f %f %f\n", feature_vals[i][0], feature_vals[i][1], feature_vals[i][2]);
    }

    //feature_vals[5] =

    Mat mat = get_matrix(4);

    GetReccct(mat,boundings[0]);
    GetReccct(mat,boundings[1]);
    GetReccct(mat,boundings[2]);

    int feature_nums[FEATURES] = {0};

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

        printf("Feature %i min %i classified as %i\n", i, minimum_num, feature_states[minimum_num]);

        feature_nums[i] = minimum_num;
    }



#ifdef RETARD
    imwrite( "I literally hate opencv.png", mat );
#else
    namedWindow( "Display window", WINDOW_AUTOSIZE );
    imshow( "Display window", mat );
    getchar();
#endif
}
