#pragma once

namespace Client
{
	struct WINDOW_DESC
	{
		HWND		hWnd;
		HINSTANCE	hInst;
		uint		iWinSizeX, iWinSizeY;
		bool		isWindowed;
	};
	
	struct VTXPOSTEX
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;

		static const unsigned int				iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[2];
	};

	struct VTXPOSCOL
	{
		XMFLOAT3	vPosition;
		XMFLOAT4	vColor;

		static const unsigned int				iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[2];
	};
}