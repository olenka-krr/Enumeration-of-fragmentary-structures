#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <cstdio>
#include <cmath>
#include <bitset>

/// ����������� ��������� �������� = 5,
/// ������� bitset<32>. ���� ����� ����� :) , ����� �������� ������ 32. � �� ����� (big numbers, time).

using namespace std;

typedef  unsigned short  vert;		 /// ��� ��� ������ ������

/*==================================================================================*/
/************************************* ������  **************************************/
/*==================================================================================*/

/*----------------------------------------------------------------------------------*/
  class  hcube;                   /// ������� ����� ��� �������� ������ ���������
  class  arc;                     /// ���� �������-��������
  class  hcubeSubGraph;           /// ������� �������� ��� ������� ��������
/*----------------------------------------------------------------------------------*/
class arc{
public:
	//int       n;
    vert      head;       /// ������� ����
  	vert      tail;       /// ����� ����
    vert      level;      /// ����� ����

    arc(){head = 0; tail = 0; level = 0;}
   	arc(vert ov, vert iv, vert l){ head = ov; tail = iv; level = l; }
    arc& operator = (const arc&);	       /// �������� ������������
};
/*----------------------------------------------------------------------------------*/
class hcube {
public:
    //vert   nl;                          /// ����� ����� = ��������� ��������� (dim)
    vert   nv;					           /// ����� ������ = 2 ^ dim

   	vert*  p;                              /// ������ �������� ������ ������
   	vert** r;                              /// ������� �����

   	hcube() { nv = 0; p = 0; r = 0; }      /// ����������� ��� ���������
   	hcube(const hcube&);				   /// ����������� ���������
   ~hcube() { cnv(); }					   /// ����������
   	hcube& operator = (const hcube&);	   /// �������� ������������
    hcube& operator << (FILE*);  		   /// �������� � ���������� ogt-�����
   	hcube& operator >> (FILE*);			   /// ��������� � ��������� ogt-����
   	hcube& write(FILE*);		           /// ��������� � ��������� ���� ��� �� �����
   	hcube& read(FILE*);		               /// �������� � ���������� �����
   	void   cnv();				           /// �������� ograph-��'���
   	vert   nar() const; 	   	           /// ������� ��� = dim * 2 ^ (dim - 1)
};
/*----------------------------------------------------------------------------------*/
class hcubeSubGraph{     /// ����������� ����������� �����, ���� ������� ������� ��������
private:
    vert nlvl;           /// ������������ ����� ��� = ������� ���� �������
    vert** tbl;          /// ������� �������� ��� ������� ��������
    ///*** for debugging
    int nar;            /// number of the arcs of the subgraph
    int* arcs;          /// vector of the arcs of the subgraph
    ///***
    vert** vertbylvl;    /// ����� vertbylvl[i][r] - ������ r ������, ������������� �� i-�� ���
    vert* lvl_nv;        /// ����� ��������� ������ �� ������
    bool frStruct;       /// �� ������� �������?
    bool isConnected(const vert&);
public:
    static vert  dim;    /// ��������� ��������
    hcubeSubGraph() {}
    hcubeSubGraph(const arc*, const int*, const int);  /// Ar, Ix, nIx
   ~hcubeSubGraph();
    bool isFragmentaryStructure();
    bool isThereArc(vert, vert);                       /// head, tail
    bool isDimensionEqualDim();                        /// �� ������� ��������� ������� dim?
};
/*----------------------------------------------------------------------------------*/
void arcset(hcube&, arc*&);                         /// ����� ������� ���
///������� � �������� �.�. ϳ����� combinator.h
unsigned long next_subset(int, int, int*& );        ///����������� ��������� � �������
vert n_cmb(vert n, vert r);                         /// ����� ��������� � n �� r
/*==================================================================================*/
/**************************** ������ ����� hcube  *********************************/
/*==================================================================================*/
//------------------------------------------------------------------------------
hcube::hcube(const hcube& g){
    vert i, j, k;
    //nl = g.nl;
    nv = g.nv;
    if (nv == 0) { p = 0; r = 0; return; }
    p = new vert[nv];
    r = new vert*[nv];
    for (i = 0; i < nv; i++){
        k = p[i] = g.p[i];
        r[i] = new vert[k];
        for (j = 0; j < k; j++) r[i][j] = g.r[i][j];
    }
}
//------------------------------------------------------------------------------
hcube& hcube::operator = (const hcube& g){
    if (this == &g) return *this;
    cnv();
    if (g.nv == 0) return *this;
    vert i, j;
    //nl = g.nl;
    nv = g.nv;
    p = new vert[nv];
    r = new vert*[nv];
    for (i = 0; i < nv; i++){
        p[i] = g.p[i];
        r[i] = new vert[p[i]];
	    for (j = 0; j < p[i]; j++) r[i][j] = g.r[i][j];
    }
    return *this;
}
//------------------------------------------------------------------------------
hcube& hcube::operator << (FILE* f){
    read(f);
    return *this;
}
//------------------------------------------------------------------------------
hcube& hcube::operator >> (FILE* f){
    write(f);
    return *this;
}
//------------------------------------------------------------------------------
hcube& hcube::write(FILE* f = stdout){
    char *frm,
   *frm1="%1d ",
   *frm2="%2d ",
   *frm3="%3d ";

    if (nv < 10)       frm = frm1;
    else if (nv < 100) frm = frm2;
    else               frm = frm3;

    int i, j;

    fprintf(f, "%3d \n", nv);
    for (i = 0; i < nv; i++){
        fprintf(f, "%3d  %3d    ", i, p[i]);
        for (j = 0; j < p[i]; j++) fprintf(f, frm, r[i][j]);
        fprintf(f, "\n");
    }
    return *this;
}
//------------------------------------------------------------------------------
hcube& hcube::read(FILE* f){
    int i, j, k, v, R;
    cnv();
    fscanf(f, "%d", &nv);
    //nl = log2(nv);
    if (f == 0) { cnv(); return *this; }
    p = new vert [nv];
    r = new vert* [nv];
    for (i = 0; i < nv; i++) r[i] = NULL;
    for (i = 0; i < nv; i++){
        fscanf(f, "%d %d", &v, &k);
        p[v] = (vert)k;
        r[v] = new vert [k];
        for (j = 0; j < k; j++){
            fscanf(f, "%d", &R);
            r[v][j] = (vert)R;
        }
    }
    return *this;
}
//------------------------------------------------------------------------------
void hcube::cnv(){
	if (nv) {
        for (int i = 0; i < nv; i++)  delete[] r[i];
        delete[] r;
        delete[] p;
        nv = 0;
    }
    p = 0;
    r = 0;
}
//------------------------------------------------------------------------------
vert hcube::nar() const{
    vert narc = 0;
    for (vert i = 0; i < nv; i++) narc += p[i];
    return narc;
}

/*==================================================================================*/
/**************************** ������ ����� arc  *********************************/
/*==================================================================================*/
//------------------------------------------------------------------------------
arc& arc::operator = (const arc& a){
    if (this == &a) return *this;
    head = a.head;
    tail = a.tail;
    level = a.level;
	return *this;
}
//------------------------------------------------------------------------------
/*==================================================================================*/
/**************************** ������ ����� hcubeSubGraph  *********************************/
/*==================================================================================*/
//------------------------------------------------------------------------------
hcubeSubGraph::hcubeSubGraph(const arc* Ar, const int* Ix, const int nIx){

    /// ����������� ����������� �����, ���� � ������� ������� �������� ���� � ���� ����
    vert nv = pow(2, dim);

    nar = nIx;
    arcs = new int[nar];
    for (int i = 0; i < nar; ++i){
        arcs[i] = Ix[i];
    }
/*if(nIx == 7){
    cout << "\nAr (constructor hcubeSubGraph): " << nar << ": ";
    for (int i = 0; i < nar; i++)
        cout << arcs[i] << ' ';
    cout << endl;
}*/
    /// ��������� ������� ��������, �� ��������, ��� �� ��������� �������
    tbl = new vert*[nv - 1];            /// ������ ����� ��� ������� ���
    for (int i = 0; i < nv - 1; ++i){
        tbl[i] = new vert[nv - 1];      /// ������ ������ ��� i-� ������� (����� ��� � i-� �������)
    }
    /// ����������� ������
    for (int i = 0; i < nv - 1; ++i)
        for (int j = 0; j < nv -1 ; ++j)
            tbl[i][j] = 0;
    /// ���� � ���� �� i-� �� j-� ���������, �������� tbl[i][j - 1] = 1
    vert k;
    vert i, j;

    for (int r = 0; r < nIx; ++r){   /// ���� �� ������
        k = Ix[r];
        i = Ar[k].head;
        j = Ar[k].tail - 1;   /// � ������� ������� ���� �� �������
        tbl[i][j] = 1;
    }
    /// �����
/*
    cout << "\nAdjacency matrix\n";
    for (int i = 0; i < nv - 1; ++i){
        for (int j = 0; j < nv - 1; ++j)
            cout << tbl[i][j] << ' ';
        cout << endl;
    }
     cout << endl;
*/
    frStruct = isConnected(nv);  /// this->

    if (frStruct){
        /// ��������� ������ ������ �� ����������� ������
        vert* vert_level = new vert[nv];  /// level of the arc`s tail = level of the arc; = 0 if the vertex is not in the subgraph
        for (vert i = 1; i < nv; ++i)
            vert_level[i] = 0;
        vert_level[0] = dim + 1;

        k = Ix[nIx - 1];
        nlvl = Ar[k].level;   /// max ����� ��������� ���
        //cout << "nlvl = " << nlvl << endl;
        /// ��������� �� ����������� ������ vertbylvl[i][r] - ������ r ������, ������������� �� i-�� ���
        vertbylvl = new vert* [nlvl];
        lvl_nv = new vert[nlvl]; /// ����� ������� ������ �� ������
        for (vert i = 0; i < nlvl; i++){
            /// n_cmb(int n, int m) - ������� �������� � n �� m = ������� ������ �� ��� m
            /// n-������ �������� = max ������� ������ �� ��� m �������
            vert r = n_cmb(dim, i + 1);
            vertbylvl[i] = new vert[r];
            for (vert j = 0; j < r; j++) vertbylvl[i][j] = 0;
            lvl_nv[i] = 0;
        }

        vert l;
        /// ���� ������� �� ������� �� �������, �� ����� ���������� ����� 0
        for (int r = 0; r < nIx; ++r){
            //vert l;
            k = Ix[r];
            l = Ar[k].level;
            j = Ar[k].tail;
            i = lvl_nv[l - 1];

            /// ���� ������� �� �� ������� �� ������ vertbylvl
            if (!vert_level[j]) {
                //cout << "k = " << k << " l = " << l << " j = " << j << " i = " << i << endl;
                vertbylvl[l - 1][i] = j;
                vert_level[j] = l;   ///  = Ar[k].level;
                lvl_nv[l - 1]++;
            }
        }
        /// �����
/*if(nIx == 7){
        cout << "\nVertices by level\n";
        for (vert i = 0; i < nlvl; ++i){
            cout << "lvl " << i + 1 << ": " << lvl_nv[i] << ": ";
            for (vert j = 0; j < lvl_nv[i]; ++j)
                cout << vertbylvl[i][j] << ' ';
            cout << endl;
        }
        cout << endl;
}*/
        delete[] vert_level;
        vert_level = 0;
    }
}
//------------------------------------------------------------------------------
hcubeSubGraph::~hcubeSubGraph(){
    //cout << "destructor\n";
    vert nv = pow(2, dim);
    if (tbl != 0){
        for (int i = 0; i < nv - 1; ++i)  delete[] tbl[i];
        delete[] tbl;
    }
    tbl = 0;

    if (vertbylvl != 0){
        for (int i = 0; i < nlvl; ++i)  delete[] vertbylvl[i];
        delete[] vertbylvl;
    }
    vertbylvl = 0;

    if (lvl_nv != 0) delete[] lvl_nv;
    lvl_nv = 0;

    if (arcs != 0) delete[] arcs;
    arcs = 0;
    //cout << "destructor2\n";
}
//------------------------------------------------------------------------------
bool hcubeSubGraph::isConnected(const vert& nvert){
    /// count degrees of vertices
    vert ind, outd;
    /// ������� ������� �� ������������, ������� ���� �������� (� ���� ��������)
    /// ������� � ������� ������� ���� �� ��������, ��� �� ���� ���������� ����� � �������
    /// ������� �� ������� ������� ���� �� �������, ��� �� ���� ���������� ��������� � �������
    for (int i = 1; i < nvert - 1; ++i){
        ind = outd = 0;
        for (int j = 0; j < nvert - 1; ++j){
            outd += tbl[i][j];   /// omit zero-vertex
        }
        for (int j = 0; j < nvert - 1; ++j){
            ind += tbl[j][i - 1];
        }
        //cout << "ind = " << ind << " outd = " << outd << endl;
        if (outd > 0 && ind == 0) return false;
    }
    return true;
}
//------------------------------------------------------------------------------
 bool hcubeSubGraph::isFragmentaryStructure(){

    if (!frStruct) return false;
    if (!isDimensionEqualDim()) return false;

    for (vert i = 0; i < nlvl - 1; i++){                /// ���� �� ������ ���� ����������
        for (vert j = 0; j < lvl_nv[i]; j++){           /// ���� �� ��������� ��������� ����
            bitset<32> binThisVert(vertbylvl[i][j]);    /// ������� ������������� ������ ������� vertbylvl[i][j]
            bitset<32> binNextVert;                     /// ������� ������������� ������ ������� ���������� ����
            for (vert k = 0; k < lvl_nv[i + 1]; k++) {       /// ���� �� ��������� ���������� ����
                binNextVert = vertbylvl[i + 1][k];
                /*
                if (nar == 7 and vertbylvl[i][j] == 5 and vertbylvl[i + 1][k] == 7)
                    cout << " isHead = " << vertbylvl[i][j] << " = " << binThisVert
                         << " isTail = " << vertbylvl[i + 1][k] << " = " << binNextVert << " count = "
                         << (binThisVert ^ binNextVert).count() << endl;*/
                if ((binThisVert ^ binNextVert).count() == 1)
                    if(!isThereArc(vertbylvl[i][j], vertbylvl[i + 1][k]))
                        return false;


            }
        }
    }
    /*if (nar == 9){
                    //cout << " isHead = " << vertbylvl[i][j] //<< " = " << binThisVert
                    //     << " isTail = " << vertbylvl[i + 1][k] //<< " = " << binNextVert
                    //     << " count = " << (binThisVert ^ binNextVert).count() << endl;
                    cout << "Arcs: ";
                    for (vert r = 0; r < nar; ++r){
                        cout << arcs[r] << ' ';
                    }
                    cout << endl;
    }*/
    return true;
}
//------------------------------------------------------------------------------
bool hcubeSubGraph::isThereArc(vert h, vert t){
    return tbl[h][t - 1];
}
//------------------------------------------------------------------------------
bool hcubeSubGraph::isDimensionEqualDim(){
    vert D = 0;
    for (vert i = 0; i < nlvl; i++)               /// ���� �� ������ ���� �������
        for (vert j = 0; j < lvl_nv[i]; j++){       /// ���� �� ��������� ��������� ����
            //if (nar == 4) cout << vertbylvl[i][j] << ' ';

            D |= vertbylvl[i][j];  /// ������� "���"
        }

    //if (nar == 4) cout << "\nD = " << D << " 2^dim - 1 = " << pow(2,dim) - 1 << endl;
    if (D != (pow(2,dim) - 1)) return 0;
    return 1;
}
/*==================================================================================*/
void arcset(hcube& g, arc*& A){
    vert i, j;
    vert V = g.nv;

    vert N = 0, m = g.p[0];

    for (i = 0; i < V - 1 ; i++){
        for (j = 0; j <  g.p[i]; j++){
            A[N] = arc(i, g.r[i][j], m - g.p[i] + 1);
            N++;
        }
    }
    for (i = 0; i < N ; i++){
        printf("%3d %3d %3d %2d\n", i, A[i].head, A[i].tail, A[i].level);
	}
    printf("\n ");
}

/*-------------------------------------------------------------------------------------*/
/// ������� � �������� �.�. ϳ�����

unsigned long next_subset(int n, int r, int*& B){
    int i, k, q = 1;
    static int* A;
    static unsigned long m;

    /*** my code for breaking while-loop ***/
    if (n == 0) { m = 0; return 0; }
    /****************************************/

    m++;
    if (m == 1){
        A = new int[n];
        for (i = 0; i < n; i++) A[i] = i;
        B = A;
        return m;
    }
    k = r - 1;
    while (q){
        A[k]++;
		if (A[k] + r - k - 1 < n) q = 0;
		else k--;
        if (k < 0){
            m = 0;
            break;
        }
		for(i = k + 1; i < r; i++) A[i] = A[i - 1] + 1;
    }
    if (m == 0) delete[] A;
    else B = A;
    return m;
}
/*-------------------------------------------------------------------------------------*/
vert n_cmb(vert n, vert r){
	if (r > n)  return 0;
    if (r == 0) return 1;
    vert C = n;
    for (int k = 1; k < r; k++) C = C * (n - k) / (k + 1);
    return C;
}
/*-------------------------------------------------------------------------------------*/
void errhalt(int q, char* mes){
    if (q){
        puts(mes);
        puts("\7\7");
        getchar();
        exit(0);
    }
}
