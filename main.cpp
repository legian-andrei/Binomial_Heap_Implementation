#include <fstream>
#include <iostream>
#include <list>

using namespace std;
ifstream inp("mergeheap.in");
ofstream out("mergeheap.out");

struct Node{
    int value, degree;
    Node *parent, *child, *sibling;
};

Node* newNode(int value){
    Node* temp = new Node;
    temp->value = value;
    temp->degree = 0;
    temp->parent = nullptr;
    temp->child = nullptr;
    temp->sibling = nullptr;
    return temp;
}

class BinomialHeap{
    list<Node*> heap;
protected:
    void mergeTree(Node* t1, Node* t2){
        /// If the values of first node is lower than the value of the second,
        /// we swap them and make the lower one a child of the other one
        if(t1->value < t2->value){
            swap(*t1, *t2);
        }
        t2->sibling = t1->child;
        t2->parent = t1;
        t1->child = t2;
        t1->degree++;
    }

    void adjustHeap(){
        if(heap.size() > 1){
            list<Node*> :: iterator previous, current, next, temp;
            previous = heap.begin();
            current = previous; current++;
            next = ++current;

            while(current != heap.end()){
                /// If degree of only 2 trees in heap are the same, merge them
                /// If there are 3 trees in a row with the same degree, we merge only the last 2 of them
                while((*previous)->degree == (*current)->degree && current != heap.end() &&
                        (next == heap.end() || (*current)->degree) < (*next)->degree){
                    mergeTree(*current, *previous);
                    temp = previous;
                    /// Move forward in heap
                    if(previous == heap.begin()){
                        previous++; current++;
                        if(next != heap.end()){
                            next++;
                        }
                    } else {
                        previous--;
                    }
                    heap.erase(temp);
                }

                /// If we haven't found 2 trees having the same degree, we simply move forward in heap
                previous++;
                if(current != heap.end()){
                    current++;
                }
                if(next != heap.end()){
                    next++;
                }
            }

        }
    }

    void deleteRoot(Node* toBeDeletedTree, BinomialHeap& newHeap){
        /// If the tree has degree = 0 (or has no childs) we simply delete it
        if(toBeDeletedTree->degree == 0){
            delete toBeDeletedTree;
            return;
        }
        Node* aux = toBeDeletedTree;

        /// We move all the children to a new heap after unassigning their parents
        aux->child->parent = nullptr;
        newHeap.heap.push_front(aux->child);
        aux = aux->child;
        while (aux->sibling){
            aux->sibling->parent = nullptr;
            newHeap.heap.push_front(aux->sibling);
            aux = aux->sibling;
        }

        delete toBeDeletedTree;
    }

    list<Node*> :: iterator getRoot(){
        list<Node*> :: iterator i, pozMax;
        Node* maxVal = newNode((*heap.begin())->value);
        /// Find the max value
        for(i = heap.begin(); i != heap.end(); i++){
            if((*i)->value > maxVal->value){
                maxVal = *i;
                pozMax = i;
            }
        }
        return pozMax;
    }
public:
    BinomialHeap() {}

    BinomialHeap(int value){
        heap.push_back(newNode(value));
    }

    void heapsUnion(BinomialHeap& toBeMerged){
        list<Node*> :: iterator h1, h2;
        list<Node*> newHeap;
        h1 = heap.begin(); h2 = toBeMerged.heap.begin();

        /// Merge the heaps
        while(h1 != heap.end() && h2 != toBeMerged.heap.end()){
            if((*h1)->degree <= (*h2)->degree){
                newHeap.push_back(*h1);
                h1++;
            } else {
                newHeap.push_back(*h2);
                h2++;
            }
        }
        while(h1 != heap.end()){
            newHeap.push_back(*h1);
            h1++;
        }
        while(h2 != toBeMerged.heap.end()){
            newHeap.push_back(*h2);
            h2++;
        }

        /// Clear the second heap and adjust the final heap
        toBeMerged.heap.clear();
        heap = newHeap;
        adjustHeap();

        /*
        heap.clear();
        if(newHeap.empty()){
            return;
        }
        heap.push_back(*newHeap.begin());
        for(auto i = newHeap.begin(); i != newHeap.end(); i++){
            if(heap.back()->degree == (*i)->degree){
                mergeTree(heap.back(), (*i));
            } else {
                heap.push_back(*i);
            }
        }*/
    }

    void push(int value){
        Node* x = newNode(value);
        heap.push_front(x);
        adjustHeap();
        //BinomialHeap temp(value);
        //mergeHeaps(temp);
    }

    int top(){
        return (*getRoot())->value;
    }

    void pop(){
        auto root = this->getRoot();
        BinomialHeap newHeap;
        deleteRoot((*root), newHeap);
        heap.erase(root);
        heapsUnion(newHeap);
        //mergeHeaps(newHeap);
    }
};

int main() {
    int n, m;
    inp >> n >> m;
    BinomialHeap* heaps = new BinomialHeap[n];


    int taskNo, value, heapNo, heap1, heap2;
    for(int i = 0; i < m; i++){
        inp >> taskNo;
        switch (taskNo) {
            case 1:
                inp >> heapNo >> value;
                heaps[heapNo].push(value);
                break;

            case 2:
                inp >> heapNo;
                out << heaps[heapNo].top() << endl;
                heaps[heapNo].pop();
                break;

            case 3:
                inp >> heap1 >> heap2;
                heaps[heap1].heapsUnion(heaps[heap2]);
                break;

        }
    }
    return 0;
}
