#include "CRenderer.h"
#include "CRenderTarget.h"
#include "CObject.h"

CRenderer* CRenderer::m_pInstance = nullptr;
CRenderer* CRenderer::GetInstance()
{
	return m_pInstance;
}
CRenderer* CRenderer::GetInstance(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	if (nullptr == m_pInstance)
	{
		m_pInstance = DBG_NEW CRenderer(_pDevice, _pContext);
		m_pInstance->Initialize();
	}
	return m_pInstance;
}
uint CRenderer::DestroyInstance()
{
	uint iRefCnt = { 0 };
	if (nullptr != m_pInstance)
	{
		iRefCnt = m_pInstance->Release();
		if (0 == iRefCnt)
			m_pInstance = nullptr;
	}

	return iRefCnt;
}

CRenderer::CRenderer(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_pDevice(_pDevice), m_pContext(_pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CRenderer::Initialize()
{
	uint iNumViewports = { 1 };
	D3D11_VIEWPORT tViewportDesc{};

	m_pContext->RSGetViewports(&iNumViewports, &tViewportDesc);

	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDER_GROUP _eRenderGroup, CObject* _pObject)
{
	if (_eRenderGroup < 0 || _eRenderGroup >= RG_END || !_pObject)
	{
		MSG_BOX("Failed : Add_RenderGroup");
		return E_FAIL;
	}

	m_RenderObject_list[_eRenderGroup].push_back(_pObject);
	Safe_AddRef(_pObject);

	return S_OK;
}

HRESULT CRenderer::Render()
{
	for (auto& iter : m_RenderTargets)
		iter.second->New_Frame();

	if (FAILED(Render_Priority()))
		return E_FAIL;

	if (FAILED(Render_NonBlend()))
		return E_FAIL;
	
	if (FAILED(Render_Blend()))
		return E_FAIL;

	if (FAILED(Render_Blend_Final()))
		return E_FAIL;
	
	if (FAILED(Render_UI()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Init_RenderTarget(uint _iWidth, uint _iHeight)
{
	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{

	for (auto& iter : m_RenderObject_list[RG_PRIORITY])
	{
		if (iter)
			iter->Render();

		Safe_Release(iter);
	}
	m_RenderObject_list[RG_PRIORITY].clear();

	return S_OK;
}

HRESULT CRenderer::Render_NonBlend()
{
	for (auto& iter : m_RenderObject_list[RG_NONBLEND])
	{
		if (iter)
			iter->Render();

		Safe_Release(iter);
	}
	m_RenderObject_list[RG_NONBLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
	//if (FAILED(Begin_MRT(TEXT("MRT_Blend"))))
	//	return E_FAIL;

	/*for (auto it = m_Buffers.begin(); it != m_Buffers.end(); ++it)
	{
		CHECK_FAILED(it->second->Begin_Shader());

		m_pContext->VSSetConstantBuffers(0, 1, &m_pPerFrameInfo_Buffer);
		m_pContext->PSSetConstantBuffers(0, 1, &m_pPerFrameInfo_Buffer);
		m_pContext->GSSetConstantBuffers(0, 1, &m_pPerFrameInfo_Buffer);
		m_pContext->CSSetSamplers(0, 1, &m_pSampler_WrapLinear);

		CHECK_FAILED(it->second->Render());
	}*/

	for (auto& iter : m_RenderObject_list[RG_BLEND])
	{
		if (iter)
			iter->Render();

		Safe_Release(iter);
	}
	m_RenderObject_list[RG_BLEND].clear();

	//if (FAILED(End_MRT(TEXT("MRT_Blend"))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Blend_Final()
{
	//if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Effect"), nullptr, false)))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Bind_RT_SRV(m_pShader, "g_NormalTexture", TEXT("Target_Accum"))))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Bind_RT_SRV(m_pShader, "g_DepthTexture", TEXT("Target_Reveal"))))
	//	return E_FAIL;
	//
	//m_pShader->Begin(13);			//Combine Accum & Reveal
	//m_pVIBuffer->Bind_Buffers();
	//m_pVIBuffer->Render();
	//
	//if (FAILED(m_pGameInstance->End_MRT(TEXT("MRT_Effect"))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	for (auto& iter : m_RenderObject_list[RG_UI])
	{
		if (iter)
			iter->Render();

		Safe_Release(iter);
	}

	m_RenderObject_list[RG_UI].clear();

	return S_OK;
}

HRESULT CRenderer::Begin_MRT(const _wstring& _strMRTTag, ID3D11DepthStencilView* _pDSV, bool _bClear)
{
	m_pContext->OMGetRenderTargets(1, &m_pBackBufferView, &m_pDSV);

	vector<CRenderTarget*>* pMRTs = Find_MRT(_strMRTTag);
	if (nullptr == pMRTs)
		return E_FAIL;

	ID3D11ShaderResourceView* pSRV[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = {
		nullptr
	};

	m_pContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pSRV);

	ID3D11RenderTargetView* RTVs[8] = { nullptr };

	uint iNumRenderTargets = (uint)pMRTs->size();

	for (uint i = 0; i < iNumRenderTargets; i++)
	{
		if (true == _bClear)
			(*pMRTs)[i]->Clear();
		RTVs[i] = (*pMRTs)[i]->Get_RTV();
	}

	m_pContext->OMSetRenderTargets(iNumRenderTargets, RTVs, nullptr != _pDSV ? _pDSV : m_pDSV);

	return S_OK;
}

HRESULT CRenderer::End_MRT(const _wstring& _strMRTTag)
{
	ID3D11RenderTargetView* pRTVs[] = {
		m_pBackBufferView,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
	};

	m_pContext->OMSetRenderTargets(8, pRTVs, m_pDSV);

	Safe_Release(m_pBackBufferView);
	Safe_Release(m_pDSV);

	return S_OK;
}

HRESULT CRenderer::Bind_SRV(CShader* _pShader, const char* _pConstantName, const _wstring& _strTargetTag)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(_strTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	return pRenderTarget->Bind_ShaderResource(_pShader, _pConstantName);
}

HRESULT CRenderer::Add_RenderTarget(const _wstring& _strTargetTag, void* _pArg)
{
	if (nullptr != Find_RenderTarget(_strTargetTag))
		return E_FAIL;

	CRenderTarget* pRenderTarget = CRenderTarget::Create(m_pDevice, m_pContext, _pArg);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	m_RenderTargets.emplace(_strTargetTag, pRenderTarget);

	return S_OK;
}

HRESULT CRenderer::Add_MRT(const _wstring& _strMRTTag, const _wstring& _strTargetTag)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(_strTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	vector<CRenderTarget*>* pMRTs = Find_MRT(_strMRTTag);

	if (nullptr == pMRTs)
	{
		vector<CRenderTarget*>		RenderTargets;

		RenderTargets.reserve(8);

		RenderTargets.push_back(pRenderTarget);

		m_MRTs.emplace(_strMRTTag, RenderTargets);
	}
	else
		pMRTs->push_back(pRenderTarget);

	Safe_AddRef(pRenderTarget);

	return S_OK;
}

CRenderTarget* CRenderer::Find_RenderTarget(const _wstring& _strTargetTag)
{
	auto	iter = m_RenderTargets.find(_strTargetTag);

	if (iter == m_RenderTargets.end())
		return nullptr;

	return iter->second;
}

vector<class CRenderTarget*>* CRenderer::Find_MRT(const _wstring& _strMRTTag)
{
	auto iter = m_MRTs.find(_strMRTTag);

	if (iter == m_MRTs.end())
		return nullptr;

	return &iter->second;
}

void CRenderer::Free()
{
	__super::Free();

	for (auto& Pair : m_MRTs)
	{
		for (auto& pRenderTarget : Pair.second)
			Safe_Release(pRenderTarget);

		Pair.second.clear();
	}
	m_MRTs.clear();


	for (auto& Pair : m_RenderTargets)
		Safe_Release(Pair.second);
	m_RenderTargets.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
