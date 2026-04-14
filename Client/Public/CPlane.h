#pragma once
#include "CObject.h"

BEGIN(Client)

class CPlane : public CObject
{
protected:
	CPlane(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CPlane() = default;

public:
	virtual HRESULT Initialize() = 0;
	virtual void	Update(float _fTimeDelta);
	virtual void    Late_Update(float fTimeDelta);
	virtual HRESULT Render() = 0;

protected:
    ID3D11Device*			m_pDevice				= { nullptr };
    ID3D11DeviceContext*	m_pContext				= { nullptr };
	
	class CShader*			m_pShader				= { nullptr };
	class CBuffer*			m_pBuffer				= { nullptr };

public:
	virtual void Free();
};

END