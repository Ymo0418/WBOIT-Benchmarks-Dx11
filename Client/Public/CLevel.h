#pragma once
#include "CBase.h"

BEGIN(Client)

class CLevel : public CBase
{
protected:
    CLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual ~CLevel() = default;

public:
    virtual HRESULT Initialize();
    virtual void    Update(float fTimeDelta);
    virtual void    Late_Update(float fTimeDelta);
    HRESULT         Add_Object(class CObject* _pObject);
    virtual void    Delete_DeadObj();

protected:
    ID3D11Device*           m_pDevice = { nullptr };
    ID3D11DeviceContext*    m_pContext = { nullptr };

    list<class CObject*>    m_Object_list;

public:
    virtual void    Free() override;
};

END