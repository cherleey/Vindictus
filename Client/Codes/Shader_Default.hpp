#include "Shader_Base.hpp"

texture				g_BaseTexture;

sampler	BaseSampler = sampler_state
{
	texture = g_BaseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

struct VS_IN // ���ؽ� ���ۿ��� ������ ������ ����
{
	vector	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
};

struct VS_OUT // ���ؽ� ���̴��� �����ϰ� �� ������ ��������
{
	vector	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
};

// ���ؽ� ���̴� : ������ ��ȯ + ������ ����
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out;	

	// ������ ��ȯ
	// mul:��İ� ����� ���� �����ϰ� ����� �����Ѵ�.
	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	// ������ı��� ���ߴ�.	
	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}


// �����Ͷ���� �����Ѵ�. (VS_OUT ���������� ������� �ȼ��� �����)

struct PS_IN
{	
	//vector	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector	vColor : COLOR0;
};

// �ȼ����̴� : �ȼ��� ������ ���������Ͽ� �ȼ��� �������� ���� �����Ѵ�.
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);	

	//Out.vColor.gb = 0.f;

	return Out;
}


technique Default_Device
{
	pass Default_Rendering
	{
/*
		LPDIRECT3DDEVICE9		pGraphicDev;
		pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
		pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		// �������������� �����Ѵ�.
		
		Cullmode = none;
		FillMode = wireframe;
	
		alphatestenable = true;
*/
		// ��������� �����ض�.
		// ���ؽ����̴��� �����ϱ����� �������Լ��� �����Ѵ�.	
		lighting = false;
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		VertexShader = compile vs_3_0 VS_MAIN(); 
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	
};