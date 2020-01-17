
// MFCApplication1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include "SecondDialog.h"

#include <ostream>
#include <sstream>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef long long long64;
extern CString global_m_PicPath;
CString m_PicPath;

// CAboutDlg dialog used for App About
LPCWSTR convStringToLPCWSTR(string String)
{
	char *cString = &String[0u];
	long64 size = strlen(cString);
	wchar_t* wString = new wchar_t[size];
	for (long64 i = 0; i < size; i++)
	{
		wString[i] = cString[i];
	}
	wString[size] = 0; //important, otherwise Windows will print whatever next in memmory until it finds a zero.
	return wString;
}

float Truncate(float value)
{
	if (value <= 0)
		value = 0;
	if (value >= 255)
		value = 255;

	return value;
}
int myMax(int x, int y, int z)
{
	if (x > y && x > z)
		return x;
	else if (y > x && y > z)
		return y;
	else if (z>x && z>y)
		return z;
}

void mySharpen(Mat& myImage, Mat& Result)
{

	Result.create(myImage.size(), myImage.type());

	for (int x = 1; x < myImage.rows - 1; ++x)
	{
		for (int y = 1; y < myImage.cols - 1; ++y)
		{

			Vec3b BGR;
			BGR[0] = Truncate(5 * myImage.at<Vec3b>(x, y)[0] - myImage.at<Vec3b>(x - 1, y)[0] - myImage.at<Vec3b>(x + 1, y)[0]
				- myImage.at<Vec3b>(x, y + 1)[0] - myImage.at<Vec3b>(x, y - 1)[0]);
			BGR[1] = Truncate(5 * myImage.at<Vec3b>(x, y)[1] - myImage.at<Vec3b>(x - 1, y)[1] - myImage.at<Vec3b>(x + 1, y)[1]
				- myImage.at<Vec3b>(x, y + 1)[1] - myImage.at<Vec3b>(x, y - 1)[1]);
			BGR[2] = Truncate(5 * myImage.at<Vec3b>(x, y)[2] - myImage.at<Vec3b>(x - 1, y)[2] - myImage.at<Vec3b>(x + 1, y)[2]
				- myImage.at<Vec3b>(x, y + 1)[2] - myImage.at<Vec3b>(x, y - 1)[2]);

			Result.at<Vec3b>(x, y)[0] = BGR[0];
			Result.at<Vec3b>(x, y)[1] = BGR[1];
			Result.at<Vec3b>(x, y)[2] = BGR[2];



		}
	}

}

void myBlur(Mat& myImage, Mat& Result)
{
	Result.create(myImage.size(), myImage.type());
	Result = myImage;

	for (int x = 1; x < myImage.rows - 1; ++x)
	{
		for (int y = 1; y < myImage.cols - 1; ++y)
		{
			Vec3b avgBGR;
			avgBGR[0] = (myImage.at<Vec3b>(x - 1, y)[0] + myImage.at<Vec3b>(x - 1, y - 1)[0] + myImage.at<Vec3b>(x - 1, y + 1)[0]
				+ myImage.at<Vec3b>(x, y + 1)[0] + myImage.at<Vec3b>(x, y - 1)[0] + myImage.at<Vec3b>(x + 1, y)[0]
				+ myImage.at<Vec3b>(x + 1, y + 1)[0] + myImage.at<Vec3b>(x + 1, y - 1)[0]) / 8;


			avgBGR[1] = (myImage.at<Vec3b>(x - 1, y)[1] + myImage.at<Vec3b>(x - 1, y - 1)[1] + myImage.at<Vec3b>(x - 1, y + 1)[1]
				+ myImage.at<Vec3b>(x, y + 1)[1] + myImage.at<Vec3b>(x, y - 1)[1] + myImage.at<Vec3b>(x + 1, y)[1]
				+ myImage.at<Vec3b>(x + 1, y + 1)[1] + myImage.at<Vec3b>(x + 1, y - 1)[1]) / 8;

			avgBGR[2] = (myImage.at<Vec3b>(x - 1, y)[2] + myImage.at<Vec3b>(x - 1, y - 1)[2] + myImage.at<Vec3b>(x - 1, y + 1)[2]
				+ myImage.at<Vec3b>(x, y + 1)[2] + myImage.at<Vec3b>(x, y - 1)[2] + myImage.at<Vec3b>(x + 1, y)[2]
				+ myImage.at<Vec3b>(x + 1, y + 1)[2] + myImage.at<Vec3b>(x + 1, y - 1)[2]) / 8;

			Result.at<Vec3b>(x, y)[0] = avgBGR[0];
			Result.at<Vec3b>(x, y)[1] = avgBGR[1];
			Result.at<Vec3b>(x, y)[2] = avgBGR[2];

		}
	}


}

void myContrast(Mat& myImage, Mat& Result, float contrast)
{

	float f = (259 * (contrast + 255)) / (255 * (259 - contrast));
	Result.create(myImage.size(), myImage.type());
	int nChannels = myImage.channels();

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Vec3b bright;
			int average = (myImage.at<Vec3b>(x, y)[0] + myImage.at<Vec3b>(x, y)[1] + myImage.at<Vec3b>(x, y)[2]) / 3;

			bright[2] = Truncate(f * (myImage.at<Vec3b>(x, y)[2] - 128) + 128);
			bright[1] = Truncate(f * (myImage.at<Vec3b>(x, y)[1] - 128) + 128);
			bright[0] = Truncate(f * (myImage.at<Vec3b>(x, y)[0] - 128) + 128);


			Result.at<Vec3b>(x, y)[0] = bright[0];
			Result.at<Vec3b>(x, y)[1] = bright[1];
			Result.at<Vec3b>(x, y)[2] = bright[2];

		}
	}


}

void myEdgeFilter(Mat& myImage, Mat& Result)
{

	Result.create(myImage.size(), myImage.type());

	for (int x = 1; x < myImage.rows - 1; ++x)
	{
		for (int y = 1; y < myImage.cols - 1; ++y)
		{


			Vec3b BGR;
			BGR[0] = Truncate(abs(myImage.at<Vec3b>(x + 1, y)[0] - myImage.at<Vec3b>(x - 1, y)[0]) + abs(myImage.at<Vec3b>(x, y + 1)[0] - myImage.at<Vec3b>(x, y - 1)[0]));
			BGR[1] = Truncate(abs(myImage.at<Vec3b>(x + 1, y)[1] - myImage.at<Vec3b>(x - 1, y)[1]) + abs(myImage.at<Vec3b>(x, y + 1)[1] - myImage.at<Vec3b>(x, y - 1)[1]));
			BGR[2] = Truncate(abs(myImage.at<Vec3b>(x + 1, y)[2] - myImage.at<Vec3b>(x - 1, y)[2]) + abs(myImage.at<Vec3b>(x, y + 1)[2] - myImage.at<Vec3b>(x, y - 1)[2]));


			Result.at<Vec3b>(x, y)[0] = BGR[0];
			Result.at<Vec3b>(x, y)[1] = BGR[1];
			Result.at<Vec3b>(x, y)[2] = BGR[2];




		}
	}

	myContrast(Result, Result, 10);
}


void myDither(Mat& myImage, Mat& Result)
{
	Result.create(myImage.size(), myImage.type());

	for (int x = 1; x < myImage.rows - 1; ++x)
	{
		for (int y = 1; y < myImage.cols - 1; ++y)
		{
			Vec3b avgBGR;
			avgBGR[0] = Truncate((0 * myImage.at<Vec3b>(x, y)[0] + 1 * myImage.at<Vec3b>(x - 1, y)[0] + (-5)*myImage.at<Vec3b>(x - 1, y - 1)[0] + 6 * myImage.at<Vec3b>(x - 1, y + 1)[0]
				+ 8 * myImage.at<Vec3b>(x, y + 1)[0] + 2 * myImage.at<Vec3b>(x, y - 1)[0] + 3 * myImage.at<Vec3b>(x + 1, y)[0]
				+ 4 * myImage.at<Vec3b>(x + 1, y + 1)[0] + 7 * myImage.at<Vec3b>(x + 1, y - 1)[0]) / 21);


			avgBGR[1] = Truncate((myImage.at<Vec3b>(x - 1, y)[1] + (-5)*myImage.at<Vec3b>(x - 1, y - 1)[1] + 6 * myImage.at<Vec3b>(x - 1, y + 1)[1]
				+ 8 * myImage.at<Vec3b>(x, y + 1)[1] + 2 * myImage.at<Vec3b>(x, y - 1)[1] + 3 * myImage.at<Vec3b>(x + 1, y)[1]
				+ 4 * myImage.at<Vec3b>(x + 1, y + 1)[1] + 7 * myImage.at<Vec3b>(x + 1, y - 1)[1]) / 21);

			avgBGR[2] = Truncate((myImage.at<Vec3b>(x - 1, y)[2] + (-5)*myImage.at<Vec3b>(x - 1, y - 1)[2] + 6 * myImage.at<Vec3b>(x - 1, y + 1)[2]
				+ 8 * myImage.at<Vec3b>(x, y + 1)[2] + 2 * myImage.at<Vec3b>(x, y - 1)[2] + 3 * myImage.at<Vec3b>(x + 1, y)[2]
				+ 4 * myImage.at<Vec3b>(x + 1, y + 1)[2] + 7 * myImage.at<Vec3b>(x + 1, y - 1)[2]) / 21);

			Result.at<Vec3b>(x, y)[0] = avgBGR[0];
			Result.at<Vec3b>(x, y)[1] = avgBGR[1];
			Result.at<Vec3b>(x, y)[2] = avgBGR[2];

		}
	}


}



void myNoiseRemover(Mat& myImage, Mat& Result)
{
	Result.create(myImage.size(), myImage.type());

	for (int x = 1; x < myImage.rows - 1; ++x)
	{
		for (int y = 1; y < myImage.cols - 1; ++y)
		{
			Vec3b  median;

			//////////////////////////////////////////////////////////////////////////////////////////////
			int blues[] = { myImage.at<Vec3b>(x - 1, y)[0], myImage.at<Vec3b>(x - 1, y - 1)[0], myImage.at<Vec3b>(x - 1, y + 1)[0]
				, myImage.at<Vec3b>(x, y + 1)[0], myImage.at<Vec3b>(x, y - 1)[0], myImage.at<Vec3b>(x + 1, y)[0]
				, myImage.at<Vec3b>(x + 1, y + 1)[0], myImage.at<Vec3b>(x + 1, y - 1)[0] };

			vector<int>blueMedian(blues, blues + 8);
			sort(blueMedian.begin(), blueMedian.end());

			median[0] = (blueMedian[3] + blueMedian[4]) / 2;
			////////////////////////////////////////////////////////////////////////////////////////////

			int greens[] = { myImage.at<Vec3b>(x - 1, y)[1], myImage.at<Vec3b>(x - 1, y - 1)[1], myImage.at<Vec3b>(x - 1, y + 1)[1]
				, myImage.at<Vec3b>(x, y + 1)[1], myImage.at<Vec3b>(x, y - 1)[1], myImage.at<Vec3b>(x + 1, y)[1]
				, myImage.at<Vec3b>(x + 1, y + 1)[1], myImage.at<Vec3b>(x + 1, y - 1)[1] };

			vector<int>greenMedian(greens, greens + 8);
			sort(greenMedian.begin(), greenMedian.end());

			median[1] = (greenMedian[3] + greenMedian[4]) / 2;

			///////////////////////////////////////////////////////////////////////////////////////////
			int reds[] = { myImage.at<Vec3b>(x - 1, y)[2], myImage.at<Vec3b>(x - 1, y - 1)[2], myImage.at<Vec3b>(x - 1, y + 1)[2]
				, myImage.at<Vec3b>(x, y + 1)[2], myImage.at<Vec3b>(x, y - 1)[2], myImage.at<Vec3b>(x + 1, y)[2]
				, myImage.at<Vec3b>(x + 1, y + 1)[2], myImage.at<Vec3b>(x + 1, y - 1)[2] };

			vector<int>redMedian(reds, reds + 8);
			sort(redMedian.begin(), redMedian.end());

			median[2] = (redMedian[3] + redMedian[4]) / 2;
			////////////////////////////////////////////////////////////////////////////////////////////

			Result.at<Vec3b>(x, y)[0] = median[0];
			Result.at<Vec3b>(x, y)[1] = median[1];
			Result.at<Vec3b>(x, y)[2] = median[2];

		}
	}


}


void myFlipHorizontal(const Mat& myImage, Mat& Result)
{

	Result.create(myImage.size(), myImage.type());
	int nChannels = myImage.channels();
	int counter = 0;
	for (int x = 0; x < myImage.rows; ++x)
	{

		for (int y = 1; y < myImage.cols; ++y)
		{

			Result.at<Vec3b>(x, y)[0] = myImage.at<Vec3b>(x, myImage.cols - y)[0];
			Result.at<Vec3b>(x, y)[1] = myImage.at<Vec3b>(x, myImage.cols - y)[1];
			Result.at<Vec3b>(x, y)[2] = myImage.at<Vec3b>(x, myImage.cols - y)[2];

		}

	}


	Result.row(0).setTo(Scalar(0));

	Result.row(Result.rows - 1).setTo(Scalar(0));

	Result.col(0).setTo(Scalar(0));
	Result.col(Result.cols - 1).setTo(Scalar(0));
}

void myFlipVertical(const Mat& myImage, Mat& Result)
{

	Result.create(myImage.size(), myImage.type());
	int nChannels = myImage.channels();

	for (int x = 1; x < myImage.rows; ++x)
	{
		for (int y = 1; y < myImage.cols; ++y)
		{

			Result.at<Vec3b>(x, y)[0] = myImage.at<Vec3b>(myImage.rows - x, y)[0];
			Result.at<Vec3b>(x, y)[1] = myImage.at<Vec3b>(myImage.rows - x, y)[1];
			Result.at<Vec3b>(x, y)[2] = myImage.at<Vec3b>(myImage.rows - x, y)[2];

		}

	}

	Result.row(0).setTo(Scalar(0));

	Result.row(Result.rows - 1).setTo(Scalar(0));

	Result.col(0).setTo(Scalar(0));
	Result.col(Result.cols - 1).setTo(Scalar(0));
}


void myRotateClockwise(const Mat& myImage, Mat& Result)
{

	Result.create(myImage.cols, myImage.rows, CV_8UC(3));
	int nChannels = myImage.channels();

	for (int x = 0, j = Result.cols - 1; x < myImage.rows && j >= 0; ++x, --j)
	{
		for (int y = 0, i = 0; y < myImage.cols && i<Result.rows; ++y, ++i)
		{
			Result.at<Vec3b>(i, j)[0] = myImage.at<Vec3b>(x, y)[0];
			Result.at<Vec3b>(i, j)[1] = myImage.at<Vec3b>(x, y)[1];
			Result.at<Vec3b>(i, j)[2] = myImage.at<Vec3b>(x, y)[2];
		}

	}


}


void myRotateAntiClockwise(const Mat& myImage, Mat& Result)
{
	Result.create(myImage.cols, myImage.rows, CV_8UC(3));
	int nChannels = myImage.channels();

	for (int x = 0, j = 0; x < myImage.rows && j < Result.cols; ++x, ++j)
	{
		for (int y = 0, i = Result.rows - 1; y < myImage.cols && i >= 0; ++y, --i)
		{
			Result.at<Vec3b>(i, j)[0] = myImage.at<Vec3b>(x, y)[0];
			Result.at<Vec3b>(i, j)[1] = myImage.at<Vec3b>(x, y)[1];
			Result.at<Vec3b>(i, j)[2] = myImage.at<Vec3b>(x, y)[2];
		}
	}

}

void myGreyScale(Mat& myImage, Mat& Result)
{
	Result.create(myImage.size(), myImage.type());
	int nChannels = myImage.channels();

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{

			int average = (myImage.at<Vec3b>(x, y)[0] + myImage.at<Vec3b>(x, y)[1] + myImage.at<Vec3b>(x, y)[2]) / 3;
			Result.at<Vec3b>(x, y)[0] = average;
			Result.at<Vec3b>(x, y)[1] = average;
			Result.at<Vec3b>(x, y)[2] = average;

		}
	}


}


void mySepia(Mat& myImage, Mat& Result)
{
	Result.create(myImage.size(), myImage.type());
	int nChannels = myImage.channels();

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{

			Result.at<Vec3b>(x, y)[2] = Truncate((myImage.at<Vec3b>(x, y)[2] * .393) + (myImage.at<Vec3b>(x, y)[1] * .769) + (myImage.at<Vec3b>(x, y)[0] * .189));
			Result.at<Vec3b>(x, y)[1] = Truncate((myImage.at<Vec3b>(x, y)[2] * .349) + (myImage.at<Vec3b>(x, y)[1] * .686) + (myImage.at<Vec3b>(x, y)[0] * .168));
			Result.at<Vec3b>(x, y)[0] = Truncate((myImage.at<Vec3b>(x, y)[2] * .272) + (myImage.at<Vec3b>(x, y)[1] * .534) + (myImage.at<Vec3b>(x, y)[0] * .131));

		}
	}


}


void myBlue(Mat& myImage, Mat& Result)
{
	Result.create(myImage.size(), myImage.type());
	int nChannels = myImage.channels();

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Result.at<Vec3b>(x, y)[0] = 255;
			Result.at<Vec3b>(x, y)[1] = myImage.at<Vec3b>(x, y)[1];
			Result.at<Vec3b>(x, y)[2] = myImage.at<Vec3b>(x, y)[2];

		}
	}


}

void myGreen(Mat& myImage, Mat& Result)
{
	Result.create(myImage.size(), myImage.type());
	int nChannels = myImage.channels();

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Result.at<Vec3b>(x, y)[0] = myImage.at<Vec3b>(x, y)[0];
			Result.at<Vec3b>(x, y)[1] = 180;
			Result.at<Vec3b>(x, y)[2] = myImage.at<Vec3b>(x, y)[2];

		}
	}


}

void myRed(Mat& myImage, Mat& Result)
{
	Result.create(myImage.size(), myImage.type());
	int nChannels = myImage.channels();

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Result.at<Vec3b>(x, y)[0] = myImage.at<Vec3b>(x, y)[0];
			Result.at<Vec3b>(x, y)[1] = myImage.at<Vec3b>(x, y)[1];
			Result.at<Vec3b>(x, y)[2] = 255;

		}
	}


}

void myShadowRemover(const Mat& myImage, Mat& Result)
{
	int f = 25;
	Result.create(myImage.size(), myImage.type());
	int nChannels = myImage.channels();

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Vec3b bright;
			int average = (myImage.at<Vec3b>(x, y)[0] + myImage.at<Vec3b>(x, y)[1] + myImage.at<Vec3b>(x, y)[2]) / 3;
			if (average>40)
			{
				if (average>100)
				{
					bright[0] = Truncate(myImage.at<Vec3b>(x, y)[0]);
					bright[1] = Truncate(myImage.at<Vec3b>(x, y)[1]);
					bright[2] = Truncate(myImage.at<Vec3b>(x, y)[2]);
				}
				if (average>60)
				{
					bright[0] = Truncate(myImage.at<Vec3b>(x, y)[0] + (f % 4));
					bright[1] = Truncate(myImage.at<Vec3b>(x, y)[1] + (f % 4));
					bright[2] = Truncate(myImage.at<Vec3b>(x, y)[2] + (f % 4));
				}
				else
				{
					bright[0] = Truncate(myImage.at<Vec3b>(x, y)[0] + (f % 2));
					bright[1] = Truncate(myImage.at<Vec3b>(x, y)[1] + (f % 2));
					bright[2] = Truncate(myImage.at<Vec3b>(x, y)[2] + (f % 2));
				}
			}

			else
			{
				bright[0] = Truncate(f + myImage.at<Vec3b>(x, y)[0]);
				bright[1] = Truncate(f + myImage.at<Vec3b>(x, y)[1]);
				bright[2] = Truncate(f + myImage.at<Vec3b>(x, y)[2]);
			}

			Result.at<Vec3b>(x, y)[0] = (bright[0]);
			Result.at<Vec3b>(x, y)[1] = (bright[1]);
			Result.at<Vec3b>(x, y)[2] = (bright[2]);

		}
	}


}



void myLight(Mat& myImage, Mat& Result)
{
	float f = 40;
	Result.create(myImage.size(), myImage.type());
	int nChannels = myImage.channels();

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Vec3b bright;
			int average = (myImage.at<Vec3b>(x, y)[0] + myImage.at<Vec3b>(x, y)[1] + myImage.at<Vec3b>(x, y)[2]) / 3;

			bright[0] = Truncate(myImage.at<Vec3b>(x, y)[0] + f);
			bright[1] = Truncate(myImage.at<Vec3b>(x, y)[1] + f);
			bright[2] = Truncate(myImage.at<Vec3b>(x, y)[2] + f);

			Result.at<Vec3b>(x, y)[0] = (bright[0]);
			Result.at<Vec3b>(x, y)[1] = (bright[1]);
			Result.at<Vec3b>(x, y)[2] = (bright[2]);

		}
	}


}

void myBrighten(Mat& myImage, Mat& Result)
{
	float f = 1.5;
	Result.create(myImage.size(), myImage.type());
	int nChannels = myImage.channels();

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Vec3b bright;
			float threshold = 255.999;
			int m = myMax(f*myImage.at<Vec3b>(x, y)[0], f*myImage.at<Vec3b>(x, y)[1], f*myImage.at<Vec3b>(x, y)[2]);
			if (m<threshold)
			{
				bright[0] = f*myImage.at<Vec3b>(x, y)[0];
				bright[1] = f*myImage.at<Vec3b>(x, y)[1];
				bright[2] = f*myImage.at<Vec3b>(x, y)[2];

			}
			else
			{
				int total = f*myImage.at<Vec3b>(x, y)[0] + f*myImage.at<Vec3b>(x, y)[1] + f*myImage.at<Vec3b>(x, y)[2];

				if (total >= 3 * threshold)
				{
					bright[0] = threshold;
					bright[1] = threshold;
					bright[2] = threshold;
				}
				else
				{
					int x = (3 * threshold - total) / (3 * m - total);
					int gray = threshold - x * m;
					bright[0] = gray + x * f*myImage.at<Vec3b>(x, y)[0];
					bright[1] = gray + x * f*myImage.at<Vec3b>(x, y)[1];
					bright[2] = gray + x * f*myImage.at<Vec3b>(x, y)[2];
				}
			}

			Result.at<Vec3b>(x, y)[0] = Truncate(bright[0]);
			Result.at<Vec3b>(x, y)[1] = Truncate(bright[1]);
			Result.at<Vec3b>(x, y)[2] = Truncate(bright[2]);

		}
	}


}

void myBrighten2(Mat& myImage, Mat& Result)
{
	float f = 1.5;
	Result.create(myImage.size(), myImage.type());
	int nChannels = myImage.channels();

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Vec3b bright;
			int average = (myImage.at<Vec3b>(x, y)[0] + myImage.at<Vec3b>(x, y)[1] + myImage.at<Vec3b>(x, y)[2]) / 3;

			bright[0] = Truncate(f * myImage.at<Vec3b>(x, y)[0]);
			bright[1] = Truncate(f * myImage.at<Vec3b>(x, y)[1]);
			bright[2] = Truncate(f * myImage.at<Vec3b>(x, y)[2]);

			Result.at<Vec3b>(x, y)[0] = (bright[0]);
			Result.at<Vec3b>(x, y)[1] = (bright[1]);
			Result.at<Vec3b>(x, y)[2] = (bright[2]);

		}
	}


}



void myDark(Mat& myImage, Mat& Result, float f)
{

	Result.create(myImage.size(), myImage.type());
	int nChannels = myImage.channels();

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Vec3b bright;
			int average = (myImage.at<Vec3b>(x, y)[0] + myImage.at<Vec3b>(x, y)[1] + myImage.at<Vec3b>(x, y)[2]) / 3;

			bright[0] = Truncate(myImage.at<Vec3b>(x, y)[0] - f);
			bright[1] = Truncate(myImage.at<Vec3b>(x, y)[1] - f);
			bright[2] = Truncate(myImage.at<Vec3b>(x, y)[2] - f);

			Result.at<Vec3b>(x, y)[0] = bright[0];
			Result.at<Vec3b>(x, y)[1] = bright[1];
			Result.at<Vec3b>(x, y)[2] = bright[2];

		}
	}


}

void myThreshold(const Mat& myImage, Mat& Result)
{
	int f = 55;
	Result.create(myImage.size(), myImage.type());
	int nChannels = myImage.channels();

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Vec3b bright;
			int average = (myImage.at<Vec3b>(x, y)[0] + myImage.at<Vec3b>(x, y)[1] + myImage.at<Vec3b>(x, y)[2]) / 3;
			if (average>f)
			{
				bright[0] = 255;
				bright[1] = 255;
				bright[2] = 255;
			}

			else
			{
				bright[0] = 0;
				bright[1] = 0;
				bright[2] = 0;
			}

			Result.at<Vec3b>(x, y)[0] = (bright[0]);
			Result.at<Vec3b>(x, y)[1] = (bright[1]);
			Result.at<Vec3b>(x, y)[2] = (bright[2]);

		}
	}


}


void myInvert(const Mat& myImage, Mat& Result)
{
	Result.create(myImage.size(), myImage.type());
	int nChannels = myImage.channels();

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{

			Result.at<Vec3b>(x, y)[2] = 255 - myImage.at<Vec3b>(x, y)[2];
			Result.at<Vec3b>(x, y)[1] = 255 - myImage.at<Vec3b>(x, y)[1];
			Result.at<Vec3b>(x, y)[0] = 255 - myImage.at<Vec3b>(x, y)[0];

		}
	}


}

void myBlend(const Mat& myImage1, const Mat& myImage2, Mat& Result)
{
	Result.create(max(myImage1.rows, myImage2.rows), max(myImage1.cols, myImage2.cols), CV_8UC(3));
	float f = 00.4;

	int flag;

	for (int x = 0; x < Result.rows; ++x)
	{
		for (int y = 0; y < Result.cols; ++y)
		{
			Result.at<Vec3b>(x, y)[0] = 0;
			Result.at<Vec3b>(x, y)[1] = 0;
			Result.at<Vec3b>(x, y)[2] = 0;

			flag = 0;
			if (myImage1.cols>y && myImage1.rows>x)
			{
				Result.at<Vec3b>(x, y)[0] = (1 - f)* myImage1.at<Vec3b>(x, y)[0];
				Result.at<Vec3b>(x, y)[1] = (1 - f)* myImage1.at<Vec3b>(x, y)[1];
				Result.at<Vec3b>(x, y)[2] = (1 - f)* myImage1.at<Vec3b>(x, y)[2];
				flag = 1;
			}
			if (myImage2.cols>y && myImage2.rows>x || (myImage1.cols>y && myImage1.rows>x))
			{
				if (myImage2.cols > y && myImage2.rows > x)
				{
					if (flag == 1)
					{
						Result.at<Vec3b>(x, y)[0] = Result.at<Vec3b>(x, y)[0] + f*myImage2.at<Vec3b>(x, y)[0];
						Result.at<Vec3b>(x, y)[1] = Result.at<Vec3b>(x, y)[1] + f*myImage2.at<Vec3b>(x, y)[1];
						Result.at<Vec3b>(x, y)[2] = Result.at<Vec3b>(x, y)[2] + f*myImage2.at<Vec3b>(x, y)[2];
					}
					else
					{
						Result.at<Vec3b>(x, y)[0] = myImage2.at<Vec3b>(x, y)[0];
						Result.at<Vec3b>(x, y)[1] = myImage2.at<Vec3b>(x, y)[1];
						Result.at<Vec3b>(x, y)[2] = myImage2.at<Vec3b>(x, y)[2];
					}
				}
				else
				{
					Result.at<Vec3b>(x, y)[0] = myImage1.at<Vec3b>(x, y)[0];
					Result.at<Vec3b>(x, y)[1] = myImage1.at<Vec3b>(x, y)[1];
					Result.at<Vec3b>(x, y)[2] = myImage1.at<Vec3b>(x, y)[2];
				}

			}


		}

	}

}

void myBlend1(const Mat& myImage1, Mat& Result)
{
	Mat myImage2;
	myImage2 = imread("logo1.png",3);
	//imshow("window", myImage2);
	Result.create(max(myImage1.rows, myImage2.rows), max(myImage1.cols, myImage2.cols), CV_8UC(3));

	float f = 1;

	int flag;
	Result = myImage1;

	for (int x = 0; x < myImage2.rows; ++x)
	{
		for (int y = 0; y < myImage2.cols; ++y)
		{
			if (myImage2.at<Vec3b>(x, y)[2]<200)
			{
				
			}
			else
			{
				Result.at<Vec3b>(x, y)[0] = (1 - f)* myImage1.at<Vec3b>(x, y)[0] + f*myImage2.at<Vec3b>(x, y)[0];
				Result.at<Vec3b>(x, y)[1] = (1 - f)* myImage1.at<Vec3b>(x, y)[1] + f*myImage2.at<Vec3b>(x, y)[1];
				Result.at<Vec3b>(x, y)[2] = (1 - f)* myImage1.at<Vec3b>(x, y)[2] + f*myImage2.at<Vec3b>(x, y)[2];
			}

		}

	}

}


void myVignette(Mat& myImage, Mat& Result, float f)
{

	Result.create(myImage.size(), myImage.type());
	int nChannels = myImage.channels();
	Point centre;
	centre.x = myImage.rows / 2;
	centre.y = myImage.cols / 2;
	float maxDistance = sqrt((centre.x*centre.x) + (centre.y*centre.y));
	maxDistance = maxDistance;
	//cout << "image centre:" << centre.x << "," << centre.y << endl << "max distance is: " << maxDistance;
	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{

			float distance = sqrt((x - centre.x)*(x - centre.x) + (y - centre.y)*(y - centre.y));

			Result.at<Vec3b>(x, y)[0] = Truncate(myImage.at<Vec3b>(x, y)[0] * (1 - (1 + f)*(distance / maxDistance)));
			Result.at<Vec3b>(x, y)[1] = Truncate(myImage.at<Vec3b>(x, y)[1] * (1 - (1 + f)*(distance / maxDistance)));
			Result.at<Vec3b>(x, y)[2] = Truncate(myImage.at<Vec3b>(x, y)[2] * (1 - (1 + f)*(distance / maxDistance)));


		}
	}


}

/*
void myFilter1(const Mat& myImage, Mat& Result)
{
	Result.create(myImage.size(), myImage.type());

	Result = myImage;
	myVignette(Result, Result, -0.3);
	myDark(Result, Result, 20);

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Result.at<Vec3b>(x, y)[0] = 150;

		}
	}
}

void myFilter2(const Mat& myImage, Mat& Result)
{
	Result.create(myImage.size(), myImage.type());

	Result = myImage;
	myVignette(Result, Result, -0.4);

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Result.at<Vec3b>(x, y)[0] = 100;

		}
	}
}

void myFilter3(const Mat& myImage, Mat& Result)
{
	Result.create(myImage.size(), myImage.type());

	Result = myImage;
	myVignette(Result, Result, -0.2);
	//myContrast(Result, Result, 20);
	myDark(Result, Result, 10);

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Result.at<Vec3b>(x, y)[0] = 70;

			Result.at<Vec3b>(x, y)[2] = 0;

		}
	}
}

void myFilter4(const Mat& myImage, Mat& Result)
{
	Result.create(myImage.size(), myImage.type());

	Result = myImage;
	myVignette(Result, Result, -0.8);
	//myContrast(Result, Result, 20);
	//myDark(Result, Result, 10);

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Result.at<Vec3b>(x, y)[2] = 140;

		}
	}
}

void myFilter5(const Mat& myImage, Mat& Result)
{
	Result.create(myImage.size(), myImage.type());

	Result = myImage;
	//myVignette(Result, Result, -0.2);
	//myContrast(Result, Result, 20);
	//myDark(Result, Result, 10);

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Result.at<Vec3b>(x, y)[0] = 170;

		}
	}
}

void myFilter6(const Mat& myImage, Mat& Result)
{
	Result.create(myImage.size(), myImage.type());

	Result = myImage;
	myVignette(Result, Result, -0.1);
	//myContrast(Result, Result, 20);
	//myDark(Result, Result, 10);

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Vec3b bright;
			bright[0] = Truncate(Result.at<Vec3b>(x, y)[0] + 10);
			bright[1] = Truncate(Result.at<Vec3b>(x, y)[1] + 20);
			bright[2] = Truncate(Result.at<Vec3b>(x, y)[2] + 5);

			Result.at<Vec3b>(x, y)[0] = bright[0];
			Result.at<Vec3b>(x, y)[1] = bright[1];
			Result.at<Vec3b>(x, y)[2] = bright[2];

		}
	}
}

void myFilter7(const Mat& myImage, Mat& Result)
{
	Result.create(myImage.size(), myImage.type());

	Result = myImage;
	myVignette(Result, Result, -0.1);
	//myContrast(Result, Result, 20);
	//myDark(Result, Result, 10);

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Vec3b bright;
			bright[0] = Truncate(Result.at<Vec3b>(x, y)[0] + 10);
			bright[1] = Truncate(Result.at<Vec3b>(x, y)[1] + 20);
			bright[2] = Truncate(Result.at<Vec3b>(x, y)[2] + 50);

			Result.at<Vec3b>(x, y)[0] = bright[0];
			Result.at<Vec3b>(x, y)[1] = bright[1];
			Result.at<Vec3b>(x, y)[2] = bright[2];

		}
	}
}

void myFilter8(const Mat& myImage, Mat& Result)
{
	Result.create(myImage.size(), myImage.type());

	Result = myImage;
	myVignette(Result, Result, -0.1);
	myContrast(Result, Result, 20);
	//myDark(Result, Result, 10);

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Vec3b bright;
			bright[0] = Truncate(Result.at<Vec3b>(x, y)[0] + 50);
			bright[1] = Truncate(Result.at<Vec3b>(x, y)[1] + 50);
			bright[2] = Truncate(Result.at<Vec3b>(x, y)[2] + 50);

			Result.at<Vec3b>(x, y)[0] = bright[0];
			Result.at<Vec3b>(x, y)[1] = bright[1];
			Result.at<Vec3b>(x, y)[2] = bright[2];

		}
	}
}


void myFilter9(const Mat& myImage, Mat& Result)
{
	Result.create(myImage.size(), myImage.type());

	Result = myImage;
	myVignette(Result, Result, -0.4);
	//myContrast(Result, Result, 20);
	myDark(Result, Result, 60);

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Vec3b bright;
			bright[0] = Truncate(Result.at<Vec3b>(x, y)[0] + 60);
			bright[1] = Truncate(Result.at<Vec3b>(x, y)[1] + 90);
			bright[2] = Truncate(Result.at<Vec3b>(x, y)[2] + 120);

			Result.at<Vec3b>(x, y)[0] = bright[0];
			Result.at<Vec3b>(x, y)[1] = bright[1];
			Result.at<Vec3b>(x, y)[2] = bright[2];

		}
	}
}


void myFilter10(const Mat& myImage, Mat& Result)
{
	Result.create(myImage.size(), myImage.type());

	Result = myImage;
	myVignette(Result, Result, -0.7);
	myContrast(Result, Result, 20);
	myDark(Result, Result, 60);

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Vec3b bright;
			bright[0] = Truncate(Result.at<Vec3b>(x, y)[0] + 70);
			bright[1] = Truncate(Result.at<Vec3b>(x, y)[1] + 50);
			bright[2] = Truncate(Result.at<Vec3b>(x, y)[2] + 90);

			Result.at<Vec3b>(x, y)[0] = bright[0];
			Result.at<Vec3b>(x, y)[1] = bright[1];
			Result.at<Vec3b>(x, y)[2] = bright[2];

		}
	}
}

void myFilter11(const Mat& myImage, Mat& Result)
{
	Result.create(myImage.size(), myImage.type());

	Result = myImage;
	myVignette(Result, Result, -0.7);
	//myContrast(Result, Result, 20);
	myDark(Result, Result, 60);
	mySepia(Result, Result);
	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Vec3b bright;
			bright[0] = Truncate(Result.at<Vec3b>(x, y)[0] + 50);
			bright[1] = Truncate(Result.at<Vec3b>(x, y)[1] + 50);
			bright[2] = Truncate(Result.at<Vec3b>(x, y)[2] + 90);

			Result.at<Vec3b>(x, y)[0] = bright[0];
			Result.at<Vec3b>(x, y)[1] = bright[1];
			Result.at<Vec3b>(x, y)[2] = bright[2];

		}
	}
}

void myFilter12(const Mat& myImage, Mat& Result)
{
	Result.create(myImage.size(), myImage.type());

	Result = myImage;
	//myVignette(Result, Result, -0.7);
	myContrast(Result, Result, 20);
	myDark(Result, Result, 20);
	myGreyScale(Result, Result);
	myBrighten2(Result, Result);


}

void myFilter13(const Mat& myImage, Mat& Result)
{
	Result.create(myImage.size(), myImage.type());

	Result = myImage;
	myVignette(Result, Result, -0.7);
	//myContrast(Result, Result, 20);
	//myDark(Result, Result, 60);
	mySharpen(Result, Result);

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Vec3b bright;
			bright[0] = Truncate(Result.at<Vec3b>(x, y)[0] + 40);
			bright[1] = Truncate(Result.at<Vec3b>(x, y)[1] + 50);
			bright[2] = Truncate(Result.at<Vec3b>(x, y)[2] + 90);

			Result.at<Vec3b>(x, y)[0] = bright[0];
			Result.at<Vec3b>(x, y)[1] = bright[1];
			Result.at<Vec3b>(x, y)[2] = bright[2];

		}
	}
}

void myFilter14(const Mat& myImage, Mat& Result)
{
	Result.create(myImage.size(), myImage.type());

	Result = myImage;
	myVignette(Result, Result, -0.5);
	//myContrast(Result, Result, 20);
	myDark(Result, Result, 60);
	//	myBrighten2(Result, Result);

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Vec3b bright;
			bright[0] = Truncate(Result.at<Vec3b>(x, y)[0] + 10);
			bright[1] = Truncate(Result.at<Vec3b>(x, y)[1] + 90);
			bright[2] = Truncate(Result.at<Vec3b>(x, y)[2] + 90);

			Result.at<Vec3b>(x, y)[0] = bright[0];
			Result.at<Vec3b>(x, y)[1] = bright[1];
			Result.at<Vec3b>(x, y)[2] = bright[2];

		}
	}

}

void myFilter15(const Mat& myImage, Mat& Result)
{
	Result.create(myImage.size(), myImage.type());

	Result = myImage;
	//myVignette(Result, Result, -0.7);
	myContrast(Result, Result, 60);
	//myDark(Result, Result, 80);

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Vec3b bright;
			bright[0] = Truncate(Result.at<Vec3b>(x, y)[0] + 80);
			bright[1] = Truncate(Result.at<Vec3b>(x, y)[1] + 10);
			bright[2] = Truncate(Result.at<Vec3b>(x, y)[2] + 80);

			Result.at<Vec3b>(x, y)[0] = bright[0];
			Result.at<Vec3b>(x, y)[1] = bright[1];
			Result.at<Vec3b>(x, y)[2] = bright[2];

		}
	}

}

*/




class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg dialog



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCApplication1Dlg::IDD, pParent)
	, brightnessVariable(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_SLIDER2, brightnessVariable);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTONSharpen, &CMFCApplication1Dlg::OnBnClickedButtonsharpen)
	ON_BN_CLICKED(IDC_BUTTONBlur, &CMFCApplication1Dlg::OnBnClickedButtonblur)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCApplication1Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CMFCApplication1Dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CMFCApplication1Dlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CMFCApplication1Dlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CMFCApplication1Dlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CMFCApplication1Dlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CMFCApplication1Dlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CMFCApplication1Dlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CMFCApplication1Dlg::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON13, &CMFCApplication1Dlg::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON14, &CMFCApplication1Dlg::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON15, &CMFCApplication1Dlg::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON16, &CMFCApplication1Dlg::OnBnClickedButton16)
	ON_BN_CLICKED(IDC_BUTTON17, &CMFCApplication1Dlg::OnBnClickedButton17)
	ON_BN_CLICKED(IDC_BUTTON18, &CMFCApplication1Dlg::OnBnClickedButton18)
	ON_BN_CLICKED(IDC_BUTTON19, &CMFCApplication1Dlg::OnBnClickedButton19)
	ON_BN_CLICKED(IDC_BUTTON20, &CMFCApplication1Dlg::OnBnClickedButton20)
	ON_BN_CLICKED(IDC_BUTTON21, &CMFCApplication1Dlg::OnBnClickedButton21)
	ON_BN_CLICKED(IDC_BUTTON22, &CMFCApplication1Dlg::OnBnClickedButton22)
	ON_EN_CHANGE(IDC_EDIT3, &CMFCApplication1Dlg::OnEnChangeEdit3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS2, &CMFCApplication1Dlg::OnNMCustomdrawProgress2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CMFCApplication1Dlg::OnNMCustomdrawSlider2)
	ON_BN_CLICKED(IDC_BUTTONDiscard, &CMFCApplication1Dlg::OnBnClickedButtondiscard)
	ON_WM_HSCROLL()
//	ON_BN_CLICKED(IDC_SPLIT1, &CMFCApplication1Dlg::OnBnClickedSplit1)
//	ON_BN_CLICKED(IDC_MFCMENUBUTTON1, &CMFCApplication1Dlg::OnBnClickedMfcmenubutton1)
ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication1Dlg::OnBnClickedButton3)
ON_BN_CLICKED(IDC_BUTTONWatermark, &CMFCApplication1Dlg::OnBnClickedButtonwatermark)
END_MESSAGE_MAP()


// CMFCApplication1Dlg message handlers

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//Disabling buttons and controls here!

	//brightnessVariable.SetRange(0,100,true);
	//brightnessVariable.SetPos(0);
	CString display_logo;
	display_logo = "rfas.png";

	CImage image;
	image.Load(display_logo); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_logo);
	//hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);
	pPicDisplay->SetBitmap(bitmap);



	GetDlgItem(IDC_BUTTONSharpen)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTONWatermark)->EnableWindow(FALSE);

	GetDlgItem(IDC_BUTTONBlur)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON6)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON7)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON8)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON9)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON10)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON11)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON12)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON13)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON14)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON15)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON16)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON17)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON18)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON19)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON20)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON21)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON22)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTONDiscard)->EnableWindow(FALSE);
//	GetDlgItem(IDC_BUTTON10)->EnableWindow(FALSE);




	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}






void CMFCApplication1Dlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE);
	int result = dlg.DoModal();
	if (result == IDOK)
	{
		m_PicPath = dlg.GetPathName();
		discard_m_PicPath = dlg.GetPathName();
		global_m_PicPath = dlg.GetPathName();

	//	AfxMessageBox(_T("I was hereA!"));
		UpdateData(FALSE); 



		// Convert a TCHAR string to a LPCSTR
		CT2CA pszConvertedAnsiString(m_PicPath);

		// construct a std::string using the LPCSTR input
		std::string PicPath(pszConvertedAnsiString);

		Mat myImage = imread(PicPath, 3); // Read the file
		Mat resized;

		float ratio = 1;

		//if (myImage.cols > 674 || myImage.rows > 480)
		//{
		//	if (myImage.cols >= myImage.rows)
		//	{
		//		ratio = 674/myImage.cols;
		//	}
		//	if (myImage.rows > myImage.cols)
		//	{
		//		ratio = 480/myImage.rows;
		//	}
		//	
		//	Size size(ratio*myImage.size().width, ratio*myImage.size().height);

		//	//Size size(100,100);

		//	cv::resize(myImage, resized, size);

		//	AfxMessageBox(_T("Here lies the problem!"));

		//	cv::imwrite("tempResized.bmp", resized);

		//}


		cv::imwrite("tempResized.bmp", myImage);


		CString display_PicPath;
		display_PicPath = "tempResized.bmp";


		CImage image;
		image.Load(display_PicPath); // just change extension to load jpg
		CBitmap bitmap;
		bitmap.Attach(image.Detach());

		HBITMAP hbitmap = (HBITMAP)bitmap;

		CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC_AM);
		//hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);
		pPicDisplay->SetBitmap(bitmap);

		SetDlgItemText(IDC_EDIT2, m_PicPath);
		GetDlgItem(IDC_BUTTONSharpen)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTONWatermark)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTONBlur)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON6)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON7)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON8)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON9)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON10)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON11)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON12)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON13)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON14)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON15)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON16)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON17)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON18)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON19)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON20)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON21)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON22)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTONDiscard)->EnableWindow(TRUE);
	}
	

}


void CMFCApplication1Dlg::OnBnClickedButtonsharpen()
{


	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	Mat myImage = imread(PicPath, 3); // Read the file
	Mat Result;			// New Matrix

	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));	
	}

	CV_Assert(myImage.depth() == CV_8U);
	
	//imshow("original", myImage);

	mySharpen(myImage, Result);
	cv::imwrite("tempBitmap.bmp", Result);

	m_PicPath = "tempBitmap.bmp";

	CImage image;
	image.Load(m_PicPath); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC_AM);
	hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);
	pPicDisplay->SetBitmap(bitmap);


}


void CMFCApplication1Dlg::OnBnClickedButtonblur()
{
	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	Mat myImage = imread(PicPath, 3); // Read the file
	Mat Result;			// New Matrix

	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);

	myBlur(myImage, Result);
	cv::imwrite("tempBitmap.bmp", Result);

	m_PicPath = "tempBitmap.bmp";

	CImage image;
	image.Load(m_PicPath); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC_AM);
	hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);
	pPicDisplay->SetBitmap(bitmap);

}


void CMFCApplication1Dlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	Mat myImage = imread(PicPath, 3); // Read the file
	Mat Result;			// New Matrix

	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);

	myEdgeFilter(myImage, Result);
	cv::imwrite("tempBitmap.bmp", Result);

	m_PicPath = "tempBitmap.bmp";

	CImage image;
	image.Load(m_PicPath); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC_AM);
	hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);
	pPicDisplay->SetBitmap(bitmap);
}


void CMFCApplication1Dlg::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	Mat myImage = imread(PicPath, 3); // Read the file
	Mat Result;			// New Matrix

	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);

	myContrast(myImage, Result,20);
	cv::imwrite("tempBitmap.bmp", Result);

	m_PicPath = "tempBitmap.bmp";

	CImage image;
	image.Load(m_PicPath); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC_AM);
	hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);
	pPicDisplay->SetBitmap(bitmap);
}


void CMFCApplication1Dlg::OnBnClickedButton6()
{
	// TODO: Add your control notification handler code here

	CT2CA pszConvertedAnsiString(m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	Mat myImage = imread(PicPath, 3); // Read the file
	Mat Result;			// New Matrix

	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);

	myBrighten2(myImage, Result);
	cv::imwrite("tempBitmap.bmp", Result);

	m_PicPath = "tempBitmap.bmp";

	CImage image;
	image.Load(m_PicPath); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC_AM);
	hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);
	pPicDisplay->SetBitmap(bitmap);
}


void CMFCApplication1Dlg::OnBnClickedButton7()
{
	// TODO: Add your control notification handler code here
	CT2CA pszConvertedAnsiString(m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	Mat myImage = imread(PicPath, 3); // Read the file
	Mat Result;			// New Matrix

	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);

	myGreyScale(myImage, Result);
	cv::imwrite("tempBitmap.bmp", Result);

	m_PicPath = "tempBitmap.bmp";

	CImage image;
	image.Load(m_PicPath); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC_AM);
	hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);
	pPicDisplay->SetBitmap(bitmap);
}


void CMFCApplication1Dlg::OnBnClickedButton8()
{
	// TODO: Add your control notification handler code here
	CT2CA pszConvertedAnsiString(m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	Mat myImage = imread(PicPath, 3); // Read the file
	Mat Result;			// New Matrix

	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);

	mySepia(myImage, Result);
	cv::imwrite("tempBitmap.bmp", Result);

	m_PicPath = "tempBitmap.bmp";

	CImage image;
	image.Load(m_PicPath); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC_AM);
	hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);
	pPicDisplay->SetBitmap(bitmap);
}


void CMFCApplication1Dlg::OnBnClickedButton9()
{
	// TODO: Add your control notification handler code here
	CT2CA pszConvertedAnsiString(m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	Mat myImage = imread(PicPath, 3); // Read the file
	Mat Result;			// New Matrix

	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);

	myThreshold(myImage, Result);
	cv::imwrite("tempBitmap.bmp", Result);

	m_PicPath = "tempBitmap.bmp";

	CImage image;
	image.Load(m_PicPath); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC_AM);
	hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);
	pPicDisplay->SetBitmap(bitmap);
}


void CMFCApplication1Dlg::OnBnClickedButton10()
{
	// TODO: Add your control notification handler code here
	CT2CA pszConvertedAnsiString(m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	Mat myImage = imread(PicPath, 3); // Read the file
	Mat Result;			// New Matrix

	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);

	myNoiseRemover(myImage, Result);
	cv::imwrite("tempBitmap.bmp", Result);

	m_PicPath = "tempBitmap.bmp";

	CImage image;
	image.Load(m_PicPath); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC_AM);
	hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);
	pPicDisplay->SetBitmap(bitmap);
}


void CMFCApplication1Dlg::OnBnClickedButton11()
{
	// TODO: Add your control notification handler code here
	CT2CA pszConvertedAnsiString(m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	Mat myImage = imread(PicPath, 3); // Read the file
	Mat Result;			// New Matrix

	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);

	myLight(myImage, Result);
	cv::imwrite("tempBitmap.bmp", Result);

	m_PicPath = "tempBitmap.bmp";

	CImage image;
	image.Load(m_PicPath); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC_AM);
	hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);
	pPicDisplay->SetBitmap(bitmap);
}


void CMFCApplication1Dlg::OnBnClickedButton12()
{
	// TODO: Add your control notification handler code here
	CT2CA pszConvertedAnsiString(m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	Mat myImage = imread(PicPath, 3); // Read the file
	Mat Result;			// New Matrix

	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);

	myDark(myImage, Result,20);
	cv::imwrite("tempBitmap.bmp", Result);

	m_PicPath = "tempBitmap.bmp";

	CImage image;
	image.Load(m_PicPath); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC_AM);
	hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);
	pPicDisplay->SetBitmap(bitmap);
}


void CMFCApplication1Dlg::OnBnClickedButton13()
{
	// TODO: Add your control notification handler code here
	CT2CA pszConvertedAnsiString(m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	Mat myImage = imread(PicPath, 3); // Read the file
	Mat Result;			// New Matrix

	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);

	myShadowRemover(myImage, Result);
	cv::imwrite("tempBitmap.bmp", Result);

	m_PicPath = "tempBitmap.bmp";

	CImage image;
	image.Load(m_PicPath); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC_AM);
	hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);
	pPicDisplay->SetBitmap(bitmap);
}


void CMFCApplication1Dlg::OnBnClickedButton14()
{
	// TODO: Add your control notification handler code here
	CT2CA pszConvertedAnsiString(m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	Mat myImage = imread(PicPath, 3); // Read the file
	Mat Result;			// New Matrix

	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);

	myVignette(myImage, Result, 0.1);
	cv::imwrite("tempBitmap.bmp", Result);

	m_PicPath = "tempBitmap.bmp";

	CImage image;
	image.Load(m_PicPath); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC_AM);
	hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);
	pPicDisplay->SetBitmap(bitmap);
}


void CMFCApplication1Dlg::OnBnClickedButton15()
{
	// TODO: Add your control notification handler code here
	CT2CA pszConvertedAnsiString(m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	Mat myImage = imread(PicPath, 3); // Read the file
	Mat Result;			// New Matrix

	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);

	myInvert(myImage, Result);
	cv::imwrite("tempBitmap.bmp", Result);

	m_PicPath = "tempBitmap.bmp";

	CImage image;
	image.Load(m_PicPath); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC_AM);
	hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);
	pPicDisplay->SetBitmap(bitmap);
}


void CMFCApplication1Dlg::OnBnClickedButton16()
{
	// TODO: Add your control notification handler code here
	CT2CA pszConvertedAnsiString(m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	Mat myImage = imread(PicPath, 3); // Read the file
	Mat Result;			// New Matrix

	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);

	myRotateClockwise(myImage, Result);
	cv::imwrite("tempBitmap.bmp", Result);

	m_PicPath = "tempBitmap.bmp";

	CImage image;
	image.Load(m_PicPath); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC_AM);
	hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);
	pPicDisplay->SetBitmap(bitmap);
}


void CMFCApplication1Dlg::OnBnClickedButton17()
{
	// TODO: Add your control notification handler code here
	CT2CA pszConvertedAnsiString(m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	Mat myImage = imread(PicPath, 3); // Read the file
	Mat Result;			// New Matrix

	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);

	myRotateAntiClockwise(myImage, Result);
	cv::imwrite("tempBitmap.bmp", Result);

	m_PicPath = "tempBitmap.bmp";

	CImage image;
	image.Load(m_PicPath); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC_AM);
	hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);
	pPicDisplay->SetBitmap(bitmap);
}




void CMFCApplication1Dlg::OnBnClickedButton18()
{
	// TODO: Add your control notification handler code here
	CT2CA pszConvertedAnsiString(m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	Mat myImage = imread(PicPath, 3); // Read the file
	Mat Result;			// New Matrix

	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);

	myDither(myImage, Result);
	cv::imwrite("tempBitmap.bmp", Result);

	m_PicPath = "tempBitmap.bmp";

	CImage image;
	image.Load(m_PicPath); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC_AM);
	hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);
	pPicDisplay->SetBitmap(bitmap);

}


void CMFCApplication1Dlg::OnBnClickedButton19()
{
	// TODO: Add your control notification handler code here
	CT2CA pszConvertedAnsiString(m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	Mat myImage = imread(PicPath, 3); // Read the file
	Mat Result;			// New Matrix

	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);

	myFlipHorizontal(myImage, Result);
	cv::imwrite("tempBitmap.bmp", Result);

	m_PicPath = "tempBitmap.bmp";

	CImage image;
	image.Load(m_PicPath); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC_AM);
	hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);
	pPicDisplay->SetBitmap(bitmap);
}


void CMFCApplication1Dlg::OnBnClickedButton20()
{
	// TODO: Add your control notification handler code here
	CT2CA pszConvertedAnsiString(m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	Mat myImage = imread(PicPath, 3); // Read the file
	Mat Result;			// New Matrix

	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);

	myFlipVertical(myImage, Result);
	cv::imwrite("tempBitmap.bmp", Result);

	m_PicPath = "tempBitmap.bmp";

	CImage image;
	image.Load(m_PicPath); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC_AM);
	hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);
	pPicDisplay->SetBitmap(bitmap);
}


void CMFCApplication1Dlg::OnBnClickedButton21()
{
	// TODO: Add your control notification handler code here
	CT2CA pszConvertedAnsiString(m_PicPath);
	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	Mat myImage1 = imread(PicPath, 3); // Read the file


	// TODO: Add your control notification handler code here
	CFileDialog dlg1(TRUE);
	int result = dlg1.DoModal();
	if (result == IDOK)
	{
		overlay_PicPath = dlg1.GetPathName();
		UpdateData(FALSE);
	}

	CT2CA overlay_pszConvertedAnsiString(overlay_PicPath);
	// construct a std::string using the LPCSTR input
	std::string overlayPicPath(overlay_pszConvertedAnsiString);

	
	Mat myImage2 = imread(overlayPicPath, 3);

	Mat Result;			// New Matrix

	

	if (!myImage1.data && !myImage2.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}



	CV_Assert(myImage1.depth() == CV_8U && myImage2.depth() == CV_8U);

	myBlend(myImage1,myImage2, Result);

	cv::imwrite("tempBitmap.bmp", Result);

	m_PicPath = "tempBitmap.bmp";

	CImage image;
	image.Load(m_PicPath); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC_AM);
	hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);
	pPicDisplay->SetBitmap(bitmap);

	
}


void CMFCApplication1Dlg::OnBnClickedButton22()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlgSave(FALSE);
//	dlgSave.m_ofn.nMaxFile = 511;
	std::string filter = "JPEG File (*.jpg)\0*.jpg\0\0PNG File (*.png)\0*.png\0\0";
//	std::string filter = "XML Files (*.xml)| *.xml |";

	std::string title = "Save Picture As";

	dlgSave.m_ofn.lpstrFilter = convStringToLPCWSTR(filter);
	dlgSave.m_ofn.lpstrTitle = convStringToLPCWSTR(title);

	int result = dlgSave.DoModal();

	if (result == IDOK)
	{
		save_m_PicPath = dlgSave.GetPathName();
		//save_m_PicPath = dlgSave.Get
		UpdateData(FALSE);
	}

	SetDlgItemText(IDC_EDIT3, save_m_PicPath);
	
	Mat saveImage = imread("tempBitmap.bmp", 3);
	
	//imshow("ye lo", saveImage);

	CT2CA save_pszConvertedAnsiString(save_m_PicPath);
	// construct a std::string using the LPCSTR input
	std::string save_PicPath(save_pszConvertedAnsiString);
	
	cv::imwrite(save_PicPath, saveImage);
	


	
}


void CMFCApplication1Dlg::OnEnChangeEdit3()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

}


void CMFCApplication1Dlg::OnNMCustomdrawProgress2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 100;

}


void CMFCApplication1Dlg::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CMFCApplication1Dlg::OnBnClickedButtondiscard()
{
	// TODO: Add your control notification handler code here
	m_PicPath = discard_m_PicPath;
	CImage image;
	image.Load(m_PicPath); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC_AM);
	hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);
	pPicDisplay->SetBitmap(bitmap);

}


void CMFCApplication1Dlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	if (pScrollBar == (CScrollBar*)&brightnessVariable)
	{
		//int value = brightnessVariable.GetPos();
	}
	
	else
	{
		CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	}
}


void CMFCApplication1Dlg::OnBnClickedSplit1()
{
	// TODO: Add your control notification handler code here
	ShellExecute(NULL, _T("open"), _T("http://www.codexpert.ro"), NULL, NULL, SW_SHOWNORMAL);
}


void CMFCApplication1Dlg::OnBnClickedMfcmenubutton1()
{
	// TODO: Add your control notification handler code here
}


void CMFCApplication1Dlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	SecondDialog Dlg;

	Dlg.DoModal();


	
}


void CMFCApplication1Dlg::OnBnClickedButtonwatermark()
{
	// TODO: Add your control notification handler code here
	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	Mat myImage = imread(PicPath, 3); // Read the file
	Mat Result;			// New Matrix

	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);

	//mySharpen(myImage, Result);
	myBlend1(myImage,Result);
	cv::imwrite("tempBitmap.bmp", Result);

	m_PicPath = "tempBitmap.bmp";

	CImage image;
	image.Load(m_PicPath); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC_AM);
	hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);
	pPicDisplay->SetBitmap(bitmap);


}
