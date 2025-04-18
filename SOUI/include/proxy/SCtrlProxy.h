#ifndef _SCTRL_PROXY_H_
#define _SCTRL_PROXY_H_

SNSBEGIN

template <class T, class Base>
class TCtrlProxy
	: public T
    , public Base
{
public:
	STDMETHOD_(long, AddRef)(THIS) OVERRIDE
	{
		return Base::AddRef();
	}
	STDMETHOD_(long, Release)(THIS) OVERRIDE
	{
		return Base::Release();
	}
	STDMETHOD_(void, OnFinalRelease)(THIS) OVERRIDE
	{
		return Base::OnFinalRelease();
	}

	STDMETHOD_(IWindow *, ToIWindow)(THIS) OVERRIDE
	{
		return (IWindow *)this;
	}

	STDMETHOD_(HRESULT, QueryInterface)(REFGUID id, IObjRef **ppRet) OVERRIDE
	{
		if (id == T::GetIID())
		{
			*ppRet = (T *)this;
			AddRef();
			return S_OK;
		}
		else
		{
			return Base::QueryInterface(id, ppRet);
		}
	}
};

SNSEND

#endif//_SCTRL_PROXY_H_