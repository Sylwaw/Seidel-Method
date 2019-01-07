#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <list>

class Matrix
{
public:
	void readMatrixFromFile(std::string fileName) { // wczytywanie danych z pliku do tablic
		std::fstream file;
		int size = 0; // zmienna odpowiedzialna za okreslenie wielkosci macierzy
		bool checkSizeMatrix = true;
		bool checkCreateMatrix = true;
		bool createMatrix = true;
		bool createVector = true;
		int increment_I = 0, increment_J = 0; // zmienna okreslajaca aktualna iteracje w tablicy

		file.open(fileName, std::ios::in); // otwieranie polaczenia

		if (file.good() == true) // sprawdzanie czy polaczenie zostalo otwarte
		{
			std::string tmp;
			std::cout << "Udane otwarcie pliku" << std::endl;

			while (!file.eof())
			{
				file >> tmp; // wczytywanie zmiennej z pliku do spacji

				if (tmp == "/") {
					increment_I++;
					increment_J = 0;
					checkSizeMatrix = false;
				}
				if (tmp != "/" && tmp != "/0" && tmp != "A" && tmp != "B") {
					if (createMatrix == true) {
						if (checkCreateMatrix == true) {
							if (checkSizeMatrix == true) {
								size = sizeMatrix(std::stoi(tmp)); // okreslanie rozmiaru macierzy
							}
							else {
								tabMatrixA = createTabMatrix(tabMatrixA, size); // alokacja pamieci dla nowej macierzy A
								copyListToTab(tabMatrixA, listTMP);
								checkCreateMatrix = false;
							}
						}
						if (checkCreateMatrix == false) {
							tabMatrixA[increment_I][increment_J] = std::stod(tmp); // przypisywanie wartosci dla istniejacej macierzy
							increment_J++;
						}
					}
					else {
						if (createVector == true) {
							tabVectorB = createTabVector(tabVectorB, size); // alokacja pamieci dla wektora B
							createVector = false;
						}
						tabVectorB[increment_J] = std::stoi(tmp); // przypisywanie wartosci dla istniejacego wektora B
						increment_J++;
					}
				}
				else if (tmp == "/0") {
					increment_J = 0;
					createMatrix = false;
				}
			}
			file.close(); // konczenie polaczenia z plikiem
			std::cout << "\nROZMIAR: " << size;
			print(size, tabMatrixA, tabVectorB, "MACIERZ A", "WEKTOR B"); // wypisywanie macierzy i wektora do konsoli, coœ jest Ÿle bo na przek¹tnej powinny byæ same zera
		}
		double** matrixAlfa = createTabMatrixAlfa(tabMatrixAlfa, size, tabMatrixA);
		double* vectorBeta = createTabVectorBeta(tabVectorBeta, size, tabVectorB, tabMatrixA);
		tabX = createTabx(tabX, size, tabVectorB);
		print(size, matrixAlfa, tabVectorB, "ALFA", "BETA");
		double* seidel = seidelMethod(tabMatrixA,matrixAlfa, vectorBeta, tabX, size);
	}

	double* seidelMethod(double** tabMatrixA, double** tabMatrixAlfa, double* tabVectorBeta, double* tabX, int size) // to jest to co robilas???
	{
		int i = 0, j = 0, iterationNumber = 0;
		double normI = 0;
		double sum1 = 0, sum2 = 0;
		bool a = divideByZero(tabMatrixA, size);

		if (a == false)
		{
			do {

				if (iterationNumber == 0) //powinno w to wejœæ gdzie jest to add to watch -.-
				{
					for (int i = 0; i < size; i++)
						tabX[i] = tabVectorBeta[i];
					iterationNumber++;
				}
				else
				{
					for (int it = 0; it < size; it++)
					{
						if (it == 0)//albo chyba jednak jest ok, juz sie w tym pogubilam troche, mysle ¿e jest ok czyli powinno dzialac ja:D?
						{
							for (int j = 1; j < size; j++)
							{
								sum1 += tabMatrixAlfa[i][j] * tabX[j];//a tu by³o ok?
							}
							tabX[it] = sum1 + tabVectorBeta[it];
							iterationNumber++;
						}
						else
						{
							for (int i = 1; i <= size; i++)
							{
								for (int j = 0; j <= size - 1; j++)//a mo¿e wypiszmy najpierw alfe i bete mo¿e przy tworzeniu jest b³¹d?
								{
									sum1 += tabMatrixAlfa[i][j] * tabX[j];  // czekaj stop bo widze b³¹d we wzorze, zobacz sobie w tym czasie na to widzisz tu co? a co tu jest nie tak? tak chocby nie umiol komorki odczytac
								}
							}
							for (int i = 1; i <= size; i++)
							{
								for (int j = 2; j <= size; j++)
								{
									sum2 += tabMatrixAlfa[i][j] * tabX[j];
								}
							}
							tabX[it] = sum1 + sum2 + tabVectorBeta[it];
							iterationNumber++;
						}
					}

				}
			} while (normI <= epsilon && iterationNumber >= MLI);
		}

		return tabX;//to sie mog³o nigdy nie koñczyæ chyba kuzwa no moze tak byc :D
		//dodaæ sprawdzanie normy
	}

	void generateRaportToFile(std::string fileName, int ** tabMatrixA, int * tabVectorB, int ** tabMatrixAlfa, int * tabVectorBeta) { // funkcja generujaca raport do pliku

	}

	void setMLI() { // wczytywanie maxymalnej liczby iteracji
		std::cout << "Podaj maxymalna liczbe iteracji: ";
		std::cin >> MLI;
		std::cout << "\n";
	}
	int getMLI() {
		return MLI;
	}

private:
	std::list<double> listTMP; // zmienna uzyta dla okreslenia wielkosci macierzy
	double ** tabMatrixA = 0;
	double ** tabMatrixAlfa = 0;
	double * tabVectorB = 0;
	double * tabVectorBeta = 0;
	double* tabX = 0;
	double epsilon = 0.0001, x = 0;
	int MLI = 0;


	int sizeMatrix(int tmpValue) { // okreslenie rozmiaru tablicy
		listTMP.push_back(tmpValue);
		return listTMP.size();
	}

	double** createTabMatrix(double ** tabMatrix, int size) { // funkcja alokujaca pamiec dla nowej macierzy
		tabMatrix = new double *[size];
		for (int i = 0; i < size; i++)
		{
			tabMatrix[i] = new double[size];
		}
		return tabMatrix; // a nie szlo z tych skorzystac? to jest to samo
	}

	double** copyListToTab(double ** tabMatrix, std::list<double> listTMP) { // kopiowanie wartosci z listy do tablicy
		int i = 0;
		for (double item : listTMP) { //co to? //wypisanie listy double
			tabMatrix[0][i] = item;
			i++;
		}
		return tabMatrix;
	}

	void print(int size, double ** tabMatrix, double * tabVector, std::string matrixA, std::string vectorB) { // wypisywanie wartosci do konsoli
		std::cout << "\n" << matrixA << std::endl;
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				std::cout << tabMatrix[i][j] << "\t\t";
			}
			std::cout << "\n";
		}

		std::cout << vectorB << std::endl;
		for (int i = 0; i < size; i++)
		{
			std::cout << tabVector[i] << " ";
		}
	}

	void printX(int size, double *tabX)
	{
		std::cout << "tabX" << std::endl;
		for (int i = 0; i < size; i++)
		{
			std::cout << tabX[i] << ", ";
		}
	}

	double* createTabVector(double * tabVector, int size) { // funkcja alokujaca pamiec dla nowego wektora
		return tabVector = new double[size];
	}

	double* createTabx(double * tabX, int size, double* tabVectorB) { // funkcja alokujaca pamiec dla nowego wektora
		tabX = new double[size];
		for (int i = 0; i < size; i++)
		{
			tabX[i] = tabVectorB[i];
		}
		return tabX;
	}

	double* createTabVectorBeta(double * tabVectorBeta, int size, double * tabVectorB, double **tabMatrixA) { // funkcja alokujaca pamiec dla nowego wektora
		return tabVectorBeta = new double[size];
		for (int i = 0; i < size; i++)
		{
			tabVectorBeta[i] = tabVectorB[i] / tabMatrixA[i][i];
		}
		return tabVectorBeta;
	}

	double** createTabMatrixAlfa(double ** tabMatrixAlfa, int size, double** tabMatrixA) { // funkcja alokujaca pamiec dla nowej macierzy
		tabMatrixAlfa = new double *[size];
		for (int i = 0; i < size; i++)
		{
			tabMatrixAlfa[i] = new double[size];
		}

		for (int j = 0; j < size; j++)
		{
			for (int k = 0; k < size; k++)
			{
				if (j == k)
				{
					tabMatrixAlfa[j][k] = 0.0;
				}
				else
				{
					tabMatrixAlfa[j][k] = -(tabMatrixA[j][k] / tabMatrixA[j][j]);
				}

			}
		}

		return tabMatrixAlfa;
	}

	bool divideByZero(double **matrixA, int size)
	{
		bool a = false;
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				if (i == j && matrixA[i][j] == 0)
				{
					std::cout << "Obliczenia przerwane, nie mo¿na dzieliæ przez 0" << std::endl;
					a = true;
					exit(1);
				}
			}
		}
		return a;
	}


};

