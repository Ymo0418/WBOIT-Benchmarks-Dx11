#pragma once
#include "CPlane.h"

BEGIN(Client)

class CPlane_CPU_Sort final : public CPlane
{
protected:
	CPlane_CPU_Sort(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CPlane_CPU_Sort() = default;

public:
	virtual HRESULT		Initialize() override;
	virtual HRESULT		Render() override;

public:
	static CPlane_CPU_Sort* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual void Free() override;
};

END