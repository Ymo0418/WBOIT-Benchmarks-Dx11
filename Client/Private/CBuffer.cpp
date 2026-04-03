#include "CBuffer.h"

CBuffer::CBuffer(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_pDevice{ _pDevice }, m_pContext{ _pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CBuffer::Render()
{
	if (!m_pContext)
	{
		MSG_BOX("Failed : Render _ m_pContext");
		return E_FAIL;
	}

	m_pContext->DrawIndexed(m_iNumIndex, 0, 0);

	return S_OK;
}

HRESULT CBuffer::Bind_Buffers()
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

HRESULT CBuffer::Create_Buffer(ID3D11Buffer** _ppOut)
{
	HRESULT hr = m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitialData, _ppOut);
	HRESULT hr2 = m_pDevice->GetDeviceRemovedReason();

	return hr;
}

void CBuffer::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pIB);
	Safe_Release(m_pVB);
}
