
// MFCApplication1.h : main header file for the PROJECT_NAME application
//
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMFCApplication1App:
// See MFCApplication1.cpp for the implementation of this class
//

class CMFCApplication1App : public CWinApp
{
public:
	CMFCApplication1App();

// Overrides
public:
	virtual BOOL InitInstance();
	

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMFCApplication1App theApp;





