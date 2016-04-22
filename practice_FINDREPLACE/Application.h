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
	BOOL IsRepeatSong;								//Replay
	HSTREAM hStream;								//����� ��������������� �����
	INT contour_red, contour_green, contour_blue;	//
	INT fill_red, fill_green, fill_blue;			//
public:
	Application(VOID);								//����������� �� ���������
	~Application();									//����������
	static Application* _this;						//����������� ���������
	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);				//���������� ��������� ��� ����������� ����
	
	VOID Cls_OnCommand(HWND hwnd, INT id, HWND hwndCtl, UINT codeNotify);								//WM_COMMAND
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);									//WM_INITDIALOG
	VOID Cls_OnHScroll(HWND hwnd, HWND hwndCtl, UINT code, INT pos);									//WM_HSCROLL
	VOID Cls_OnTimer(HWND hwnd, UINT id);																//WM_TIMER
	VOID Cls_OnSysCommand(HWND hwnd, UINT cmd, INT x, INT y);											//WM_SYSCOMMAND
	VOID Cls_OnRButtonDown(HWND hwnd, BOOL fDoubleClick, INT x, INT y, UINT keyFlags);
	INT runProgramm();									//����� ���������
	INT CheckedInitBASS();								//�������� ������������� BASS
	BOOL openFile_LoadMusic(HWND hWnd);					//�������� ����� � ���������� OPENFILENAME
	VOID setRangeTrackBarPlaySong(HSTREAM stream);		//��������� ��������� �� TrackBar ��� ������� ����� �����
	VOID stop(HSTREAM stream);							//Stop
	VOID play(HSTREAM stream);							//Play
	VOID pause();										//Pause
	VOID next();										//Next Song
	VOID prev();										//Prev Song
	VOID DeleteIconOfTray(HWND hWnd);					//�������������� ���� �� ����
	VOID AddIconInTray(HWND hWnd);						//������������ ���� � ����
	VOID CheckOpeningCopy(HWND hwnd);					//�������� �� �������� ����� ����������
	VOID ColorFillSpectrum(INT r, INT g, INT b);
	VOID ColorContourSpectrum(INT r, INT g, INT b);
	VOID TransparencyWindow(HWND hWnd, INT value);
	friend class DlgPlayList;
};