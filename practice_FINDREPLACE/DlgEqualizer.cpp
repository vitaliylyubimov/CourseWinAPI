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
DlgEqualizer::~DlgEqualizer(){}
/*
	Отображение эквалайзера
*/
VOID DlgEqualizer::ShowEqualizer(INT isShow)
{
	RECT child;						//Эквалайзер
	RECT parent;					//Плеер
	GetWindowRect(GetParent(hDlg), &parent);	//получение размера основного окна приложения
	GetWindowRect(hDlg, &child);				//получение размера окна эквалайзера
	SetWindowPos(hDlg, 0, parent.left + 3, parent.bottom - 3, child.right, child.bottom, SWP_NOSIZE);
	ShowWindow(hDlg, isShow);
}
/*
	WM_VSCROLL
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
	установка FX для канала
*/
VOID DlgEqualizer::SetFX(HSTREAM stream)
{
	/*
		Установка FX для устройства
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
		Получение дескрипторов
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
		установка диапазона слайдеров и позиции
	*/
	for (int i = 0; i < 10;i++)
	{
		SendMessage(hSlidersEqualizer[i], TBM_SETRANGE, TRUE, (LPARAM)MAKELPARAM(0, 15));
	}
	return TRUE;
}
/*
	WM_CTLCOLORDLG
*/
HBRUSH OnColorDlg(HWND hwnd, HDC hdc, HWND hwndChild, INT type)
{
	static HBRUSH brush = CreatePatternBrush(LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAPABSTRACTION)));
	//static HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
	return brush;
}
/*
	Отображение статика
*/
HBRUSH OnColorStatic(HWND hwnd, HDC hdc, HWND hwndChild, INT type)
{
	if (hwndChild != GetDlgItem(hwnd, IDC_STATIC_10kHz) &&
		hwndChild != GetDlgItem(hwnd, IDC_STATIC_12kHz) &&
		hwndChild != GetDlgItem(hwnd, IDC_STATIC_14kHz) &&
		hwndChild != GetDlgItem(hwnd, IDC_STATIC_170Hz) &&
		hwndChild != GetDlgItem(hwnd, IDC_STATIC_1kHz) &&
		hwndChild != GetDlgItem(hwnd, IDC_STATIC_310Hz) &&
		hwndChild != GetDlgItem(hwnd, IDC_STATIC_3kHz) &&
		hwndChild != GetDlgItem(hwnd, IDC_STATIC_600Hz) &&
		hwndChild != GetDlgItem(hwnd, IDC_STATIC_6kHz) &&
		hwndChild != GetDlgItem(hwnd, IDC_STATIC_80Hz)
		)
	{
		SetBkMode(hdc, TRANSPARENT);
		return (HBRUSH)GetStockObject(BLACK_BRUSH);
	}
	else
	{
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(255, 255, 255));
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	}
}
/*
	Диалоговая процедура
*/
INT_PTR WINAPI DlgEqualizer::DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		HANDLE_MSG(hWnd, WM_CTLCOLORDLG, OnColorDlg);					
		HANDLE_MSG(hWnd, WM_CTLCOLORSTATIC, OnColorStatic);				
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
			{
				SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
			}
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