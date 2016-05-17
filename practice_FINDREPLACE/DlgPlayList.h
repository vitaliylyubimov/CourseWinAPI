#pragma once
#include "include.h"
#include "structSong.h"

class DlgPlayList
{
private:
	HMENU hContextMenu;					//Handle menu
	WNDPROC origProcContextMenu;		//Procedure by PlayList
	HWND hDlg;							//Handle dlg
	HWND hPlayList;						//Handle PlayList
	std::vector<infoSong> songs;		//Vector songs. All songs in PlayList
	INT red, green, blue;				//Color text in PlayList
public:
	static DlgPlayList* _this;			//Pointer

	DlgPlayList();			//Contructor
	~DlgPlayList();			//Destructor


	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);				//Procedure dlg
	static INT_PTR CALLBACK ProcPlayList(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);			//Procedure dlg by PlayList
	VOID Cls_OnCommand(HWND hwnd, INT id, HWND hwndCtl, UINT codeNotify);								//Processing WM_COMMAND
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);									//Processing WM_INITDIALOG
	VOID Cls_OnContextMenu(HWND hwnd, HWND hwndContext, UINT xPos, UINT yPos);							//Processing WM_CONTEXTMENU
	VOID Cls_OnDropFiles(HWND hwnd, HDROP hdrop);														//Processing WM_DROPFILES
	VOID addSongToPlayList(HSTREAM stream, TCHAR*path);													//Add song by PlayList
	VOID showPlayList(INT isShow);																		//Show PlayList
	VOID changeColorTextInPlayList(HWND hWnd, INT r, INT g, INT b);										//Chahbe color text			
	HBRUSH OnListColor(HWND hwnd, HDC hdc, HWND hwndChild, INT type);
	VOID SavePlayList();
	VOID LoadPlayList();
	friend class Application;
};