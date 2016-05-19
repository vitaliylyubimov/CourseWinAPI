#pragma once
#include "include.h"
#include "structSong.h"

class DlgPlayList
{
private:
	HMENU hContextMenu;					//Дескриптор контекстного меню
	WNDPROC origProcContextMenu;		//Процедура для плейлиста
	HWND hDlg;							//Дескриптор основного окна приложения
	HWND hPlayList;						//Дескриптор плейлиста
	std::vector<infoSong> songs;		//Вектор всех песен в плейлисте
	INT red, green, blue;				//Цвет текста в плейлисте
public:
	static DlgPlayList* _this;			//Собственный указатель

	DlgPlayList();					//Конструктор
	~DlgPlayList();					//Деструктор


	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);				//Диалоговая процедура
	static INT_PTR CALLBACK ProcPlayList(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);			//диалоговая процедура для обрабокти сообщений в ListBox
	VOID Cls_OnCommand(HWND hwnd, INT id, HWND hwndCtl, UINT codeNotify);								//WM_COMMAND
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);									//WM_INITDIALOG
	VOID Cls_OnContextMenu(HWND hwnd, HWND hwndContext, UINT xPos, UINT yPos);							//WM_CONTEXTMENU
	VOID Cls_OnDropFiles(HWND hwnd, HDROP hdrop);														//WM_DROPFILES
	VOID addSongToPlayList(HSTREAM stream, TCHAR*path);													//Добавление песни в плейлист
	VOID showPlayList(INT isShow);																		//Отображение плейлиста
	VOID changeColorTextInPlayList(HWND hWnd, INT r, INT g, INT b);										//Изменение цвета текста в плейлисте
	HBRUSH OnListColor(HWND hwnd, HDC hdc, HWND hwndChild, INT type);									//Цвет фона ListBox
	HBRUSH OnBckgColor(HWND hwnd, HDC hdc, HWND hwndChild, INT type);									//Фон
	VOID UncheckedAllItemContextMenu(HMENU hMenu);
	VOID SavePlayList();
	VOID LoadPlayList();
	friend class Application;
};