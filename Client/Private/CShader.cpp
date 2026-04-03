#include "CShader.h"

CShader::CShader(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	:m_pDevice{ _pDevice }, m_pContext{ _pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CShader::Initialize(const _tchar* _pFilePath, const D3D11_INPUT_ELEMENT_DESC* _Elements, uint _iNumElements, D3D_SHADER_MACRO* _pMacros)
{
	// 디버그 모드 일 떄 : 디버그 | 코드 최적화 제외
	// 아닐 떄 : 코드 최적화 기본값
#ifdef _DEBUG
	uint iHlslFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	uint iHlslFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif

	// FAILED 체크하면서 D3DX11CompileEffectFromFile로 m_pEffect에 셰이더 파일 주소값 받기
	if (FAILED(D3DX11CompileEffectFromFile(_pFilePath, _pMacros, D3D_COMPILE_STANDARD_FILE_INCLUDE, iHlslFlag, 0, m_pDevice, &m_pEffect, nullptr)))
	{
		MSG_BOX("Failed : D3DX11CompileEffectFromFile");
		return E_FAIL;
	}

	// ID3D11EffectTechique* pTechnique에 m_pEffect에서 GetTechniqueByIndex해오기
	ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (!pTechnique)
	{
		MSG_BOX("Failed : GetTechniqueByIndex");
		return E_FAIL;
	}

	//D3DX11_TECHNIQUE_DESC TechniqueDesc에 pTechnique를 이용하여 값 받아오기
	D3DX11_TECHNIQUE_DESC tTechniqueDesc{};
	pTechnique->GetDesc(&tTechniqueDesc);

	// TechniqueDesc에서 m_iNumPasses 값 저장
	m_iNumPasses = tTechniqueDesc.Passes;

	// m_iNumPasses만큼 반복문을 돌며 ID3DX11EffectPass* pPass 겟해와서 D3DX11_PASS_DESC로 CreateInputLayout하기, m_vecInputLayout 에 push back하기
	for (uint i = 0; i < m_iNumPasses; i++)
	{
		ID3D11InputLayout* pLayout;

		ID3DX11EffectPass* pPass = pTechnique->GetPassByIndex(i);
		if (!pPass)
		{
			MSG_BOX("Failed : GetPassByIndex");
			return E_FAIL;
		}

		D3DX11_PASS_DESC tPassDesc{};
		pPass->GetDesc(&tPassDesc);

		if (FAILED(m_pDevice->CreateInputLayout(_Elements, _iNumElements, tPassDesc.pIAInputSignature, tPassDesc.IAInputSignatureSize, &pLayout)))
		{
			MSG_BOX("Failed : CreateInputLayout");
			return E_FAIL;
		}

		m_vecInputLayout.push_back(pLayout);
	}

	return S_OK;
}

HRESULT CShader::Begin(uint _iPassIdx)
{
	if (_iPassIdx >= m_iNumPasses)
	{
		MSG_BOX("Failed : iPassIdx");
		return E_FAIL;
	}

	// IASetInputLayout
	m_pContext->IASetInputLayout(m_vecInputLayout[_iPassIdx]);

	// m_pEffect에서 ID3DX11EffectPass 가져오기 
	ID3DX11EffectPass* pPass = m_pEffect->GetTechniqueByIndex(0)->GetPassByIndex(_iPassIdx);

	if (!pPass)
	{
		MSG_BOX("Failed : pPass");
		return E_FAIL;
	}

	// pPass로 Apply 호출 (반드시 전달해야 할 모드 변수 값 설정 후 호출해야 적용됨)
	pPass->Apply(0, m_pContext);

	return S_OK;
}

HRESULT CShader::Bind_Matrix(const char* _pConstantName, const _float4x4* _pMatrix)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(_pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
	if (nullptr == pMatrixVariable)
		return E_FAIL;

	return pMatrixVariable->SetMatrix(&_pMatrix->_11);
}

HRESULT CShader::Bind_SRV(const char* _pConstantName, ID3D11ShaderResourceView* _pSRV)
{
	// m_pEffect null 체크
	if (!m_pEffect)
	{
		MSG_BOX("Failed : m_pEffect");
		return E_FAIL;
	}

	//셰이더 안에 있는 전역변수 이름으로 핸들 가져오기
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(_pConstantName);
	if (!pVariable)
	{
		MSG_BOX("Failed : Bind_SRV _ pVariable");
		return E_FAIL;
	}

	// ID3DX11EffectShaderResourceVariable* 로 형변환
	ID3DX11EffectShaderResourceVariable* pShaderVariable = pVariable->AsShaderResource();
	if (!pShaderVariable)
	{
		MSG_BOX("Failed : Bind_SRV _ pShaderVariable");
		return E_FAIL;
	}

	// SetResource() 함수에 매개변수로 pSRV 전달
	return pShaderVariable->SetResource(_pSRV);
}

CShader* CShader::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, const _tchar* _pFilePath, const D3D11_INPUT_ELEMENT_DESC* _Elements, uint _iNumElements, D3D_SHADER_MACRO* _pMacros)
{
	CShader* pInstance = new CShader(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_pFilePath, _Elements, _iNumElements, _pMacros)))
	{
		MSG_BOX("Failed To Created : CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShader::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pEffect);

	for (auto& iter : m_vecInputLayout)
		Safe_Release(iter);
	m_vecInputLayout.clear();
}
