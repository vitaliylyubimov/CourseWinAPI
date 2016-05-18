#pragma once
#include "include.h"
class DlgEqualizer
{
private:
	HWND hDlg;				//Дескриптор окна
	BASS_DX8_PARAMEQ dx8_fx0_parameq;		//структура определяющая данные для эквалайзера
	HFX fx[10];										
	HWND hSlidersEqualizer[10];			//массив дескрипторов слайдеров
public:
	static DlgEqualizer* _this;			//собственный указатель
	DlgEqualizer();				//Конструктор
	~DlgEqualizer();			//Деструктор

	static INT_PTR WINAPI DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);			//Диалоговая процедура
	VOID ShowEqualizer(INT isShow);						//Отображение эквалайзера
	VOID SetFX(HSTREAM stream);							//Установка FX 
	VOID Cls_OnVScroll(HWND hwnd, HWND hwndCtl, UINT code, INT pos);		//WM_VSCROLL
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);		//WM_INITDIALOG
	friend class Application;
};