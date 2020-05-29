#include "omnieyeball_converter.h"

using namespace cv;
using namespace std;

OmniEyeballConverter::OmniEyeballConverter(){
}

OmniEyeballConverter::OmniEyeballConverter(Mat src, float low_cutter){
	low_cutter_ = low_cutter;
	ComputeRectPoints(src);
}

cv::Mat OmniEyeballConverter::Equirectangular2Round (cv::Mat src, int diameter_output)
{
    Mat image_output(diameter_output, diameter_output, CV_8UC3);
    Mat dst(diameter_remap_, diameter_remap_, CV_8UC3, Scalar(0, 0, 0));
    for (DoublePoint doublePoint : pointList_)
    {
        Vec3b color = src.at<Vec3b>(Point(doublePoint.b));
        dst.at<Vec3b>(doublePoint.a) = color;
    }
    resize(dst, image_output, Size(diameter_output, diameter_output));
    return image_output;
}

void OmniEyeballConverter::ComputeRectPoints(cv::Mat src){
    image_width_ = src.cols;
    image_height_ = src.rows * low_cutter_;
    cout << image_width_ << " " << image_height_ << endl;
    diameter_remap_ = image_height_ * 2;
    radius_remap_ = image_height_;
    
    for (int x = 0; x < diameter_remap_; x++)
    {
        for (int y = 0; y < diameter_remap_; y++)
        {
            Point difference = Point(x, y) - Point(radius_remap_, radius_remap_);
            double distance = sqrt(difference.ddot(difference));
            
            if (distance > radius_remap_)
            {   
				// TODO
				// Using euclidean distance might be faster
				// Put black color
                // dst.at<Vec3b>(Point(x, y)) = Vec3b(0, 0, 0);
            }
            else
            {
                Point rectPoint = ReturnRectPoints(Point(x, y));
                DoublePoint dP;
                dP.a = Point(x, y);
                dP.b = rectPoint;
                pointList_.push_back(dP);
                //Use point to get color from rect image to new image
            }
        }
    }

}

Point OmniEyeballConverter::ReturnRectPoints(Point point)
{
    Point centerPoint(radius_remap_, radius_remap_);
    Point zeroPoint(radius_remap_, 0);
    float angle = ComputeAngle(zeroPoint, centerPoint, point);
    if (angle < 0)
        angle += 360;
    float dist = cv::norm(point - centerPoint);
    int x = (angle / 360) * image_width_;
    if (x == diameter_remap_)
        x--;
    if (x < 0)
        x = 0;
    if (dist == radius_remap_)
        dist--;
    if (dist < 0)
        dist = 0;
    int y = dist;
    return Point(x, y);
}

float OmniEyeballConverter::ComputeAngle(Point2f a, Point2f b, Point2f c)
{   
    Point2f ab;
    Point2f cb;
    
    ab.x = b.x - a.x;
    ab.y = b.y - a.y;
    
    cb.x = b.x - c.x;
    cb.y = b.y - c.y;
    
    float angba = atan2(ab.y, ab.x);
    float angbc = atan2(cb.y, cb.x);
    float rslt = angba - angbc;
    return (rslt * 180) / 3.141592;
}
