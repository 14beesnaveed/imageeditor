// SecondDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "SecondDialog.h"
#include "afxdialogex.h"
#include "MFCApplication1Dlg.h"

#include <ostream>
#include <sstream>
#include <string>


CString global_m_PicPath;
extern CString m_PicPath;
typedef long long long641;

LPCWSTR convStringToLPCWSTR1(string String)
{
	char *cString = &String[0u];
	long641 size = strlen(cString);
	wchar_t* wString = new wchar_t[size];
	for (long641 i = 0; i < size; i++)
	{
		wString[i] = cString[i];
	}
	wString[size] = 0; //important, otherwise Windows will print whatever next in memmory until it finds a zero.
	return wString;
}


int myMax1(int x, int y, int z)
{
	if (x > y && x > z)
		return x;
	else if (y > x && y > z)
		return y;
	else if (z>x && z>y)
		return z;
}



float Truncate1(float value)
{
	if (value <= 0)
		value = 0;
	if (value >= 255)
		value = 255;

	return value;
}



void myvignette1(Mat& myImage, Mat& Result, float f)
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

			Result.at<Vec3b>(x, y)[0] = Truncate1(myImage.at<Vec3b>(x, y)[0] * (1 - (1 + f)*(distance / maxDistance)));
			Result.at<Vec3b>(x, y)[1] = Truncate1(myImage.at<Vec3b>(x, y)[1] * (1 - (1 + f)*(distance / maxDistance)));
			Result.at<Vec3b>(x, y)[2] = Truncate1(myImage.at<Vec3b>(x, y)[2] * (1 - (1 + f)*(distance / maxDistance)));


		}
	}


}

void mysharpen1(Mat& myImage, Mat& Result)
{

	Result.create(myImage.size(), myImage.type());

	for (int x = 1; x < myImage.rows - 1; ++x)
	{
		for (int y = 1; y < myImage.cols - 1; ++y)
		{

			Vec3b BGR;
			BGR[0] = Truncate1(5 * myImage.at<Vec3b>(x, y)[0] - myImage.at<Vec3b>(x - 1, y)[0] - myImage.at<Vec3b>(x + 1, y)[0]
				- myImage.at<Vec3b>(x, y + 1)[0] - myImage.at<Vec3b>(x, y - 1)[0]);
			BGR[1] = Truncate1(5 * myImage.at<Vec3b>(x, y)[1] - myImage.at<Vec3b>(x - 1, y)[1] - myImage.at<Vec3b>(x + 1, y)[1]
				- myImage.at<Vec3b>(x, y + 1)[1] - myImage.at<Vec3b>(x, y - 1)[1]);
			BGR[2] = Truncate1(5 * myImage.at<Vec3b>(x, y)[2] - myImage.at<Vec3b>(x - 1, y)[2] - myImage.at<Vec3b>(x + 1, y)[2]
				- myImage.at<Vec3b>(x, y + 1)[2] - myImage.at<Vec3b>(x, y - 1)[2]);

			Result.at<Vec3b>(x, y)[0] = BGR[0];
			Result.at<Vec3b>(x, y)[1] = BGR[1];
			Result.at<Vec3b>(x, y)[2] = BGR[2];



		}
	}

}


void myContrast1(Mat& myImage, Mat& Result, float contrast)
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

			bright[2] = Truncate1(f * (myImage.at<Vec3b>(x, y)[2] - 128) + 128);
			bright[1] = Truncate1(f * (myImage.at<Vec3b>(x, y)[1] - 128) + 128);
			bright[0] = Truncate1(f * (myImage.at<Vec3b>(x, y)[0] - 128) + 128);


			Result.at<Vec3b>(x, y)[0] = bright[0];
			Result.at<Vec3b>(x, y)[1] = bright[1];
			Result.at<Vec3b>(x, y)[2] = bright[2];

		}
	}


}


void mybrighten21(Mat& myImage, Mat& Result)
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

			bright[0] = Truncate1(f * myImage.at<Vec3b>(x, y)[0]);
			bright[1] = Truncate1(f * myImage.at<Vec3b>(x, y)[1]);
			bright[2] = Truncate1(f * myImage.at<Vec3b>(x, y)[2]);

			Result.at<Vec3b>(x, y)[0] = (bright[0]);
			Result.at<Vec3b>(x, y)[1] = (bright[1]);
			Result.at<Vec3b>(x, y)[2] = (bright[2]);

		}
	}


}



void myDark1(Mat& myImage, Mat& Result, float f)
{

	Result.create(myImage.size(), myImage.type());
	int nChannels = myImage.channels();

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Vec3b bright;
			int average = (myImage.at<Vec3b>(x, y)[0] + myImage.at<Vec3b>(x, y)[1] + myImage.at<Vec3b>(x, y)[2]) / 3;

			bright[0] = Truncate1(myImage.at<Vec3b>(x, y)[0] - f);
			bright[1] = Truncate1(myImage.at<Vec3b>(x, y)[1] - f);
			bright[2] = Truncate1(myImage.at<Vec3b>(x, y)[2] - f);

			Result.at<Vec3b>(x, y)[0] = bright[0];
			Result.at<Vec3b>(x, y)[1] = bright[1];
			Result.at<Vec3b>(x, y)[2] = bright[2];

		}
	}


}

void mygreyscale1(Mat& myImage, Mat& Result)
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


void mySepia1(Mat& myImage, Mat& Result)
{
	Result.create(myImage.size(), myImage.type());
	int nChannels = myImage.channels();

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{

			Result.at<Vec3b>(x, y)[2] = Truncate1((myImage.at<Vec3b>(x, y)[2] * .393) + (myImage.at<Vec3b>(x, y)[1] * .769) + (myImage.at<Vec3b>(x, y)[0] * .189));
			Result.at<Vec3b>(x, y)[1] = Truncate1((myImage.at<Vec3b>(x, y)[2] * .349) + (myImage.at<Vec3b>(x, y)[1] * .686) + (myImage.at<Vec3b>(x, y)[0] * .168));
			Result.at<Vec3b>(x, y)[0] = Truncate1((myImage.at<Vec3b>(x, y)[2] * .272) + (myImage.at<Vec3b>(x, y)[1] * .534) + (myImage.at<Vec3b>(x, y)[0] * .131));

		}
	}


}


void myLight1(Mat& myImage, Mat& Result)
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

			bright[0] = Truncate1(myImage.at<Vec3b>(x, y)[0] + f);
			bright[1] = Truncate1(myImage.at<Vec3b>(x, y)[1] + f);
			bright[2] = Truncate1(myImage.at<Vec3b>(x, y)[2] + f);

			Result.at<Vec3b>(x, y)[0] = (bright[0]);
			Result.at<Vec3b>(x, y)[1] = (bright[1]);
			Result.at<Vec3b>(x, y)[2] = (bright[2]);

		}
	}


}

void myFilter1(const Mat& myImage, Mat& Result)
{
	Result.create(myImage.size(), myImage.type());

	Result = myImage;
	myvignette1(Result, Result, -0.3);
	myDark1(Result, Result, 20);

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
	myvignette1(Result, Result, -0.4);

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
	myvignette1(Result, Result, -0.2);
	//myContrast1(Result, Result, 20);
	myDark1(Result, Result, 10);

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
	myvignette1(Result, Result, -0.8);
	//myContrast1(Result, Result, 20);
	//myDark1(Result, Result, 10);

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
	//myvignette1(Result, Result, -0.2);
	//myContrast1(Result, Result, 20);
	//myDark1(Result, Result, 10);

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
	myvignette1(Result, Result, -0.1);
	//myContrast1(Result, Result, 20);
	//myDark1(Result, Result, 10);

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Vec3b bright;
			bright[0] = Truncate1(Result.at<Vec3b>(x, y)[0] + 10);
			bright[1] = Truncate1(Result.at<Vec3b>(x, y)[1] + 20);
			bright[2] = Truncate1(Result.at<Vec3b>(x, y)[2] + 5);

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
	myvignette1(Result, Result, -0.1);
	//myContrast1(Result, Result, 20);
	//myDark1(Result, Result, 10);

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Vec3b bright;
			bright[0] = Truncate1(Result.at<Vec3b>(x, y)[0] + 10);
			bright[1] = Truncate1(Result.at<Vec3b>(x, y)[1] + 20);
			bright[2] = Truncate1(Result.at<Vec3b>(x, y)[2] + 50);

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
	myvignette1(Result, Result, -0.1);
	myContrast1(Result, Result, 20);
	//myDark1(Result, Result, 10);

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Vec3b bright;
			bright[0] = Truncate1(Result.at<Vec3b>(x, y)[0] + 50);
			bright[1] = Truncate1(Result.at<Vec3b>(x, y)[1] + 50);
			bright[2] = Truncate1(Result.at<Vec3b>(x, y)[2] + 50);

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
	myvignette1(Result, Result, -0.4);
	//myContrast1(Result, Result, 20);
	myDark1(Result, Result, 60);

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Vec3b bright;
			bright[0] = Truncate1(Result.at<Vec3b>(x, y)[0] + 60);
			bright[1] = Truncate1(Result.at<Vec3b>(x, y)[1] + 90);
			bright[2] = Truncate1(Result.at<Vec3b>(x, y)[2] + 120);

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
	myvignette1(Result, Result, -0.7);
	myContrast1(Result, Result, 20);
	myDark1(Result, Result, 60);

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Vec3b bright;
			bright[0] = Truncate1(Result.at<Vec3b>(x, y)[0] + 70);
			bright[1] = Truncate1(Result.at<Vec3b>(x, y)[1] + 50);
			bright[2] = Truncate1(Result.at<Vec3b>(x, y)[2] + 90);

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
	myvignette1(Result, Result, -0.7);
	//myContrast1(Result, Result, 20);
	myDark1(Result, Result, 60);
	mySepia1(Result, Result);
	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Vec3b bright;
			bright[0] = Truncate1(Result.at<Vec3b>(x, y)[0] + 50);
			bright[1] = Truncate1(Result.at<Vec3b>(x, y)[1] + 50);
			bright[2] = Truncate1(Result.at<Vec3b>(x, y)[2] + 90);

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
	//myvignette1(Result, Result, -0.7);
	myContrast1(Result, Result, 20);
	myDark1(Result, Result, 20);
	mygreyscale1(Result, Result);
	mybrighten21(Result, Result);


}

void myFilter13(const Mat& myImage, Mat& Result)
{
	Result.create(myImage.size(), myImage.type());

	Result = myImage;
	myvignette1(Result, Result, -0.7);
	//myContrast1(Result, Result, 20);
	//myDark1(Result, Result, 60);
	mysharpen1(Result, Result);

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Vec3b bright;
			bright[0] = Truncate1(Result.at<Vec3b>(x, y)[0] + 40);
			bright[1] = Truncate1(Result.at<Vec3b>(x, y)[1] + 50);
			bright[2] = Truncate1(Result.at<Vec3b>(x, y)[2] + 90);

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
	myvignette1(Result, Result, -0.5);
	//myContrast1(Result, Result, 20);
	myDark1(Result, Result, 60);
	//	mybrighten21(Result, Result);

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Vec3b bright;
			bright[0] = Truncate1(Result.at<Vec3b>(x, y)[0] + 10);
			bright[1] = Truncate1(Result.at<Vec3b>(x, y)[1] + 90);
			bright[2] = Truncate1(Result.at<Vec3b>(x, y)[2] + 90);

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
	//myvignette1(Result, Result, -0.7);
	myContrast1(Result, Result, 60);
	//myDark1(Result, Result, 80);

	for (int x = 0; x < myImage.rows; ++x)
	{
		for (int y = 0; y < myImage.cols; ++y)
		{
			Vec3b bright;
			bright[0] = Truncate1(Result.at<Vec3b>(x, y)[0] + 80);
			bright[1] = Truncate1(Result.at<Vec3b>(x, y)[1] + 10);
			bright[2] = Truncate1(Result.at<Vec3b>(x, y)[2] + 80);

			Result.at<Vec3b>(x, y)[0] = bright[0];
			Result.at<Vec3b>(x, y)[1] = bright[1];
			Result.at<Vec3b>(x, y)[2] = bright[2];

		}
	}

}






// SecondDialog dialog

IMPLEMENT_DYNAMIC(SecondDialog, CDialog)

SecondDialog::SecondDialog(CWnd* pParent /*=NULL*/)
	: CDialog(SecondDialog::IDD, pParent)
{

}

SecondDialog::~SecondDialog()
{
}



void SecondDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SecondDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &SecondDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &SecondDialog::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &SecondDialog::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &SecondDialog::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &SecondDialog::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &SecondDialog::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &SecondDialog::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &SecondDialog::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &SecondDialog::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &SecondDialog::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &SecondDialog::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &SecondDialog::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON13, &SecondDialog::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON14, &SecondDialog::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON15, &SecondDialog::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON16, &SecondDialog::OnBnClickedButton16)
END_MESSAGE_MAP()


// SecondDialog message handlers


void SecondDialog::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	
	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(global_m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	//PicPath; // = "C:/Users/Shahzeb/Documents/Visual Studio 2013/Projects/Project3/Project3/image1.jpg";
	Mat myImage = imread(PicPath, 3); // Read the file
	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}
	
	Mat Result;			// New Matrix
	//imshow("here", myImage);
	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	//AfxMessageBox(_T("Here"));

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);
	

	myFilter1(myImage, Result);
	cv::imwrite("tempBitmap1.bmp", Result);
	
	effectDisplay = "tempBitmap1.bmp";
	
	
	CImage image;
	image.Load(effectDisplay); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC);
	
	//hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), global_m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	
	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);
	
	pPicDisplay->SetBitmap(bitmap);
	

}


void SecondDialog::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here

	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(global_m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	//PicPath; // = "C:/Users/Shahzeb/Documents/Visual Studio 2013/Projects/Project3/Project3/image1.jpg";
	Mat myImage = imread(PicPath, 3); // Read the file
	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	Mat Result;			// New Matrix
	//imshow("here", myImage);
	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	//AfxMessageBox(_T("Here"));

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);


	myFilter2(myImage, Result);
	cv::imwrite("tempBitmap1.bmp", Result);

	effectDisplay = "tempBitmap1.bmp";

	CImage image;
	image.Load(effectDisplay ); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC);

	//hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), global_m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);

	pPicDisplay->SetBitmap(bitmap);
}


void SecondDialog::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here

	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(global_m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	//PicPath; // = "C:/Users/Shahzeb/Documents/Visual Studio 2013/Projects/Project3/Project3/image1.jpg";
	Mat myImage = imread(PicPath, 3); // Read the file
	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	Mat Result;			// New Matrix
	//imshow("here", myImage);
	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	//AfxMessageBox(_T("Here"));

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);


	myFilter3(myImage, Result);
	cv::imwrite("tempBitmap1.bmp", Result);

	effectDisplay = "tempBitmap1.bmp";

	CImage image;
	image.Load(effectDisplay ); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC);

	//hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), global_m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);

	pPicDisplay->SetBitmap(bitmap);
}


void SecondDialog::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here

	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(global_m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	//PicPath; // = "C:/Users/Shahzeb/Documents/Visual Studio 2013/Projects/Project3/Project3/image1.jpg";
	Mat myImage = imread(PicPath, 3); // Read the file
	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	Mat Result;			// New Matrix
	//imshow("here", myImage);
	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	//AfxMessageBox(_T("Here"));

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);


	myFilter4(myImage, Result);
	cv::imwrite("tempBitmap1.bmp", Result);

	effectDisplay = "tempBitmap1.bmp";

	CImage image;
	image.Load(effectDisplay ); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC);

	//hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), global_m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);

	pPicDisplay->SetBitmap(bitmap);
}


void SecondDialog::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here

	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(global_m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	//PicPath; // = "C:/Users/Shahzeb/Documents/Visual Studio 2013/Projects/Project3/Project3/image1.jpg";
	Mat myImage = imread(PicPath, 3); // Read the file
	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	Mat Result;			// New Matrix
	//imshow("here", myImage);
	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	//AfxMessageBox(_T("Here"));

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);


	myFilter5(myImage, Result);
	cv::imwrite("tempBitmap1.bmp", Result);

	effectDisplay = "tempBitmap1.bmp";

	CImage image;
	image.Load(effectDisplay ); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC);

	//hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), global_m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);

	pPicDisplay->SetBitmap(bitmap);
}


void SecondDialog::OnBnClickedButton6()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here

	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(global_m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	//PicPath; // = "C:/Users/Shahzeb/Documents/Visual Studio 2013/Projects/Project3/Project3/image1.jpg";
	Mat myImage = imread(PicPath, 3); // Read the file
	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	Mat Result;			// New Matrix
	//imshow("here", myImage);
	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	//AfxMessageBox(_T("Here"));

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);


	myFilter6(myImage, Result);
	cv::imwrite("tempBitmap1.bmp", Result);

	effectDisplay = "tempBitmap1.bmp";

	CImage image;
	image.Load(effectDisplay ); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC);

	//hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), global_m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);

	pPicDisplay->SetBitmap(bitmap);
}


void SecondDialog::OnBnClickedButton7()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here

	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(global_m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	//PicPath; // = "C:/Users/Shahzeb/Documents/Visual Studio 2013/Projects/Project3/Project3/image1.jpg";
	Mat myImage = imread(PicPath, 3); // Read the file
	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	Mat Result;			// New Matrix
	//imshow("here", myImage);
	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	//AfxMessageBox(_T("Here"));

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);


	myFilter7(myImage, Result);
	cv::imwrite("tempBitmap1.bmp", Result);

	effectDisplay = "tempBitmap1.bmp";

	CImage image;
	image.Load(effectDisplay ); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC);

	//hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), global_m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);

	pPicDisplay->SetBitmap(bitmap);
}


void SecondDialog::OnBnClickedButton8()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here

	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(global_m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	//PicPath; // = "C:/Users/Shahzeb/Documents/Visual Studio 2013/Projects/Project3/Project3/image1.jpg";
	Mat myImage = imread(PicPath, 3); // Read the file
	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	Mat Result;			// New Matrix
	//imshow("here", myImage);
	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	//AfxMessageBox(_T("Here"));

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);


	myFilter8(myImage, Result);
	cv::imwrite("tempBitmap1.bmp", Result);

	effectDisplay = "tempBitmap1.bmp";

	CImage image;
	image.Load(effectDisplay ); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC);

	//hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), global_m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);

	pPicDisplay->SetBitmap(bitmap);
}


void SecondDialog::OnBnClickedButton9()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here

	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(global_m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	//PicPath; // = "C:/Users/Shahzeb/Documents/Visual Studio 2013/Projects/Project3/Project3/image1.jpg";
	Mat myImage = imread(PicPath, 3); // Read the file
	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	Mat Result;			// New Matrix
	//imshow("here", myImage);
	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	//AfxMessageBox(_T("Here"));

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);


	myFilter9(myImage, Result);
	cv::imwrite("tempBitmap1.bmp", Result);

	effectDisplay = "tempBitmap1.bmp";

	CImage image;
	image.Load(effectDisplay ); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC);

	//hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), global_m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);

	pPicDisplay->SetBitmap(bitmap);
}


void SecondDialog::OnBnClickedButton10()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here

	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(global_m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	//PicPath; // = "C:/Users/Shahzeb/Documents/Visual Studio 2013/Projects/Project3/Project3/image1.jpg";
	Mat myImage = imread(PicPath, 3); // Read the file
	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	Mat Result;			// New Matrix
	//imshow("here", myImage);
	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	//AfxMessageBox(_T("Here"));

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);


	myFilter10(myImage, Result);
	cv::imwrite("tempBitmap1.bmp", Result);

	effectDisplay = "tempBitmap1.bmp";

	CImage image;
	image.Load(effectDisplay ); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC);

	//hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), global_m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);

	pPicDisplay->SetBitmap(bitmap);
}


void SecondDialog::OnBnClickedButton11()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here

	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(global_m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	//PicPath; // = "C:/Users/Shahzeb/Documents/Visual Studio 2013/Projects/Project3/Project3/image1.jpg";
	Mat myImage = imread(PicPath, 3); // Read the file
	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	Mat Result;			// New Matrix
	//imshow("here", myImage);
	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	//AfxMessageBox(_T("Here"));

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);


	myFilter11(myImage, Result);
	cv::imwrite("tempBitmap1.bmp", Result);

	effectDisplay = "tempBitmap1.bmp";

	CImage image;
	image.Load(effectDisplay ); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC);

	//hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), global_m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);

	pPicDisplay->SetBitmap(bitmap);
}


void SecondDialog::OnBnClickedButton12()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here

	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(global_m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	//PicPath; // = "C:/Users/Shahzeb/Documents/Visual Studio 2013/Projects/Project3/Project3/image1.jpg";
	Mat myImage = imread(PicPath, 3); // Read the file
	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	Mat Result;			// New Matrix
	//imshow("here", myImage);
	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	//AfxMessageBox(_T("Here"));

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);


	myFilter12(myImage, Result);
	cv::imwrite("tempBitmap1.bmp", Result);

	effectDisplay = "tempBitmap1.bmp";

	CImage image;
	image.Load(effectDisplay ); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC);

	//hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), global_m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);

	pPicDisplay->SetBitmap(bitmap);
}


void SecondDialog::OnBnClickedButton13()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here

	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(global_m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	//PicPath; // = "C:/Users/Shahzeb/Documents/Visual Studio 2013/Projects/Project3/Project3/image1.jpg";
	Mat myImage = imread(PicPath, 3); // Read the file
	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	Mat Result;			// New Matrix
	//imshow("here", myImage);
	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	//AfxMessageBox(_T("Here"));

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);


	myFilter13(myImage, Result);
	cv::imwrite("tempBitmap1.bmp", Result);

	effectDisplay = "tempBitmap1.bmp";

	CImage image;
	image.Load(effectDisplay ); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC);

	//hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), global_m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);

	pPicDisplay->SetBitmap(bitmap);
}


void SecondDialog::OnBnClickedButton14()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here

	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(global_m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	//PicPath; // = "C:/Users/Shahzeb/Documents/Visual Studio 2013/Projects/Project3/Project3/image1.jpg";
	Mat myImage = imread(PicPath, 3); // Read the file
	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	Mat Result;			// New Matrix
	//imshow("here", myImage);
	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	//AfxMessageBox(_T("Here"));

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);


	myFilter14(myImage, Result);
	cv::imwrite("tempBitmap1.bmp", Result);

	effectDisplay = "tempBitmap1.bmp";

	CImage image;
	image.Load(effectDisplay ); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC);

	//hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), global_m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);

	pPicDisplay->SetBitmap(bitmap);
}


void SecondDialog::OnBnClickedButton15()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here

	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(global_m_PicPath);

	// construct a std::string using the LPCSTR input
	std::string PicPath(pszConvertedAnsiString);

	//PicPath; // = "C:/Users/Shahzeb/Documents/Visual Studio 2013/Projects/Project3/Project3/image1.jpg";
	Mat myImage = imread(PicPath, 3); // Read the file
	if (!myImage.data) // Check for invalid input
	{
		AfxMessageBox(_T("Image could not be loaded."));
	}

	Mat Result;			// New Matrix
	//imshow("here", myImage);
	//Mat myImage2 = imread("i2.jpg", 3); // Read the file

	//AfxMessageBox(_T("Here"));

	CV_Assert(myImage.depth() == CV_8U);

	//imshow("original", myImage);


	myFilter15(myImage, Result);
	cv::imwrite("tempBitmap1.bmp", Result);

	effectDisplay = "tempBitmap1.bmp";

	CImage image;
	image.Load(effectDisplay ); // just change extension to load jpg
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	HBITMAP hbitmap = (HBITMAP)bitmap;

	CStatic* pPicDisplay = (CStatic*)GetDlgItem(IDC_STATIC);

	//hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), global_m_PicPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	pPicDisplay->ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);

	pPicDisplay->SetBitmap(bitmap);
}


void SecondDialog::OnBnClickedButton16()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlgSave(FALSE);
	//	dlgSave.m_ofn.nMaxFile = 511;
	std::string filter = "JPEG File (*.jpg)\0*.jpg\0\0PNG File (*.png)\0*.png\0\0";
	//	std::string filter = "XML Files (*.xml)| *.xml |";

	std::string title = "Save Picture As";

	dlgSave.m_ofn.lpstrFilter = convStringToLPCWSTR1(filter);
	dlgSave.m_ofn.lpstrTitle = convStringToLPCWSTR1(title);

	int result = dlgSave.DoModal();

	if (result == IDOK)
	{
		save_m_PicPath1 = dlgSave.GetPathName();
		//save_effectDisplay = dlgSave.Get
		UpdateData(FALSE);
	}

	SetDlgItemText(IDC_EDIT1, save_m_PicPath1);

	Mat saveImage = imread("tempBitmap1.bmp", 3);

	//imshow("ye lo", saveImage);

	CT2CA save_pszConvertedAnsiString(save_m_PicPath1);
	// construct a std::string using the LPCSTR input
	std::string save_PicPath(save_pszConvertedAnsiString);

	cv::imwrite(save_PicPath, saveImage);

}
