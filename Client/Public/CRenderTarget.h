#pragma once
#include "CBase.h"

BEGIN(Client)

class CRenderTarget final : public CBase
{
public:
	struct RT_DESC {
		uint iWidth, iHeight;
		DXGI_FORMAT ePixelFormat;
		const _float4& vClearColor;
		bool bClear;
	};
private:
	CRenderTarget(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CRenderTarget() = default;

public:
	HRESULT Initialize(void* _pArg);
	void New_Frame();
	void Clear();
	HRESULT Bind_ShaderResource(class CShader* _pShader, const char* _pConstantName);

	ID3D11RenderTargetView*		Get_RTV() const	{ return m_pRTV; }
	ID3D11ShaderResourceView*	Get_SRV() const	{ return m_pSRV; }

#ifdef _DEBUG
public:
	HRESULT		Render(class CShader* _pShader, class CBuffer_Rect* _pVIBuffer);
private:
	XMFLOAT4X4	m_WorldMatrix = {};
#endif

private:
	ID3D11Device*				m_pDevice		= { nullptr };
	ID3D11DeviceContext*		m_pContext		= { nullptr };
	ID3D11Texture2D*			m_pTexture2D	= { nullptr };
	ID3D11ShaderResourceView*	m_pSRV			= { nullptr };
	ID3D11RenderTargetView*		m_pRTV			= { nullptr };
	
	_float4						m_vClearColor	= {};
	bool						m_bClear		= { true };

public:
	static CRenderTarget* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, void* _pArg);
	virtual void Free() override;
};

END