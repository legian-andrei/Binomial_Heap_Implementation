#include <iostream>
#include <fstream>
#include <list>
#include <climits>
#include <algorithm>

using namespace std;

ifstream in("test.in");
ofstream out("results.out");

struct Node{
    int value, degree;
    Node *parent, *child, *sibling;
};

Node* newNode(int value){
    Node *aux = new Node;
    aux->value = value;
    aux->degree = 0;
    aux->parent = nullptr;
    aux->child = nullptr;
    aux->sibling = nullptr;
    return aux;
}

class BinomialHeap{
    list<Node*> heap;
public:
    Node* mergeHeap(Node *h1, Node *h2){
        /// Nodul cu valoare mai mare va deveni parintele nodului cu valoarea mai mica
        if(h1->value < h2->value){
            swap(h1, h2);
        }
        h2->sibling = h1->child;
        h2->parent = h1;
        h1->child = h2;
        h1->degree++;
        return h1;
    }

    void heapsUnion(list<Node*> h2){
        list<Node*> aux;
        list<Node*>::iterator it1, it2;
        it1 = heap.begin(); it2 = h2.begin();
        /// Dupa ce dau merge la 2 noduri, pastrez rezultatul in prev pentru a verifica
        /// daca ii pot da merge iarasi cu unul dintre nodurile curente
        Node* prev = nullptr;

        while(it1 != heap.end() && it2 != h2.end()){
            if(prev){
                if((*it1)->degree == (*it2)->degree && prev->degree){
                    aux.push_back(prev);
                    prev = nullptr;
                    continue;
                } else if((*it1)->degree == prev->degree){
                    prev = mergeHeap(*it1, prev);
                    it1++;
                    continue;
                } else if((*it2)->degree == prev->degree){
                    prev = mergeHeap(*it2, prev);
                    it2++;
                    continue;
                } else {
                    aux.push_back(prev);
                    prev = nullptr;
                    continue;
                }
            }

            /// Daca cele 2 noduri au acelasi grad, le dau merge. Daca nu, il bag pe cel
            /// cu grad mai mic in aux si trec mai departe
            if((*it1)->degree == (*it2)->degree){
                prev = mergeHeap(*it1, *it2);
                it1++; it2++;
            } else if((*it1)->degree > (*it2)->degree){
                aux.push_back(*it2);
                it2++;
            } else {
                aux.push_back(*it1);
                it1++;
            }
        }

        /// Daca vreunul din cele 2 noduri a ramas neparcurs, aplic aceeasi procedura
        /// ca mai sus pana nu mai are elemente neparcurse
        while(it1 != heap.end()){
            if(prev && (*it1)->degree == prev->degree){
                prev = mergeHeap(*it1, prev);
                it1++;
            } else if(prev){
                aux.push_back(prev);
                prev = nullptr;
            } else {
                aux.push_back(*it1);
                it1++;
            }
        }
        while(it2 != h2.end()){
            if(prev && (*it2)->degree == prev->degree){
                prev = mergeHeap(*it2, prev);
                it2++;
            } else if(prev){
                aux.push_back(prev);
                prev = nullptr;
            } else {
                aux.push_back(*it2);
                it2++;
            }
        }
        if(prev){
            aux.push_back(prev);
            prev = nullptr;
        }
        heap = aux;
    }

    void push(int value){
        /// Creem o lista auxiliara cu un singur nod si ii dam merge cu heapul mare
        list<Node*> aux;
        Node* auxNode = newNode(value);
        aux.push_back(auxNode);
        heapsUnion(aux);
    }

    int top(){
        /// Returneaza maximul dintre noduri
        int aux = INT_MIN;
        for(Node* it: heap){
            if(aux < it->value){
                aux = it->value;
            }
        }
        return aux;
    }

    void pop(){
        int valMax = top();
        Node* auxNode = nullptr;
        list<Node*> aux;

        /// Stergem nodul cu valoare maxima din heap
        for(auto i = heap.begin(); i != heap.end(); i++){
            if((*i)->value == valMax){
                auxNode = *i;
                heap.remove(*i);
                break;
            }
        }

        /// Continuam doar daca nodul a fost gasit (heapul nu este deja gol)
        if(!auxNode){
            return;
        }

        if(!auxNode->degree){
            delete auxNode;
            return;
        }
        /// Punem toti copiii nodului cu val maxima in aux
        Node* i = auxNode->child;
        while(i != nullptr){
            aux.push_front(i);
            i = i->sibling;
        }

        /// Facem un union pentru a introduce la loc copiii nodului extras in heap
        this->heapsUnion(aux);
    }

    /// Pentru a apela functia din main pentru cerinta 3, apelam functia aceasta intermediara
    /// Functia apeleaza heapsUnion doar cu heap-ul dat ca parametru
    void mergeH(const BinomialHeap& h2){
        heapsUnion(h2.heap);
    }

};

int main(){
    int n, q;
    in >> n >> q;
    BinomialHeap heaps[n];
    int taskNo, heapNo, value, heap1, heap2;
    for(int i = 0; i < q; i++){
        in >> taskNo;
        switch(taskNo){
            case 1:
                in >> heapNo >> value;
                heaps[heapNo - 1].push(value);
                break;
            case 2:
                in >> heapNo;
                out << heaps[heapNo - 1].top() << endl;
                heaps[heapNo - 1].pop();
                break;
            case 3:
                in >> heap1 >> heap2;
                heaps[heap1 - 1].mergeH(heaps[heap2 - 1]);
                heaps[heap2 - 1] = BinomialHeap();
                break;
        }
    }
}

