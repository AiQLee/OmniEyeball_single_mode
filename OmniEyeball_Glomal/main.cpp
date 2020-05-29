#include "omnieyeball.h"

int main(int argc, char **argv)
{
	// Initialize Parameters
	namedWindow("ProjectionToOmniEyeball", WINDOW_AUTOSIZE);
	low_cutter = 0.88; // In default, only 88% area of input image is used (From top to bottom). Because the bottom 12% area is useless
	diameter_output = 512; // The resolution parameter to set the height of output image. Default value for OmniGlobe is 768.

	// Parameter to micro adjust the position of round image. Use Key "AWSD" to control.
	offset_horizontal = 0;
	offset_vertical = 0;


	// Declare a VideoCapture object and associate to webcam, 0 => use 1st defualt webcam
	// Theta usually is at 3 or 4
	cv::VideoCapture capWebcam(0);		
	if (!capWebcam.isOpened()) {
		// The camera is not found
		return -1;
	}

	capWebcam >> src;
	if (!src.data){
		cout << "can't find the specific image." << endl;
		return -1;
	}

	// Initialize the OmniEyeball converter
	OmniEyeballConverter converter_for_theta(src, low_cutter);

	while (1) {

		// Read frames from cameras and copy to final_output Mat
		// Resolution: 1920*960 from Theta S, FPS:30
		capWebcam >> src;

		cv::imshow("src", src);
		image_equir = src.clone();
			
		// Transfer from equirectangular to round mapping
		cv::Mat image_round = converter_for_theta.Equirectangular2Round(image_equir, diameter_output);
		
		// Flip
		cv::flip(image_round, image_flip, 1);

		// Make micro adjustment
		cv::Mat image_transform(diameter_output*2, diameter_output, CV_8UC3);
		image_flip.copyTo(image_transform(cv::Rect(0, diameter_output/2, diameter_output, diameter_output)));
		cv::Mat transformedImage(diameter_output, diameter_output, CV_8UC3);
		transformedImage = image_transform(cv::Rect(0, diameter_output/2 + offset_vertical, diameter_output, diameter_output));
		
		// Make black frames at two sides.
		cv::copyMakeBorder(transformedImage, image_final, 0, 0, 128 + offset_horizontal, 128 - offset_horizontal, BORDER_CONSTANT, Scalar(0, 0, 0));

		// Show round projection image
		cv::imshow("ProjectionToOmniEyeball", image_final);

		// UI
		int key = waitKey(1);
		if (key == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			cv::destroyAllWindows();
			break;
		}
		else {
			ControlUI(key);
		}
	}
	return 0;
}


/*
	KeyBoard Controller
*/
void ControlUI(int key) {

	if (key == 32) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
	{
		cout << "space key is pressed by user" << endl;

		HWND windowHandle = ::FindWindowA(NULL, "Final_resizedImage");

		if (NULL != windowHandle) {

			SetWindowLongPtr(windowHandle, GWL_STYLE, WS_POPUP);
			SetWindowLongPtr(windowHandle, GWL_EXSTYLE, WS_EX_TOPMOST);

			ShowWindow(windowHandle, SW_MAXIMIZE);
			cvSetWindowProperty("Final_resizedImage", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

			int mainDisplayWidth = GetSystemMetrics(SM_CXSCREEN);
			int mainDisplayHeight = GetSystemMetrics(SM_CYSCREEN);

			SetWindowPos(windowHandle, NULL,
				0, 0, mainDisplayWidth, mainDisplayWidth,
				SWP_FRAMECHANGED | SWP_NOZORDER);
		}
	}
	else if (key == 119) {
		/* When press w */
		offset_vertical++;
		cout << offset_vertical << endl;
	}
	else if (key == 115) {
		/* When press s */
		offset_vertical--;
		cout << offset_vertical << endl;
	}
	else if (key == 97) {
		/* When press a */
		offset_horizontal--;
		cout << offset_horizontal << endl;
	}
	else if (key == 100) {
		/* When press d */
		offset_horizontal++;
		cout << offset_horizontal << endl;
	}
}

