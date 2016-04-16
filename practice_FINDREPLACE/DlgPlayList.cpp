#include "DlgPlayList.h"
#include "Application.h"
//Pointer
DlgPlayList* DlgPlayList::_this = NULL;
/*
	Contructor
*/
DlgPlayList::DlgPlayList()
{
	_this = this;
	origProcContextMenu = NULL;
	red = 0;
	green = 255;
	blue = 0;
}
/*
	Destructor
*/
DlgPlayList::~DlgPlayList()
{
}
/*
	Colors in PlayList
*/
HBRUSH DlgPlayList::OnListColor(HWND hwnd, HDC hdc, HWND hwndChild, INT type)
{
	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
	SetTextColor(hdc, RGB(red, green, blue));
	SetBkColor(hdc, RGB(0,0,0));
	return brush;
}
/*
	Procedure dlg
*/
INT_PTR CALLBACK DlgPlayList::DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		HANDLE_MSG(hWnd, WM_CTLCOLORLISTBOX, _this->OnListColor);
		HANDLE_MSG(hWnd, WM_INITDIALOG, _this->Cls_OnInitDialog);
		HANDLE_MSG(hWnd, WM_COMMAND, _this->Cls_OnCommand);
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
			HWND hCheck = GetDlgItem(GetParent(_this->hDlg), IDC_CHECKPLAYLIST);
			BOOL isCheck = FALSE;
			isCheck = Button_GetCheck(hCheck);
			if (isCheck == TRUE)
				SendMessage(hCheck, BM_SETCHECK, BST_UNCHECKED, 0);
			EndDialog(hWnd, 0);
			break;
		}
		default:
			return FALSE;
		}
	return FALSE;
}
/*
	Processing WM_COMMAND
*/
VOID DlgPlayList::Cls_OnCommand(HWND hwnd, INT id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
		case IDC_PLAYLIST:
		{
			if (codeNotify == LBN_DBLCLK)
			{
				INT idx = SendMessage(hPlayList, LB_GETCURSEL, 0, 0);
				Application::_this->hStream = songs[idx].hStream;
				Application::_this->secPlaying = 0;
				Application::_this->setRangeTrackBarPlaySong(Application::_this->hStream);
				DlgEqualizer::_this->SetFX(Application::_this->hStream);
				BASS_Stop();
				BASS_ChannelStop(Application::_this->hStream);
				BASS_Start();
				BASS_ChannelPlay(Application::_this->hStream, TRUE);
				SetTimer(GetParent(hDlg), Application::_this->id_timer, 1000, 0);
				SetTimer(GetParent(hDlg), Application::_this->idTimerBySpectr, 100, 0);
			}
			break;
		}
		case IDC_ADDSONG:
		{
			SendMessage(GetParent(hwnd), WM_COMMAND, IDC_ADDSONG, 0);
			break;
		}
		default:
			break;
	}
}
/*
	Processing WM_INITDIALOG
*/
BOOL DlgPlayList::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	hPlayList = GetDlgItem(hwnd, IDC_PLAYLIST);
	origProcContextMenu = (WNDPROC)SetWindowLong(hPlayList, GWL_WNDPROC, (LPARAM)ProcPlayList);
	hContextMenu = CreatePopupMenu();
	AppendMenu(hContextMenu, MF_STRING, IDC_BTNPLAY, TEXT("&Play"));
	AppendMenu(hContextMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(hContextMenu, MF_STRING, IDC_DELETE, TEXT("&Delete"));
	AppendMenu(hContextMenu, MF_STRING, IDC_CLEANPLAYLIST, TEXT("D&elete all"));
	AppendMenu(hContextMenu, MF_SEPARATOR, 0, 0);
	HMENU hColor = CreatePopupMenu();
	AppendMenu(hContextMenu, MF_POPUP | MF_STRING, (INT_PTR)hColor, TEXT("&Color text"));
	AppendMenu(hColor, MF_STRING, IDC_COLORTEXTRED, TEXT("&Red"));
	AppendMenu(hColor, MF_STRING, IDC_COLORTEXTGREEN, TEXT("&Green"));
	AppendMenu(hColor, MF_STRING, IDC_COLORTEXTBLUE, TEXT("&Blue"));
	AppendMenu(hColor, MF_STRING, IDC_COLORWHITE, TEXT("&White"));
	AppendMenu(hColor, MF_STRING, IDC_COLORYELLOW, TEXT("&Yellow"));
	AppendMenu(hColor, MF_STRING, IDC_COLORTURQUOISE, TEXT("&Turquoise"));
	return TRUE;
}
/*
	Show PlayList
*/
VOID DlgPlayList::showPlayList(INT isShow)
{	
	RECT r;						//PlayList
	RECT wnd;					//Pleer
	GetWindowRect(GetParent(hDlg), &wnd);
	GetWindowRect(hDlg, &r);
	SetWindowPos(hDlg, 0, wnd.left + 3, wnd.bottom - 3, r.right, r.bottom, SWP_NOSIZE);
	
	ShowWindow(hDlg, isShow);
	SetForegroundWindow(hDlg);
}
/*
	Add song in PlayList
*/
VOID DlgPlayList::addSongToPlayList(HSTREAM stream, TCHAR*path)
{
	TAG_ID3* id3 = (TAG_ID3*)BASS_ChannelGetTags(stream, BASS_TAG_ID3);
	std::wstringstream infoAboutTheSong;									//Буфер строки (Название песни + Исполнитель + время)
	infoSong info;
	QWORD lengthSong = BASS_ChannelGetLength(stream, BASS_POS_BYTE);		//длина песни в байтах
	INT seconds = BASS_ChannelBytes2Seconds(stream, lengthSong);			//длина песни в секундах
	INT min = seconds / 60;													//Определение минут
	seconds = seconds % 60;													//Определение секунд
	/*
		Заполнение данных о песне
	*/
	if (id3 == NULL || strlen(id3->artist) == 0)
	{
		INT idx = lstrlen(path);
		INT length = idx;
		TCHAR name[MAX_PATH];
		INT k = 0;
		while (path[idx] != '\\')
		{
			idx--;
		}
		for (INT i = idx + 1; i < length;i++)
		{
			name[k] = path[i];
			k++;
		}
		name[k - 4] = '\0';	//Удалить формат
		infoAboutTheSong << name << "               ";
	}
	else
	{
		strcpy(info.title, id3->title);							//Name
		strcpy(info.artist, id3->artist);						//Author
		strcpy(info.album, id3->album);							//Album
		strcpy(info.year, id3->year);							//Year
		infoAboutTheSong << info.artist << " "
			<< info.title << "               ";
	}
	info.minutes = min;														//Minutes
	info.seconds = seconds;													//Seconds
	info.hStream = stream;													//Stream
	lstrcpy(info.path, path);												//Path of the song
	/*
		Write time in string
	*/
	infoAboutTheSong << info.minutes << ":";
	if (seconds < 10)
		infoAboutTheSong << "0" << info.seconds;
	else
		infoAboutTheSong << info.seconds;
	ListBox_AddString(hPlayList, infoAboutTheSong.str().c_str());			//Add in PlayList
	songs.push_back(info);													//Add in vector
}
/*
	ContextMenu by PlayList
*/
VOID DlgPlayList::Cls_OnContextMenu(HWND hwnd, HWND hwndContext, UINT xPos, UINT yPos)
{
	POINT pos;
	GetCursorPos(&pos);
	INT idx = SendMessage(hPlayList, LB_GETCURSEL, 0, 0);
	if (idx >= 0)
		TrackPopupMenu(_this->hContextMenu, TPM_BOTTOMALIGN, pos.x, pos.y, 0, hwndContext, 0);
}
/*
	Me fnct is to compare char strings (strcmp not work :( )
*/
BOOL me_strcmp(CHAR *str1, CHAR* str2)
{
	INT len = strlen(str1);				//длина строки с которой сравниваем
	INT count = 0;						//количество совпавших символов
	for (INT i = 0; i < len; i++)
	{
		if (str1[i] == str2[i])
			count++;
	}
	if (count == len)				//если количество совпавших символов = длине сравниваемого слова
		return TRUE;
	return FALSE;
}
/*
	Thread by load music in PlayList
*/
DWORD WINAPI Thread(LPVOID lp)
{
	HDROP hDrop = (HDROP)lp;
	CHAR szFileName[MAX_PATH];
	TCHAR buff[MAX_PATH];
	DWORD dwCount = DragQueryFileA(hDrop, 0xFFFFFFFF, szFileName, MAX_PATH);
	for (INT i = 0; i < dwCount; i++)
	{
		DragQueryFileA(hDrop, i, szFileName, MAX_PATH);			//определения пути к файлу
		INT len = strlen(szFileName);
		CHAR tmp[4];					//буфер в который будет помещаться расширение файла, которое поступает в PlayList
		INT j = 0;						//счетчик для буфера расширения
		for (INT i = len - 4; i < len; i++)
		{
			tmp[j] = szFileName[i];
			j++;
		}
		if (me_strcmp(".mp3", tmp))
		{
			HSTREAM stream = BASS_StreamCreateFile(0, szFileName, 0, 0, 0);
			mbstowcs(buff, szFileName, MAX_PATH);			//Из CHAR в TCHAR
			DlgPlayList::_this->addSongToPlayList(stream, buff);
		}
	}
	DragFinish(hDrop);
	return FALSE;
}
/*
	Change color text in PlayList
*/
VOID DlgPlayList::changeColorTextInPlayList(HWND hWnd, INT r, INT g, INT b)
{
	red = r;
	green = g;
	blue = b;
	InvalidateRect(hWnd, NULL, TRUE);
}
/*
	Procedure dlg by PlayList
*/
INT_PTR CALLBACK DlgPlayList::ProcPlayList(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case IDC_BTNPLAY:
				{
					SendMessage(GetParent(_this->hDlg), WM_COMMAND, IDC_BTNPLAY, 0);
					break;
				}
				case IDC_DELETE:
				{
					int idxSel = ListBox_GetCurSel(_this->hPlayList);
					ListBox_DeleteString(_this->hPlayList, idxSel);
					_this->songs.erase(_this->songs.begin() + idxSel);
					break;
				}
				case IDC_CLEANPLAYLIST:
				{
					_this->songs.clear();
					ListBox_ResetContent(_this->hPlayList);
					break;
				}
				case IDC_COLORTEXTRED:
				{
					_this->changeColorTextInPlayList(hWnd, 255, 0, 0);
					break;
				}
				case IDC_COLORTEXTGREEN:
				{
					_this->changeColorTextInPlayList(hWnd, 0, 255, 0);
					break;
				}
				case IDC_COLORTEXTBLUE:
				{
					_this->changeColorTextInPlayList(hWnd, 0, 0, 255);
					break;
				}
				case IDC_COLORWHITE:
				{
					_this->changeColorTextInPlayList(hWnd, 255, 255, 255);
					break;
				}
				case IDC_COLORYELLOW:
				{
					_this->changeColorTextInPlayList(hWnd, 255, 255, 0);
					break;
				}
				case IDC_COLORTURQUOISE:
				{
					_this->changeColorTextInPlayList(hWnd, 0, 245, 255);
					break;
				}
			}
			break;
		}
		case WM_PAINT:
		{
			
			break;
		}
		case WM_DROPFILES:
		{
			HDROP hDrop = (HDROP)wParam;
			HANDLE hThread = CreateThread(0, 0, Thread, hDrop, 0, 0);			//Запуск отдельного потока для загрузки музыки в PlayList
			break;
		}
		HANDLE_MSG(hWnd, WM_CONTEXTMENU, _this->Cls_OnContextMenu);
	}
	return CallWindowProc(_this->origProcContextMenu, hWnd, uMsg, wParam, lParam);
}