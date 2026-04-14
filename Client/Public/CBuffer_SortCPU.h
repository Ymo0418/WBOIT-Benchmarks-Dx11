#pragma once
#include "CBuffer.h"

BEGIN(Client)

class CBuffer_SortCPU final : public CBuffer
{
protected:
    typedef struct tagPerFrameInfo
    {
        _float4x4	matView;
        _float4x4	matProj;
    }PER_FRAME_INFO;

protected:
    CBuffer_SortCPU(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    virtual ~CBuffer_SortCPU() = default;

public:
    virtual HRESULT Initialize() override;
    virtual void    Update(float _fTimeDelta) override;
    virtual HRESULT Render() override;

protected:
    HRESULT Ready_VertexBuffer();
    HRESULT Ready_PerFrameBuffer();
    HRESULT Bind_Buffers();

protected:
    VTXPOSCOL*          m_Vtx = { nullptr };
    PER_FRAME_INFO		m_PerFrameInfo = {};
    ID3D11Buffer*       m_pPerFrameBuffer = { nullptr };

public:
    static CBuffer_SortCPU* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    virtual void Free();
};
END
