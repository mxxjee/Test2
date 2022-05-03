// MiniView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MiniView.h"
#include "Device.h"
#include "MainFrm.h"
#include "ToolView.h"





// CMiniView

IMPLEMENT_DYNCREATE(CMiniView, CView)

CMiniView::CMiniView()
{
	

}

CMiniView::~CMiniView()
{
	Safe_Delete(m_pTerrain);
}

BEGIN_MESSAGE_MAP(CMiniView, CView)
END_MESSAGE_MAP()


// CMiniView 그리기입니다.

void CMiniView::OnDraw(CDC* pDC)
{
	
	SetTimer(0, 100, nullptr);
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.

	CDevice::Get_Instance()->Render_Begin();

	// winApp -> MainFrame 얻어옴. 
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());

	//MainFrame -> MainSplitter -> MFCToolView를 얻어옴. 
	CToolView* pView = dynamic_cast<CToolView*>(pMain->m_MainSplitter.GetPane(0, 1));

	// MFCToolView -> CTerrain 얻어옴. 
	CMyTerrain* pTerrain = pView->m_pTerrain;

	pTerrain->Render_Minimap();

	Invalidate(FALSE);

	CDevice::Get_Instance()->Render_End(m_hWnd);
	
}


// CMiniView 진단입니다.

#ifdef _DEBUG
void CMiniView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMiniView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
void CMiniView::OnDestroy()
{

}
void CMiniView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	if (!m_pTerrain)
	{
		m_pTerrain = new CMyTerrain;
		m_pTerrain->Initialize();
	}
}
#endif
#endif //_DEBUG


// CMiniView 메시지 처리기입니다.
