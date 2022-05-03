#pragma once


// CMiniView ���Դϴ�.

#include "MyTerrain.h"

class CMiniView : public CView
{
	DECLARE_DYNCREATE(CMiniView)

protected:
	CMiniView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMiniView();

public:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
	afx_msg void OnDestroy();
	virtual void OnInitialUpdate();
#endif
#endif
private:
	CMyTerrain*				m_pTerrain;


protected:
	
	DECLARE_MESSAGE_MAP()
};


