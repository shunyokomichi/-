//=============================================================================
//
// スコア処理 [score.h]
// Author :		荒谷由朗
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"
#include "number.h"

#define MAX_NUMBER (2)			// 桁数

//=============================================================================
// クラスの定義
//=============================================================================
class CScore : public CScene  // ←派生クラス
{
public:
	CScore();
	~CScore();
	static CScore *Create(D3DXVECTOR3 pos);

	HRESULT		Init	(void);
	void		Uninit	(void);
	void		Update	(void);
	void		Draw	(void);
	static void SetScore(int nScore);
	int			GetScore(void);
	static void AddScore(int nValue);

private:
	static CNumber			*m_apNumber[MAX_NUMBER];	// 桁数分のポインタ
	static int				m_nScore;					// スコア
	static D3DXVECTOR3		m_pos;						// ポリゴンの位置
};
#endif