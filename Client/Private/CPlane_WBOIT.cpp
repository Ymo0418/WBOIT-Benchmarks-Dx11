#include "CPlane_WBOIT.h"
//#include "CBuffer_SortCPU.h"
#include "CShader.h"
#include "CCamera.h"


CPlane_WBOIT::CPlane_WBOIT(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    : CPlane{ _pDevice, _pContext }
{
}

HRESULT CPlane_WBOIT::Initialize()
{
    //CHECK_NULPTR((m_pShader = CShader::Create(m_pDevice, m_pContext,
    //    TEXT("../ShaderFiles/Shader_CPU.hlsl"), VTXPOSCOL::Elements, VTXPOSCOL::iNumElements)));
    //CHECK_NULPTR((m_pBuffer = CBuffer_SortCPU::Create(m_pDevice, m_pContext)));

    return S_OK;
}

HRESULT CPlane_WBOIT::Render()
{
    //CHECK_FAILED(m_pShader->Begin(0));
    //
    //m_pContext->GSSetConstantBuffers(0, 1, &m_pPerFrameInfo_Buffer);
    //
    //CHECK_FAILED(m_pBuffer->Render());

    return S_OK;
}

HRESULT CPlane_WBOIT::Ready_PerFrameInfo(float _fTimeDelta)
{
    return S_OK;
}

CPlane_WBOIT* CPlane_WBOIT::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
    CPlane_WBOIT* pInstance = new CPlane_WBOIT(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : CPlane_WBOIT");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPlane_WBOIT::Free()
{
    __super::Free();
}
