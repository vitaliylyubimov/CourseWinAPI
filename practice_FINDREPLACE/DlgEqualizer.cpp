#include "DlgEqualizer.h"
#include "Application.h"
DlgEqualizer* DlgEqualizer::_this = NULL;
/*
	Конструктор
*/
DlgEqualizer::DlgEqualizer()
{
	_this = this;
}
/*
	Деструктор
*/
DlgEqualizer::~DlgEqualizer()
{
}
/*
	Показ Эквалайзера
*/
VOID DlgEqualizer::ShowEqualizer(INT isShow)
{
	RECT child;						//Equalizer
	RECT parent;					//Pleer
	GetWindowRect(GetParent(hDlg), &parent);
	GetWindowRect(hDlg, &child);
	SetWindowPos(hDlg, 0, parent.left + 3, parent.bottom - 3, child.right, child.bottom, SWP_NOSIZE);
	
	ShowWindow(hDlg, isShow);
	//SetForegroundWindow(hDlg);
}

/*
	Обработка WM_VSCROLL
*/
VOID DlgEqualizer::Cls_OnVScroll(HWND hwnd, HWND hwndCtl, UINT code, INT pos)
{
	if (hwndCtl == hSlider_Hz_80to16000)
	{
		INT p = SendMessage(hSlider_Hz_80to16000, TBM_GETPOS, NULL, NULL);
		BASS_FXGetParameters(fx, &dx8_fx0_parameq);
		dx8_fx0_parameq.fCenter = p;
		BASS_FXSetParameters(fx, &dx8_fx0_parameq);
		BASS_ChannelPlay(Application::_this->hStream, 0);
	}
	else if (hwndCtl == hSlider_BandWidth1to36)
	{
		INT p = SendMessage(hSlider_BandWidth1to36, TBM_GETPOS, NULL, NULL);
		BASS_FXGetParameters(fx, &dx8_fx0_parameq);
		dx8_fx0_parameq.fBandwidth = p;
		BASS_FXSetParameters(fx, &dx8_fx0_parameq);
		BASS_ChannelPlay(Application::_this->hStream, 0);
	}
	else if (hwndCtl == hSlider_Negative15toPositive15)
	{
		INT p = SendMessage(hSlider_Negative15toPositive15, TBM_GETPOS, NULL, NULL);
		BASS_FXGetParameters(fx, &dx8_fx0_parameq);
		dx8_fx0_parameq.fGain = p;
		BASS_FXSetParameters(fx, &dx8_fx0_parameq);
		BASS_ChannelPlay(Application::_this->hStream, 0);
	}
}

VOID DlgEqualizer::SetFX(HSTREAM stream)
{
	fx = BASS_ChannelSetFX(stream, BASS_FX_DX8_PARAMEQ, 0);
}

BOOL DlgEqualizer::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	/*
		
	*/
	hSlider_Hz_80to16000 = GetDlgItem(hwnd, IDC_SLIDER_Hz_80_16000);
	SendDlgItemMessage(hwnd, IDC_SLIDER_Hz_80_16000, TBM_SETRANGE, 0, (LPARAM)MAKELPARAM(80, 16000));
	/*
	
	*/
	hSlider_Negative15toPositive15 = GetDlgItem(hwnd, IDC_SLIDER_Negative15_to_Positive15);
	SendDlgItemMessage(hwnd, IDC_SLIDER_Negative15_to_Positive15, TBM_SETRANGE, 0, (LPARAM)MAKELPARAM(-15, 15));
	/*
	
	*/
	hSlider_BandWidth1to36 = GetDlgItem(hwnd, IDC_SLIDER_BANDWIDTH1_36);
	SendDlgItemMessage(hwnd, IDC_SLIDER_BANDWIDTH1_36, TBM_SETRANGE, 0, (LPARAM)MAKELPARAM(1, 36));
	return TRUE;
}

HBRUSH OnColorDlg(HWND hwnd, HDC hdc, HWND hwndChild, INT type)
{
	static HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
	return brush;
}
/*
	Диалоговая процедура 
*/
INT_PTR WINAPI DlgEqualizer::DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{	
		//HANDLE_MSG(hWnd, WM_CTLCOLORDLG, OnColorDlg);
		HANDLE_MSG(hWnd, WM_INITDIALOG, _this->Cls_OnInitDialog);
		HANDLE_MSG(hWnd, WM_VSCROLL, _this->Cls_OnVScroll);
		case WM_LBUTTONDOWN:
		{
			RECT wnd;
			GetWindowRect(hWnd, &wnd);
			POINT pos;
			GetCursorPos(&pos);
			if (pos.x >= wnd.left && pos.x <= wnd.right &&
				pos.y >= wnd.top && pos.y <= wnd.top + 30)
			SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
			break;
		}
		case WM_CLOSE:
		{
			HWND hCheck = GetDlgItem(GetParent(_this->hDlg), IDC_CHECKEQUALIZER);
			SendMessage(hCheck, BM_SETCHECK, BST_UNCHECKED, 0);
			EndDialog(hWnd, 0);
			break;
		}
		default:
			return FALSE;
	}
	return FALSE;
}