#pragma once
#include "include.h"
class DlgEqualizer
{
private:
	HWND hDlg;				//���������� ����
	BASS_DX8_PARAMEQ dx8_fx0_parameq;		//��������� ������������ ������ ��� �����������
	HFX fx[10];										
	HWND hSlidersEqualizer[10];			//������ ������������ ���������
public:
	static DlgEqualizer* _this;			//����������� ���������
	DlgEqualizer();				//�����������
	~DlgEqualizer();			//����������

	static INT_PTR WINAPI DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);			//���������� ���������
	VOID ShowEqualizer(INT isShow);						//����������� �����������
	VOID SetFX(HSTREAM stream);							//��������� FX 
	VOID Cls_OnVScroll(HWND hwnd, HWND hwndCtl, UINT code, INT pos);		//WM_VSCROLL
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);		//WM_INITDIALOG
	friend class Application;
};