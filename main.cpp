#include <iostream>
#include <string>
#include <list>
#include <vector>

class bigInteger{
private:
    bool sign; // false for positive, true for negative
    std::list<int> digits;
public:

    bool getSign(){
        return sign;
    }

    void setSign(bool x){
        sign = x;
    }

    ~bigInteger(){
        std::list<int>::iterator it1 = digits.begin(), it2 = digits.end();
        digits.erase(it1, it2);
    }

    void removeLeadingZeroes(){
        while(digits.back() == 0 && !digits.empty()){
            digits.pop_back();
        }
    }

    std::list<int>::iterator iterDigitAt(int i){
        std::list<int>::iterator it;
        int j = 0;
        for(it = digits.begin(); it != digits.end(); ++it){
            if(j==i)
                break;
            j++;
        }
        return it;
    }

    // friend functions
    friend bool isSmaller(bigInteger, bigInteger);
    friend bool isBigger(bigInteger, bigInteger);
    friend bigInteger& operator- (bigInteger, bigInteger);
    friend bigInteger& operator+ (bigInteger, bigInteger);
    friend bigInteger& operator* (bigInteger, bigInteger);
    friend bigInteger& operator/ (bigInteger, bigInteger);
    friend bigInteger& operator% (bigInteger, bigInteger);
    friend std::istream& operator>> (std::istream&, bigInteger&);
    friend std::ostream& operator<< (std::ostream&, bigInteger&);
    friend bigInteger& greatestCommonDivisor(bigInteger, bigInteger);

    // friend classes
    friend class bigRational;
};

// smaller or equal
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

bigInteger& operator- (bigInteger i1, bigInteger i2){
    bigInteger *i3 = new bigInteger;

    if(isSmaller(i1, i2)){
        i3->sign = true;
        std::swap(i1, i2);
    }
    else
        i3->sign = false;

    if(!i1.sign && i2.sign){
        // plus
        i1.sign = i2.sign = false;

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

        i3->removeLeadingZeroes();
        return *i3;

    }

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

    i3->removeLeadingZeroes();

    return *i3;
}

bigInteger& operator+ (bigInteger i1, bigInteger i2){

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

        i3->removeLeadingZeroes();
        return *i3;
    }
    else return(i1 - i2);
}

bigInteger& operator* (bigInteger i1, bigInteger i2){

    if(i1.digits.empty() || i2.digits.empty()){
        bigInteger* zero = new bigInteger;
        zero->sign = false;
        zero->digits.push_back(0);

        return *zero;
    }

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

    i3.removeLeadingZeroes();

    bigInteger *i4 = new bigInteger;

    i4->digits = i3.digits;

    if(i1.sign == i2.sign)
        i4->sign = false;
    else
        i4->sign = true;

    return *i4;
}

bigInteger& operator% (bigInteger i1, bigInteger i2){

    // if the first is zero we just return the 2nd number
    if(i1.digits.empty()){
        bigInteger *ret = new bigInteger;
        ret->sign = i2.sign;
        ret->digits = i2.digits;

        return *ret;
    }

    // if second number is zero we throw an error
    if(i2.digits.empty()){
        throw std::invalid_argument("Modulo 0");
    }

    if(i1.sign || i2.sign){
        throw std::invalid_argument("Negative modulo");
    }

    // we define 0
    bigInteger zero;
    zero.sign = false;
    zero.digits.push_back(0);

    // we substract the second number from the first until the next
    // subtraction is lesser than 0

    while(isBigger(i1-i2, zero)){
        i1 = i1 - i2;
    }

    bigInteger *ret = new bigInteger;
    ret->sign = false;

    if(i1.digits == i2.digits)
        ret->digits = zero.digits;
    else
        ret->digits = i1.digits;
    return *ret;
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

    integer.removeLeadingZeroes();

    return is;
}

std::ostream& operator<< (std::ostream& os, bigInteger &integer){

    if(integer.digits.empty()){
        os<<0;
        return os;
    }

    if(integer.sign)
        os<<'-';

    for(std::list<int>::reverse_iterator it = integer.digits.rbegin(); it != integer.digits.rend(); ++it)
        os<<*it;

    return os;
}

bigInteger& operator/ (bigInteger i1, bigInteger i2){
    // division by 0
    if(i2.digits.empty()){
        throw std::invalid_argument("Divison by 0");
    }
    if(i1.digits.empty()){
        bigInteger *zero = new bigInteger;
        zero->sign = false;
        zero->digits.push_back(0);

        return *zero;
    }

    bigInteger *result = new bigInteger;

    if(i1.sign == i2.sign)
        result->sign = false;
    else
        result->sign = true;

    i1.sign = i2.sign = false;

    // we define 0
    bigInteger zero;
    zero.sign = false;
    zero.digits.push_back(0);

    // we define base 10
    bigInteger ten;
    ten.sign = false;
    ten.digits.push_back(0);
    ten.digits.push_back(1);

    int idx = i1.digits.size() - 1;
    int tmp = *i1.iterDigitAt(idx);

    bigInteger aux;
    aux.sign = false;
    aux.digits.push_back(tmp);

    while(isBigger(i2, aux))
        aux.digits.push_front(*i1.iterDigitAt(--idx));

    while(idx >= 0){
        bigInteger div = aux;
        int c = 0;

        while(isSmaller(zero, div - i2)){
            div = div - i2;
            c++;
        }

        if(div.digits == i2.digits)
            c++;

        result->digits.push_front(c);

        aux = (aux % i2);
        aux.digits.push_front(*i1.iterDigitAt(--idx));
        aux.removeLeadingZeroes();
    }

    return *result;

}

bigInteger& greatestCommonDivisor(bigInteger i1, bigInteger i2){

    if(i1.digits.empty() || i2.digits.empty())
        throw std::invalid_argument("GCD of 0");

    bigInteger c;
    bigInteger zero;
    zero.sign = false;
    zero.digits.push_back(0);

    i1.sign = i2.sign = c.sign = false;

    while(isBigger(i2, zero)){
        c = i1 % i2;
        i1.digits = i2.digits;
        i2.digits = c.digits;
    }

    bigInteger *ret = new bigInteger;
    ret->sign = false;
    ret->digits = i1.digits;

    return *ret;
}

class bigRational{
private:
    bool sign;
    bigInteger numerator;
    bigInteger denominator;

    void evalSign(){
        if(numerator.sign == denominator.sign)
            sign = false;
        else
            sign = true;
    }

    bool isNumeratorZero(){
        return numerator.digits.empty();
    }

public:

    void simplify(){
        if(numerator.digits.empty())
            return;
        bigInteger gcd = greatestCommonDivisor(numerator, denominator);
        numerator = numerator / gcd;
        denominator = denominator / gcd;

        evalSign();
    }

    // friend functions
    friend bigRational& operator+ (bigRational, bigRational);
    friend bigRational& operator* (bigRational, bigRational);
    friend std::istream& operator>> (std::istream&, bigRational&);
    friend std::ostream& operator<< (std::ostream&, bigRational&);
};

bigRational& operator+ (bigRational i1, bigRational i2){

    bigRational *ret = new bigRational;

    i1.numerator = i1.numerator * i2.denominator;
    i2.numerator = i2.numerator * i1.denominator;

    i1.denominator = i1.denominator * i2.denominator;

    ret->numerator = i1.numerator + i2.numerator;
    ret->denominator = i1.denominator;

    ret->simplify();

    return *ret;
}

bigRational& operator* (bigRational i1, bigRational i2){
    bigRational *ret = new bigRational;

    ret->numerator = i1.numerator * i2.numerator;
    ret->denominator = i1.denominator * i2.denominator;

    ret->simplify();

    return *ret;
}

std::istream& operator>> (std::istream& is, bigRational& rational){
    is>>rational.numerator;
    is>>rational.denominator;

    rational.simplify();

    return is;
}

std::ostream& operator<< (std::ostream& os, bigRational& rational){
    if(rational.isNumeratorZero()){
        os<<"0";
        return os;
    }

    if(rational.sign)
        os<<"-";

    rational.denominator.setSign(false);
    rational.numerator.setSign(false);

    os<<rational.numerator<<"/"<<rational.denominator;

    return os;
}

int main(){
    bigInteger k, p;
    std::cin>>k>>p;
    std::cout<<k - p<<std::endl;
    return 0;
}
