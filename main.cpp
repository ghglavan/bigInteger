#include <iostream>
#include <string>
#include <list>

class bigInteger{
public:

    bool sign; // false for positive, true for negative
    std::list<int> digits;
};

bool isSmaller(bigInteger a, bigInteger b){
    if(a.sign == true && b.sign == false)
        return true;
    else if(a.sign == false && b.sign == true)
        return false;

    if(a.digits.size() < b.digits.size())
        return true;
    else if(a.digits.size() > b.digits.size())
        return false;

    std::list<int>::reverse_iterator it1 = a.digits.rbegin();
    std::list<int>::reverse_iterator it2 = b.digits.rbegin();

    for(; it1 != a.digits.rend(), it2 != b.digits.rend(); ++it1, ++it2){
        if(*it1 > *it2)
            return false;
        else if(*it1 < *it2)
            return true;
    }
    return true;
}

bool isBigger(bigInteger a, bigInteger b){
    return !isSmaller(a, b);
}

bigInteger& operator-(bigInteger i1, bigInteger i2){
    bigInteger *i3 = new bigInteger;

    if(isSmaller(i1, i2)){
        i3->sign = true;
        std::swap(i1, i2);
    }
    else
        i3->sign = false;

    int carry = 0;

    std::list<int>::iterator it1 = i1.digits.begin();
    std::list<int>::iterator it2 = i2.digits.begin();

    for(; it1 != i1.digits.end(), it2 != i2.digits.end(); ++it1, ++it2){
        int diff = *it1 - *it2 - carry;

        if(diff < 0){
            diff += 10;
            carry = 1;
        }
        else
            carry = 0;

        i3->digits.push_back(diff);
    }


    for(; it1 != i1.digits.end(); ++it1){
        int diff = *it1 - carry;

        if(diff < 0){
            diff += 10;
            carry = 1;
        }
        else
            carry = 0;

        i3->digits.push_back(diff);
    }

    return *i3;
}

bigInteger& operator+(bigInteger i1, bigInteger i2){

    if(i1.sign == i2.sign){
        bigInteger *i3 = new bigInteger;
        i3->sign = i1.sign;

        int carry = 0;

        std::list<int>::iterator it1 = i1.digits.begin();
        std::list<int>::iterator it2 = i2.digits.begin();

        for(; it1 != i1.digits.end() && it2 != i2.digits.end(); ++it1, ++it2){
            int sum = *it1 + *it2 + carry;

            if(carry)
                carry = 0;

            if(sum > 9)
                carry = 1;

            i3->digits.push_back(sum % 10);
        }

        for(; it1 != i1.digits.end(); ++it1){
            int aux = *it1 + carry;

            if(carry)
                carry = 0;

            i3->digits.push_back(aux % 10);

            if(aux > 9)
                carry = 1;
        }

        for(; it2 != i2.digits.end(); ++it2){
            int aux = *it2 + carry;

            if(carry)
                carry = 0;

            i3->digits.push_back(aux % 10);

            if(aux > 9)
                carry = 1;
        }

        if(carry)
            i3->digits.push_back(1);


        return *i3;
    }
    else return(i1 - i2);
}



std::istream& operator>> (std::istream& is, bigInteger &integer){
    char bSign;
    is >> bSign;

    if(bSign == '+')
        integer.sign = false;
    else if(bSign == '-')
        integer.sign = true;
    else if(bSign >= '0' && bSign <= '9'){
        integer.sign = false;
        integer.digits.push_front(bSign - '0');

        if(bSign == '0')
            return is;
    }

    std::string number;
    is >> number;

    for(int i = 0; i < number.length(); i++){
        int x = number[i] - '0';
        integer.digits.push_front(x);
    }

    return is;
}

std::ostream& operator<< (std::ostream& os, bigInteger &integer){
    if(integer.sign)
        std::cout<<'-';

    if(integer.digits.back()){
        std::cout<<0;
        return os;
    }

    for(std::list<int>::reverse_iterator it = integer.digits.rbegin(); it != integer.digits.rend(); ++it)
        std::cout<<*it;

    return os;
}



int main(){
    bigInteger i, j;
    std::cin >> i;
    std::cin >> j;
    //std::cout << i << std::endl << j << std::endl;
    bigInteger k = i - j;
    std::cout << k << std::endl;
    return 0;
}
