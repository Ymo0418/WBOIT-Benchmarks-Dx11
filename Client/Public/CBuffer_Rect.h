#pragma once
#include "CBuffer.h"

BEGIN(Client)

class CBuffer_Rect final : public CBuffer
{
private:
    CBuffer_Rect(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    virtual ~CBuffer_Rect() = default;

public:
    virtual HRESULT Initialize() override;
    virtual void    Update(float _fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    HRESULT Ready_VertexBuffer();
    HRESULT Reday_IndexBuffer();

public:
    static CBuffer_Rect* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    virtual void Free();
};
END
