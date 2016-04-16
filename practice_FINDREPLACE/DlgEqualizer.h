#pragma once
#include "include.h"
class DlgEqualizer
{
private:
	HWND hDlg;
	BASS_DX8_PARAMEQ dx8_fx0_parameq;
	HFX fx;
	HWND hSlider_Hz_80to16000;
	HWND hSlider_BandWidth1to36;
	HWND hSlider_Negative15toPositive15;
public:
	static DlgEqualizer* _this;
	DlgEqualizer();
	~DlgEqualizer();

	static INT_PTR WINAPI DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	VOID ShowEqualizer(INT isShow);
	
	VOID SetFX(HSTREAM stream);
	VOID Cls_OnVScroll(HWND hwnd, HWND hwndCtl, UINT code, INT pos);
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	friend class Application;
};