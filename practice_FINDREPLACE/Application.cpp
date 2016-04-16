#include "Application.h"

/*
	������������� ������������ ���������
*/
Application* Application::_this = NULL;
/*
	����������� (�� ���������)
*/
Application::Application(VOID)
{	
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);
	_this = this;
	CheckedInitBASS();					//�������� ������������� BASS
	secPlaying = 0;						//����� �������� �����
	id_timer = 0;						//������������ �������
	idTimerBySpectr = 1;
	IsRepeatSong = FALSE;				//������ �����
}
/*
	����������
*/
Application::~Application()
{
	ReleaseMutex(hMutex);
}
/*
	�������� ������ BASS � ������������� ���������� 
*/
INT Application::CheckedInitBASS()
{
	if (HIWORD(BASS_GetVersion()) != BASSVERSION)
	{
		MessageBox(NULL, TEXT("������ ������ BASS."), NULL, 0);
		return 1;
	}
	if (!BASS_Init(-1, 44100, BASS_DEVICE_3D, 0, NULL))
	{
		MessageBox(NULL, TEXT("�� ������� ���������������� BASS."), NULL, 0);
		return 1;
	}
}
/*
	�������� �� �������� ����� ����������
*/
VOID Application::CheckOpeningCopy(HWND hwnd)
{
	CONST TCHAR GUID[] = { TEXT("64AE5751 - 8A85 - 4566 - AE2E - 24C3CFD40E87") };
	hMutex = CreateMutex(NULL, FALSE, GUID);
	DWORD dwAnswer = WaitForSingleObject(hMutex, 0);
	if (dwAnswer == WAIT_TIMEOUT)
	{
		std::wstringstream ss;
		ss << NAMEPLEER << TEXT(" ��� �������");
		MessageBox(hwnd, ss.str().c_str(), TEXT("Information"), MB_ICONEXCLAMATION);
		EndDialog(hwnd, 0);
	}
}
/*
	������ ���������
*/
INT Application::runProgramm()
{
	return DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}
/*
	�������� ����� �� ����������
*/
BOOL Application::openFile_LoadMusic(HWND hWnd)
{
	TCHAR path[MAX_PATH];
	OPENFILENAME ofn = { 0 };
	path[0] = '\0';
	ofn.hInstance = GetModuleHandle(0);
	ofn.hwndOwner = hWnd;
	ofn.Flags = OFN_FILEMUSTEXIST;// | OFN_ALLOWMULTISELECT | OFN_EXPLORER;
	ofn.lpstrDefExt = TEXT("mp3");
	ofn.lpstrFile = path;
	ofn.lpstrFilter = TEXT("(.mp3)\0*.mp3\0");// (.aif)\0 * .aif\0(.wav)\0 * .wav\0(.mp2)\0 * .mp2\0(.mp1)\0 * .mp1\0(.ogg)\0 * .ogg\0");
	ofn.lpstrInitialDir = TEXT("d:\\");
	ofn.lpstrTitle = TEXT("Open file");
	ofn.lStructSize = sizeof(OPENFILENAME);
	
	ofn.nFilterIndex = 1;
	ofn.nMaxFile = MAX_PATH;

	if (GetOpenFileName(&ofn))
	{
		
		if (hStream == NULL)
		{
			hStream = BASS_StreamCreateFile(FALSE, path, 0, 0, 0);
			setRangeTrackBarPlaySong(hStream);
			dlg.addSongToPlayList(hStream, path);
		}
		else
		{
			HSTREAM stream;
			stream = BASS_StreamCreateFile(FALSE, path, 0, 0, 0);
			setRangeTrackBarPlaySong(stream);
			dlg.addSongToPlayList(stream, path);
		}
		return TRUE;
	}
	return FALSE;
}
/*
	��������� WM_INITDIALOG
*/
BOOL Application::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	CheckOpeningCopy(hwnd);
	/*
		��������� ����������� ����
	*/
	SetWindowText(hwnd, NAMEPLEER);
	/*
		���� ������������
	*/
	SetSpectrColor(hwnd);
	/*
		�������� ������
	*/	
	hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON1));
	SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);		
	/*
		��������� ���������
	*/
	hTBSoundVolume = GetDlgItem(hwnd, IDC_TRACKBARSOUND); 
	SendMessage(hTBSoundVolume, TBM_SETPOS, TRUE, (LPARAM)100);
	/*
		��������� ������������
	*/
	hTBPlayingSong = GetDlgItem(hwnd, IDC_SLIDER_TIMEPLAYING);
	/*
		�������� PlayList
	*/
	dlg.hDlg = CreateDialog(GetModuleHandle(0), MAKEINTRESOURCE(IDD_DLGPLAYLIST), hwnd, dlg.DlgProc);
	/*
		�������� Equalizer
	*/
	equalizer.hDlg = CreateDialog(GetModuleHandle(0), MAKEINTRESOURCE(IDD_DLGEQUALIZER), hwnd, equalizer.DlgProc);
	/*
		������� ������������ �� 0
	*/
	SetNullPosSpectr();
	return TRUE;
}
/*
	��������� WM_HSCROLL
*/
VOID Application::Cls_OnHScroll(HWND hwnd, HWND hwndCtl, UINT code, INT pos)
{
	if (hwndCtl == hTBSoundVolume)								//��������� ���������� ���������
	{
		INT p = SendMessage(hTBSoundVolume, TBM_GETPOS, NULL, NULL);
		BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, p * 100);
		BASS_SetVolume(p / 100.f);
	}
	else if (hwndCtl == hTBPlayingSong)							//��������� �������� ����� 
	{
		INT nPos = 0;				//����� �������
		nPos = SendMessage(hTBPlayingSong, TBM_GETPOS, 0, 0);				//��������� ����� �������
		secPlaying = nPos;													//������� ������������ = ������� �������
		QWORD bytePos = BASS_ChannelSeconds2Bytes(hStream, nPos);			//������� ������� � ����� (��� ���������)
		BASS_ChannelSetPosition(hStream, bytePos, BASS_POS_BYTE);			//��������� � ������
		SendMessage(hTBPlayingSong, TBM_SETPOS, TRUE, (LPARAM)nPos);		//������� ������� �� ��������
	}
}
/*
	��������� WM_COMMAND
*/
VOID Application::Cls_OnCommand(HWND hwnd, INT id, HWND hwndCtl, UINT codeNotify)
{
	static BOOL IsStop = FALSE;				//��� �� ����� Stop
	switch (id)
	{
		case IDC_BTNPLAY:									//Play
		{
			if (hStream == NULL && dlg.songs.size() == 0)
			{
				BOOL isOpen = openFile_LoadMusic(hwnd);
				if (isOpen == TRUE)
				{
					play(hStream);
					SetTimer(hwnd, id_timer, 1000, 0);
					SetTimer(hwnd, idTimerBySpectr, 100, 0);
				}
			}
			else if (hStream == NULL && dlg.songs.size() > 0)
			{
				hStream = dlg.songs[0].hStream;
				play(hStream);
				SetTimer(hwnd, id_timer, 1000, 0);
				SetTimer(hwnd, idTimerBySpectr, 100, 0);
			}
			else if (IsStop == FALSE)
			{
				BASS_Start();
				BASS_ChannelPlay(hStream, 0);
				SetTimer(hwnd, id_timer, 1000, 0);
				SetTimer(hwnd, idTimerBySpectr, 100, 0);
			}
			else
			{
				play(hStream);
				SetTimer(hwnd, id_timer, 1000, 0);
				SetTimer(hwnd, idTimerBySpectr, 100, 0);
			}
			IsStop = FALSE;
			break;
		}
		case IDC_BTNSTOP:									//Stop
		{
			stop(hStream);
			IsStop = TRUE;
			secPlaying = 0;
			KillTimer(hwnd, id_timer);
			KillTimer(hwnd, idTimerBySpectr);
			id_timer = !id_timer;
			idTimerBySpectr = !idTimerBySpectr;
			break;
		}
		case IDC_BTNPAUSE:									//Pause
		{
			pause();
			KillTimer(hwnd, id_timer);
			KillTimer(hwnd, idTimerBySpectr);
			break;
		}
		case IDC_CHECKPLAYLIST:
		{
			static HWND hCheckPlayList = GetDlgItem(hwnd, IDC_CHECKPLAYLIST);
			BOOL isShow = Button_GetCheck(hCheckPlayList);
			if (isShow)
			{
				SendMessage(hCheckPlayList, BM_SETCHECK, BST_CHECKED, 0);
				dlg.showPlayList(SW_SHOW);
			}
			else
			{
				SendMessage(hCheckPlayList, BM_SETCHECK, BST_UNCHECKED, 0);
				dlg.showPlayList(SW_HIDE);
			}

			break;
		}
		case IDC_CHECKEQUALIZER:
		{
			static HWND hCheckEqualizer = GetDlgItem(hwnd, IDC_CHECKEQUALIZER);
			BOOL isShow = Button_GetCheck(hCheckEqualizer);
			if (isShow)
			{
				isShow = TRUE;
				SendMessage(hCheckEqualizer, BM_SETCHECK, BST_CHECKED, 0);
				equalizer.ShowEqualizer(SW_SHOW);
			}
			else
			{
				isShow = FALSE;
				SendMessage(hCheckEqualizer, BM_SETCHECK, BST_UNCHECKED, 0);
				equalizer.ShowEqualizer(SW_HIDE);
			}
			break;
		}
		case IDC_ADDSONG:									//AddSong
		{
			openFile_LoadMusic(hwnd);
			break;
		}
		case IDC_PREVSONG:									//PrevSong
		{
			prev();
			break;
		}
		case IDC_NEXTSONG:									//NextSong
		{
			next();
			break;
		}
		case IDC_BTNCLOSE:									//CLose Programm
		{
			EndDialog(hwnd, 0);
			break;
		}
		case IDC_REPEATSONG:								//Repeat song
		{
			if (IsRepeatSong == FALSE)
			{
				SendDlgItemMessage(hwnd, IDC_REPEATSONG, WM_SETTEXT, 0, (LPARAM)TEXT("Yes"));
				IsRepeatSong = TRUE;
			}
			else
			{
				SendDlgItemMessage(hwnd, IDC_REPEATSONG, WM_SETTEXT, 0, (LPARAM)TEXT("No"));
				IsRepeatSong = FALSE;
			}
			break;
		}
		default:
			break;
	}
}
/*
	��������� WM_TIMER
*/
VOID Application::Cls_OnTimer(HWND hwnd, UINT id)
{
	static HWND hStaticTimePlaying = GetDlgItem(hwnd, IDC_STATICTIMEPLAYING);
	if (id == id_timer)
	{
		secPlaying++;
		QWORD len = BASS_ChannelGetLength(hStream, BASS_POS_BYTE);
		INT seconds = BASS_ChannelBytes2Seconds(hStream, len);
		SendMessage(hTBPlayingSong, TBM_SETPOS, TRUE, (LPARAM)secPlaying);
		if (secPlaying == seconds && IsRepeatSong == FALSE && dlg.songs.size() > 1)
		{
			next();
		}
		else if (IsRepeatSong == TRUE && secPlaying == seconds)
		{
			secPlaying = 0;
			play(hStream);
		}
		else if (dlg.songs.size() == 0 && secPlaying == seconds)
		{
			secPlaying = 0;
			KillTimer(hwnd, id_timer);
		}
	}
	else if (id == idTimerBySpectr)
	{
		ControlSpectr(hwnd);
	}
}
/*
	��������� WM_SYSCOMMAND
*/
VOID Application::Cls_OnSysCommand(HWND hwnd, UINT cmd, INT x, INT y)
{
	if (cmd == SC_MINIMIZE)
	{
		AddIconInTray(hwnd);
	}
	else if (cmd == SC_RESTORE)
	{
		DeleteIconOfTray(hwnd);
	}
}
/*
	Delete icon of tray 
*/
VOID Application::DeleteIconOfTray(HWND hWnd)
{
	NOTIFYICONDATA nid;
	memset(&nid, 0, sizeof(NOTIFYICONDATA));
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hWnd;
	nid.uID = 1;
	Shell_NotifyIcon(NIM_DELETE, &nid);
	ShowWindow(hWnd, SW_NORMAL);
}
/*
	Add icon in tray
*/
VOID Application::AddIconInTray(HWND hWnd)
{
	NOTIFYICONDATA nid;
	memset(&nid, 0, sizeof(NOTIFYICONDATA));
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hWnd;
	nid.uID = 1;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_USER + 200;
	nid.hIcon = _this->hIcon;
	lstrcpy(nid.szTip, NAMEPLEER);
	Shell_NotifyIcon(NIM_ADD, &nid);
	ShowWindow(hWnd, SW_HIDE);
}
/*
	������� �������� ����
*/
HBRUSH OnCtlColor(HWND hwnd, HDC hdc, HWND hwndChild, INT type)
{
	static HBRUSH brush = CreatePatternBrush(LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1)));
	return brush;
}
/*
	���������� ��������� ��� ����������� ����
*/
INT_PTR CALLBACK Application::DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		HANDLE_MSG(hWnd, WM_CTLCOLORDLG, OnCtlColor);
		HANDLE_MSG(hWnd, WM_TIMER, _this->Cls_OnTimer);
		HANDLE_MSG(hWnd, WM_INITDIALOG, _this->Cls_OnInitDialog);
		HANDLE_MSG(hWnd, WM_HSCROLL, _this->Cls_OnHScroll);
		HANDLE_MSG(hWnd, WM_COMMAND, _this->Cls_OnCommand);
		HANDLE_MSG(hWnd, WM_SYSCOMMAND, _this->Cls_OnSysCommand);
		case WM_CTLCOLORSTATIC:
		{
			HDC hDc = (HDC)wParam;
			if (HWND(lParam) == GetDlgItem(hWnd, IDC_CHECKPLAYLIST))
			{
				SetBkMode(hDc, TRANSPARENT);
				SetTextColor(hDc, RGB(255, 255, 255));
				return (LRESULT)GetStockObject(NULL_BRUSH);
			}
			else if (HWND(lParam) == GetDlgItem(hWnd, IDC_CHECKEQUALIZER))
			{
				SetBkMode(hDc, TRANSPARENT);
				SetTextColor(hDc, RGB(255, 255, 255));
				return (LRESULT)GetStockObject(NULL_BRUSH);
			}
			break;
		}
		case WM_USER + 200:
		{
			switch (lParam)
			{
				case WM_LBUTTONDOWN:
				{
					_this->DeleteIconOfTray(hWnd);
					break;
				}
				case WM_RBUTTONDOWN:
				{
					HMENU hContextByTray;
					POINT pos;
					GetCursorPos(&pos);
					hContextByTray = CreatePopupMenu();
					AppendMenu(hContextByTray, MF_STRING, IDC_BTNPLAY, TEXT("Play"));
					AppendMenu(hContextByTray, MF_STRING, IDC_BTNPAUSE, TEXT("Pause"));
					AppendMenu(hContextByTray, MF_STRING, IDC_BTNSTOP, TEXT("Stop"));
					AppendMenu(hContextByTray, MF_STRING, IDC_NEXTSONG, TEXT("Next"));
					AppendMenu(hContextByTray, MF_STRING, IDC_PREVSONG, TEXT("Prev"));
					AppendMenu(hContextByTray, MF_SEPARATOR, 0, 0);
					AppendMenu(hContextByTray, MF_STRING, IDC_BTNCLOSE, TEXT("&Close"));
					SetForegroundWindow(hWnd);
					TrackPopupMenu(hContextByTray, TPM_LEFTALIGN, pos.x, pos.y, 0, hWnd, 0);
					DestroyMenu(hContextByTray);
					break;
				}
				default:
					break;
			}
			break;
		}
		case WM_CLOSE:
		{
			BASS_Free();
			EndDialog(hWnd, NULL);
			break;
		}
		default:
			return FALSE;
	}
	
	return FALSE;
}
/*
	Stop
*/
VOID Application::stop(HSTREAM stream)
{
	BASS_Stop();
	BASS_ChannelStop(stream);
	SetNullPosSpectr();
}
/*
	Play
*/
VOID Application::play(HSTREAM stream)
{
	BASS_Start();
	BASS_ChannelPlay(stream, TRUE);
	equalizer.SetFX(stream);
}
/*
	Pause
*/
VOID Application::pause()
{
	BASS_Pause();
	SetNullPosSpectr();
}
/*
	Prev song
*/
VOID Application::prev()
{
	INT allSongs = dlg.songs.size();		//����� �����
	INT prev = 0;							//���� �������� �����
	if (allSongs > 0)
	{
		//����� ������� �������� �����
		for (INT i = 0; i < allSongs; i++)
		{
			if (hStream == dlg.songs[i].hStream)
			{
				break;
			}
			prev++;
		}
		//���� ��� ������� �� PrevtSong ��������� ������ ������, ��������� ��������� �� ����� ������
		if (prev - 1 < 0)
			prev = allSongs - 1;
		else
			prev--;
		stop(hStream);				//��������� ������
		hStream = dlg.songs[prev].hStream;		//�������� ��������� ����� � �����
		secPlaying = 0;
		setRangeTrackBarPlaySong(hStream);
		play(hStream);				//������������� �����
	}
}
/*
	Next song
*/
VOID Application::next()
{
	INT allSongs = dlg.songs.size();		//����� �����
	INT next = 0;						//���� �������� �����
	if (allSongs > 0)
	{
		//����� ������� �������� �����
		for (INT i = 0; i < allSongs; i++)
		{
			if (hStream == dlg.songs[i].hStream)
			{
				next++;
				break;
			}
			next++;
		}
		//���� ��� ������� �� NextSong ��������� ����� ������, ��������� ��������� �� ������ ������
		if (next + 1 > allSongs)
			next = 0;
		stop(hStream);				//��������� ������
		hStream = dlg.songs[next].hStream;		//�������� ��������� ����� � �����
		secPlaying = 0;
		setRangeTrackBarPlaySong(hStream);
		play(hStream);				//������������� �����
	}
}
/*
	��������� ��������� TrackBar (����� ������������) ������� �������� �����
*/
VOID Application::setRangeTrackBarPlaySong(HSTREAM stream)
{
	QWORD len = BASS_ChannelGetLength(stream, BASS_POS_BYTE);
	INT seconds = BASS_ChannelBytes2Seconds(stream, len);
	SendMessage(hTBPlayingSong, TBM_SETRANGE, 0, (LPARAM)MAKELPARAM(0, seconds + 1));
}
/*
	������
*/
VOID Application::ControlSpectr(HWND hWnd)
{
	FLOAT buffer[128];
	BASS_ChannelGetData(hStream, buffer, BASS_DATA_FFT256);
	INT idx = 10;
	for (INT i = 0; i < 44; i++)
	{
		SendMessage(spectrs[i], PBM_SETPOS, buffer[idx] * 3 * 368 - 4, 0);
		idx++;
	}
}
/*
	��������� ����� � ��������� ������������
*/
VOID Application::SetSpectrColor(HWND hWnd)
{
	INT color = 255;
	for (INT i = 0; i < 44; i++)
	{
		spectrs[i] = GetDlgItem(hWnd, i + 1032);
		SendMessage(spectrs[i], PBM_SETBKCOLOR, 0, (LPARAM)RGB(0, 0, 0));
		if (i > 35)
			SendMessage(spectrs[i], PBM_SETRANGE32, 0, 80);
		if (i <= 15)
			color -= 5;
		else if (i <= 30 && i > 15)
			color -= 3;
		else
			color += 10;

		SendMessage(spectrs[i], PBM_SETBARCOLOR, 0, (LPARAM)RGB(color, color, color));
	}
}
/*
	��������� ������������ �� 0 (��� ������� �� �����/����)
*/
VOID Application::SetNullPosSpectr()
{
	for (INT i = 0; i < 44; i++)
	{
		SendMessage(spectrs[i], PBM_SETPOS, TRUE, (LPARAM)0);
	}
}