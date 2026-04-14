#include "CLevel_WBOIT.h"
#include "CPlane_CPU_Sort.h"

CLevel_WBOIT::CLevel_WBOIT(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CLevel{ _pDevice, _pContext }
{
}

HRESULT CLevel_WBOIT::Initialize()
{
	CHECK_FAILED(Add_Object(CPlane_CPU_Sort::Create(m_pDevice, m_pContext)));

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
