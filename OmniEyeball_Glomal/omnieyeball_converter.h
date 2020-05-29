/*
//  @ This code is used for converting equirectangular format to round format
//  @ Created by Li Zhengqing, Jonathon and Miyafuji Shio
//  @ Koike Lab
//  @ 2017
*/

#ifndef OmniEyeballConverter_h
#define OmniEyeballConverter_h

#include <iostream>
#include "opencv2/opencv.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <unordered_map>

class OmniEyeballConverter {
    
public:
    
    OmniEyeballConverter();
    OmniEyeballConverter(cv::Mat src, float low_cutter);
    cv::Mat Equirectangular2Round (cv::Mat src, int );
    void ComputeRectPoints(cv::Mat src);
    cv::Point ReturnRectPoints(cv::Point point);
    float ComputeAngle(cv::Point2f a, cv::Point2f b, cv::Point2f c);
    
    struct DoublePoint
    {
        cv::Point a;
        cv::Point b;
    };
    
private:
    /** Class variables */
    int image_width_;
    int image_height_;
    int diameter_remap_;
    int radius_remap_;
	float low_cutter_;
    std::list<DoublePoint> pointList_;
    
};

#endif /* OmniEyeballConverter_h */

