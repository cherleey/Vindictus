#include "Shader_Base.hpp"

texture				g_BaseTexture;
float				g_fTimeAcc;
bool				g_bStart;
int					g_iState;

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

	/*if(!g_bStart)
		Out.vColor.a = 0.f;

	if(Out.vColor.a > 0.8f)
		Out.vColor.a /= g_fTimeAcc * 50.f;
	else if(Out.vColor.a > 0.7f)
		Out.vColor.a =  1.f;
	else
		Out.vColor.a *= g_fTimeAcc * 50.f;*/

	if(Out.vColor.a > 0.8f)
		Out.vColor.a /= g_fTimeAcc * 30.f;
	else
		Out.vColor.a /= g_fTimeAcc * 200.f;

	//Out.vColor.a /= g_fTimeAcc * 200.f;

	return Out;
}


technique Default_Device
{
	pass Default_Rendering
	{		
		lighting = false;
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = one;
		Cullmode = none;
		zwriteenable = false;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass Default_Rendering
	{		
		lighting = false;
		alphaTestenable = true;
		AlphaFunc = greater;
		Alpharef = 0x0f;

		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		VertexShader = compile vs_3_0 VS_MAIN(); 
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	
};