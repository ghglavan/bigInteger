#include <iostream>
#include <string>
#include <list>
#include <vector>

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

bigInteger& operator* (bigInteger i1, bigInteger i2){
    std::vector<bigInteger> sums;

    int shift = 0;
    int carry = 0;

    for(std::list<int>::iterator it1 = i1.digits.begin(); it1 != i1.digits.end(); ++it1){
        bigInteger aux;
        for(int j = 0; j < shift; ++j)
            aux.digits.push_back(0);

        shift++;

        for(std::list<int>::iterator it2 = i2.digits.begin(); it2 != i2.digits.end(); ++it2){
            int product = (*it1) * (*it2) + carry;
            carry = product/10;
            product %= 10;

            aux.digits.push_back(product);
        }

        aux.digits.push_back(carry);
        carry = 0;

        sums.push_back(aux);
    }

    bigInteger i3;

    i3 = sums[0];

    for(int i = 1; i < sums.size(); ++i){
        i3 = i3 + sums[i];
    }

    bigInteger *i4 = new bigInteger;

    i4->digits = i3.digits;

    if(i1.sign == i2.sign)
        i4->sign = false;
    else
        i4->sign = true;

    return *i4;
}

bigInteger& operator/ (bigInteger i1, bigInteger i2){

}

std::istream& operator>> (std::istream& is, bigInteger &integer){

    std::string number;
    is >> number;

    if(number[0] == '-')
        integer.sign = true;
    else
        integer.sign = false;

    for(int i = 0; i < number.length(); i++){
        if(number[i] >= '0' && number[i] <= '9'){
            int x = number[i] - '0';
            integer.digits.push_front(x);
        }
    }

    return is;
}

std::ostream& operator<< (std::ostream& os, bigInteger &integer){

    bool flag = true;

    for(std::list<int>::reverse_iterator it = integer.digits.rbegin(); it != integer.digits.rend(); ++it){
        if(*it != 0){
            flag = false;
            break;
        }
    }

    if(flag){
        os<<0;
        return os;
    }

    if(integer.sign)
        os<<'-';

    flag = false;

    for(std::list<int>::reverse_iterator it = integer.digits.rbegin(); it != integer.digits.rend(); ++it)
        if(*it != 0){
            flag = true;
            os<<*it;
        }
        else if(!flag){
            continue;
        }
        else{
            os << *it;
        }

    return os;
}



int main(){
    bigInteger i, j;
    std::cin >> i;
    std::cin >> j;
    //std::cout << i << std::endl << j << std::endl;
    bigInteger k = i * j;
    std::cout << k << std::endl;
    return 0;
}
