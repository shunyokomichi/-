//=============================================================================
//
// ライト処理 [light.cpp]
// Author :		荒谷由朗
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "light.h"

//=============================================================================
//　コンストラクタ
//=============================================================================
CLight::CLight()
{

}

//=============================================================================
//　デストラクタ
//=============================================================================
CLight::~CLight()
{

}

//=============================================================================
//　生成処理
//=============================================================================
CLight *CLight::Create()
{
	CLight *pLight = NULL;

	if (pLight == NULL)
	{
		pLight = new CLight;
		pLight->Init();
	}

	return pLight;
}

//=============================================================================
// ライトの初期化処理
//=============================================================================
void CLight::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9		pDevice = CManager::GetRenderer()->GetDevice();

	D3DXVECTOR3 vecDir0;
	D3DXVECTOR3 vecDir1;
	D3DXVECTOR3 vecDir2;

	// ライトをクリアする
	ZeroMemory(&m_light, sizeof(D3DLIGHT9));

	// ライトの種類を設定
	m_light[0].Type = D3DLIGHT_DIRECTIONAL;
	m_light[1].Type = D3DLIGHT_DIRECTIONAL;
	m_light[2].Type = D3DLIGHT_DIRECTIONAL;

	// ライトの拡散光を設定
	m_light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_light[1].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);
	m_light[2].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

	// ライトの方向の設定
	vecDir0 = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vecDir1 = D3DXVECTOR3(-0.18f, -0.88f, -0.44f);
	vecDir2 = D3DXVECTOR3(0.89f, -0.11f, 0.44f);

	D3DXVec3Normalize(&vecDir0, &vecDir0);				// 正規化にする
	D3DXVec3Normalize(&vecDir1, &vecDir1);				// 正規化にする
	D3DXVec3Normalize(&vecDir2, &vecDir2);				// 正規化にする
	m_light[0].Direction = vecDir0;
	m_light[1].Direction = vecDir1;
	m_light[2].Direction = vecDir2;

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		// ライトを設定する
		pDevice->SetLight(nCntLight, &m_light[nCntLight]);

		// ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//=============================================================================
// ライトの終了処理
//=============================================================================
void CLight::Uninit(void)
{
}

//=============================================================================
// ライトの更新処理
//=============================================================================
void CLight::Update(void)
{
}