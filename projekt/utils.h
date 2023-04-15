#ifndef UTILS_H
#define UTILS_H

void chart_check(char *str); // 1.feladat

void version_argument(); // 1.feladat

void help_argument(); // 1.feladat

double randfrom(double min, double max); // 2.feladat.

int Measurement(int **Values); // 2.feladat

void BMPcreator(int *Values, int NumValues); // 3.feladat

int FindPID(); // 4-5.feladat

void SendViaFile(int *Values, int NumValues); // 5.feladat

void ReceiveViaFile(int sig); // 5.feladat

void SendViaSocket(int *Values, int NumValues); // 6.feladat

#endif
