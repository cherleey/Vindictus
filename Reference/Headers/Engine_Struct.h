#ifndef Engine_Struct_h__
#define Engine_Struct_h__

BEGIN(Engine)

typedef struct tagD3DXMeshContainerDerived : public D3DXMESHCONTAINER
{
	LPD3DXMESH					pOriMesh;
	LPDIRECT3DTEXTURE9*			ppTextures;	

	DWORD						dwNumBone;
	D3DXMATRIX**				ppCombinedTransformationMatrices;
	D3DXMATRIX*					pOffsetMatrices; // 부모뼈와 자식뼈를 완전이 이어주기위한
	D3DXMATRIX*					pRenderingMatrices; // 그리기위한 행렬(OffsetMatrix * CombinedMatrix)


}D3DXMESHCONTAINER_DERIVED;

typedef struct tagD3DXFrameDerived : public D3DXFRAME
{
	D3DXMATRIX			CombinedTransformationMatrix;
}D3DXFRAME_DERIVED;

typedef struct tagVertex_Cube_Color
{
	D3DXVECTOR3			vPosition;
	DWORD				dwColor;	
}VTXCUBECOL;

const unsigned long D3DFVF_VTXCUBECOL = D3DFVF_XYZ | D3DFVF_DIFFUSE;

typedef struct tagVertex_Cube_Texture
{
	D3DXVECTOR3			vPosition;
	D3DXVECTOR3			vTexUV;	
}VTXCUBETEX;

const unsigned long D3DFVF_VTXCUBETEX = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

typedef struct tagVertex_Normal_Texture
{
	D3DXVECTOR3			vPosition;
	D3DXVECTOR3			vNormal;
	D3DXVECTOR2			vTexUV;
}VTXNORTEX;

const unsigned long D3DFVF_VTXNORTEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

typedef struct tagVertex_Texture
{	
	D3DXVECTOR3			vPosition;
	D3DXVECTOR2			vTexUV;
}VTXTEX;

const unsigned long D3DFVF_VTXTEX = D3DFVF_XYZ | D3DFVF_TEX1;

typedef struct tagIndex16
{
	unsigned short _0;
	unsigned short _1;
	unsigned short _2;
}INDEX16;

typedef struct tagIndex32
{
	unsigned long _0;
	unsigned long _1;
	unsigned long _2;
}INDEX32;

END

#endif // Engine_Struct_h__
