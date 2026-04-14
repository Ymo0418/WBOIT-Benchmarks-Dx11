#include "CPlane.h"
#include "CBuffer.h"
#include "CShader.h"
#include "CRenderer.h"

CPlane::CPlane(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    : m_pDevice{ _pDevice }, m_pContext{ _pContext }
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

void CPlane::Update(float _fTimeDelta)
{
    m_pBuffer->Update(_fTimeDelta);
}

void CPlane::Late_Update(float fTimeDelta)
{
    CRenderer::GetInstance()->Add_RenderGroup(CRenderer::RG_BLEND, this);
}

void CPlane::Free()
{
    Safe_Release(m_pShader);
    Safe_Release(m_pBuffer);
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
