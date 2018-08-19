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
	vector	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	vector	vWorldPos : TEXCOORD1;
	vector	vProjPos : TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0.f;

	matrix			matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));
	Out.vTexUV = In.vTexUV;
	Out.vWorldPos = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN
{	
	vector	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	vector	vWorldPos : TEXCOORD1;
	vector	vProjPos : TEXCOORD2;
};

struct PS_OUT
{
	vector	vColor : COLOR0;
	vector	vDepth : COLOR1;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0.f;

	vector		vTextureCol = tex2D(BaseSampler, In.vTexUV);

	vector		vShade = max(dot(normalize(g_vLightDir) * -1.f, In.vNormal), 0.f);

	vector		vReflect = reflect(normalize(g_vLightDir), In.vNormal);
	vector		vLook = In.vWorldPos - g_vCameraPos; 

	vector		vSpecular = pow(max(dot(normalize(vLook) * -1.f, vReflect), 0.f), g_fPower);

	Out.vColor = (vTextureCol * (g_vLightDiffuse * g_vMtrlDiffuse) + vSpecular * (g_vLightSpecular * g_vMtrlSpecular)) 
		* (vShade + (g_vLightAmbient * g_vMtrlAmbient));	

	Out.vDepth = In.vProjPos.w * 0.001f;

	return Out;	
}

PS_OUT PS_MAIN_ALPHATEST(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0.f;

	vector		vTextureCol = tex2D(BaseSampler, In.vTexUV);

	Out.vColor = vTextureCol;

	Out.vDepth = In.vProjPos.w * 0.001f;

	return Out;	
};

technique Default_Device
{
	pass Default_Rendering
	{	
		CullMode = ccw;			
		AlphaTestEnable = false;
		VertexShader = compile vs_3_0 VS_MAIN(); 
		PixelShader = compile ps_3_0 PS_MAIN();
	}	

	pass AlphaTest_Rendering
	{	
		CullMode = none;	
		Lighting = false;
		AlphaTestEnable = true;
		AlphaRef = 0x5f;
		AlphaFunc = greater;
		VertexShader = compile vs_3_0 VS_MAIN(); 
		PixelShader = compile ps_3_0 PS_MAIN_ALPHATEST();
	}	
};