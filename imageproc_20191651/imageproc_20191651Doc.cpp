
// imageproc_20191651Doc.cpp: Cimageproc20191651Doc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "imageproc_20191651.h"
#endif

#include "imageproc_20191651Doc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Cimageproc20191651Doc

IMPLEMENT_DYNCREATE(Cimageproc20191651Doc, CDocument)

BEGIN_MESSAGE_MAP(Cimageproc20191651Doc, CDocument)
END_MESSAGE_MAP()


// Cimageproc20191651Doc 생성/소멸

Cimageproc20191651Doc::Cimageproc20191651Doc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	InputIMG = NULL;
	InputIMG2 = NULL;
	ResultIMG = NULL;

	gResultIMG = NULL;
}

Cimageproc20191651Doc::~Cimageproc20191651Doc()
{
	int i;
	if (InputIMG != NULL)
	{
		for (i = 0; i < ImageHeight; i++)
			free(InputIMG[i]);
		free(InputIMG);
	}
	if (InputIMG2 != NULL)
	{
		for (i = 0; i < ImageHeight; i++)
			free(InputIMG2[i]);
		free(InputIMG2);
	}
	if (ResultIMG != NULL)
	{
		for (i = 0; i < ImageHeight; i++)
			free(ResultIMG[i]);
		free(ResultIMG);
	}

	if (gResultIMG != NULL)
	{
		for (i = 0; i < gImageHeight; i++)
			free(gResultIMG[i]);
		free(gResultIMG);
	}
}

BOOL Cimageproc20191651Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// Cimageproc20191651Doc serialization

void Cimageproc20191651Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
		LoadImageFile(ar);
		/*CFile* fp = ar.GetFile();
		if (fp->GetLength() == 256 * 256) ar.Read(InputIMG, 256 * 256); // GetLength() : 파일의 크기 반환 / ar.Read() : 파일에서 256*256 바이트 데이터를 읽어와서 InputIMG 배열에 저장
		else AfxMessageBox("256 x 256 크기의 화일만 사용 가능합니다.");
		*/
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void Cimageproc20191651Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void Cimageproc20191651Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void Cimageproc20191651Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Cimageproc20191651Doc 진단

#ifdef _DEBUG
void Cimageproc20191651Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void Cimageproc20191651Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Cimageproc20191651Doc 명령


void Cimageproc20191651Doc::LoadImageFile(CArchive& ar)
{
	// TODO: 여기에 구현 코드 추가.
	char type[16], buf[256];
	int i, maxValue;
	CFile* fp = ar.GetFile();// 파일 포인터 읽어오기
	CString fname = fp->GetFilePath(); // 파일 경로, 이름, 확장자 찾기
	bool isbmp = false;

	// 파일 헤더 읽기
	if (strcmp(strrchr(fname, '.'), ".ppm") == 0 || strcmp(strrchr(fname, '.'), ".PPM") == 0 ||
		strcmp(strrchr(fname, '.'), ".pgm") == 0 || strcmp(strrchr(fname, '.'), ".PGM") == 0)	// 파일 확장자만 찾기(.이 있는 위치를 찾아서 .ppm인 파일 확장자만 넘어옴)
	{
		ar.ReadString(type, 15); //P5
		do {
			ar.ReadString(buf, 255);
		} while (buf[0] == '#');
		sscanf_s(buf, "%d %d", &ImageWidth, &ImageHeight); // 문자열에서 데이터를 받는 함수 sscanf

		do {
			ar.ReadString(buf, 255);
		} while (buf[0] == '#');
		sscanf_s(buf, "%d", &maxValue);

		if (strcmp(type, "P5") == 0) depth = 1;
		else depth = 3;
	}
	else if (strcmp(strchr(fname, '.'), ".bmp") == 0 || strcmp(strchr(fname, '.'), ".BMP") == 0)
	{
		// bitmap file header읽기
		BITMAPFILEHEADER bmfh;
		ar.Read((LPSTR)&bmfh, sizeof(bmfh));
		// bmp화일임을 나타내는 "BM"마커가 있는지 확인
		if (bmfh.bfType != (WORD)(('M' << 8) | 'B')) return; // 8비트 만큼 옆으로 이동시켜 B의 아스키 코드랑 합침

		// bitmap info header읽기
		BITMAPINFOHEADER bmif;
		ar.Read((LPSTR)&bmif, sizeof(bmif));
		ImageWidth = bmif.biWidth;
		ImageHeight = bmif.biHeight;
		depth = bmif.biBitCount / 8;

		// palette처리
		if (depth == 1)
		{ // palette존재
			BYTE palette[256 * 4];
			ar.Read(palette, 256 * 4);
		}
		isbmp = true;
	}
	else if (strcmp(strchr(fname, '.'), ".raw") == 0 || strcmp(strchr(fname, '.'), ".RAW") == 0)
	{
		if (fp->GetLength() != 256 * 256) {
			AfxMessageBox("256 * 256 크기의 raw파일만 읽을 수 있습니다.");
			return;
		}

		ImageWidth = 256;
		ImageHeight = 256;
		depth = 1;
	}

	// 메모리 할당
	InputIMG = (unsigned char**)malloc(ImageHeight * sizeof(unsigned char*));
	ResultIMG = (unsigned char**)malloc(ImageHeight * sizeof(unsigned char*));

	for (i = 0; i < ImageHeight; i++) {
		InputIMG[i] = (unsigned char*)malloc(ImageWidth * depth);
		ResultIMG[i] = (unsigned char*)malloc(ImageWidth * depth);
	}

	if (!isbmp)
	{
		// 파일에 읽어서 저장
		for (i = 0; i < ImageHeight; i++)
			ar.Read(InputIMG[i], ImageWidth * depth);
	}
	else
	{
		// 파일에 읽어서 저장
		BYTE nu[4 * 3];
		int widthfile;
		widthfile = (ImageWidth * 8 + 31) / 32 * 4;
		for (i = 0; i < ImageHeight; i++)
		{
			if(depth == 1)
				ar.Read(InputIMG[ImageHeight - 1 - i], ImageWidth * depth);
			else
			{
				// BGR -> RGB // bmp파일은 BGR순서로 저장되어 있음
				BYTE r, g, b;
				for (int j = 0; j < ImageWidth; j++) {
					ar.Read(&b, 1); ar.Read(&g, 1);	ar.Read(&r, 1);
					InputIMG[ImageHeight - 1 - i][3 * j + 0] = r;
					InputIMG[ImageHeight - 1 - i][3 * j + 1] = g;
					InputIMG[ImageHeight - 1 - i][3 * j + 2] = b;
				}

			}

			if ((widthfile - ImageWidth) != 0)
				ar.Read(nu, (widthfile - ImageWidth) * depth);
		}
	}

	return;
}

void Cimageproc20191651Doc::LoadSecondImageFile(CArchive& ar)
{
	// TODO: 여기에 구현 코드 추가.
	char type[16], buf[256];
	int i, maxValue;
	int temp_w, temp_h, temp_depth;
	CFile* fp = ar.GetFile();// 파일 포인터 읽어오기
	CString fname = fp->GetFilePath(); // 파일 경로, 이름, 확장자 찾기
	bool isbmp = false;

	if (strcmp(strrchr(fname, '.'), ".ppm") == 0 || strcmp(strrchr(fname, '.'), ".PPM") == 0 ||
		strcmp(strrchr(fname, '.'), ".pgm") == 0 || strcmp(strrchr(fname, '.'), ".PGM") == 0)	// 파일 확장자만 찾기(.이 있는 위치를 찾아서 .ppm인 파일 확장자만 넘어옴)
	{
		ar.ReadString(type, 15); //P5, 타입 읽기
		do {
			ar.ReadString(buf, 255);
		} while (buf[0] == '#');
		sscanf_s(buf, "%d %d", &temp_w, &temp_h); // 문자열에서 데이터를 받는 함수 sscanf, 가로세로 크기 확인

		do {
			ar.ReadString(buf, 255);
		} while (buf[0] == '#');
		sscanf_s(buf, "%d", &maxValue);

		if (strcmp(type, "P5") == 0) temp_depth = 1;
		else temp_depth = 3;
	}
	else if (strcmp(strchr(fname, '.'), ".bmp") == 0 || strcmp(strchr(fname, '.'), ".BMP") == 0)
	{
		// bitmap file header읽기
		BITMAPFILEHEADER bmfh;
		ar.Read((LPSTR)&bmfh, sizeof(bmfh));
		// bmp화일임을 나타내는 "BM"마커가 있는지 확인
		if (bmfh.bfType != (WORD)(('M' << 8) | 'B')) return; // 8비트 만큼 옆으로 이동시켜 B의 아스키 코드랑 합침

		// bitmap info header읽기
		BITMAPINFOHEADER bmif;
		ar.Read((LPSTR)&bmif, sizeof(bmif));
		temp_w = bmif.biWidth;
		temp_h = bmif.biHeight;
		temp_depth = bmif.biBitCount / 8;

		// palette처리
		if (temp_depth == 1)
		{ // palette존재
			BYTE palette[256 * 4];
			ar.Read(palette, 256 * 4);
		}
		isbmp = true;
	}

	else if (strcmp(strrchr(fname, '.'), ".raw") == 0 || strcmp(strrchr(fname, '.'), ".RAW") == 0)
	{
		if (fp->GetLength() != 256 * 256) {
			AfxMessageBox("256 * 256 크기의 raw파일만 읽을 수 있습니다.");
			return;
		}

		temp_w = 256;
		temp_h = 256;
		temp_depth = 1;
	}

	if (ImageWidth != temp_w || ImageHeight != temp_h || depth != temp_depth) {
		AfxMessageBox("두 번째 파일의 width, height, depth가 다르면 읽을 수 없습니다.");
		return;
	}

	// 메모리 할당
	InputIMG2 = (unsigned char**)malloc(ImageHeight * sizeof(unsigned char*));

	for (i = 0; i < ImageHeight; i++)
	{
		InputIMG2[i] = (unsigned char*)malloc(ImageWidth * depth);
	}
	if (!isbmp)
	{
		// 파일에 읽어서 저장
		for (i = 0; i < ImageHeight; i++)
			ar.Read(InputIMG2[i], ImageWidth * depth);
	}
	else
	{
		BYTE nu[4 * 3];
		int widthfile;
		widthfile = (ImageWidth * 8 + 31) / 32 * 4;
		for (i = 0; i < ImageHeight; i++)
		{
			if (depth == 1)
				ar.Read(InputIMG2[ImageHeight - 1 - i], ImageWidth * depth);
			else
			{
				// BGR -> RGB // bmp파일은 BGR순서로 저장되어 있음
				BYTE r, g, b;
				for (int j = 0; j < ImageWidth; j++) {
					ar.Read(&b, 1); ar.Read(&g, 1);	ar.Read(&r, 1);
					InputIMG2[ImageHeight - 1 - i][3 * j + 0] = r;
					InputIMG2[ImageHeight - 1 - i][3 * j + 1] = g;
					InputIMG2[ImageHeight - 1 - i][3 * j + 2] = b;
				}

			}

			if ((widthfile - ImageWidth) != 0)
				ar.Read(nu, (widthfile - ImageWidth) * depth);
		}
	}
	return;
}
