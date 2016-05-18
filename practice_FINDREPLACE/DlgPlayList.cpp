#include "DlgPlayList.h"
#include "Application.h"


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
DlgPlayList::~DlgPlayList(){}
/*
	��������� ����� ���� ���������
*/
HBRUSH DlgPlayList::OnListColor(HWND hwnd, HDC hdc, HWND hwndChild, INT type)
{
	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
	SetTextColor(hdc, RGB(red, green, blue));
	SetBkColor(hdc, RGB(0,0,0));
	return brush;
}
/*
	���������� ���������
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
	WM_COMMAND
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
		case IDC_SAVEPLAYLIST:
		{
			//SavePlayList();
			break;
		}
		case IDC_LOADPLAYLIST:
		{
			//LoadPlayList();
			break;
		}
		default:
			break;
	}
}
/*
	WM_INITDIALOG
*/
BOOL DlgPlayList::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	//��������� ����������� ���������
	hPlayList = GetDlgItem(hwnd, IDC_PLAYLIST);
	/*
		�������� ��������� ��� ��������� ��������� � ListBox ���������
	*/
	origProcContextMenu = (WNDPROC)SetWindowLong(hPlayList, GWL_WNDPROC, (LPARAM)ProcPlayList);
	/*
		�������� ������������ ����
	*/
	hContextMenu = CreatePopupMenu();
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
	/*
		�������� ����������� �� ������
	*/
	HBITMAP bmp = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAPSAVE));
	SendMessage(GetDlgItem(hwnd, IDC_SAVEPLAYLIST), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
	return TRUE;
}
/*
	����������� ���������
*/
VOID DlgPlayList::showPlayList(INT isShow)
{	
	RECT r;						//��������
	RECT wnd;					//�����
	GetWindowRect(GetParent(hDlg), &wnd);		//��������� ������� ���� ����������
	GetWindowRect(hDlg, &r);					//��������� ������� ���������
	SetWindowPos(hDlg, 0, wnd.left + 3, wnd.bottom - 3, r.right, r.bottom, SWP_NOSIZE);			//����������� ����	
	ShowWindow(hDlg, isShow);		//����������� ����
}
/*
	���������� ����� � ��������
*/
VOID DlgPlayList::addSongToPlayList(HSTREAM stream, TCHAR* path)
{
	TAG_ID3* id3 = (TAG_ID3*)BASS_ChannelGetTags(stream, BASS_TAG_ID3);
	std::wstringstream infoAboutTheSong;									//����� ������ (�������� ����� + ����������� + �����)
	infoSong info;
	QWORD lengthSong = BASS_ChannelGetLength(stream, BASS_POS_BYTE);		//����� ����� � ������
	INT seconds = BASS_ChannelBytes2Seconds(stream, lengthSong);			//����� ����� � ��������
	INT min = seconds / 60;													//����������� �����
	seconds = seconds % 60;							//����������� ������
	/*
		���������� ������ � �����
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
		name[k - 4] = '\0';					//�������� ������� �����
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
		������ ������� ����� � ������
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
	����������� ���� ��� ���������
*/
VOID DlgPlayList::Cls_OnContextMenu(HWND hwnd, HWND hwndContext, UINT xPos, UINT yPos)
{
	POINT pos;
	GetCursorPos(&pos);
	INT idx = SendMessage(hPlayList, LB_GETCURSEL, 0, 0);
	/*
		����������� ���� ������ � ��� ������ ���� ���� ����� � ��������� � ���� ����� ��������
	*/
	if (idx >= 0)
		TrackPopupMenu(_this->hContextMenu, TPM_BOTTOMALIGN, pos.x, pos.y, 0, hwndContext, 0);
}
/*
	����������� ������� ��������� �����
*/
BOOL me_strcmp(CHAR *str1, CHAR* str2)
{
	INT len = strlen(str1);				//����� ������
	INT count = 0;						//���������� ��������� ��������
	for (INT i = 0; i < len; i++)
	{
		if (str1[i] == str2[i])
			count++;
	}
	if (count == len)				//���� ���������� ��������� �������� = ����� ������������� �����
		return TRUE;
	return FALSE;
}
/*
	����� ��� �������� ������ � ��������
*/
DWORD WINAPI Thread(LPVOID lp)
{
	HDROP hDrop = (HDROP)lp;
	CHAR szFileName[MAX_PATH];
	TCHAR buff[MAX_PATH];
	DWORD dwCount = DragQueryFileA(hDrop, 0xFFFFFFFF, szFileName, MAX_PATH);		//����������� ���������� ����������� �����
	for (INT i = 0; i < dwCount; i++)
	{
		DragQueryFileA(hDrop, i, szFileName, MAX_PATH);			//����������� ���� � �����
		INT len = strlen(szFileName);
		CHAR buffFormat[4];									//������ �����
		INT j = 0;											//��� ������� �� ������ ������� �����
		for (INT i = len - 4; i < len; i++)
		{
			buffFormat[j] = szFileName[i];
			j++;
		}
		if (me_strcmp(".mp3", buffFormat))
		{
			HSTREAM stream = BASS_StreamCreateFile(0, szFileName, 0, 0, 0);			//�������� ������ 
			mbstowcs(buff, szFileName, MAX_PATH);							//�������������� CHAR to TCHAR
			DlgPlayList::_this->addSongToPlayList(stream, buff);			//���������� ����� � ��������
		}
	}
	DragFinish(hDrop);
	return FALSE;
}
/*
	��������� ����� ������ � ���������
*/
VOID DlgPlayList::changeColorTextInPlayList(HWND hWnd, INT r, INT g, INT b)
{
	red = r;
	green = g;
	blue = b;
	InvalidateRect(hWnd, NULL, TRUE);
}
/*
	������� ���� ������� � ����������� ���� ������ �����
*/
VOID DlgPlayList::UncheckedAllItemContextMenu(HMENU hMenu)
{
	CheckMenuItem(hMenu, IDC_COLORTEXTRED, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, IDC_COLORTEXTBLUE, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, IDC_COLORTEXTGREEN, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, IDC_COLORTURQUOISE, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, IDC_COLORWHITE, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, IDC_COLORYELLOW, MF_BYCOMMAND | MF_UNCHECKED);
}
/*
	���������� ���������
*/
INT_PTR CALLBACK DlgPlayList::ProcPlayList(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HMENU hColorText = GetSubMenu(_this->hContextMenu, 3);
	switch (uMsg)
	{
		HANDLE_MSG(hWnd, WM_CONTEXTMENU, _this->Cls_OnContextMenu);
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
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
					_this->UncheckedAllItemContextMenu(hColorText);
					CheckMenuItem(hColorText, IDC_COLORTEXTRED, MF_BYCOMMAND | MF_CHECKED);
					_this->changeColorTextInPlayList(hWnd, 255, 0, 0);
					break;
				}
				case IDC_COLORTEXTGREEN:
				{
					_this->UncheckedAllItemContextMenu(hColorText);
					CheckMenuItem(hColorText, IDC_COLORTEXTGREEN, MF_BYCOMMAND | MF_CHECKED);
					_this->changeColorTextInPlayList(hWnd, 0, 255, 0);
					break;
				}
				case IDC_COLORTEXTBLUE:
				{
					_this->UncheckedAllItemContextMenu(hColorText);
					CheckMenuItem(hColorText, IDC_COLORTEXTBLUE, MF_BYCOMMAND | MF_CHECKED);
					_this->changeColorTextInPlayList(hWnd, 0, 0, 255);
					break;
				}
				case IDC_COLORWHITE:
				{
					_this->UncheckedAllItemContextMenu(hColorText);
					CheckMenuItem(hColorText, IDC_COLORWHITE, MF_BYCOMMAND | MF_CHECKED);
					_this->changeColorTextInPlayList(hWnd, 255, 255, 255);
					break;
				}
				case IDC_COLORYELLOW:
				{
					_this->UncheckedAllItemContextMenu(hColorText);
					CheckMenuItem(hColorText, IDC_COLORYELLOW, MF_BYCOMMAND | MF_CHECKED);
					_this->changeColorTextInPlayList(hWnd, 255, 255, 0);
					break;
				}
				case IDC_COLORTURQUOISE:
				{
					_this->UncheckedAllItemContextMenu(hColorText);
					CheckMenuItem(hColorText, IDC_COLORTURQUOISE, MF_BYCOMMAND | MF_CHECKED);
					_this->changeColorTextInPlayList(hWnd, 0, 245, 255);
					break;
				}
			}
			break;
		}
		/*
			WM_DROPFILES
		*/
		case WM_DROPFILES:
		{
			HDROP hDrop = (HDROP)wParam;
			HANDLE hThread = CreateThread(0, 0, Thread, hDrop, 0, 0);			//�������� ���������� ������ ��� �������� ������ � PlayList
			break;
		}
	}
	return CallWindowProc(_this->origProcContextMenu, hWnd, uMsg, wParam, lParam);
}
/*
	Save the playlist to file 
*/
VOID DlgPlayList::SavePlayList()
{
	if (songs.size() != 0)
	{
		std::ofstream fout("Playlist.txt", std::ios::binary);
		//������ ���������� ����� � ���������
		fout << songs.size();
		for (int i = 0;i < songs.size();i++)
			fout.write((char*)&songs[i], sizeof(infoSong));

		fout.close();
	}
}
/*
	Load the playlist to file
*/ 
VOID DlgPlayList::LoadPlayList()
{
	/*
		Clear playlist
	*/
	ListBox_ResetContent(hPlayList);
	std::ifstream fin("Playlist.txt", std::ios::binary);
	int count;
	fin >> count;
	infoSong iS;
	if (fin.is_open())
	{
		for (int i = 0;i < count;i++) 
		{
			fin.read((char*)&iS, sizeof(infoSong));
			HSTREAM stream = BASS_StreamCreateFile(0, iS.path, 0, 0, 0);
			if (stream == 0)
			{
				addSongToPlayList(iS.hStream, iS.path);
			}
			else {
				iS.hStream = stream;
				addSongToPlayList(iS.hStream, iS.path);
			}
		}
		fin.close();
	}
}