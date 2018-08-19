#include "Shader_Base.hpp"

vector			g_vLightDir;
vector			g_vLightDiffuse;
vector			g_vLightAmbient;
vector			g_vLightSpecular;

vector			g_vMtrlDiffuse;
vector			g_vMtrlAmbient;
vector			g_vMtrlSpecular;
float			g_fPower = 10.f;

vector			g_vCameraPos; // In.World

texture			g_BaseTexture;

sampler	BaseSampler = sampler_state
{
	texture = g_BaseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

struct VS_IN
{
	vector	vPosition : POSITION;
	vector	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	vector	vPosition : POSITION;
	vector	vShade : COLOR0;
	vector	vSpecular : COLOR1;
	float2	vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0.f;

	matrix			matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);

	vector	vWorldNormal = mul(vector(In.vNormal.xyz, 0.f), g_matWorld);	

	Out.vShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(vWorldNormal)), 0.f);

	vector	vReflect = reflect(normalize(g_vLightDir), normalize(vWorldNormal));

	vector	vWorldPos = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);

	vector	vLook = vWorldPos - g_vCameraPos;

	Out.vSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), g_fPower);
	
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	vector	vShade : COLOR0;
	vector	vSpecular : COLOR1;
	float2	vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector	vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0.f;

	vector		vTextureCol = tex2D(BaseSampler, In.vTexUV * 20.f);

	Out.vColor = (vTextureCol * (g_vLightDiffuse * g_vMtrlDiffuse)) * (In.vShade + (g_vLightAmbient * g_vMtrlAmbient))
		+ In.vSpecular * (g_vLightSpecular * g_vMtrlSpecular);	

	return Out;	
}


technique Default_Device
{
	pass Default_Rendering
	{	
		VertexShader = compile vs_3_0 VS_MAIN(); 
		PixelShader = compile ps_3_0 PS_MAIN();
	}	
};