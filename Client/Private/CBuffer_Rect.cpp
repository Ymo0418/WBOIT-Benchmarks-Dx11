#include "CBuffer_Rect.h"

CBuffer_Rect::CBuffer_Rect(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    : CBuffer{ _pDevice , _pContext }
{
}

HRESULT CBuffer_Rect::Initialize()
{
	m_iVertexStride = sizeof(VTXPOSTEX);
	m_iNumVertex = 4;
	m_iNumVertexBuffer = 1;

	m_iIndexStride = sizeof(ushort);
	m_iNumIndex = 6;

	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	if (FAILED(Ready_VertexBuffer()))
	{
		MSG_BOX("Failed : Ready_VertexBuffer");
		return E_FAIL;
	}

	if (FAILED(Ready_IndexBuffer()))
	{
		MSG_BOX("Failed : Reday_IndexBuffer");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CBuffer_Rect::Ready_VertexBuffer()
{
	ZeroMemory(&m_tBufferDesc, sizeof(m_tBufferDesc));

	m_tBufferDesc.ByteWidth = m_iVertexStride * m_iNumVertex;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;
	m_tBufferDesc.StructureByteStride = m_iVertexStride;

	ZeroMemory(&m_tInitialData, sizeof m_tInitialData);

	VTXPOSTEX v[4];

	v[0].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	v[0].vTexcoord = _float2(0.f, 0.f);


	v[1].vPosition = _float3(0.5f, 0.5f, 0.5f);
	v[1].vTexcoord = _float2(1.f, 0.f);


	v[2].vPosition = _float3(0.5f, -0.5f, 0.5f);
	v[2].vTexcoord = _float2(1.f, 1.f);


	v[3].vPosition = _float3(-0.5f, -0.5f, 0.5f);
	v[3].vTexcoord = _float2(0.f, 1.f);

	m_tInitialData.pSysMem = v;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
	{
		MSG_BOX("Failed : Ready_VertexBuffer _ Create_Buffer");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CBuffer_Rect::Ready_IndexBuffer()
{
	ZeroMemory(&m_tBufferDesc, sizeof(m_tBufferDesc));

	m_tBufferDesc.ByteWidth = m_iIndexStride * m_iNumIndex;
	m_tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags = 0;
	m_tBufferDesc.MiscFlags = 0;
	m_tBufferDesc.StructureByteStride = 0;

	ZeroMemory(&m_tInitialData, sizeof m_tInitialData);

	ushort u[6];

	u[0] = 0;
	u[1] = 1;
	u[2] = 2;

	u[3] = 0;
	u[4] = 2;
	u[5] = 3;

	m_tInitialData.pSysMem = u;

	if (FAILED(Create_Buffer(&m_pIB)))
	{
		MSG_BOX("Failed : Reday_IndexBuffer _ Create_Buffer");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CBuffer_Rect::Bind_Buffers()
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
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_ePrimitiveTopology);

	return S_OK;
}

HRESULT CBuffer_Rect::Render()
{
	if (!m_pContext)
	{
		MSG_BOX("Failed : Render _ m_pContext");
		return E_FAIL;
	}

	m_pContext->DrawIndexed(m_iNumIndex, 0, 0);

	return S_OK;
}

CBuffer_Rect* CBuffer_Rect::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CBuffer_Rect* pInstance = new CBuffer_Rect(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CBuffer_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBuffer_Rect::Free()
{
	__super::Free();

	Safe_Release(m_pIB);
}