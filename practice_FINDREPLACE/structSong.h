#include <Windows.h>

struct infoSong
{
	HSTREAM hStream;		//HANDLE песни 
	CHAR title[40];			//Название 
	CHAR artist[30];		//Исполнитель
	CHAR album[30];			//Альбом
	CHAR year[10];			//год
	INT minutes;			//Минут
	INT seconds;			//Секунд
	TCHAR path[200];		//путь к песне
};