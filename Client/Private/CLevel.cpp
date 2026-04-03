#include "CLevel.h"
#include "CObject.h"

CLevel::CLevel(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_pDevice(_pDevice), m_pContext(_pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CLevel::Initialize()
{
    return S_OK;
}

void CLevel::Update(float fTimeDelta)
{
	for (auto& iter : m_Object_list)
	{
		if (iter)
		{
			iter->Update(fTimeDelta);
		}
	}
}

void CLevel::Late_Update(float fTimeDelta)
{
	for (auto& iter : m_Object_list)
	{
		if (iter)
		{
			iter->Late_Update(fTimeDelta);
		}
	}
}

HRESULT CLevel::Add_Object(CObject* _pObject)
{
	if (!_pObject)
		return E_FAIL;

	m_Object_list.push_back(_pObject);

	return S_OK;
}

void CLevel::Delete_DeadObj()
{
	for (auto iter = m_Object_list.begin(); iter != m_Object_list.end(); )
	{
		if (*iter && (*iter)->Get_Dead())
		{
			Safe_Release(*iter);
			iter = m_Object_list.erase(iter);
		}
		else
			iter++;
	}
}

void CLevel::Free()
{
	__super::Free();

	for (auto& iter : m_Object_list)
		Safe_Release(iter);
	m_Object_list.clear();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
