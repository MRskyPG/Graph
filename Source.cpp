#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <locale.h>
#include <stdio.h>
#include <vector>

bool flag = false;
int ver, kVer, kolvo, N; //ver - номер вершины, kVer - кол-во выходящих дуг, kolvo - общее кол-во вершин. 
std::vector <std::vector <int>> graph, backup; //Основной граф типа вектор, состоящий из векторов целого типа и его бэкап.
std::vector <int> visitedPoint;
FILE* fout;

//Функция обхода в глубину, ставящая метку при нахождении цикла.
void Obhod(int vr) {
	if (flag) return; //Если цикл есть - выход.
	else {
		visitedPoint[vr] = 1; //Посетить вершину.

		for (int i = 0; i < graph[vr].size(); i++) {
			if (visitedPoint[graph[vr][i]] == 1) { //Если посетили вершину, но она не черная - значит зациклились.
				flag = true;
				return;
			}
			else Obhod(graph[vr][i]);
			if (flag) return; //Если цикл есть - выход.
		}

		visitedPoint[vr] = 2; //Закрашиваем вершину в черный. 

	}

}

void searchOfCycle() {
	for (int i = 0; i < kolvo; i++) {
		if (visitedPoint[i] == 0) { //Если белая вершина, посещаем ее обходом.
			Obhod(i);
			if (flag) return;
		}
	}
}

//Удаляем сами дороги из вершины и идущие в нее, чтобы не было смещений индексов элементов.
void deleteElem(int vr) {
	graph[vr].clear();
	for (int j = 0; j < kolvo; j++) {
		for (int i = 0; i < graph[j].size(); i++) {
			if (graph[j][i] == vr) graph[j].erase(graph[j].begin() + i);
		}
	}

}

bool deleteWithSearchCycle() {
	for (int i = 0; i < kolvo; i++) {
		deleteElem(i);
		visitedPoint.assign(kolvo, 0); //Занести в вектор элементы со значением 0 в размере kolvo.
		flag = false;
		searchOfCycle();
		if (flag) {
			graph = backup; //Смотреть присвоение в int main().
		}
		else {
			fprintf(fout, "Если удалить вершину [%d], то в орграфе не будет циклов.", i);
			return true;
		}
	}
	return false;
}

int main()
{
	setlocale(LC_ALL, "ru");
	FILE* f;
	char fileOutName[15] = "output.txt";

	fopen_s(&fout, "output.txt", "w");

	fopen_s(&f, "graph.txt", "r");
	fscanf(f, "%d", &kolvo);

	graph.resize(kolvo);
	visitedPoint.resize(kolvo, 0); //заносим в вектор нули в количестве вершин.

	while (!feof(f)) {

		fscanf(f, "%d%d", &ver, &kVer);


		//Считывание дорог для текущей вершины.
		for (int j = 0; j < kVer; j++) {
			fscanf(f, "%d", &N);
			graph[ver].push_back(N);
			//Заполняем граф (в вектор текущей вершины добавляем вершины, куда есть дороги из нее).
		}
	}

	backup = graph;
	searchOfCycle();

	if (flag) {
		fprintf(fout, "В исходном орграфе имеются циклы в количестве >= 1.\n");
		flag = false;
		bool func = deleteWithSearchCycle();
		if (!func) fprintf(fout, "Нельзя удалить вершину так, чтобы не было циклов.");

	}

	else {
		fprintf(fout, "Орграф задан без циклов, то есть не по условию задачи.");

		std::cout << "Результат выведен в файл " << fileOutName << ", находящийся в папке с программой.";
		fclose(f);
		fclose(fout);
		return 0;
	}

	std::cout << "Результат выведен в файл " << fileOutName << ", находящийся в папке с программой.";
	fclose(f);
	fclose(fout);
	return 1;
}
