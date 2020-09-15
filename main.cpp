/// ����������� ��������� �������� = 5,
/// ������� bitset<32>. ���� ����� ����� :) , ����� �������� ������ 32. � �� �����.

//#include <iostream>
//#include <string>
//#include <stdlib.h>
//#include <bitset>

#include "org_.h"

using namespace std;

void sortbylevel(arc*, vert);
int check(arc*, int*, int);

//hcube mCube;                     // ���������� ������� dimCube (��������) ���� hcube
vert hcubeSubGraph::dim;           /// ��������� ���������, ���  vert =  unsigned short

int main(){
    vert m;                             /// ��������� ��������, ���  vert =  unsigned short
    char fnamei[6], fnameo[7];          /// ���� ��� ���� �������� �� ��������� �����

    cout << "Enter dimension of hypercube (2 .. 5) m = " ;
    cin >> m;

    hcubeSubGraph::dim = m;
    cout << "hcubeSubGraph::dim: " << hcubeSubGraph::dim << endl;

    sprintf(fnamei, "%d", m);         /// integer to string //itoa(m, fnamei, 10);
    strcpy(fnameo, fnamei);
    strcat(fnamei, "n.ogt");
    cout << "fnamei = " << fnamei << endl;
    FILE* in = fopen(fnamei, "r");
    errhalt(in == 0,"Cannot open Input File!");
    strcat(fnameo, "n_.ogt");
    cout << "fnameo = " << fnameo << endl;

    FILE* out = fopen(fnameo, "w");
    errhalt(out == 0,"Cannot open Output File!");

    hcube mCube;                       /// ���������� ��'���� mCube (�������) ���� hcube

    mCube.read(in);                    // ������ �� �������� ogt-����� ������� ������������� � ������ � ������
    fclose(in);
    cout <<  "mCube.nv = " << mCube.nv << "\n";
    cout << endl << "Inc. table" << endl;

    mCube.write();                     // ����� �� ����� ����������� �������
    mCube.write(out);                  // ������ � �������� ogt-����

    vert N = vert(mCube.nar());        /// ������� ��� � �������

    cout << endl << "Set of arcs" << endl;
    printf("%3d \n", N);

    fprintf(out, "\n%2d \n", N);

    arc* a = new arc[N];               /// ���������� ������ ���� arc ��� ������� ��� ��������
    arcset(mCube, a);                  /// ���������� ����� ������

    //������ ��������� ��� � �������� ogt-����
    for (vert i = 0; i < N ; i++)
        fprintf(out, "%3d %3d %2d %3d\n", i, a[i].head, a[i].tail, a[i].level);

    fprintf(out, "\n");
    cout << endl;

    sortbylevel(a, N);

    for (vert i = 0; i < N ; i++)
        fprintf(out, "%3d %3d %2d %3d\n", i, a[i].head, a[i].tail, a[i].level);

    fprintf(out, "\n");

    /// ����������� �������� ���:
    int i;                                  /// ��������
    int r;                                  /// ��-�� ��� � ����-� >= hcubeSubGraph::dim
    unsigned long nrss, nssr;               /// nrss - ������� �������� � N �� r
    //unsigned int nss = 0;                  /// nss - �������� ������� �������� ������ ���
    unsigned long nfrr;                     /// ��-�� ���������� ���������� � r �������� (��� ��. ���-� ��������� r)
    int* indx;                              /// ����� ������� �������� (���) ��������
    unsigned long nfr = 1;    /// �������� ������� ��. ���-� ��������� hcubeSubGraph::dim
                             /// ��������� �������� = 1 ����� ��. ���-��

    cout << endl;

    for (r = hcubeSubGraph::dim; r <= N - 1; r++){
        nfrr = 0;
        printf("r = %2d nfr = %3d \n", r, nfr);
        puts("working hard...\n");
        while ( (nrss = next_subset(N, r, indx)) ){
            i = indx[0];
        	if(a[i].level != 1) { next_subset(0, r, indx); break; }  /// ����� ���� �� ���� ����� ������� ����

            /// �������� - �� ������� ��������� ��� ������������ ��������

            //if (check(a, indx, r)){
            nfrr += check(a, indx, r);  //1;
                //cout << "\nyes\n";}
            //else cout << "\nnot\n";

            //cout << ++nss << endl; //printf("nss = %2d \n\n", nss);
            nssr = nrss;
            //nss++;
        }

        nfr += nfrr;
        printf("\nr = %2d nfrr = %3d nfr = %3d \n\n", r, nfrr, nfr);
        fprintf(out, "%2d %10d %10d %10d \n\n", r, nssr, nfrr, nfr);
    }

    //printf("\nnss = %d \n", nss);
    printf("\nnfr = %d \n", nfr);
    fprintf(out, "\n%10d \n", nfr);
    fclose(out);
    delete[] a;
    if (indx != 0) delete[] indx;
    puts("\n---Yes!!! Press any key---------");
    //pause;
    //getchar();
}
//----------------------------------------------------------------------------//
void sortbylevel(arc* ar, vert n){
    arc buff;
	for(int i = 0; i < n - 1; i++)
        for(int j = i + 1; j < n; j++)
            if (ar[j].level < ar[i].level){
                buff = ar[j];
                ar[j] = ar[i];
                ar[i] = buff;
            }
    cout << "Set of arcs ordered by level" << endl;
    for (vert i = 0; i < n ; i++)
        printf("%3d %3d %2d %3d\n", i, ar[i].head, ar[i].tail, ar[i].level);

}
//----------------------------------------------------------------------------//
int check(arc* ar, int* ix, int nix){
    int i = ix[0];                        /// �������� ��� ������� ���
/*
    cout << "\ncombi: ";
    for (int i = 0; i < nix; i++)
        cout << i << ' ';
    cout << endl;
    cout << "ar: ";
    for (int i = 0; i < nix; i++)
        cout << ix[i] << ' ';
    cout << endl;
*/
	if(ar[i].level != 1) return 0;         /// ���� �� �� ��� ������� ����

    int l = 1,                             /// �������� �����
    ia = 0,                                /// �������� ��� �񳺿 ���������
    ial = 0,                               /// �������� ��� �� l-��� ���
    k = 0;                                 /// �������� ��� ������� ��� ������� ��������� ���
    /// �� ������� ��� �� ���� �����
    while (ar[i].level == l){
        i = ix[++k];
        //cout << " i = " << i << endl;
        ia++;
        ial++;
        if (ia == nix) return 1;   /// ���� � ���� ����� 1-�� ����
    }

    hcubeSubGraph subGr(ar, ix, nix);
    if (!subGr.isFragmentaryStructure()) return 0;

    return 1;
}

