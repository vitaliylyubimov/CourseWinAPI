#include "DlgEqualizer.h"
#include "Application.h"
DlgEqualizer* DlgEqualizer::_this = NULL;
/*
	Constructor
*/
DlgEqualizer::DlgEqualizer()
{
	_this = this;
}
/*
	Destructor
*/
DlgEqualizer::~DlgEqualizer()
{}
/*
	Show equalizer
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
	Processing WM_VSCROLL
*/
VOID DlgEqualizer::Cls_OnVScroll(HWND hwnd, HWND hwndCtl, UINT code, INT pos)
{
	if (hwndCtl == hSlidersEqualizer[0])
	{
		INT p = SendMessage(hSlidersEqualizer[0], TBM_GETPOS, NULL, NULL);
		dx8_fx0_parameq.fGain = p;
		dx8_fx0_parameq.fBandwidth = 3;
		dx8_fx0_parameq.fCenter = 80;
		BASS_FXSetParameters(fx[0], &dx8_fx0_parameq);
	}
	else if (hwndCtl == hSlidersEqualizer[1])
	{
		INT p = SendMessage(hSlidersEqualizer[1], TBM_GETPOS, NULL, NULL);
		dx8_fx0_parameq.fGain = p;
		dx8_fx0_parameq.fBandwidth = 3;
		dx8_fx0_parameq.fCenter = 170;
		BASS_FXSetParameters(fx[1], &dx8_fx0_parameq);
	}
	else if (hwndCtl == hSlidersEqualizer[2])
	{
		INT p = SendMessage(hSlidersEqualizer[2], TBM_GETPOS, NULL, NULL);
		dx8_fx0_parameq.fGain = p;
		dx8_fx0_parameq.fBandwidth = 3;
		dx8_fx0_parameq.fCenter = 310;
		BASS_FXSetParameters(fx[2], &dx8_fx0_parameq);
	}
	else if (hwndCtl == hSlidersEqualizer[3])
	{
		INT p = SendMessage(hSlidersEqualizer[3], TBM_GETPOS, NULL, NULL);
		dx8_fx0_parameq.fGain = p;
		dx8_fx0_parameq.fBandwidth = 3;
		dx8_fx0_parameq.fCenter = 600;
		BASS_FXSetParameters(fx[3], &dx8_fx0_parameq);
	}
	else if (hwndCtl == hSlidersEqualizer[4])
	{
		INT p = SendMessage(hSlidersEqualizer[4], TBM_GETPOS, NULL, NULL);
		dx8_fx0_parameq.fGain = p;
		dx8_fx0_parameq.fBandwidth = 3;
		dx8_fx0_parameq.fCenter = 1000;
		BASS_FXSetParameters(fx[4], &dx8_fx0_parameq);
	}
	else if (hwndCtl == hSlidersEqualizer[5])
	{
		INT p = SendMessage(hSlidersEqualizer[5], TBM_GETPOS, NULL, NULL);
		dx8_fx0_parameq.fGain = p;
		dx8_fx0_parameq.fBandwidth = 3;
		dx8_fx0_parameq.fCenter = 3000;
		BASS_FXSetParameters(fx[5], &dx8_fx0_parameq);
	}
	else if (hwndCtl == hSlidersEqualizer[6])
	{
		INT p = SendMessage(hSlidersEqualizer[6], TBM_GETPOS, NULL, NULL);
		dx8_fx0_parameq.fGain = p;
		dx8_fx0_parameq.fBandwidth = 3;
		dx8_fx0_parameq.fCenter = 6000;
		BASS_FXSetParameters(fx[6], &dx8_fx0_parameq);
	}
	else if (hwndCtl == hSlidersEqualizer[7])
	{
		INT p = SendMessage(hSlidersEqualizer[7], TBM_GETPOS, NULL, NULL);
		dx8_fx0_parameq.fGain = p;
		dx8_fx0_parameq.fBandwidth = 3;
		dx8_fx0_parameq.fCenter = 10000;
		BASS_FXSetParameters(fx[7], &dx8_fx0_parameq);
	}
	else if (hwndCtl == hSlidersEqualizer[8])
	{
		INT p = SendMessage(hSlidersEqualizer[8], TBM_GETPOS, NULL, NULL);
		dx8_fx0_parameq.fGain = p;
		dx8_fx0_parameq.fBandwidth = 3;
		dx8_fx0_parameq.fCenter = 12000;
		BASS_FXSetParameters(fx[8], &dx8_fx0_parameq);
	}
	else if (hwndCtl == hSlidersEqualizer[9])
	{
		INT p = SendMessage(hSlidersEqualizer[9], TBM_GETPOS, NULL, NULL);
		dx8_fx0_parameq.fGain = p;
		dx8_fx0_parameq.fBandwidth = 3;
		dx8_fx0_parameq.fCenter = 14000;
		BASS_FXSetParameters(fx[9], &dx8_fx0_parameq);
	}
}
/*
	Set FX by channel
*/
VOID DlgEqualizer::SetFX(HSTREAM stream)
{
	/*
		Set FX by device
	*/
	for (int i = 0;i < 10;i++)
	{
		fx[i] = BASS_ChannelSetFX(stream, BASS_FX_DX8_PARAMEQ, 1);
	}
}
/*
	Processing WM_INITDIALOG
*/
BOOL DlgEqualizer::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	/*
		Get handle
	*/
	hSlidersEqualizer[0] = GetDlgItem(hwnd, IDC_SLIDER_EQUALIZER1);
	hSlidersEqualizer[1] = GetDlgItem(hwnd, IDC_SLIDER_EQUALIZER2);
	hSlidersEqualizer[2] = GetDlgItem(hwnd, IDC_SLIDER_EQUALIZER3);
	hSlidersEqualizer[3] = GetDlgItem(hwnd, IDC_SLIDER_EQUALIZER4);
	hSlidersEqualizer[4] = GetDlgItem(hwnd, IDC_SLIDER_EQUALIZER5);
	hSlidersEqualizer[5] = GetDlgItem(hwnd, IDC_SLIDER_EQUALIZER6);
	hSlidersEqualizer[6] = GetDlgItem(hwnd, IDC_SLIDER_EQUALIZER7);
	hSlidersEqualizer[7] = GetDlgItem(hwnd, IDC_SLIDER_EQUALIZER8);
	hSlidersEqualizer[8] = GetDlgItem(hwnd, IDC_SLIDER_EQUALIZER9);
	hSlidersEqualizer[9] = GetDlgItem(hwnd, IDC_SLIDER_EQUALIZER10);
	/*
		Set range and positions by equalizer
	*/
	for (int i = 0; i < 10;i++)
	{
		SendMessage(hSlidersEqualizer[i], TBM_SETRANGE, TRUE, (LPARAM)MAKELPARAM(0, 15));
	}
	return TRUE;
}
/*
	Processing WM_CTLCOLORDLG
*/
HBRUSH OnColorDlg(HWND hwnd, HDC hdc, HWND hwndChild, INT type)
{
	static HBRUSH brush = CreatePatternBrush(LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAPEQUAlIZER)));
	return brush;
}
HBRUSH OnColorStatic(HWND hwnd, HDC hdc, HWND hwndChild, INT type)
{
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(0, 245, 255));
	return (HBRUSH)GetStockObject(BLACK_BRUSH);
}
/*
	Dlg procedure
*/
INT_PTR WINAPI DlgEqualizer::DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		HANDLE_MSG(hWnd, WM_CTLCOLORDLG, OnColorDlg);					//Processing WM_CTLCOLORDLG
		HANDLE_MSG(hWnd, WM_CTLCOLORSTATIC, OnColorStatic);				//Processing WM_CTLCOLORSTATIC
		HANDLE_MSG(hWnd, WM_INITDIALOG, _this->Cls_OnInitDialog);		//Processing WM_INITDIALOG
		HANDLE_MSG(hWnd, WM_VSCROLL, _this->Cls_OnVScroll);				//Processing WM_VSCROLL
		/*
			Processing WM_LBUTTONDOWN
		*/
		case WM_LBUTTONDOWN:
		{
			RECT wnd;
			GetWindowRect(hWnd, &wnd);
			POINT pos;
			GetCursorPos(&pos);
			if (pos.x >= wnd.left && pos.x <= wnd.right &&
				pos.y >= wnd.top && pos.y <= wnd.top + 30)
			{
				SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
			}
			break;
		}
		/*
			Processing WM_PAINT
		*/
		case WM_PAINT:
		{
			/*PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			RECT wnd;
			HFONT hFont;
			GetClientRect(hWnd, &wnd);	*/
			/*
				Create the font by text
			*/
			//hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Times New Roman Cyr"));
			/*
				Settings text
			*/
			/*SelectObject(hdc, hFont);
			SetTextColor(hdc, RGB(255, 255, 255));
			SetBkMode(hdc, TRANSPARENT);*/
			/*
				Output text
			*/
			//TextOut(hdc, 40, 50, TEXT("Balance"), 7);
			/*
			EndPaint(hWnd, &ps);
			ReleaseDC(hWnd, hdc);*/
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