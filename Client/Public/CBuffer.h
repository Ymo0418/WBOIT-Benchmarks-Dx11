#pragma once
#include "CBase.h"

BEGIN(Client)

class CBuffer abstract : public CBase
{
protected:
    CBuffer(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    virtual ~CBuffer() = default;

public:
    virtual HRESULT Initialize() = 0;
    virtual void    Update(float _fTimeDelta) = 0;
    virtual HRESULT Render();
    virtual HRESULT Bind_Buffers();

protected:
    HRESULT Create_Buffer(ID3D11Buffer** _ppOut);

protected:
    ID3D11Buffer*           m_pVB               = { nullptr };
    ID3D11Buffer*           m_pIB               = { nullptr };

    D3D11_BUFFER_DESC       m_tBufferDesc       = {};
    D3D11_SUBRESOURCE_DATA  m_tInitialData      = {};

    uint                    m_iVertexStride     = { 0 };
    uint                    m_iNumVertex        = { 0 };
    uint                    m_iNumVertexBuffer  = { 0 };

    uint                    m_iIndexStride      = { 0 };
    uint                    m_iNumIndex         = { 0 };


    DXGI_FORMAT             m_eIndexFormat          = {};
    D3D_PRIMITIVE_TOPOLOGY  m_ePrimitiveTopology    = {};

    ID3D11Device*           m_pDevice               = { nullptr };
    ID3D11DeviceContext*    m_pContext              = { nullptr };
        
public:
    virtual void Free();
};

END