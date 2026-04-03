#pragma once
#include "CRenderTarget.h"

BEGIN(Client)

class CRenderer final : public CBase
{
private:
	static	CRenderer* m_pInstance;

public:
	static	CRenderer*  GetInstance();
	static	CRenderer*  GetInstance(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	static	uint		DestroyInstance();
	enum	RENDER_GROUP { RG_PRIORITY, RG_NONBLEND, RG_BLEND, RG_UI, RG_END };

private:
	CRenderer(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CRenderer() = default;
	CRenderer(const CRenderer&) = delete;
	CRenderer& operator = (const CRenderer&) = delete;

public:
	HRESULT Initialize();
	HRESULT Add_RenderGroup(RENDER_GROUP _eRenderGroup, class CObject* _pObject);
	HRESULT Render();

private:
	HRESULT Init_RenderTarget(uint _iWidth, uint _iHeight);
	HRESULT Render_Priority();
	HRESULT Render_NonBlend();
	HRESULT Render_Blend();
	HRESULT Render_Blend_Final();
	HRESULT Render_UI();
	HRESULT Add_RenderTarget(const _wstring& _strTargetTag, void* _pArg);
	HRESULT Add_MRT(const _wstring& _strMRTTag, const _wstring& _strTargetTag);
	class CRenderTarget* Find_RenderTarget(const _wstring& _strTargetTag);
	vector<class CRenderTarget*>* Find_MRT(const _wstring& _strMRTTag);
	HRESULT Begin_MRT(const _wstring& _strMRTTag, ID3D11DepthStencilView* _pDSV = nullptr, bool _bClear = true);
	HRESULT End_MRT(const _wstring& _strMRTTag);
	HRESULT Bind_SRV(class CShader* _pShader, const char* _pConstantName, const _wstring& _strTargetTag);

private:
	ID3D11Device*				m_pDevice					= { nullptr };
	ID3D11DeviceContext*		m_pContext					= { nullptr };

	ID3D11RenderTargetView*		m_pBackBufferView			= { nullptr };
	ID3D11DepthStencilView*		m_pDSV						= { nullptr };

	map<const _wstring, class CRenderTarget*>				m_RenderTargets;
	map<const _wstring, vector<class CRenderTarget*>>		m_MRTs;
	list<class CObject*>									m_RenderObject_list[RG_END]	{};

private:
	virtual void Free() override;
};

END