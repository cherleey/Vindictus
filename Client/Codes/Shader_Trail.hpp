#include "Shader_Base.hpp"

vector				g_TransformedVertecise[22];
texture				g_BaseTexture;

sampler	BaseSampler = sampler_state
{
	texture = g_BaseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

struct VS_IN // 버텍스 버퍼에서 가져온 정점의 정보
{
	vector	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
};

struct VS_OUT // 버텍스 셰이더를 수행하고 난 이후의 정점정보
{
	vector	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
};

// 버텍스 셰이더 : 정점의 변환 + 정점의 수정
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out;

	// 정점의 변환
	// mul:행렬과 행렬의 곱을 수행하고 결과를 리턴한다.
	matrix		matWV, matWVP;

	if(0 == In.vTexUV.y)
		Out.vPosition = g_TransformedVertecise[In.vTexUV.x * 10 * 2];
	else
		Out.vPosition = g_TransformedVertecise[In.vTexUV.x * 10 * 2 + 1];

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	// 투영행렬까지 곱했다.	
	Out.vPosition = mul(vector(Out.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}


// 래스터라이즈를 수행한다. (VS_OUT 정점정보를 기반으로 픽셀을 만든다)

struct PS_IN
{	
	//vector	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector	vColor : COLOR0;
};

// 픽셀셰이더 : 픽셀의 정보를 바탕으로하여 픽셀의 최종적인 색을 결정한다.
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);		

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
		//fillmode = wireframe;

		VertexShader = compile vs_3_0 VS_MAIN(); 
		PixelShader = compile ps_3_0 PS_MAIN();
	}
	
};