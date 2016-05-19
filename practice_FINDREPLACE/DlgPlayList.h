#pragma once
#include "include.h"
#include "structSong.h"

class DlgPlayList
{
private:
	HMENU hContextMenu;					//���������� ������������ ����
	WNDPROC origProcContextMenu;		//��������� ��� ���������
	HWND hDlg;							//���������� ��������� ���� ����������
	HWND hPlayList;						//���������� ���������
	std::vector<infoSong> songs;		//������ ���� ����� � ���������
	INT red, green, blue;				//���� ������ � ���������
public:
	static DlgPlayList* _this;			//����������� ���������

	DlgPlayList();					//�����������
	~DlgPlayList();					//����������


	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);				//���������� ���������
	static INT_PTR CALLBACK ProcPlayList(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);			//���������� ��������� ��� ��������� ��������� � ListBox
	VOID Cls_OnCommand(HWND hwnd, INT id, HWND hwndCtl, UINT codeNotify);								//WM_COMMAND
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);									//WM_INITDIALOG
	VOID Cls_OnContextMenu(HWND hwnd, HWND hwndContext, UINT xPos, UINT yPos);							//WM_CONTEXTMENU
	VOID Cls_OnDropFiles(HWND hwnd, HDROP hdrop);														//WM_DROPFILES
	VOID addSongToPlayList(HSTREAM stream, TCHAR*path);													//���������� ����� � ��������
	VOID showPlayList(INT isShow);																		//����������� ���������
	VOID changeColorTextInPlayList(HWND hWnd, INT r, INT g, INT b);										//��������� ����� ������ � ���������
	HBRUSH OnListColor(HWND hwnd, HDC hdc, HWND hwndChild, INT type);									//���� ���� ListBox
	HBRUSH OnBckgColor(HWND hwnd, HDC hdc, HWND hwndChild, INT type);									//���
	VOID UncheckedAllItemContextMenu(HMENU hMenu);
	VOID SavePlayList();
	VOID LoadPlayList();
	friend class Application;
};