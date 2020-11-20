// Ag2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//DECODIFICARE SI VECINI ALGORITMI

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

double a=-5.12, b=5.12; //capete interval
int prec=5; //precizie;
unsigned int n=10; // nr de dimensiuni

double func(vector<double> values)
{
    double ret = 0;

    for (int ii = 0; ii < values.size(); ++ii)
    {
        ret += (ii+1) * pow(values[ii], 2);
    }

    return ret;
}

int length(double a,double b,int prec)
{
    return ceil( log( (b - a) * pow(10, prec) ) / log(2) );
}

double decodeDimension(vector<char>::iterator itStart, vector<char>::iterator itEnd, int l, double a, double b) 
{
    unsigned long bi=0; //binar integer
    for (auto ii = itStart; ii != itEnd; ++ii)
    {
        bi *= 2;
        bi += *ii;
    }

    double s = bi / (pow(2, l) - 1);  //intre 0 si 1 ajunge s afie ni
    return s * (b - a) + a;

}

vector<double> decode(vector<char>& bits, int l, unsigned int n, double a, double b)
{
    vector<double> ret;
    vector<char>::iterator itStart, itEnd;
    for (int ii = 0; ii < n; ++ii) {
        itStart = bits.begin() + ii * l;
        // l poziite de start;
        itEnd = itStart + l;

        double x = decodeDimension(itStart, itEnd, l, a, b);
        cout << x << endl;
        ret.push_back(x);
    }


    return ret;
}

void inspectNeighbours(vector<char>& bits, unsigned int n, int l, double a, double b)
{

    double min = func( decode(bits, l, n, a, b) );

    for (int ii = 0; ii < bits.size(); ++ii)
    {
        bits[ii] = !bits[ii];



       // decode(bits, l, n, a, b);

        float val = func( decode(bits, l, n, a, b) );
        if (val < min)
        {
            min = val;
            cout << "Found a new minimal value of " << min << " for neighbour number " << ii << endl;
        }




        bits[ii] = !bits[ii];
        cout << "--------------" << endl;
    }

    // determina maxim-ul ==))))) (nu ii pasa)
}

int main()
{
    int L, l;
    l = length(a, b, prec);
    L = l * n;


    //10 numere *10k;sir de 20 = 200/10 = L/n;
    //cout << L << endl;

    vector<char> bits = vector<char>(L, 1); //pune 1 cat L
    bits[3] = 0; //o mica variatie

    decode(bits, l, n, a, b);
    inspectNeighbours(bits, n, l, a, b);
    return 0;
}


// simmulated annealing -> diferenta la trecerea prin vecini;
/*mai bun decat punctul curent
    daca e mai bun, il luam si ne mutam pe el si ne uitam la vecinii noului punct
    dar
    la fiecare vecin, daca nu e mai bun decat ala curent,m ii dam o sansa sa-l luam
    esenta la simmulated

    calcul -> descoperim singuri din formula (face parte din barem)

    temperatura *= 0.9
    rezultate cat de cat ok;
    ex de temperatura de start=100;
    de stop = pow(10,-8);


    am trecut prin toti vecinii -> conditie de stop; 
    
    n-am ales punct nou; optim local
*/




/*

EVALUARE TEMA

Explicatie la introducere



*/





// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
