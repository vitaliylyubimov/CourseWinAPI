#pragma once
#include "include.h"
#include "DlgPlayList.h"
#include "DlgEqualizer.h"

class Application
{
private:
	RECT spectrRECT;
	HANDLE hMutex;
	DlgPlayList dlg;								//Class object PlayList
	DlgEqualizer equalizer;							//Class object Equalizer
	HWND hTBSoundVolume;							//TRackBar - volume 
	HWND hTBPlayingSong;							//TRackBAr - current time songs
	HWND hSlider_Balance;							//Slider balance
	UINT id_timer;									//ID timer time by current playing songs
	UINT idTimerBySpectr;							//ID timer by Spectr
	INT secPlaying;									//Count of seconds played of the current song
	HWND spectrs[44];								//Columns spectrs
	HICON hIcon;									//Icon pleer
	BOOL IsRepeatSong;								//Replay
	HSTREAM hStream;								//Поток воспроизведения песни
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
	
	INT runProgramm();									//старт программы
	INT CheckedInitBASS();								//проверка инициализации BASS
	BOOL openFile_LoadMusic(HWND hWnd);					//открытие песни с компьютера OPENFILENAME
	VOID setRangeTrackBarPlaySong(HSTREAM stream);		//установка диапазона на TrackBar при запуске новой песни
	VOID stop(HSTREAM stream);							//Stop
	VOID play(HSTREAM stream);							//Play
	VOID pause();										//Pause
	VOID next();										//Next Song
	VOID prev();										//Prev Song
	VOID ControlSpectr(HWND hWnd);						//Контроль звуковых частот при проигрывании
	VOID SetSpectrColor(HWND hWnd);						//Установка цвета визуализации
	VOID SetNullPosSpectr();							//Установка визуализации на 0 (При нажатии на Паузу/Стоп)
	VOID DeleteIconOfTray(HWND hWnd);					//Разворачивание окна из трея
	VOID AddIconInTray(HWND hWnd);						//Сворачивание окна в Трей
	VOID CheckOpeningCopy(HWND hwnd);					//Проверка на открытие копий приложения
	friend class DlgPlayList;
};