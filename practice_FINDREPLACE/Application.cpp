#include "Application.h"

Application* Application::_this = NULL;
/*
	Constructor 
*/
Application::Application(VOID)
{	
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);
	_this = this;
	CheckedInitBASS();					//проверка инициализации BASS
	secPlaying = 0;						//время играющей песни
	id_timer = 0;						//ID timer by PlayingSongs
	idTimerBySpectr = 1;				//ID timer by Spectrum
	IsRepeatSong = FALSE;				//повтор песни
}
/*
	Destructor
*/
Application::~Application()
{
	ReleaseMutex(hMutex);
}
/*
	Проверка версии BASS и инициализация устройства
*/
INT Application::CheckedInitBASS()
{
	if (HIWORD(BASS_GetVersion()) != BASSVERSION)
	{
		MessageBox(NULL, TEXT("Ошибка версии BASS."), NULL, 0);
		return 1;
	}
	if (!BASS_Init(-1, 44100, BASS_DEVICE_3D, 0, NULL))
	{
		MessageBox(NULL, TEXT("Не удалось инициализировать BASS."), NULL, 0);
		return 1;
	}
}
/*
	Проверка открытия повторных копий приложения 
*/
VOID Application::CheckOpeningCopy(HWND hwnd)
{
	CONST TCHAR GUID[] = { TEXT("64AE5751 - 8A85 - 4566 - AE2E - 24C3CFD40E87") };
	hMutex = CreateMutex(NULL, FALSE, GUID);
	DWORD dwAnswer = WaitForSingleObject(hMutex, 0);
	if (dwAnswer == WAIT_TIMEOUT)
	{
		std::wstringstream ss;
		ss << NAMEPLEER << TEXT(" уже запущен");
		MessageBox(hwnd, ss.str().c_str(), TEXT("Information"), MB_ICONEXCLAMATION);
		EndDialog(hwnd, 0);
	}
}
/*
	Цвет заливки спектра
*/
VOID Application::ColorFillSpectrum(INT r, INT g, INT b)
{
	fill_red = r;
	fill_green = g;
	fill_blue = b;
}
/*
	Цвет контура спектра
*/
VOID Application::ColorContourSpectrum(INT r, INT g, INT b)
{
	contour_red = r;
	contour_green = g;
	contour_blue = b;
}
/*
	Прозрачность окна
*/
VOID Application::TransparencyWindow(HWND hWnd, INT value)
{
	// Set WS_EX_LAYERED on this window
	SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	// Make this window 70% alpha
	SetLayeredWindowAttributes(hWnd, 0, (255 * value) / 100, LWA_ALPHA);
}
/*
	Вывод времени проигрывания
*/
VOID Application::showTimePlaying(HWND hWnd, INT secPlaying)
{
	INT sec, min;
	min = secPlaying / 60;
	sec = secPlaying % 60;
	std::wstringstream ss;
	if (min < 10 && sec < 10)
	{
		ss << "0" << min << ":0" << sec;
	}
	else if (min < 10 && sec >= 10)
	{
		ss << "0" << min << ":" << sec;
	}
	else if (min >= 10 && sec < 10)
	{
		ss << min << ":0" << sec;
	}
	else if (min >= 10 && sec >= 10)
	{
		ss << min << ":" << sec;
	}
	SetWindowText(GetDlgItem(hWnd, IDC_OUTTIME), ss.str().c_str());
}
/*
	Запуск программы
*/
INT Application::runProgramm()
{
	return DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}
/*
	Загрузка песни с компьютера
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
			playlist.addSongToPlayList(hStream, path);
		}
		else
		{
			HSTREAM stream;
			stream = BASS_StreamCreateFile(FALSE, path, 0, 0, 0);
			setRangeTrackBarPlaySong(stream);
			playlist.addSongToPlayList(stream, path);
		}
		return TRUE;
	}
	return FALSE;
}
/*
	Processing WM_INITDIALOG
*/
BOOL Application::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	CheckOpeningCopy(hwnd);				//запуск проверки на открытие копий
	/*
		Установка заголовка окна
	*/
	SetWindowText(hwnd, NAMEPLEER);
	/*
		Загрузка иконки приложения
	*/	
	hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON1));
	SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);		
	/*
		Слайдер громкости
	*/
	hTBSoundVolume = GetDlgItem(hwnd, IDC_TRACKBARSOUND); 
	SendMessage(hTBSoundVolume, TBM_SETPOS, TRUE, (LPARAM)100);
	/*
		Слайдер полосы перемотки 
	*/
	hTBPlayingSong = GetDlgItem(hwnd, IDC_SLIDER_TIMEPLAYING);
	/*
		Слайдер баланса
	*/
	hSlider_Balance = GetDlgItem(hwnd, IDC_SLIDERBALANCE);
	SendMessage(hSlider_Balance, TBM_SETPOS, TRUE, (LPARAM)50);
	SendMessage(hSlider_Balance, TBM_SETRANGE, 0, (LPARAM)MAKELPARAM(-5, 5));
	/*
		Создание окна плейлист
	*/
	playlist.hDlg = CreateDialog(GetModuleHandle(0), MAKEINTRESOURCE(IDD_DLGPLAYLIST), hwnd, playlist.DlgProc);
	/*
		Создание окна эквалайзер
	*/
	equalizer.hDlg = CreateDialog(GetModuleHandle(0), MAKEINTRESOURCE(IDD_DLGEQUALIZER), hwnd, equalizer.DlgProc);
	/*
		Установка времени на статике
	*/
	Static_SetText(GetDlgItem(hwnd, IDC_OUTTIME), TEXT("00:00"));
	/*
		Загрузка изображений на элементы управления
	*/
	//No repeat
	HBITMAP bmp = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAPNOREPEAT));
	SendMessage(GetDlgItem(hwnd, IDC_REPEATSONG), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
	//Prev
	bmp = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAPPREV));
	SendMessage(GetDlgItem(hwnd, IDC_PREVSONG), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
	//Pause
	bmp = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAPPAUSE));
	SendMessage(GetDlgItem(hwnd, IDC_BTNPAUSE), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
	//Play
	bmp = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAPBTNPLAY));
	SendMessage(GetDlgItem(hwnd, IDC_BTNPLAY), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
	//Stop
	bmp = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAPSTOP));
	SendMessage(GetDlgItem(hwnd, IDC_BTNSTOP), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
	//Next
	bmp = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAPNEXT));
	SendMessage(GetDlgItem(hwnd, IDC_NEXTSONG), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
	/*
		Создание контекстного меню
	*/
	AppendMenu(hContextMenu, MF_STRING | MF_POPUP, (INT_PTR)hColor, TEXT("Color spectrum"));

	AppendMenu(hColor, MF_STRING | MF_POPUP, (INT_PTR)hColorContour, TEXT("Color contour"));
	AppendMenu(hColor, MF_STRING | MF_POPUP, (INT_PTR)hColorFill, TEXT("Color fill"));

	AppendMenu(hColorContour, MF_STRING, COLOR_CONTOUR_RED, TEXT("Red"));
	AppendMenu(hColorContour, MF_STRING, COLOR_CONTOUR_GREEN, TEXT("Green"));
	AppendMenu(hColorContour, MF_STRING, COLOR_CONTOUR_BLUE, TEXT("Blue"));
	AppendMenu(hColorContour, MF_STRING, COLOR_CONTOUR_WHITE, TEXT("White"));
	AppendMenu(hColorContour, MF_STRING, COLOR_CONTOUR_BLACK, TEXT("Black"));

	AppendMenu(hColorFill, MF_STRING, COLOR_FILL_RED, TEXT("Red"));
	AppendMenu(hColorFill, MF_STRING, COLOR_FILL_GREEN, TEXT("Green"));
	AppendMenu(hColorFill, MF_STRING, COLOR_FILL_BLUE, TEXT("Blue"));
	AppendMenu(hColorFill, MF_STRING, COLOR_FILL_WHITE, TEXT("White"));
	AppendMenu(hColorFill, MF_STRING, COLOR_FILL_BLACK, TEXT("Black"));

	/*
		Transperency
	*/
	AppendMenu(hContextMenu, MF_STRING | MF_POPUP, (INT_PTR)hTranperency, TEXT("Transparency window"));

	AppendMenu(hTranperency, MF_STRING, TRANSPARENCY_100, TEXT("100%"));
	AppendMenu(hTranperency, MF_STRING, TRANSPARENCY_90, TEXT("90%"));
	AppendMenu(hTranperency, MF_STRING, TRANSPARENCY_80, TEXT("80%"));
	AppendMenu(hTranperency, MF_STRING, TRANSPARENCY_70, TEXT("70%"));
	AppendMenu(hTranperency, MF_STRING, TRANSPARENCY_60, TEXT("60%"));
	AppendMenu(hTranperency, MF_STRING, TRANSPARENCY_50, TEXT("50%"));
	AppendMenu(hTranperency, MF_STRING, TRANSPARENCY_40, TEXT("40%"));
	AppendMenu(hTranperency, MF_STRING, TRANSPARENCY_30, TEXT("30%"));
	AppendMenu(hTranperency, MF_STRING, TRANSPARENCY_20, TEXT("20%"));
	AppendMenu(hTranperency, MF_STRING, TRANSPARENCY_10, TEXT("10%"));
	/*
		Инициализация цветов спектра
	*/
	contour_red = contour_green = contour_blue = 0;
	fill_red = 255;
	fill_green = fill_blue = 0;
	/*
		Установка галочек в меню и цвет спектра
	*/
	CheckMenuItem(hColorContour, COLOR_CONTOUR_BLACK, MF_BYCOMMAND | MF_CHECKED);
	CheckMenuItem(hColorFill, COLOR_FILL_RED, MF_BYCOMMAND | MF_CHECKED);
	CheckMenuItem(hTranperency, TRANSPARENCY_100, MF_BYCOMMAND | MF_CHECKED);
	return TRUE;
}
/*
	Processing WM_HSCROLL
*/
VOID Application::Cls_OnHScroll(HWND hwnd, HWND hwndCtl, UINT code, INT pos)
{
	/*
		Громкость
	*/
	if (hwndCtl == hTBSoundVolume)											//Adjusting the volume control (Настройка регулятора громкости)
	{
		INT p = SendMessage(hTBSoundVolume, TBM_GETPOS, NULL, NULL);		//Получение позиции
		DOUBLE nPos = p * 0.01;		//новая позиция уровня громкости
		/*
			Собственный регулятор громкости
		*/
		SendMessage(hTBSoundVolume, TBM_SETPOS, TRUE, (LPARAM)p);
		BASS_ChannelSetAttribute(hStream, BASS_ATTRIB_VOL, nPos);
		/*
			Привязка к windows регулятору громкости
		*/
		//BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, p * 100);
		//BASS_SetVolume(p / 100.f);
		
	}
	/*
		Время проигрывания
	*/	
	else if (hwndCtl == hTBPlayingSong)										
	{
		INT nPos = 0;														//new position
		nPos = SendMessage(hTBPlayingSong, TBM_GETPOS, 0, 0);				//Get new position
		secPlaying = nPos;													//Seconds playing = current position
		QWORD bytePos = BASS_ChannelSeconds2Bytes(hStream, nPos);			//Translated seconds in bytes 
		BASS_ChannelSetPosition(hStream, bytePos, BASS_POS_BYTE);			//Rewind in bytes
		SendMessage(hTBPlayingSong, TBM_SETPOS, TRUE, (LPARAM)nPos);		//Translated position	
	}
	/*
		Баланс
	*/
	else if (hwndCtl == hSlider_Balance)
	{
		INT p = SendMessage(hSlider_Balance, TBM_GETPOS, NULL, NULL);
		BASS_ChannelSetAttribute(Application::_this->hStream, BASS_ATTRIB_PAN, p / 5);
	}
}
/*
	Processing WM_COMMAND
*/
VOID Application::Cls_OnCommand(HWND hwnd, INT id, HWND hwndCtl, UINT codeNotify)
{
	static BOOL IsStop = FALSE;				//был ли нажат Stop
	switch (id)
	{
		case IDC_BTNPLAY:						//Play
		{
			if (hStream == NULL && playlist.songs.size() == 0)
			{
				BOOL isOpen = openFile_LoadMusic(hwnd);	
				if (isOpen == TRUE)
				{
					play(hStream);
					SetTimer(hwnd, id_timer, 1000, 0);
					SetTimer(hwnd, idTimerBySpectr, 100, 0);
				}
			}
			else if (hStream == NULL && playlist.songs.size() > 0)
			{
				hStream = playlist.songs[0].hStream;
				play(hStream);
				SetTimer(hwnd, id_timer, 1000, 0);
				SetTimer(hwnd, idTimerBySpectr, 100, 0);
			}
			else if (IsStop == FALSE)
			{
				BASS_Start();
				BASS_ChannelPlay(hStream, 0);
				SetTimer(hwnd, id_timer, 1000, 0);
				SetTimer(hwnd, idTimerBySpectr, 100 , 0);
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
		/*
			Checkbox плейлист
		*/
		case IDC_CHECKPLAYLIST:
		{
			static HWND hCheckPlayList = GetDlgItem(hwnd, IDC_CHECKPLAYLIST);
			BOOL isShow = Button_GetCheck(hCheckPlayList);
			if (isShow)
			{
				SendMessage(hCheckPlayList, BM_SETCHECK, BST_CHECKED, 0);
				playlist.showPlayList(SW_SHOW);
			}
			else
			{
				SendMessage(hCheckPlayList, BM_SETCHECK, BST_UNCHECKED, 0);
				playlist.showPlayList(SW_HIDE);
			}

			break;
		}
		/*
			Checkbox эквалайзер
		*/
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
		case IDC_ADDSONG:									//Добавление песни
		{
			openFile_LoadMusic(hwnd);
			break;
		}
		case IDC_PREVSONG:									//Предыдущая песня
		{
			prev();
			break;
		}
		case IDC_NEXTSONG:									//Следующая песня
		{
			next();
			break;
		}
		case IDC_BTNCLOSE:									//Закрытие программы
		{
			EndDialog(hwnd, 0);
			break;
		}
		case IDC_REPEATSONG:								//Повтор песни
		{
			if (IsRepeatSong == FALSE)
			{
				SendDlgItemMessage(hwnd, IDC_REPEATSONG, WM_SETTEXT, 0, (LPARAM)TEXT("Yes"));
				HBITMAP bmp = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAPREPEAT));
				SendMessage(GetDlgItem(hwnd, IDC_REPEATSONG), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
				IsRepeatSong = TRUE;
			}
			else
			{
				SendDlgItemMessage(hwnd, IDC_REPEATSONG, WM_SETTEXT, 0, (LPARAM)TEXT("No"));
				HBITMAP bmp = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAPNOREPEAT));
				SendMessage(GetDlgItem(hwnd, IDC_REPEATSONG), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
				IsRepeatSong = FALSE;
			}
			break;
		}
		/*
			Цвета контура
		*/
		case COLOR_CONTOUR_RED:
		{
			UncheckedAllMenuItemContour();
			CheckMenuItem(hColorContour, COLOR_CONTOUR_RED, MF_BYCOMMAND | MF_CHECKED);
			ColorContourSpectrum(255, 0, 0);
			break;
		}
		case COLOR_CONTOUR_GREEN:
		{
			UncheckedAllMenuItemContour();
			CheckMenuItem(hColorContour, COLOR_CONTOUR_GREEN, MF_BYCOMMAND | MF_CHECKED);
			ColorContourSpectrum(0, 255, 0);
			break;
		}
		case COLOR_CONTOUR_BLUE:
		{
			UncheckedAllMenuItemContour();
			CheckMenuItem(hColorContour, COLOR_CONTOUR_BLUE, MF_BYCOMMAND | MF_CHECKED);
			ColorContourSpectrum(0, 0, 255);
			break;
		}
		case COLOR_CONTOUR_WHITE:
		{
			UncheckedAllMenuItemContour();
			CheckMenuItem(hColorContour, COLOR_CONTOUR_WHITE, MF_BYCOMMAND | MF_CHECKED);
			ColorContourSpectrum(255, 255, 255);
			break;
		}
		case COLOR_CONTOUR_BLACK:
		{
			UncheckedAllMenuItemContour();
			CheckMenuItem(hColorContour, COLOR_CONTOUR_BLACK, MF_BYCOMMAND | MF_CHECKED);
			ColorContourSpectrum(0, 0, 0);
			break;
		}
		/*
			Цвета заливки
		*/
		case COLOR_FILL_RED:
		{
			UncheckedAllMenuItemFill();
			CheckMenuItem(hColorFill, COLOR_FILL_RED, MF_BYCOMMAND | MF_CHECKED);
			ColorFillSpectrum(255, 0, 0);
			break;
		}
		case COLOR_FILL_GREEN:
		{
			UncheckedAllMenuItemFill();
			CheckMenuItem(hColorFill, COLOR_FILL_GREEN, MF_BYCOMMAND | MF_CHECKED);
			ColorFillSpectrum(0, 255, 0);
			break;
		}
		case COLOR_FILL_BLUE:
		{
			UncheckedAllMenuItemFill();
			CheckMenuItem(hColorFill, COLOR_FILL_BLUE, MF_BYCOMMAND | MF_CHECKED);
			ColorFillSpectrum(0, 0, 255);
			break;
		}
		case COLOR_FILL_WHITE:
		{
			UncheckedAllMenuItemFill();
			CheckMenuItem(hColorFill, COLOR_FILL_WHITE, MF_BYCOMMAND | MF_CHECKED);
			ColorFillSpectrum(255, 255, 255);
			break;
		}
		case COLOR_FILL_BLACK:
		{
			UncheckedAllMenuItemFill();
			CheckMenuItem(hColorFill, COLOR_FILL_BLACK, MF_BYCOMMAND | MF_CHECKED);
			ColorFillSpectrum(0, 0, 0);
			break;
		}
		/*
			Прозрачность окна
		*/
		case TRANSPARENCY_100:
		{
			UncheckedAllMenuItemTransperency();
			CheckMenuItem(hTranperency, TRANSPARENCY_100, MF_BYCOMMAND | MF_CHECKED);
			TransparencyWindow(hwnd, 100);
			break;
		}
		case TRANSPARENCY_90:
		{
			UncheckedAllMenuItemTransperency();
			CheckMenuItem(hTranperency, TRANSPARENCY_90, MF_BYCOMMAND | MF_CHECKED);
			TransparencyWindow(hwnd, 90);
			break;
		}
		case TRANSPARENCY_80:
		{
			UncheckedAllMenuItemTransperency();
			CheckMenuItem(hTranperency, TRANSPARENCY_80, MF_BYCOMMAND | MF_CHECKED);
			TransparencyWindow(hwnd, 80);
			break;
		}
		case TRANSPARENCY_70:
		{
			UncheckedAllMenuItemTransperency();
			CheckMenuItem(hTranperency, TRANSPARENCY_70, MF_BYCOMMAND | MF_CHECKED);
			TransparencyWindow(hwnd, 70);
			break;
		}
		case TRANSPARENCY_60:
		{
			UncheckedAllMenuItemTransperency();
			CheckMenuItem(hTranperency, TRANSPARENCY_60, MF_BYCOMMAND | MF_CHECKED);
			TransparencyWindow(hwnd, 60);
			break;
		}
		case TRANSPARENCY_50:
		{
			UncheckedAllMenuItemTransperency();
			CheckMenuItem(hTranperency, TRANSPARENCY_50, MF_BYCOMMAND | MF_CHECKED);
			TransparencyWindow(hwnd, 50);
			break;
		}
		case TRANSPARENCY_40:
		{
			UncheckedAllMenuItemTransperency();
			CheckMenuItem(hTranperency, TRANSPARENCY_40, MF_BYCOMMAND | MF_CHECKED);
			TransparencyWindow(hwnd, 40);
			break;
		}
		case TRANSPARENCY_30:
		{
			UncheckedAllMenuItemTransperency();
			CheckMenuItem(hTranperency, TRANSPARENCY_30, MF_BYCOMMAND | MF_CHECKED);
			TransparencyWindow(hwnd, 30);
			break;
		}
		case TRANSPARENCY_20:
		{
			UncheckedAllMenuItemTransperency();
			CheckMenuItem(hTranperency, TRANSPARENCY_20, MF_BYCOMMAND | MF_CHECKED);
			TransparencyWindow(hwnd, 20);
			break;
		}
		case TRANSPARENCY_10:
		{
			UncheckedAllMenuItemTransperency();
			CheckMenuItem(hTranperency, TRANSPARENCY_10, MF_BYCOMMAND | MF_CHECKED);
			TransparencyWindow(hwnd, 10);
			break;
		}
		default:
			break;
	}
}
/*
	Отображение название текущей играемой песни
*/
VOID Application::showNameSong(HSTREAM stream, HWND hWnd)
{
	TAG_ID3* id3 = (TAG_ID3*)BASS_ChannelGetTags(stream, BASS_TAG_ID3);		//получение данных о песне
	std::wstringstream infoAboutTheSong;									//Буфер строки (Название песни + Исполнитель + время)
	infoSong info;			
	TCHAR path[MAX_PATH];
	for (int i = 0;i < playlist.songs.size();i++)
	{
		if (stream == playlist.songs[i].hStream)
		{
			lstrcpy(path, playlist.songs[i].path);
			break;
		}
	}
	/*
		Если при загрузке какие то данные загрузились с результатом NULL, 
		то данные о песне вырезаются с пути
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
		name[k - 4] = '\0';			//Delete format songs
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
	SetWindowText(hWnd, infoAboutTheSong.str().c_str());
}
/*
	WM_TIMER
*/
VOID Application::Cls_OnTimer(HWND hwnd, UINT id)
{
	if (id == id_timer)
	{
		QWORD len = BASS_ChannelGetLength(hStream, BASS_POS_BYTE);
		INT seconds = BASS_ChannelBytes2Seconds(hStream, len);
		SendMessage(hTBPlayingSong, TBM_SETPOS, TRUE, (LPARAM)secPlaying);
		INT pos = SendMessage(hTBPlayingSong, TBM_GETPOS, 0, 0);
		if (secPlaying == seconds && IsRepeatSong == FALSE && playlist.songs.size() > 1)
		{
			next();
		}
		else if (IsRepeatSong == TRUE && secPlaying == seconds)
		{
			SendMessage(hTBPlayingSong, TBM_SETPOS, TRUE, (LPARAM)0);
			secPlaying = 0;
			play(hStream);
		}
		else if (playlist.songs.size() == 1 && secPlaying == seconds)
		{
			secPlaying = 0;
			KillTimer(hwnd, id_timer);
		}else if (pos == seconds)
		{
			secPlaying = 0;
			stop(hStream);
			KillTimer(hwnd, id_timer);
		}
		/*
			Показ времени проигрывания
		*/
		showTimePlaying(hwnd, secPlaying);
		if (secPlaying == 0)
			showNameSong(hStream, hwnd);
		secPlaying++;
	}
	else if (id == idTimerBySpectr)
	{
		InvalidateRect(hwnd, &spectrRECT, TRUE);
	}
}
/*
	WM_SYSCOMMAND
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
	Удаление иконки из трея
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
	Добавление иконки в трей
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
	WM_RBUTTONDOWN
*/
VOID Application::Cls_OnRButtonDown(HWND hwnd, BOOL fDoubleClick, INT x, INT y, UINT keyFlags)
{
	POINT p;
	GetCursorPos(&p);
	TrackPopupMenu(hContextMenu, TPM_LEFTALIGN, p.x, p.y, 0, hwnd, 0);
}
/*
	Загрузка фона
*/
HBRUSH Application::OnCtlColor(HWND hwnd, HDC hdc, HWND hwndChild, INT type)
{
	static HBRUSH brush = CreatePatternBrush(LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAPBCKG)));
	return brush;
}
/*
	отображение статиков
*/
HBRUSH Application::OnColorStatic(HWND hwnd, HDC hdc, HWND hwndChild, INT type)
{
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 255, 255));
	return (HBRUSH)GetStockObject(BLACK_BRUSH);
}
/*
	Диалоговая процедура
*/
INT_PTR CALLBACK Application::DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		HANDLE_MSG(hWnd, WM_CTLCOLORSTATIC, _this->OnColorStatic);
		HANDLE_MSG(hWnd, WM_CTLCOLORDLG, _this->OnCtlColor);
		HANDLE_MSG(hWnd, WM_TIMER, _this->Cls_OnTimer);
		HANDLE_MSG(hWnd, WM_INITDIALOG, _this->Cls_OnInitDialog);
		HANDLE_MSG(hWnd, WM_HSCROLL, _this->Cls_OnHScroll);
		HANDLE_MSG(hWnd, WM_COMMAND, _this->Cls_OnCommand);
		HANDLE_MSG(hWnd, WM_SYSCOMMAND, _this->Cls_OnSysCommand);
		HANDLE_MSG(hWnd, WM_RBUTTONDOWN, _this->Cls_OnRButtonDown);
		/*
			Управление с трея
		*/
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
		/*
			Отрисовка спектра
		*/
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			static RECT rect;
			GetClientRect(hWnd, &rect);
			static INT height = rect.bottom / 2 - 10;
			/*
				Initialization size rectangle by redraw spectrum
			*/
			_this->spectrRECT.left = rect.left;
			_this->spectrRECT.top = rect.top;
			_this->spectrRECT.right = rect.right;
			_this->spectrRECT.bottom = height;
			/*
				Yellow pen
			*/
			HBRUSH black_brush = CreateSolidBrush
				(
				RGB(_this->fill_red, 
					_this->fill_green, 
					_this->fill_blue)
				);
			/*
				Red pen
			*/
			HPEN red_pen = CreatePen
				(PS_SOLID, 1, 
					RGB(_this->contour_red, 
						_this->contour_green,
						_this->contour_blue)
				);
			int x;
			int y = height;
			int offset = 35;
			int columns = 77;			//count columns
			int w = (rect.right - rect.left - 35) / columns ; // ширина столбика
			FLOAT buffer_peaks[128] = { height };
			BASS_ChannelGetData(_this->hStream, buffer_peaks, BASS_DATA_FFT256);

			for (int i = 0; i < columns;i++)
			{
				SelectPen(hdc, red_pen);
				x = rect.left + offset + w * i;
				y = abs(buffer_peaks[i] * 400 * 3 - height);
				if (y > height)
					y = height;
				SelectObject(hdc, black_brush);				
				SelectPen(hdc, red_pen);
				if(height - y > 1)
					Rectangle(hdc, x, y, x + w, height);
			}
			
			EndPaint(hWnd, &ps);
			ReleaseDC(hWnd, hdc);
			break;
		}
		case WM_CLOSE:
		{
			BASS_Free();
			BASS_StreamFree(_this->hStream);
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
}
/*
	Prev song
*/
VOID Application::prev()
{
	INT allSongs = playlist.songs.size();		//всего песен
	INT prev = 0;							//след играющая песня
	if (allSongs > 0)
	{
		//Поиск текущей играемой песни
		for (INT i = 0; i < allSongs; i++)
		{
			if (hStream == playlist.songs[i].hStream)
			{
				break;
			}
			prev++;
		}
		//Если при нажатии на PrevtSong достигнут начало списка, перевести указатель на конец списка
		if (prev - 1 < 0)
			prev = allSongs - 1;
		else
			prev--;

		stop(hStream);				//остановка потока
		hStream = playlist.songs[prev].hStream;		//загрузка следующей песни в поток
		secPlaying = 0;				//обнуление проигранных секунд
		setRangeTrackBarPlaySong(hStream);		//установка диапазона полосы проигрывания
		equalizer.SetFX(hStream);
		play(hStream);				//воспроизвести поток
		SetTimer(GetParent(hTBPlayingSong), id_timer, 1000, 0);
		SendMessage(playlist.hPlayList, LB_SETCURSEL, prev, 0);	//выделяет текущую песня в плейлисте
	}
}
/*
	Next song
*/
VOID Application::next()
{
	INT allSongs = playlist.songs.size();		//всего песен
	INT next = 0;						//след играющая песня
	if (allSongs > 0)
	{
		//Поиск текущей играемой песни
		for (INT i = 0; i < allSongs; i++)
		{
			if (hStream == playlist.songs[i].hStream)
			{
				next++;
				break;
			}
			next++;
		}
		//Если при нажатии на NextSong достигнут конец списка, перевести указатель на начало списка
		if (next + 1 > allSongs)
			next = 0;

		stop(hStream);								//остановка потока
		hStream = playlist.songs[next].hStream;		//загрузка следующей песни в поток
		secPlaying = 0;								//сброс проигранных секунд
		setRangeTrackBarPlaySong(hStream);			//установка диапазона TrackBar под играющую песню 
		equalizer.SetFX(hStream);					
		play(hStream);								//воспроизвести поток
		SetTimer(GetParent(hTBPlayingSong), id_timer, 1000, 0);	//Запуск таймера 
		SendMessage(playlist.hPlayList, LB_SETCURSEL, next, 0);	//выделяет текущую песня в плейлисте
	}
}
/*
	Установка диапазона полосы перемотки
*/
VOID Application::setRangeTrackBarPlaySong(HSTREAM stream)
{
	QWORD len = BASS_ChannelGetLength(stream, BASS_POS_BYTE);			//получение длины песни в байтах
	INT seconds = BASS_ChannelBytes2Seconds(stream, len);				//преобразование байт в секунды
	SendMessage(hTBPlayingSong, TBM_SETRANGE, 0, (LPARAM)MAKELPARAM(0, seconds + 1));		//установка диапазон полосы прокрутки (диапазон = количеству секунд текущей песни)
}
/*
	Снятие галочек с элементов меню
*/
	//Контур
VOID Application::UncheckedAllMenuItemContour()
{
	CheckMenuItem(hColorContour, COLOR_CONTOUR_RED, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hColorContour, COLOR_CONTOUR_BLACK, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hColorContour, COLOR_CONTOUR_BLUE, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hColorContour, COLOR_CONTOUR_GREEN, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hColorContour, COLOR_CONTOUR_WHITE, MF_BYCOMMAND | MF_UNCHECKED);
}
	//Заливка
VOID Application::UncheckedAllMenuItemFill()
{
	CheckMenuItem(hColorFill, COLOR_FILL_RED, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hColorFill, COLOR_FILL_BLACK, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hColorFill, COLOR_FILL_BLUE, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hColorFill, COLOR_FILL_GREEN, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hColorFill, COLOR_FILL_WHITE, MF_BYCOMMAND | MF_UNCHECKED);
}
	//Прозрачность
VOID Application::UncheckedAllMenuItemTransperency()
{
	CheckMenuItem(hTranperency, TRANSPARENCY_10, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hTranperency, TRANSPARENCY_20, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hTranperency, TRANSPARENCY_30, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hTranperency, TRANSPARENCY_40, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hTranperency, TRANSPARENCY_50, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hTranperency, TRANSPARENCY_60, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hTranperency, TRANSPARENCY_70, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hTranperency, TRANSPARENCY_80, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hTranperency, TRANSPARENCY_90, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hTranperency, TRANSPARENCY_100, MF_BYCOMMAND | MF_UNCHECKED);
}