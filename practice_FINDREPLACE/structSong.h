#include <Windows.h>
struct infoSong
{
	HSTREAM hStream;		//HANDLE ����� 
	CHAR title[40];			//�������� 
	CHAR artist[30];		//�����������
	CHAR album[30];			//������
	CHAR year[10];			//���
	INT minutes;			//�����
	INT seconds;			//������
	TCHAR path[200];			//���� � �����
};