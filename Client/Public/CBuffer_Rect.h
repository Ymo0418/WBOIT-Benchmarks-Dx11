#pragma once
#include "CBuffer.h"

BEGIN(Client)

class CBuffer_Rect final : public CBuffer
{
protected:
    CBuffer_Rect(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    virtual ~CBuffer_Rect() = default;

public:
    virtual HRESULT Initialize() override;
    virtual void    Update(float _fTimeDelta) {};
    virtual HRESULT Render() override;
    virtual HRESULT Bind_Buffers();

protected:
    HRESULT Ready_VertexBuffer();
    HRESULT Ready_IndexBuffer();

protected:
    ID3D11Buffer*   m_pIB           = { nullptr };
    uint            m_iIndexStride  = { 0 };
    uint            m_iNumIndex     = { 0 };
    DXGI_FORMAT     m_eIndexFormat  = { };

public:
    static CBuffer_Rect* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    virtual void Free();
};


END