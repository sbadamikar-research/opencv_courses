// QR-Code-Assignment
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;


int main(){
	// Image Path
	// string imgPath = DATA_PATH + "/images/IDCard-Satya.png";
	string imgPath = "../resources/IDCard-Satya.png";

	// Read image and store it in variable img
	Mat img = imread(imgPath, IMREAD_COLOR_BGR);
	
	cout << img.size().height << " " << img.size().width << endl;
	
	Mat bbox, rectifiedImage;

	// Create a QRCodeDetector Object
	// Variable name should be qrDecoder
	QRCodeDetector qrDecoder = QRCodeDetector();

	// Detect QR Code in the Image
	// Output should be stored in opencvData

    string opencvData = qrDecoder.detectAndDecode(img, bbox);

	// Check if a QR Code has been detected
	if(opencvData.length()>0)
		cout << "QR Code Detected" << endl;
	else
		cout << "QR Code NOT Detected" << endl;
	
	// Draw the bounding box
    int n = bbox.cols;
    rectifiedImage = img;

    Scalar BLUE = {255, 0, 0};
    int thickness = int(0.05 * img.rows);

    for (auto i = 0; i < n; i++)
    {
        auto pt1 = Vec2i(bbox.at<Vec2f>(0, (i)));
        auto pt2 = Vec2i(bbox.at<Vec2f>(0, ((i + 1) % n)));
        
        line(rectifiedImage, pt1, pt2, BLUE, thickness, LINE_AA);
    }
	
	// Since we have already detected and decoded the QR Code
	// using qrDecoder.detectAndDecode, we will directly
	// use the decoded text we obtained at that step (opencvData)

	cout << "QR Code Detected!" << endl;
	cout << opencvData << endl;
	
	// Write the result image
	string resultImagePath = "./QRCode-Output-cpp.png";

    imwrite(resultImagePath, rectifiedImage);
	
	return 0;
}