#include "stdafx.h"
#include "MyTerrain.h"
#include "TextureMgr.h"
#include "Device.h"


CMyTerrain::CMyTerrain()
{
}


CMyTerrain::~CMyTerrain()
{
	Release();
}

void CMyTerrain::Tile_Change(const D3DXVECTOR3 & vPos, const BYTE & byDrawID)
{
	int		iIndex = GetTileIndex(vPos);

	if (-1 == iIndex)
		return;

	m_vecTile[iIndex]->byDrawID = byDrawID;
	m_vecTile[iIndex]->byOption = 1;
}

int CMyTerrain::GetTileIndex(const D3DXVECTOR3 & vPos)
{
	for (size_t iIndex = 0; iIndex < m_vecTile.size(); ++iIndex)
	{
		if (PickingDot(vPos, iIndex))
		{
			return iIndex;
		}
	}

	return -1;		// 피킹이 제대로 수행되지 않았을 경우 -1을 반환
}

bool CMyTerrain::Picking(const D3DXVECTOR3 & vPos, const int & iIndex)
{
	float		fGradient[4] = {

		((TILECY / 2.f) / (TILECX / 2.f)) * -1.f,
		((TILECY / 2.f) / (TILECX / 2.f)),
		((TILECY / 2.f) / (TILECX / 2.f)) * -1.f,
		((TILECY / 2.f) / (TILECX / 2.f))
	};

	// 12시, 3시, 6시, 9시 순서
	D3DXVECTOR3	vPoint[4] = {
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + (TILECY / 2.f), 0.f },
		{ m_vecTile[iIndex]->vPos.x + (TILECX / 2.f), m_vecTile[iIndex]->vPos.y, 0.f },
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - (TILECY / 2.f), 0.f },
		{ m_vecTile[iIndex]->vPos.x - (TILECX / 2.f), m_vecTile[iIndex]->vPos.y, 0.f }		
	};

	// 네 점을 대한 각각의 y절편을 구한다
	// y = ax + b => b = y - ax

	float	fB[4] = {

		vPoint[0].y - fGradient[0] * vPoint[0].x,
		vPoint[1].y - fGradient[1] * vPoint[1].x,
		vPoint[2].y - fGradient[2] * vPoint[2].x,
		vPoint[3].y - fGradient[3] * vPoint[3].x

	};

	// 0 == ax + b - y : 직선 상에 있다.
	// 0 > ax + b - y  : 직선 위에 있다.
	// 0 < ax + b - y  : 직선 아래 있다.

	bool	bCheck[4] = { false };

	if (0.f < fGradient[0] * vPos.x + fB[0] - vPos.y)
		bCheck[0] = true;
	
	if (0.f > fGradient[1] * vPos.x + fB[1] - vPos.y)
		bCheck[1] = true;

	if (0.f > fGradient[2] * vPos.x + fB[2] - vPos.y)
		bCheck[2] = true;

	if (0.f < fGradient[3] * vPos.x + fB[3] - vPos.y)
		bCheck[3] = true;

	return bCheck[0] && bCheck[1] && bCheck[2] && bCheck[3];
}

bool CMyTerrain::PickingDot(const D3DXVECTOR3& vPos, const int& iIndex)
{
	// 12시, 3시, 6시, 9시 순서
	D3DXVECTOR3	vPoint[4] = {
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + (TILECY / 2.f), 0.f },
		{ m_vecTile[iIndex]->vPos.x + (TILECX / 2.f), m_vecTile[iIndex]->vPos.y, 0.f },
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - (TILECY / 2.f), 0.f },
		{ m_vecTile[iIndex]->vPos.x - (TILECX / 2.f), m_vecTile[iIndex]->vPos.y, 0.f }
	};

	// 시계 방향으로 향하는 네 방향벡터를 구하자

	D3DXVECTOR3	vDir[4] ={

		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};

	// 각 법선 벡터를 구하자
	// (x, y)에 대한 법선벡터는 (-y, x)
	D3DXVECTOR3	vNormal[4] = {

		{ -vDir[0].y, vDir[0].x, 0.f },
		{ -vDir[1].y, vDir[1].x, 0.f },
		{ -vDir[2].y, vDir[2].x, 0.f },
		{ -vDir[3].y, vDir[3].x, 0.f }

	};
	
	
	D3DXVECTOR3	vMouseDir[4] = {

		vPos - vPoint[0],
		vPos - vPoint[1],
		vPos - vPoint[2],
		vPos - vPoint[3]
	};

	for (int i = 0; i < 4; ++i)
	{
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);
		D3DXVec3Normalize(&vMouseDir[i], &vMouseDir[i]);
	}

	for (int i = 0; i < 4; ++i)
	{
		// 예각인 경우 양수, 예각인 경우 사각형 바깥쪽을 피킹한 상황
		if (0.f < D3DXVec3Dot(&vNormal[i], &vMouseDir[i]))
			return false;
	}

	return true;
}

void CMyTerrain::Initialize(void)
{
	if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Stage/Terrain/Tile/Tile%d.png", TEX_MULTI, L"Terrain", L"Tile", 36)))
	{
		AfxMessageBox(L"Tile Image Insert Failed");
		return;
	}


	m_vecTile.reserve(TILEX * TILEY);

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			TILE*		pTile = new TILE;

			float fX = (TILECX * j) + ((i % 2) * (TILECX / 2.f));
			float fY = (TILECY / 2.f) * i;

			pTile->vPos = { fX , fY , 0.f };
			pTile->vSize = { (float)TILECX, (float)TILECY, 0.f };
			pTile->byOption = 0;
			pTile->byDrawID = 3;

			m_vecTile.push_back(pTile);

		}
	}



}

void CMyTerrain::Update(void)
{
}

void CMyTerrain::Render(void)
{
	D3DXMATRIX		matWorld, matScale, matTrans;

	TCHAR		szBuf[MIN_STR] = L"";
	int			iIndex = 0;


	for (auto iter : m_vecTile)
	{
		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		

		D3DXMatrixTranslation(&matTrans, iter->vPos.x, iter->vPos.y, iter->vPos.z);

		matWorld = matScale *  matTrans;

		swprintf_s(szBuf, L"%d", iIndex);
					
		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		const TEXINFO*		pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", iter->byDrawID);

		float		fX = pTexInfo->tImgInfo.Width / 2.f;
		float		fY = pTexInfo->tImgInfo.Height / 2.f;
				
		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, 
													nullptr,	
													&D3DXVECTOR3(fX, fY, 0.f),
													nullptr,	
													D3DCOLOR_ARGB(255, 255, 255, 255));	


		CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
			szBuf,
			lstrlen(szBuf),
			nullptr,
			0,
			D3DCOLOR_ARGB(255, 0, 0, 0));

		++iIndex;
	}

}

void CMyTerrain::Render_Minimap(void)
{
	D3DXMATRIX		matWorld, matScale, matTrans;

	TCHAR		szBuf[MIN_STR] = L"";
	int			iIndex = 0;


	for (auto iter : m_vecTile)
	{
		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixScaling(&matScale, 0.3f, 0.3f, 0.3f);


		D3DXMatrixTranslation(&matTrans, iter->vPos.x * 0.3f, iter->vPos.y* 0.3f, iter->vPos.z);

		matWorld = matScale *  matTrans;

		swprintf_s(szBuf, L"%d", iIndex);

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		const TEXINFO*		pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", iter->byDrawID);

		float		fX = pTexInfo->tImgInfo.Width / 2.f;
		float		fY = pTexInfo->tImgInfo.Height / 2.f;

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,
			&D3DXVECTOR3(fX, fY, 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));


		CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
			szBuf,
			lstrlen(szBuf),
			nullptr,
			0,
			D3DCOLOR_ARGB(255, 0, 0, 0));

		++iIndex;
	}
}

void CMyTerrain::Release(void)
{
	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();

	vector<TILE*>().swap(m_vecTile);
	//vector <TILE*>().shrink_to_fit();
}
