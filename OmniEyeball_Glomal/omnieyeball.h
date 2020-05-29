#pragma once
/*
//  @ Created by Li Zhengqing, Jonathon
//  @ Koike Lab
//  @ 2017
*/

#include <windows.h>
#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <unordered_map>
#include "omnieyeball_converter.h"

using namespace cv;
using namespace std;

/** Global variables */
std::list<OmniEyeballConverter::DoublePoint> facePointList;
int diameter_output;
int offset_horizontal;
int offset_vertical;
float low_cutter;
Mat src;
Mat image_flip(diameter_output, diameter_output, CV_8UC3);
Mat image_equir(diameter_output, diameter_output, CV_8UC3);
Mat image_final(diameter_output, diameter_output, CV_8UC3);

void ControlUI(int key);
