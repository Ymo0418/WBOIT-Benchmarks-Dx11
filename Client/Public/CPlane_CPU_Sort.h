#pragma once
#include "CPlane.h"

BEGIN(Client)

class CPlane_CPU_Sort : public CPlane
{
private:
	typedef struct tagPerFrameInfo
	{
		_float4x4	matView;
		_float4x4	matProj;
	}PER_FRAME_INFO;

protected:
	CPlane_CPU_Sort(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CPlane_CPU_Sort() = default;

public:
	virtual HRESULT		Initialize() override;
	virtual HRESULT		Render() override;

protected:
	virtual HRESULT		Ready_PerFrameInfo(float _fTimeDelta) override;

protected:
	PER_FRAME_INFO		m_PerFrameInfo = {};

public:
	static CPlane_CPU_Sort* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual void Free() override;
};

END