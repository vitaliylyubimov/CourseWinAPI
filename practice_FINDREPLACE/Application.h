#pragma once
#include "include.h"
#include "DlgPlayList.h"
#include "DlgEqualizer.h"

class Application
{
private:
	RECT spectrRECT;								//������ �������
	HANDLE hMutex;									//������ ��� �������� ���������� �������� ����
	DlgPlayList playlist;							//������ ������ PlayList
	DlgEqualizer equalizer;							//������ ������ Equalizer
	HWND hTBSoundVolume;							//Slider - ��������� 
	HWND hTBPlayingSong;							//Slider - ������� ����� �����
	HWND hSlider_Balance;							//Slider �������
	UINT id_timer;									//ID �������, ������� �������� �� ������ ���������
	UINT idTimerBySpectr;							//ID �������, ������� �������� �� ������
	INT secPlaying;									//���������� ������ ����������� ������� ������
	HICON hIcon;									//������ ����������
	BOOL IsRepeatSong;								//������ �����
	HSTREAM hStream;								//����� ��������������� �����
	INT contour_red, contour_green, contour_blue;	//RGB ���������� �������
	INT fill_red, fill_green, fill_blue;			//RGB ���������� �������
	/*
		����������� ����
	*/
	HMENU hContextMenu = CreatePopupMenu();
	HMENU hColor = CreatePopupMenu();
	HMENU hColorContour = CreatePopupMenu();
	HMENU hColorFill = CreatePopupMenu();
	HMENU hTranperency = CreatePopupMenu();
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
	VOID Cls_OnRButtonDown(HWND hwnd, BOOL fDoubleClick, INT x, INT y, UINT keyFlags);					//WM_RBUTTONDOWN
	HBRUSH OnColorStatic(HWND hwnd, HDC hdc, HWND hwndChild, INT type);							//����������� �������
	HBRUSH OnCtlColor(HWND hwnd, HDC hdc, HWND hwndChild, INT type);							//����������� ���� ����
	INT runProgramm();												//����� ���������
	INT CheckedInitBASS();											//�������� ������������� BASS
	BOOL openFile_LoadMusic(HWND hWnd);								//�������� ����� � ���������� OPENFILENAME
	VOID setRangeTrackBarPlaySong(HSTREAM stream);					//��������� ��������� �� TrackBar ��� ������� ����� �����
	VOID stop(HSTREAM stream);										//Stop
	VOID play(HSTREAM stream);										//Play
	VOID pause();													//Pause
	VOID next();													//Next Song
	VOID prev();													//Prev Song
	VOID DeleteIconOfTray(HWND hWnd);								//�������������� ���� �� ����
	VOID AddIconInTray(HWND hWnd);									//������������ ���� � ����
	VOID CheckOpeningCopy(HWND hwnd);								//�������� �� �������� ����� ����������
	VOID ColorFillSpectrum(INT r, INT g, INT b);					//���� ������� �������
	VOID ColorContourSpectrum(INT r, INT g, INT b);					//���� ������� �������
	VOID TransparencyWindow(HWND hWnd, INT value);					//������������ ����
	VOID showTimePlaying(HWND hWnd, INT secPlaying);				//����� �������� �����
	VOID showNameSong(HSTREAM stream, HWND hWnd);
	VOID UncheckedAllMenuItemContour();								//������ ���� ������� � ��������� ���� �������
	VOID UncheckedAllMenuItemFill();								//������ ���� ������� � ��������� ���� �������
	VOID UncheckedAllMenuItemTransperency();						//������ ���� ������� � ��������� ���� ������������
	friend class DlgPlayList;
};