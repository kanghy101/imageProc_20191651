﻿// COpenCVDlg.cpp: 구현 파일
//

#include "pch.h"
#include "imageproc_20191651.h"
#include "COpenCVDlg.h"
#include "afxdialogex.h"

#include <opencv2/opencv.hpp>

// COpenCVDlg 대화 상자

IMPLEMENT_DYNAMIC(COpenCVDlg, CDialogEx)

COpenCVDlg::COpenCVDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_OPENCV, pParent)
{

}

COpenCVDlg::~COpenCVDlg()
{
}

void COpenCVDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COpenCVDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_FILE_SELECT, &COpenCVDlg::OnBnClickedButtonFileSelect)
	ON_BN_CLICKED(IDC_BUTTON_VIDEO, &COpenCVDlg::OnBnClickedButtonVideo)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA, &COpenCVDlg::OnBnClickedButtonCamera)
END_MESSAGE_MAP()


// COpenCVDlg 메시지 처리기


void COpenCVDlg::OnBnClickedButtonFileSelect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog dlg(true, "", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"JPG파일(*.jpg) | *.jpg | 모든파일(*.*) | *.* |");
	if (dlg.DoModal() == IDOK)
	{
		cv::Mat frame;
		frame = cv::imread((LPSTR)(LPCTSTR)dlg.GetPathName(), 1);

		cv::imshow("Image", frame);
		cv::waitKey(0);
	}
}


void COpenCVDlg::OnBnClickedButtonVideo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog dlg(true, "", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"AVI파일(*.avi) | *.avi | 모든파일(*.*) | *.* |");
	if (dlg.DoModal() == IDOK)
	{
		cv::VideoCapture cap; // ((LPSTR)(LPCTSTR)dlg.GetPathName());

		cap.open((LPSTR)(LPCTSTR)dlg.GetPathName());

		cv::Mat frame;
		while (1)
		{
			cap.read(frame); // 한 프레임만 잘라서 가져옴
			if (frame.empty()) // 재생종료 or 재생불가
				break;

			cv::imshow("video", frame);
			if (cv::waitKey(30) == 'q')
				break;
		}
		cv::destroyWindow("video");
	}
}


void COpenCVDlg::OnBnClickedButtonCamera()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

		cv::VideoCapture cap(0);

		cv::Mat frame;
		while (1)
		{
			cap.read(frame); // 한 프레임만 잘라서 가져옴
			if (frame.empty()) // 재생종료 or 재생불가
				break;

			cv::imshow("camera", frame);
			if (cv::waitKey(30) == 'q')
				break;
		}
		cv::destroyWindow("camera");
}
