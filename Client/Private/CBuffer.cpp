#include "CBuffer.h"

CBuffer::CBuffer(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_pDevice{ _pDevice }, m_pContext{ _pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CBuffer::Create_Buffer(ID3D11Buffer** _ppOut)
{
	HRESULT hr = m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitialData, _ppOut);

	return hr;
}

void CBuffer::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pVB);
}
