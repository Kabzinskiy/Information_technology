#include <iostream>           //���������� ����������� ����������
#include <fstream>
#include <cmath>
#include <string>

using namespace std;          //���������� ������������ ����

const int arrSize = 256;      //������� ������������ ���������� �� ������� �������� �������

string makeDocName( string startName ){                     //������� ������� ��� �������� ������������ ��������������� ���������
	string expansion;                                       //���������� "����������"
	for (int i = 0; i < int(startName.size()+1) ; i++)      //����� ������� �� �����, ��������� ������� substr ������� ���������� ����������
	{
		if ( startName[i] == '.' ){
			
			expansion = startName.substr(i+1);
		}
	}
	expansion = expansion + "08.tab";                       // � ���������� ��������� ����� ������ �������� � ���������� ��������� ���������
	return expansion;
}

string enterFileName(){       //������� ������� ��� ������ ����� ��������������� ���������
	string fileName;
	cout << "Enter the path to file with extension.\n";
	cin >> fileName;
	return fileName;
}

int charToInt(unsigned char value){               // ������� ������� ������� ���������� ��� �������
	(int)value;
	return value;
}

void countFileBytes(long int& bytesCount, long int*  countArr, string path){       //������� �������� � ���������� �����
	                              //��������� - ������� ������, ������ ��������� ���������, ���� � �����(��� �� ��� ���)
	ifstream ourFile(path);                                                        //��������� ���� ���������� � ����� � ��������

	if ( !ourFile.is_open() )                                                      //�������� �� �������� �����
	{
		cout << "File was not found!" << endl;
	}
	else
	{
		cout << "File is opened." << endl;                                         //����� ���� ������
		char byte;                                                                 //������� ���������� byte ���� char
		while ( ourFile.get(byte) )                                                //��������� ���� �� ����� � ��������� ����������
		{
			bytesCount++;                                                          //����������� ������� ������ �� 1
			countArr[ charToInt(byte) ]++;                                         //�������� �� ������� � ������� ����������� �� 1
		}
	}

	ourFile.close();                                                               //��������� �������� ����
}

void freqCalc( long int&  bytesCount,  long int*   countArr, double*  frequencyArr ){      //������� �������� ������

	if (bytesCount)                                                                        //���� ������� ������ 0
	{
		for (int i = 0; i < arrSize; i++)                                                  
		{
			frequencyArr[i] = ( double( countArr[i] ) / double( bytesCount ) );            //������������ ������� �� ���������
		}                                                                                  //���������� ��������� � ������ ������� � ������
	}                                                                                      //���������� ���������
}

double log_2(double value){              //������� ��� �������� ��������� �� ��������� 2

	value = log10(value) / log10(2);     //������� ���������� �������� �� � � ������� �� ���������� �������� �� 2
	return value;
}

double infoAmount( double value ){        //������� ��� �������� ���������� ����������
	if (value){
		value = value * log_2(value);     //����������� �������� �� �������� �� 2 �� ���� �����������
		return value;
	}
	else return 0;
}

double entropy(const double* frequencyArr){               //������� �������� ��������

	double result = 0;
	
	for (int i = 0; i < arrSize; i++)
	{
		result += infoAmount( frequencyArr[i] );         //��������� ��� ���������� ����������
	}
	result = -result;                                    //������ ���� 

	return result;
}

int maxElement( double* frequencyArr ){    //������� ������� ������� ������������ ������� � �������
	double maxEl = 0;                      //� ���������� ��� �������� (�������)
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

void saveResult(long int&  bytesCount, string pathName, double* frequencyArr){   //������� ������ ���������� � ���� .tab
	double entrop = entropy(frequencyArr);                                       //������� ���������� � ���������� � ��� ��������
	int iter;                                                                    //������� ����������-��������
	string docPathName = makeDocName(pathName);                  //����������� � ������. ���������� ��������� ������� makeDocName
	double maxElem = 1;                                          //������� ���������� ��� ������������� ��������

	ofstream ourFile(docPathName);                               //��������� ���� ��� ������(�������)
	ourFile << pathName << "\n";                                 //���������� ��� ��������������� ����� 
	ourFile << bytesCount << "\n\n";                             //���������� ����� ���������� ��������� ������
	while (maxElem)                                              //���� maxElem �� ����� 0 ��������� ����
	{
		iter = maxElement(frequencyArr);                         //������� ������ ������������� ��������
		maxElem = frequencyArr[iter];                            //���������� ������������ �������
		if (!maxElem) break;                                     //���� ������������ ������� ����� 0 ������� �� �����
		ourFile << iter << "\t" << maxElem << "\n";              //���������� ������ � ��� ����. �������
		frequencyArr[iter] = 0;                                  //�������� ������������ �������
	}

	
	ourFile << "\n" << entrop;                                   //���������� ��������
	ourFile.close();                                             //��������� ����
}

int main(){

	setlocale(LC_ALL, "ru");

	long int bytesCount = 0;                               //���������� �������� ����� ���������� ������
	long int countArr[arrSize] = {};                       //������ �� ���������� ���������
	double frequencyArr[arrSize] = {};                     //������ ������

	string pathName = enterFileName();                     //�������� ������� ����� ����� � ��������� ���������

	countFileBytes( bytesCount, countArr, pathName );      //�������� ������� �������� ��������������� ����� � �������� ������

	freqCalc( bytesCount, countArr, frequencyArr );        //�������� ������� �������� ������

	saveResult(bytesCount, pathName, frequencyArr);        //�������� ������� ������������ ���������

	cout << "Your results was writed.\n";                  //��������� �� ��������� ������

	system("pause");                                       
	return 0;
}