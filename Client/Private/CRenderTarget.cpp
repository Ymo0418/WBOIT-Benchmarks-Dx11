#include "CRenderTarget.h"
#include "CShader.h"
#include "CBuffer_Rect.h"

CRenderTarget::CRenderTarget(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_pDevice{ _pDevice }, m_pContext{ _pContext }
{
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

HRESULT CRenderTarget::Initialize(void* _pArg)
{
	RT_DESC* pDesc = static_cast<RT_DESC*>(_pArg);

	m_vClearColor = pDesc->vClearColor;
	m_bClear = pDesc->bClear;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = pDesc->iWidth;
	TextureDesc.Height = pDesc->iHeight;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = pDesc->ePixelFormat;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pTexture2D)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateShaderResourceView(m_pTexture2D, nullptr, &m_pSRV)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateRenderTargetView(m_pTexture2D, nullptr, &m_pRTV)))
		return E_FAIL;

	Clear();

	return S_OK;
}

void CRenderTarget::New_Frame()
{
	if (m_bClear)
		Clear();
}

void CRenderTarget::Clear()
{
	m_pContext->ClearRenderTargetView(m_pRTV, (float*)&m_vClearColor);
}

HRESULT CRenderTarget::Bind_ShaderResource(CShader* _pShader, const char* _pConstantName)
{
	return _pShader->Bind_SRV(_pConstantName, m_pSRV);
}

#ifdef _DEBUG
HRESULT CRenderTarget::Render(CShader* _pShader, CBuffer_Rect* _pBuffer)
{
	if (FAILED(_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;

	if (FAILED(_pShader->Bind_SRV("g_FinalTexture", m_pSRV)))
		return E_FAIL;

	if (FAILED(_pShader->Begin(0)))
		return E_FAIL;

	if (FAILED(_pBuffer->Render()))
		return E_FAIL;

	return S_OK;
}
#endif

CRenderTarget* CRenderTarget::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, void* _pArg)
{
	CRenderTarget* pInstance = new CRenderTarget(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_pArg)))
	{
		MSG_BOX("Failed to Created : CRenderTarget");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRenderTarget::Free()
{
	__super::Free();

	Safe_Release(m_pRTV);
	Safe_Release(m_pSRV);
	Safe_Release(m_pTexture2D);

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
