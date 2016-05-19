#pragma once
#include "include.h"
#include "DlgPlayList.h"
#include "DlgEqualizer.h"

class Application
{
private:
	RECT spectrRECT;								//Размер спектра
	HANDLE hMutex;									//Мютекс для проверки повторного открытия окна
	DlgPlayList playlist;							//Объект класса PlayList
	DlgEqualizer equalizer;							//Объект класса Equalizer
	HWND hTBSoundVolume;							//Slider - громкость 
	HWND hTBPlayingSong;							//Slider - текущее время песни
	HWND hSlider_Balance;							//Slider баланса
	UINT id_timer;									//ID таймера, который отвечает за полосу перемотки
	UINT idTimerBySpectr;							//ID таймера, который отвечает за спектр
	INT secPlaying;									//Количество секунд проигранных текущей песней
	HICON hIcon;									//Иконка приложения
	BOOL IsRepeatSong;								//Повтор песни
	HSTREAM hStream;								//Поток воспроизведения песни
	INT contour_red, contour_green, contour_blue;	//RGB смешивание контура
	INT fill_red, fill_green, fill_blue;			//RGB смешивание заливки
	/*
		Контекстное меню
	*/
	HMENU hContextMenu = CreatePopupMenu();
	HMENU hColor = CreatePopupMenu();
	HMENU hColorContour = CreatePopupMenu();
	HMENU hColorFill = CreatePopupMenu();
	HMENU hTranperency = CreatePopupMenu();
public:
	Application(VOID);								//конструктор по умолчанию
	~Application();									//деструктор
	static Application* _this;						//собственный указатель
	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);				//Диалоговая процедура для диалогового окна
	
	VOID Cls_OnCommand(HWND hwnd, INT id, HWND hwndCtl, UINT codeNotify);								//WM_COMMAND
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);									//WM_INITDIALOG
	VOID Cls_OnHScroll(HWND hwnd, HWND hwndCtl, UINT code, INT pos);									//WM_HSCROLL
	VOID Cls_OnTimer(HWND hwnd, UINT id);																//WM_TIMER
	VOID Cls_OnSysCommand(HWND hwnd, UINT cmd, INT x, INT y);											//WM_SYSCOMMAND
	VOID Cls_OnRButtonDown(HWND hwnd, BOOL fDoubleClick, INT x, INT y, UINT keyFlags);					//WM_RBUTTONDOWN
	HBRUSH OnColorStatic(HWND hwnd, HDC hdc, HWND hwndChild, INT type);							//Отображение статика
	HBRUSH OnCtlColor(HWND hwnd, HDC hdc, HWND hwndChild, INT type);							//Отображение фона окна
	INT runProgramm();												//старт программы
	INT CheckedInitBASS();											//проверка инициализации BASS
	BOOL openFile_LoadMusic(HWND hWnd);								//открытие песни с компьютера OPENFILENAME
	VOID setRangeTrackBarPlaySong(HSTREAM stream);					//установка диапазона на TrackBar при запуске новой песни
	VOID stop(HSTREAM stream);										//Stop
	VOID play(HSTREAM stream);										//Play
	VOID pause();													//Pause
	VOID next();													//Next Song
	VOID prev();													//Prev Song
	VOID DeleteIconOfTray(HWND hWnd);								//Разворачивание окна из трея
	VOID AddIconInTray(HWND hWnd);									//Сворачивание окна в Трей
	VOID CheckOpeningCopy(HWND hwnd);								//Проверка на открытие копий приложения
	VOID ColorFillSpectrum(INT r, INT g, INT b);					//Цвет заливки спектра
	VOID ColorContourSpectrum(INT r, INT g, INT b);					//Цвет контура заливки
	VOID TransparencyWindow(HWND hWnd, INT value);					//Прозрачность окна
	VOID showTimePlaying(HWND hWnd, INT secPlaying);				//Время играющей песни
	VOID showNameSong(HSTREAM stream, HWND hWnd);					//отображение названия текущей песни
	VOID UncheckedAllMenuItemContour();								//Снятие всех галочек с элементов меню Контура
	VOID UncheckedAllMenuItemFill();								//Снятие всех галочек с элементов меню Заливки
	VOID UncheckedAllMenuItemTransperency();						//Снятие всех галочек с элементов меню Прозрачности
	friend class DlgPlayList;
};