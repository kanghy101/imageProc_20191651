
// imageproc_20191651Doc.h: Cimageproc20191651Doc 클래스의 인터페이스
//


#pragma once


class Cimageproc20191651Doc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	Cimageproc20191651Doc() noexcept;
	DECLARE_DYNCREATE(Cimageproc20191651Doc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~Cimageproc20191651Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	//unsigned char InputIMG[256][256] // [y][x] 영상은 픽셀 값들의 2차원 형태로 나열된 것이므로 2차원 배열 사용
	//unsigned char ResultIMG[256][256]

	unsigned char** InputIMG; // [y][x]
	unsigned char** InputIMG2; // [y][x]
	unsigned char** ResultIMG; // [y][x]
	
	int ImageWidth;
	int ImageHeight;
	int depth; // 1 = 흑백, 3 = 컬러

	// 기하학적 변수
	int gImageWidth;
	int gImageHeight;
	unsigned char** gResultIMG;

	void LoadImageFile(CArchive& ar);
	void LoadSecondImageFile(CArchive& ar);
};
