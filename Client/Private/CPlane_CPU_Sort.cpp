#include "CPlane_CPU_Sort.h"
#include "CBuffer_SortCPU.h"
#include "CShader.h"
#include "CCamera.h"

CPlane_CPU_Sort::CPlane_CPU_Sort(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CPlane{ _pDevice, _pContext }
{
}

HRESULT CPlane_CPU_Sort::Initialize()
{
    CHECK_NULPTR((m_pShader = CShader::Create(m_pDevice, m_pContext,
        TEXT("../ShaderFiles/Shader_CPU.hlsl"), VTXPOSCOL::Elements, VTXPOSCOL::iNumElements)));
    CHECK_NULPTR((m_pBuffer = CBuffer_SortCPU::Create(m_pDevice, m_pContext)));

    return S_OK;
}

HRESULT CPlane_CPU_Sort::Render()
{
    CHECK_FAILED(m_pShader->Begin(0));

    CHECK_FAILED(m_pBuffer->Render());

    return S_OK;
}

CPlane_CPU_Sort* CPlane_CPU_Sort::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
    CPlane_CPU_Sort* pInstance = new CPlane_CPU_Sort(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : CPlane_Sort");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPlane_CPU_Sort::Free()
{
	__super::Free();
}
