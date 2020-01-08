#include <iostream>           //подключаем необходимые библиотеки
#include <fstream>
#include <cmath>
#include <string>

using namespace std;          //подключаем пространства имен

const int arrSize = 256;      //создаем констатнтную переменную по размеру будущего массива

string makeDocName( string startName ){                     //создаем функцию для создания наименования результирующего документа
	string expansion;                                       //переменная "расширение"
	for (int i = 0; i < int(startName.size()+1) ; i++)      //когда доходим до точки, применяем функцию substr которая возвращает расширение
	{
		if ( startName[i] == '.' ){
			
			expansion = startName.substr(i+1);
		}
	}
	expansion = expansion + "08.tab";                       // к расширению добавляем номер нашего варианта и расширения будующего документа
	return expansion;
}

string enterFileName(){       //создаем функцию для записи имени обрабатываемого документа
	string fileName;
	cout << "Enter the path to file with extension.\n";
	cin >> fileName;
	return fileName;
}

int charToInt(unsigned char value){               // создаем функцию которая возвращает код символа
	(int)value;
	return value;
}

void countFileBytes(long int& bytesCount, long int*  countArr, string path){       //функция открытия и считывания файла
	                              //параметры - счетчик байтов, массив счетчиков элементов, путь к файлу(оно же его имя)
	ifstream ourFile(path);                                                        //открываем файл помещенный в папку с проектом

	if ( !ourFile.is_open() )                                                      //проверка на открытие файла
	{
		cout << "File was not found!" << endl;
	}
	else
	{
		cout << "File is opened." << endl;                                         //когда файл открыт
		char byte;                                                                 //создаем переменную byte типа char
		while ( ourFile.get(byte) )                                                //считываем байт из файла в созданную переменную
		{
			bytesCount++;                                                          //увеличиваем счетчик байтов на 1
			countArr[ charToInt(byte) ]++;                                         //значение по индексу в массиве увеличиваем на 1
		}
	}

	ourFile.close();                                                               //закрываем открытый файл
}

void freqCalc( long int&  bytesCount,  long int*   countArr, double*  frequencyArr ){      //функция подсчета частот

	if (bytesCount)                                                                        //если счетчик больше 0
	{
		for (int i = 0; i < arrSize; i++)                                                  
		{
			frequencyArr[i] = ( double( countArr[i] ) / double( bytesCount ) );            //рассчитываем частоту по отношению
		}                                                                                  //количества элементов в ячейке массива к общему
	}                                                                                      //количеству элементов
}

double log_2(double value){              //функция для подсчета логарифма по основанию 2

	value = log10(value) / log10(2);     //берется десятичный логарифм от Х и делится на десятичный логарифм от 2
	return value;
}

double infoAmount( double value ){        //функция для подсчета количества информации
	if (value){
		value = value * log_2(value);     //вероятность умножаем на логарифм по 2 от этой вероятности
		return value;
	}
	else return 0;
}

double entropy(const double* frequencyArr){               //функция подсчета энтропии

	double result = 0;
	
	for (int i = 0; i < arrSize; i++)
	{
		result += infoAmount( frequencyArr[i] );         //суммируем все количества информации
	}
	result = -result;                                    //меняем знак 

	return result;
}

int maxElement( double* frequencyArr ){    //функция которая находит максимальный элемент в массиве
	double maxEl = 0;                      //и возвращает его итератор (позицию)
	int iter = 0;
	for (int i = 0; i < arrSize; i++)
	{
		if (maxEl < frequencyArr[i])
		{
			maxEl = frequencyArr[i];
			iter = i;
		}
	}
	return iter;
}

void saveResult(long int&  bytesCount, string pathName, double* frequencyArr){   //функция записи результата в файл .tab
	double entrop = entropy(frequencyArr);                                       //создаем переменную и записываем в нее энтропию
	int iter;                                                                    //создаем переменную-итератор
	string docPathName = makeDocName(pathName);                  //присваиваем в стринг. переменную результат функции makeDocName
	double maxElem = 1;                                          //создаем перемунную для максимального элемента

	ofstream ourFile(docPathName);                               //открываем файл для записи(создаем)
	ourFile << pathName << "\n";                                 //записываем имя обрабатываемого файла 
	ourFile << bytesCount << "\n\n";                             //записываем общее количество считанных байтов
	while (maxElem)                                              //пока maxElem не равен 0 повторяем цикл
	{
		iter = maxElement(frequencyArr);                         //находим индекс максимального элемента
		maxElem = frequencyArr[iter];                            //записываем максимальный элемент
		if (!maxElem) break;                                     //если максимальный элемент равен 0 выходим из цикла
		ourFile << iter << "\t" << maxElem << "\n";              //записываем индекс и его макс. элемент
		frequencyArr[iter] = 0;                                  //обнуляем максимальный элемент
	}

	
	ourFile << "\n" << entrop;                                   //записываем энтропию
	ourFile.close();                                             //закрываем файл
}

int main(){

	setlocale(LC_ALL, "ru");

	long int bytesCount = 0;                               //переменная хранящая общее количество байтов
	long int countArr[arrSize] = {};                       //массив со счетчиками элементов
	double frequencyArr[arrSize] = {};                     //массив частот

	string pathName = enterFileName();                     //вызываем функцию ввода имени и сохраняем результат

	countFileBytes( bytesCount, countArr, pathName );      //вызываем функцию открытия обрабатываемого файла и подсчета байтов

	freqCalc( bytesCount, countArr, frequencyArr );        //вызываем функцию подсчета частот

	saveResult(bytesCount, pathName, frequencyArr);        //вызываем функцию записывающую результат

	cout << "Your results was writed.\n";                  //сообщение об окончании работы

	system("pause");                                       
	return 0;
}