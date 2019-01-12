#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <cmath>

class Matrix
{
public:

	void run() {
		readMatrixFromFile("matrix.txt");
		double** matrixAlfa = createTabMatrixAlfa(tabMatrixAlfa, size, tabMatrixA); // tworzenie macierzy alfa i jej uzupelnienie
		double* vectorBeta = createTabVectorBeta(tabVectorBeta, size, tabVectorB, tabMatrixA); // tworzenie vectoru beta i jego uzupelnienie
		tabX = createTabx(tabX, size, vectorBeta);

		setMLI();
		double* seidel = seidelMethod(tabMatrixA, matrixAlfa, vectorBeta, tabX, size, MLI); // SEIDEL
		std::cout << "\n";

		generateRaportToFile("raport.txt", "ZESTAW 1", tabMatrixA, tabVectorB, matrixAlfa, vectorBeta, MLI, tabX, tabY, iterationNumber); // generowanie raportu

	}

private:
	std::list<double> listTMP; // zmienna uzyta dla okreslenia wielkosci macierzy
	double ** tabMatrixA = 0; // macierz poczatkowa
	double ** tabMatrixAlfa = 0;
	double * tabVectorB = 0;
	double * tabVectorBeta = 0;
	double* tabX = 0; // ostatnia iteracja
	double* tabY = 0; // poprzednia iteracja
	double x = 0;
	int MLI = 0; // maksymalna liczba iteracji
	int iterationNumber = 0; // ilosc iteracji w seidlu
	int size = 0; // zmienna odpowiedzialna za okreslenie wielkosci macierzy

	void readMatrixFromFile(std::string fileName) { // wczytywanie danych z pliku do tablic
		std::fstream file;
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
		}
	}

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
		return tabMatrix; 
	}

	double** copyListToTab(double ** tabMatrix, std::list<double> listTMP) { // kopiowanie wartosci z listy do tablicy
		int i = 0;
		for (double item : listTMP) { 
			tabMatrix[0][i] = item;
			i++;
		}
		return tabMatrix;
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

	double* createTabVectorBeta(double * tabVectorBeta, int size, double * tabVectorB, double **tabMatrixA) { // funkcja alokujaca pamiec dla nowego wektora i tworzaca wektor Beta
		tabVectorBeta = new double[size];
		for (int i = 0; i < size; i++)
		{
			tabVectorBeta[i] = tabVectorB[i] / tabMatrixA[i][i];
		}
		return tabVectorBeta;
	}

	double** createTabMatrixAlfa(double ** tabMatrixAlfa, int size, double** tabMatrixA) { // funkcja alokujaca pamiec dla nowej macierzy i tworz¹ca macierz Alfa
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

	//funkcja sprawdzaj¹ca czy na przekatnej macierzy znajduja sie zera
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
				}
			}
		}
		return a;
	}

	//implementacja glownego algorytmu
	double* seidelMethod(double** tabMatrixA, double** tabMatrixAlfa, double* tabVectorBeta, double* tabX, int size, int MLI)
	{
		int i = 0, j = 0;
		double normI = 1;
		double *prevX = 0, *tmpTabX;
		double tmpX;
		double epsilon = 0.0001;
		double sum1 = 0, sum2 = 0;


		bool a = divideByZero(tabMatrixA, size);
		int iterationNumber = 0;
		if (a == false)
		{
			//zerowa iteracja
			for (int i = 0; i < size; i++)
			{
				tabX[i] = tabVectorBeta[i];
			}
			do {

				prevX = tabX;
				tmpX = tabX[size - 1];

				for (int it = 0; it < size; it++) // it - index x 
				{
					sum1 = 0;	
					sum2 = 0;
					
					if (it == 0)
					{
						for (int j = 1; j < size; j++)
						{
							sum1 += tabMatrixAlfa[0][j] * prevX[j];
						}
						tabX[it] = sum1 + tabVectorBeta[it];
						double aab = sum1 + tabVectorBeta[it];
						normI = fabs(tabX[size - 1] - tmpX);
					}
					else
					{
						for (int j = 0; j < size - 1; j++)
						{
							sum1 += tabMatrixAlfa[it][j] * tabX[j];
						}

						for (int j = it + 1; j < size; j++)
						{
							sum2 += tabMatrixAlfa[it][j] * prevX[j];
						}
						double ab = sum1 + sum2 + tabVectorBeta[it];
						tabX[it] = sum1 + sum2 + tabVectorBeta[it];
						normI = fabs(tabX[size - 1] - tmpX);


					}
					iterationNumber++;
					std::cout << "ITERACJA " << iterationNumber << "\n";
					for (int i = 0; i < size; i++)
					{
						std::cout << tabX[i] << " ";
					}
					std::cout << "\n";

				}

			} while (normI >= epsilon && iterationNumber < MLI);
			return tabX;
		}
		else
		{
			std::cout << "Nie mo¿na wykonaæ obliczeñ - dzielenie przez zero";
			exit(1);
		}

	}

	void generateRaportToFile(std::string fileName, std::string raportName, double ** tabMatrixA, double * tabVectorB,
		double ** tabMatrixAlfa, double * tabVectorBeta, int mli, double* tabX, double*tabY, int iteration) { // funkcja generujaca raport do pliku
		std::fstream file;

		file.open(fileName, std::ios::out | std::ios::trunc);

		if (file.good() == true) {
			file << "\n############# DANE WEJSCIOWE #############";
			file << "\nn = 5";
			file << "\nepsilon = 0.0001";
			file << "\nMLI = " << mli << "\n";
			file << "\n\t" << raportName << "\n";
			file << "\n" << "MACIERZ A" << "\n";
			for (int i = 0; i < size; i++)
			{
				for (int j = 0; j < size; j++)
				{
					file << tabMatrixA[i][j] << "\t";
				}
				file << "\n";
			}

			file << "\n" << "WEKTOR B" << "\n";
			for (int i = 0; i < size; i++)
			{
				file << tabVectorB[i] << "\t";
			}
			file << "\n##########################################\n";
			file << "\n############# DANE POSREDNIE #############\n";
			file << "\n" << "MACIERZ ALFA" << "\n";
			for (int i = 0; i < size; i++)
			{
				for (int j = 0; j < size; j++)
				{
					file << tabMatrixAlfa[i][j] << "\t";
				}
				file << "\n";
			}

			file << "\n" << "WEKTOR BETA" << "\n";
			for (int i = 0; i < size; i++)
			{
				file << tabVectorBeta[i] << "\t";
			}
			file << "\n##########################################\n";
			file << "\n################# WYNIKI #################\n";
			file << "\n" << "WEKTOR - OSTATNIA ITERACJA" << "\n";
			for (int i = 0; i < size; i++)
			{
				file << tabX[i] << "\t\t";
			}

			file << "\n" << "WEKTOR - POPRZEDNIA ITERACJA" << "\n";
			for (int i = 0; i < size; i++)
			{
				//file << tabY[i] << "\t\t";
			}
			file << "\n" << "ILOSC ITERACJI: " << iteration << "\n";
			file << "\n##########################################\n";
		}
	}

	void setMLI() { // wczytywanie maxymalnej liczby iteracji
		std::cout << "\nPodaj maxymalna liczbe iteracji: ";
		std::cin >> MLI;
		std::cout << "\n";
	}

};

