#include "CPlane_CPU_Sort.h"
#include "CBuffer_Rect.h"
#include "CShader.h"
#include "CCamera.h"

CPlane_CPU_Sort::CPlane_CPU_Sort(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CPlane{ _pDevice, _pContext }
{
}

HRESULT CPlane_CPU_Sort::Initialize()
{
    CHECK_NULPTR((m_pShader = CShader::Create(m_pDevice, m_pContext,
        TEXT("../ShaderFiles/Shader_CPU.hlsl"), PARTICLE_DATA::Elements, PARTICLE_DATA::iNumElement)));
    CHECK_NULPTR((m_pBuffer = CBuffer_CPU::Create(m_pDevice, m_pContext)));

    D3D11_BUFFER_DESC Desc;
    ZeroMemory(&Desc, sizeof Desc);
    Desc.Usage = D3D11_USAGE_DYNAMIC;
    Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    Desc.ByteWidth = sizeof(m_PerFrameInfo);
    CHECK_FAILED(m_pDevice->CreateBuffer(&Desc, nullptr, &m_pPerFrameInfo_Buffer));

    return S_OK;
}

HRESULT CPlane_CPU_Sort::Render()
{
    CHECK_FAILED(m_pShader->Begin(0));

    m_pContext->GSSetConstantBuffers(0, 1, &m_pPerFrameInfo_Buffer);

    CHECK_FAILED(m_pBuffer->Render());

    return S_OK;
}

HRESULT CPlane_CPU_Sort::Ready_PerFrameInfo(float _fTimeDelta)
{
    CCamera::GetInstance()->Get_CameraMat(&m_PerFrameInfo.matView, CCamera::MAT_VIEW);
    CCamera::GetInstance()->Get_CameraMat(&m_PerFrameInfo.matProj, CCamera::MAT_PROJ);

    D3D11_MAPPED_SUBRESOURCE MappedResource;
    if (FAILED(m_pContext->Map(m_pPerFrameInfo_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource)))
        __debugbreak();

    memcpy(MappedResource.pData, &m_PerFrameInfo, sizeof PER_FRAME_INFO);

    m_pContext->Unmap(m_pPerFrameInfo_Buffer, 0);

    m_pContext->CSSetConstantBuffers(0, 1, &m_pPerFrameInfo_Buffer);

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
