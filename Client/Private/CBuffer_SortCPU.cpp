#include "CBuffer_SortCPU.h"
#include "CCamera.h"

CBuffer_SortCPU::CBuffer_SortCPU(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    : CBuffer{ _pDevice , _pContext }
{
}

HRESULT CBuffer_SortCPU::Initialize()
{
	m_iVertexStride = sizeof(VTXPOSCOL);
	m_iNumVertex = 5;
	m_iNumVertexBuffer = 1;
	m_ePrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

	if (FAILED(Ready_VertexBuffer()))
	{
		MSG_BOX("Failed : Ready_VertexBuffer");
		return E_FAIL;
	}

	if (FAILED(Ready_PerFrameBuffer()))
	{
		MSG_BOX("Failed : Reday_PerFrameBuffer");
		return E_FAIL;
	}

	return S_OK;
}

void CBuffer_SortCPU::Update(float _fTimeDelta)
{
	CCamera::GetInstance()->Get_CameraMat(&m_PerFrameInfo.matView, CCamera::MAT_VIEW);
	CCamera::GetInstance()->Get_CameraMat(&m_PerFrameInfo.matProj, CCamera::MAT_PROJ);

	/* Update Camera Info */
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	if (SUCCEEDED(m_pContext->Map(m_pPerFrameBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource)))
	{
		memcpy(MappedResource.pData, &m_PerFrameInfo, sizeof PER_FRAME_INFO);
		m_pContext->Unmap(m_pPerFrameBuffer, 0);
	}

	/* Update Blend Sorting */
	_vector vCamPos = XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_PerFrameInfo.matView)).r[3];
	sort(m_Vtx, m_Vtx + m_iNumVertex, [&](const VTXPOSCOL& a, const VTXPOSCOL& b) {
		_vector vPosA = XMLoadFloat3(&a.vPosition);
		_vector vPosB = XMLoadFloat3(&b.vPosition);

		float fDistA = XMVectorGetX(XMVector3Length(vPosA - vCamPos));
		float fDistB = XMVectorGetX(XMVector3Length(vPosB - vCamPos));

		return fDistA >	 fDistB;
		});
	
	if (SUCCEEDED(m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource)))
	{
		memcpy(MappedResource.pData, m_Vtx, sizeof(VTXPOSCOL) * m_iNumVertex);
		m_pContext->Unmap(m_pVB, 0);		
	}
}

HRESULT CBuffer_SortCPU::Bind_Buffers()
{
	if (!m_pContext)
	{
		MSG_BOX("Failed : Bind_Buffers _ m_pContext");
		return E_FAIL;
	}

	ID3D11Buffer* pVBs[] = {
		m_pVB,
	};

	uint pVertexStrides[] = {
		m_iVertexStride,
	};

	uint pOffsets[] = {
		0,
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffer, pVBs, pVertexStrides, pOffsets);
	m_pContext->IASetPrimitiveTopology(m_ePrimitiveTopology);
	m_pContext->GSSetConstantBuffers(0, 1, &m_pPerFrameBuffer);

	return S_OK;
}

HRESULT CBuffer_SortCPU::Render()
{
	Bind_Buffers();

	m_pContext->Draw(m_iNumVertex, 0);

	return S_OK;
}

HRESULT CBuffer_SortCPU::Ready_VertexBuffer()
{
	ZeroMemory(&m_tBufferDesc, sizeof(m_tBufferDesc));

	m_tBufferDesc.ByteWidth = m_iVertexStride * m_iNumVertex;
	m_tBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_tBufferDesc.MiscFlags = 0;
	m_tBufferDesc.StructureByteStride = m_iVertexStride;

	ZeroMemory(&m_tInitialData, sizeof m_tInitialData);

	m_Vtx = new VTXPOSCOL[m_iNumVertex];

	m_Vtx[0].vPosition	= _float3(0.0f, 0.0f, -10.0f);
	m_Vtx[1].vPosition	= _float3(0.0f, 0.0f, -5.0f);
	m_Vtx[2].vPosition	= _float3(0.0f, 0.0f, 0.0f);
	m_Vtx[3].vPosition	= _float3(0.0f, 0.0f, 5.0f);
	m_Vtx[4].vPosition	= _float3(0.0f, 0.0f, 10.0f);

	m_Vtx[0].vColor		= _float4(1.0f, 0.0f, 0.0f, 0.3f);
	m_Vtx[1].vColor		= _float4(0.0f, 1.0f, 0.0f, 0.3f);
	m_Vtx[2].vColor		= _float4(0.0f, 0.0f, 1.0f, 0.3f);
	m_Vtx[3].vColor		= _float4(0.0f, 1.0f, 1.0f, 0.3f);
	m_Vtx[4].vColor		= _float4(1.0f, 1.0f, 1.0f, 0.3f);

	m_tInitialData.pSysMem = m_Vtx;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
	{
		MSG_BOX("Failed : Ready_VertexBuffer _ Create_Buffer");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CBuffer_SortCPU::Ready_PerFrameBuffer()
{
	D3D11_BUFFER_DESC Desc;
	ZeroMemory(&Desc, sizeof Desc);
	Desc.Usage = D3D11_USAGE_DYNAMIC;
	Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Desc.ByteWidth = sizeof(m_PerFrameInfo);
	CHECK_FAILED(m_pDevice->CreateBuffer(&Desc, nullptr, &m_pPerFrameBuffer));

	return S_OK;
}

CBuffer_SortCPU* CBuffer_SortCPU::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CBuffer_SortCPU* pInstance = new CBuffer_SortCPU(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CBuffer_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBuffer_SortCPU::Free()
{
	__super::Free();

	Safe_Release(m_pPerFrameBuffer);
	Safe_Delete_Array(m_Vtx);
}
