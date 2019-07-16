//=============================================================================
//
// ���b�V���t�B�[���h���� [MeshField.h]
// Author :		�r�J�R�N
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"
#include "scene.h"

#define POYGON_TEXTURENAME00 "data/TEXTURE/field.jpg"

//=============================================================================
// �N���X�̒�`
//=============================================================================
class CMeshField : public CScene  // ���h���N���X
{
public:
	CMeshField(int nPriority = 2, CScene::OBJTYPE objType = CScene::OBJTYPE_MESHFILED);
	~CMeshField();
	static CMeshField *Create(D3DXVECTOR3 pos, float width, float height, int meshX , int meshY, int nTexType);

	HRESULT Init		(void);
	void	Uninit		(void);
	void	Update		(void);
	void	Draw		(void);
	float	GetHeight	(D3DXVECTOR3 pos, D3DXVECTOR3 posold);

private:
	LPDIRECT3DTEXTURE9		m_pTexture;				// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9  m_pldxBuff;				// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX				m_mtxWorld;				// ���[���h�}�g���b�N�X
	D3DXVECTOR3				m_pos;					// �ʒu
	D3DXVECTOR3				m_rot;					// ����
	float					m_fWidth, m_fHeight;	// ���A����
	int						m_meshX, m_meshZ;		// ���A�c�̕�����
	int						m_posMAX, m_meshMAX;	// ���_�A���b�V���̍ő吔
	int						m_nTexType;				// �e�N�X�`���̔ԍ�
};
#endif