#include "CLevel_WBOIT.h"

CLevel_WBOIT::CLevel_WBOIT(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CLevel{ _pDevice, _pContext }
{
}

HRESULT CLevel_WBOIT::Initialize()
{
    return S_OK;
}

CLevel_WBOIT* CLevel_WBOIT::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CLevel_WBOIT* pInstance = new CLevel_WBOIT(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_WBOIT");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_WBOIT::Free()
{
    __super::Free();
}
