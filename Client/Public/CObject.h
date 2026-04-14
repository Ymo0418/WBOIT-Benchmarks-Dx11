#pragma once
#include "CBase.h"

BEGIN(Client)

class CObject : public CBase
{
public:
    virtual HRESULT Initialize();
    virtual void    Update(float fTimeDelta);
    virtual void    Late_Update(float fTimeDelta);
    virtual HRESULT Render() = 0;
	bool            Get_Dead() { return m_isDead; }

protected:
	bool m_isDead = { false };

};

END