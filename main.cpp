#include <iostream>

using namespace std;

class Multime{
private:
    int *date;
    int dim;
    int n;
public:
    Multime(){
        dim = 100;
        n = 0;
        date = new int[dim];
    }

    Multime(int x){
        dim = x;
        n = 0;
        date = new int[dim];
    }

    ~Multime(){
        delete date;
    }

    void adauga(int);

    void extrage(int);

    void afisare();
};

void Multime::adauga(int x){
    if(n == dim){
        cout << "Multimea este plina" << endl;
        return;
    }

    // verificam daca elementul este in vector

    int i;
    for(i = 0; i < n; i++){
        if(date[i] == x)
            break;
    }

    if(i < n)
        return;

    // in acest caz, am gasit elementul si il inseram

    date[n++] = x;
}

void Multime::extrage(int x){
    // verificam daca elementul este in vector

    int i;
    for(i = 0; i < n; i++){
        if(date[i] == x)
            break;
    }

    if(i >= n)
        return;

    // in acest caz, vom extrage elementul
    for(int j = i; j < n-1; j++){
        date[j] = date[j+1];
    }

    n--;
}

void Multime::afisare(){
    for(int i = 0; i < n; i++){
        cout<<date[i]<<" ";
    }
    cout<<endl;
}

int main(){
    Multime m(10);
    m.adauga(4);
    m.adauga(3);
    m.afisare();
    m.extrage(4);
    m.extrage(4);
    m.afisare();
    m.adauga(9);
    m.adauga(2);
    m.adauga(2);
    m.afisare();
    return 0;
}
