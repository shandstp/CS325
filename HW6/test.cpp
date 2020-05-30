#include <iostream>

using namespace std;

class equal {
    private:
        int x = 3;
    public:
        bool check(int num){
            if(num == this->x){
                return true;
            }
            else { return false; }
        }
        void printx(){
            cout << x;
        }
};

int main(int argc, char* argv[]){
    class equal z;
    if(z.check(3)){
        cout << "The number is equal to ";
        z.printx();
    }
    else {
        cout << "Meh" << endl;
    }

    return 0;
}
