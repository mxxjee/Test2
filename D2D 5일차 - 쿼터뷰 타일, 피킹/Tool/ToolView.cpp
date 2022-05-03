
// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "Device.h"
#include "TextureMgr.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


HWND	g_hWnd;

// CToolView

IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CToolView ����/�Ҹ�

CToolView::CToolView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CToolView::~CToolView()
{
}

void CToolView::OnInitialUpdate()
{
	// AfxGetMainWnd : ������ ���� �����츦 ��ȯ�ϴ� ���� �Լ�
	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();

	RECT	rcWnd{};

	// GetWindowRect : ���� �������� ��Ʈ ������ ���� ������ִ� �Լ�
	pMainFrm->GetWindowRect(&rcWnd);

	// SetRect : ������ ���ڰ����� ��Ʈ�� ������ ������ִ� �Լ�
	// ���� 0,0 ��ǥ �������� �ٽ� ��Ʈ�� �����ϴ� ��
	SetRect(&rcWnd, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);

	RECT	rcMainView{};

	// GetClientRect : ���� ��â�� ��Ʈ ������ ������ �Լ�
	GetClientRect(&rcMainView);

	// �������� ����, ���� ����� ���Ѵ�
	float	fRowFrm = float(rcWnd.right - rcMainView.right);
	float	fColFrm = float(rcWnd.bottom - rcMainView.bottom);

	/*SetWindowPos : ���ڰ���� ���Ӱ� ������ ��ġ�� ũ�⸦ �����ϴ� �Լ�

	1. ��ġ�� �������� Z������ ���� ������(NULL : ���� �������� ����)
	2,3. X,Y��ǥ
	4,5. ����, ���� ũ��
	6. ��ġ�� �������� ũ�� �� ��ġ ���� �ɼ�(SWP_NOZORDER : ���� ������ �����ϰ���)*/

	pMainFrm->SetWindowPos(nullptr, 0, 0, int(WINCX + fRowFrm), int(WINCY + fColFrm), SWP_NOZORDER);
	
	CView::OnInitialUpdate();

	g_hWnd = m_hWnd;

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	if (FAILED(CDevice::Get_Instance()->Initialize()))
		AfxMessageBox(L"Device Create Failed");

	if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Cube.png", TEX_SINGLE, L"Cube")))
	{
		AfxMessageBox(L"Cube Image Insert Failed");
		return;
	}

	m_pTerrain = new CMyTerrain;
	m_pTerrain->Initialize();
	
}

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	

	CDevice::Get_Instance()->Render_Begin();

	m_pTerrain->Render();

	CDevice::Get_Instance()->Render_End();

}

void CToolView::OnDestroy()
{
	CView::OnDestroy();

	Safe_Delete(m_pTerrain);

	CTextureMgr::Get_Instance()->Destroy_Instance();
	CDevice::Get_Instance()->Destroy_Instance();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CView::OnLButtonDown(nFlags, point);

	// ������ ������ y = a(����)x + b(y����) 

	m_pTerrain->Tile_Change(D3DXVECTOR3(float(point.x), float(point.y), 0.f), 0);

	// Invalidate : ȣ�� �� �����쿡 WM_PAINT�� WM_ERASEBKGND �޼��� �߻��� ��Ŵ. �� �� �ڵ������� OnDraw�Լ��� ȣ��
	// FALSE : WM_PAINT �޽����� �߻�
	// TRUE : WM_PAINT�� WM_ERASEBKGND ���ÿ� �޼��� �߻�

	// WM_ERASEBKGND : ����� ����� �޽���

	Invalidate(FALSE);


}


#pragma region ���ʿ�

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}
// CToolView �׸���



// CToolView �μ�

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView �޽��� ó����


#pragma endregion ���ʿ�






void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CView::OnMouseMove(nFlags, point);

	if (GetAsyncKeyState(VK_LBUTTON))
	{
		m_pTerrain->Tile_Change(D3DXVECTOR3(float(point.x), float(point.y), 0.f), 0);
		Invalidate(FALSE);
	}
}
