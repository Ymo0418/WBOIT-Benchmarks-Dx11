#pragma once
#include "CPlane.h"

BEGIN(Client)

class CPlane_WBOIT final : public CPlane
{
private:
	typedef struct tagPerFrameInfo
	{
		_float4x4	matView;
		_float4x4	matProj;
	}PER_FRAME_INFO;

protected:
	CPlane_WBOIT(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CPlane_WBOIT() = default;

public:
	virtual HRESULT		Initialize() override;
	virtual HRESULT		Render() override;

protected:
	virtual HRESULT		Ready_PerFrameInfo(float _fTimeDelta);

protected:
	PER_FRAME_INFO		m_PerFrameInfo = {};

public:
	static CPlane_WBOIT* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual void Free() override;
};

END
