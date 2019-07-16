//=============================================================================
//
// �o���b�g���� [bullet.cpp]
// Author :		�r�J�R�N
//
//=============================================================================
#include "bullet.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "object.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "wall.h"

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = NULL;

//=============================================================================
//�@�R���X�g���N�^
//=============================================================================
CBullet::CBullet() : CScene3D(BULLET_PRIOTITY, CScene::OBJTYPE_BULLET)
{
	m_move		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size		= D3DXVECTOR2(0.0f, 0.0f);
	m_nLife		= 50;
	m_nCntLife	= 0;
	m_pEnemy	= NULL;
}

//=============================================================================
//�@�f�X�g���N�^
//=============================================================================
CBullet::~CBullet()
{

}

//=============================================================================
//�@��������
//=============================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR2 size, CEnemy *pEnemy)
{
	CBullet *pBullet = NULL;

	if (pBullet == NULL)
	{
		pBullet = new CBullet;
		if (pBullet != NULL)
		{
			// �I�u�W�F�N�g�N���X�̐���
			pBullet->Init();
			// �e�N�X�`�������蓖�Ă�
			pBullet->BindTexture(m_pTexture);
			// �ړ��ʂ̊��蓖��
			pBullet->m_move = move;
			// �T�C�Y�̊��蓖��
			pBullet->m_size = size;
			// �N���ł����̂�
			pBullet->m_pEnemy = pEnemy;
			// �e�̈ʒu�ƃT�C�Y���w��
			pBullet->SetPosSize(pos, size);
		}
	}

	return pBullet;
}
//===============================================================================
// �e�N�X�`���̓ǂݍ���
//===============================================================================
HRESULT CBullet::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, BULLET_TEXTURENAME00, &m_pTexture);

	return S_OK;
}
//===============================================================================
// �e�N�X�`���̔j��
//===============================================================================
void CBullet::UnLoad(void)
{
	// �e�N�X�`���[�̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
//=============================================================================
// ����������
//=============================================================================
HRESULT CBullet::Init(void)
{
	CScene3D::Init();
	// ��ނ̐ݒ�
	SetTypeNumber(TYPE_BULLET);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBullet::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CBullet::Update(void)
{
	// m_pos����
	m_pos = CScene3D::GetPosition();

	// �ړ��ʂ̑��
	m_pos += m_move;

	// �ړ��l�̃Z�b�g
	CScene3D::SetMove(m_move);
	// �ʒu�̃Z�b�g
	CScene3D::SetPosition(m_pos);

	if (m_nCntLife % 60)
	{// ���C�t�̌��Z
		m_nLife--;
		if (m_nLife < 0)
		{// 0�ɂȂ�����
			Uninit();
		}
	}
	m_nCntLife++;	// �J�E���^�[�̉��Z
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBullet::Draw(void)
{
#ifdef  _DEBUG
	//�v���C���[�̈ʒu���
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
	//�߂��̒e�ȊO������
	if (m_pos.x + 3000.0f >= PlayerPos.x && m_pos.x - 3000.0f <= PlayerPos.x)
	{
		//CScene3D::Draw();
	}
#endif
}

//=============================================================================
// �ǂ̓����蔻��
//=============================================================================
bool CBullet::CollisionBulletWall(CWall *pWall)
{
	bool bCollision = false;

	D3DXVECTOR3 BulletPos	= CScene3D::GetPosition();
	D3DXVECTOR2 BulletSize	= CScene3D::GetSize();

	D3DXVECTOR3 WallPos		= pWall->GetPosition();
	D3DXVECTOR3 WallRot		= pWall->GetRotation();
	D3DXVECTOR2 WallSize	= pWall->GetSize();

	if (WallRot.y == (0.0f * D3DX_PI))
	{// Z�����̕�
		if (BulletPos.x + BulletSize.x <= WallPos.x + WallSize.x
			&& BulletPos.x - BulletSize.x >= WallPos.x - WallSize.x)
		{// �ǂƓ���z���ɑ��݂��Ă���
			if (BulletPos.z + BulletSize.x >= WallPos.z
				&& BulletPos.z - BulletSize.x <= WallPos.z)
			{// �ǂƓ���x���ɑ��݂��Ă���
				if (BulletPos.y - BulletSize.y < WallPos.y + WallSize.y && BulletPos.y + BulletSize.y >= WallPos.y)
				{// �ǂ����Ⴂ
					Uninit();
					bCollision = true;
				}
			}
		}
	}
	else if (WallRot.y == (0.5f * D3DX_PI))
	{// X�����̕�
		if (BulletPos.x + BulletSize.x >= WallPos.x
			&& BulletPos.x - BulletSize.x <= WallPos.x)
		{// �ǂƓ���z���ɑ��݂��Ă���
			if (BulletPos.z + BulletSize.x <= WallPos.z + WallSize.x
				&& BulletPos.z - BulletSize.x >= WallPos.z - WallSize.x)
			{// �ǂƓ���x���ɑ��݂��Ă���
				if (BulletPos.y - BulletSize.y < WallPos.y + WallSize.y && BulletPos.y + BulletSize.y >= WallPos.y)
				{// �ǂ����Ⴂ
					Uninit();
					bCollision = true;
				}
			}
		}
	}

	return bCollision;
}
//=============================================================================
// �I�u�W�F�N�g�̓����蔻��
//=============================================================================
bool CBullet::CollisionBulletObje(CObject * pObje)
{
	bool bCollision = false;

	D3DXVECTOR3 BulletPos = CScene3D::GetPosition();
	D3DXVECTOR2 BulletSize = CScene3D::GetSize();

	D3DXVECTOR3 ObjePos = pObje->GetPosition();
	D3DXVECTOR3 ObjeMin = pObje->GetPosition() + pObje->VtxMin();
	D3DXVECTOR3 ObjeMax = pObje->GetPosition() + pObje->VtxMax();

	if (BulletPos.x >= ObjeMin.x && BulletPos.x <= ObjeMax.x)
	{// Z�͈͓̔��ɂ���
		if (BulletPos.z >= ObjeMin.z && BulletPos.z <= ObjeMax.z)
		{// X�͈͓̔��ɂ���
			if (!(BulletPos.y - BulletSize.y >= ObjeMax.y) && !(BulletPos.y + BulletSize.y <= ObjeMin.y))
			{// �I�u�W�F�N�g���Ƃ̓����蔻��
				if (BulletPos.x - BulletSize.x >= ObjeMax.x && BulletPos.x - BulletSize.x >= ObjeMin.x)
				{// �E���甲�����ꍇ
					Uninit();
					bCollision = true;
				}
				else if (BulletPos.x + BulletSize.x <= ObjeMin.x && BulletPos.x + BulletSize.x > ObjeMin.x)
				{// �����甲�����ꍇ
					Uninit();
					bCollision = true;
				}
				if (BulletPos.z - BulletSize.x <= ObjeMax.z && BulletPos.z - BulletSize.x >= ObjeMin.z)
				{// �����甲�����ꍇ
					Uninit();
					bCollision = true;
				}
				else if (BulletPos.z + BulletSize.x <= ObjeMin.z && BulletPos.z + BulletSize.x > ObjeMin.z)
				{// ��O���甲�����ꍇ
					Uninit();
					bCollision = true;
				}
			}
		}
	}

	return bCollision;
}

//=============================================================================
// �{���̓����蔻��
//=============================================================================
bool CBullet::CollisionBulletSearch(void)
{
	bool bCollision = false;

	D3DXVECTOR3 BulletPos = CScene3D::GetPosition();
	D3DXVECTOR2 BulletSize = CScene3D::GetSize();

	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();

	// �G�Ǝ����̋����𑪂�
	float fLength = (((PlayerPos.x - BulletPos.x) * (PlayerPos.x - BulletPos.x)) + ((PlayerPos.z - BulletPos.z) * (PlayerPos.z - BulletPos.z)));

	if (PLAYER_RADIUS * PLAYER_RADIUS > fLength)
	{// �͈͓���������
		if (BulletPos.y - BulletSize.y >= PlayerPos.y - PLAYER_DOWN_HEIGHT)
		{
			Uninit();
			bCollision = true;
		}
	}

	return bCollision;
}
