#pragma once

#include "Include.h"

class CMyTerrain
{
public:
	CMyTerrain();
	~CMyTerrain();

public:
	void		Tile_Change(const D3DXVECTOR3& vPos, const BYTE& byDrawID);
	int			GetTileIndex(const D3DXVECTOR3& vPos);
	bool		Picking(const D3DXVECTOR3& vPos, const int& iIndex);
	bool		PickingDot(const D3DXVECTOR3& vPos, const int& iIndex);

public:
	void		Initialize(void);
	void		Update(void);
	void		Render(void);
	void		Render_Minimap(void);
	void		Release(void);

private:
	vector<TILE*>		m_vecTile;
};

