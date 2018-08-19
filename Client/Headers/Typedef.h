#ifndef Typedef_h__
#define Typedef_h__

typedef signed char			_byte;
typedef unsigned char		_ubyte;

typedef signed short		_short;
typedef unsigned short		_ushort;

typedef signed int			_int;
typedef unsigned int		_uint;

typedef signed long			_long;
typedef unsigned long		_ulong;

typedef float				_float;

typedef double				_double;

typedef bool				_bool;

typedef wchar_t				_tchar;


typedef D3DXVECTOR2			_vec2;

typedef struct tagMyVector3 : public D3DXVECTOR3
{
	tagMyVector3(void) {}
	tagMyVector3(const _float& fX, const _float& fY, const _float& fZ) :D3DXVECTOR3(fX, fY, fZ) {}
	tagMyVector3(const tagMyVector3& rhs) : D3DXVECTOR3(rhs) {}
	tagMyVector3(const D3DXVECTOR3& rhs) : D3DXVECTOR3(rhs) {}
	~tagMyVector3(void) {}

	void operator = (const tagMyVector3& Vector) 
	{
		x = Vector.x;
		y = Vector.y;
		z = Vector.z;
	}

	void operator = (const D3DXVECTOR3& Vector) 
	{
		x = Vector.x;
		y = Vector.y;
		z = Vector.z;
	}

	float Get_Length(void) 
	{
		return sqrtf(x * x + y * y + z * z);
	}

	D3DXVECTOR3 Get_Normalize(void)
	{		
		float	fDistance = sqrtf(x * x + y * y + z * z);
		return D3DXVECTOR3(x / fDistance, y / fDistance, z / fDistance);
	}

	float Get_Distance(const D3DXVECTOR3* pDest)
	{
		D3DXVECTOR3	vDir = *pDest - D3DXVECTOR3(x, y, z);
		return sqrtf(vDir.x * vDir.x + vDir.y * vDir.y + vDir.z * vDir.z);
	}
}_vec3;

typedef D3DXVECTOR4			_vec4;

typedef D3DXMATRIX			_matrix;



#endif // Typedef_h__
