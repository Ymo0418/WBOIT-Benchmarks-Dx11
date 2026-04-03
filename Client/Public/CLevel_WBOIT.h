#pragma once
#include "CLevel.h"

BEGIN(Client)

class CLevel_WBOIT final : public CLevel
{
protected:
    CLevel_WBOIT(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual ~CLevel_WBOIT() = default;

public:
    virtual HRESULT Initialize();

public:
    static CLevel_WBOIT* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual void Free() override;

};

END