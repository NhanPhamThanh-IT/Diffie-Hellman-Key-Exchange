#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <stack>
#include <vector>

struct LargeNumber {
    int digits[309] = {};
    bool is_negative = false;
};

struct DivisionResult{
    LargeNumber quotient;
    LargeNumber remainder;
};

namespace LargeNumberConversion {
    LargeNumber copyLargeNumber(LargeNumber large_number);
    LargeNumber addLeadingDigit(LargeNumber large_number, int digit);
    bool isEqualToZero(LargeNumber large_number);
}

namespace LargeNumberArithmetic {
    LargeNumber addLargeNumbers(LargeNumber first, LargeNumber second);
    LargeNumber subtractLargeNumbers(LargeNumber minuend, LargeNumber subtrahend);
    LargeNumber multiplyLargeNumbers(LargeNumber first, LargeNumber second);
    DivisionResult divideBySmallNumber(LargeNumber dividend, LargeNumber divisor);
    DivisionResult divideByLargeNumber(LargeNumber dividend, LargeNumber divisor);
}

namespace LargeNumberSpecialOperations {
    LargeNumber modularExponentiation(LargeNumber base, LargeNumber exponent, LargeNumber modulus);
}

struct LargeNumberArray {
    LargeNumber result;
    LargeNumber count;
};

class BigInteger {
public:
    std::string digits;
    BigInteger(std::string &);
    BigInteger(BigInteger &);
    BigInteger(unsigned long long n = 0);
    BigInteger(const char *);
    friend void divide_by_2(BigInteger &a);
    friend bool isNull(const BigInteger &);
    friend int getLength(const BigInteger &);
    int operator[](const int) const;
    BigInteger &operator=(const BigInteger &);
    BigInteger &operator++();
    BigInteger operator++(int temp);
    BigInteger &operator--();
    BigInteger operator--(int temp);
    friend BigInteger &operator+=(BigInteger &, const BigInteger &);
    friend BigInteger operator+(const BigInteger &, const BigInteger &);
    friend BigInteger &operator-=(BigInteger &, const BigInteger &);
    friend BigInteger operator-(const BigInteger &, const BigInteger &);
    friend bool operator==(const BigInteger &, const BigInteger &);
    friend bool operator>(const BigInteger &, const BigInteger &);
    friend bool operator<(const BigInteger &, const BigInteger &);
    friend bool operator<=(const BigInteger &, const BigInteger &);
    friend BigInteger &operator*=(BigInteger &, const BigInteger &);
    friend BigInteger operator*(const BigInteger &, const BigInteger &);
    friend BigInteger &operator/=(BigInteger &, const BigInteger &);
    friend BigInteger operator/(const BigInteger &, const BigInteger &);
    friend BigInteger operator%(const BigInteger &, const BigInteger &);
    friend BigInteger &operator%=(BigInteger &, const BigInteger &);
    friend BigInteger &operator^=(BigInteger &, const BigInteger &);
    friend BigInteger operator^(BigInteger &, const BigInteger &);
};

namespace ConversionOperations {
    LargeNumber convertStringToArray(std::string large_number);
    std::string convertDecimalToHexBigEndian(LargeNumber decimal);
    BigInteger convertHexadecimalToDecimal(std::string hexVal);
    std::string convertBigIntegerToString(BigInteger &bigInteger);
}

int main(int argc, char **argv) {
    std::string hexPrime, hexGenerator, hexPrivateKeyA, hexPrivateKeyB;
    BigInteger decimalPrime, decimalGenerator, decimalPrivateKeyA, decimalPrivateKeyB;
    if (argc < 3) std::cout << "Not enough Command Line Arguments passed!";
    else {
        std::ifstream inputFile(argv[1]);
        inputFile >> hexPrime;
        inputFile >> hexGenerator;
        inputFile >> hexPrivateKeyA;
        inputFile >> hexPrivateKeyB;
        inputFile.close();
        decimalPrime = ConversionOperations::convertHexadecimalToDecimal(hexPrime);
        decimalGenerator = ConversionOperations::convertHexadecimalToDecimal(hexGenerator);
        decimalPrivateKeyA = ConversionOperations::convertHexadecimalToDecimal(hexPrivateKeyA);
        decimalPrivateKeyB = ConversionOperations::convertHexadecimalToDecimal(hexPrivateKeyB);
        LargeNumber prime, generator, privateKeyA, privateKeyB, publicKeyA, publicKeyB, sharedKey, productOfKeys;
        std::string tempString = ConversionOperations::convertBigIntegerToString(decimalPrime);
        prime = ConversionOperations::convertStringToArray(tempString);
        tempString = ConversionOperations::convertBigIntegerToString(decimalGenerator);
        generator = ConversionOperations::convertStringToArray(tempString);
        tempString = ConversionOperations::convertBigIntegerToString(decimalPrivateKeyA);
        privateKeyA = ConversionOperations::convertStringToArray(tempString);
        tempString = ConversionOperations::convertBigIntegerToString(decimalPrivateKeyB);
        privateKeyB = ConversionOperations::convertStringToArray(tempString);
        publicKeyA = LargeNumberSpecialOperations::modularExponentiation(generator, privateKeyA, prime);
        publicKeyB = LargeNumberSpecialOperations::modularExponentiation(generator, privateKeyB, prime);
        productOfKeys = LargeNumberArithmetic::multiplyLargeNumbers(privateKeyA, privateKeyB);
        sharedKey = LargeNumberSpecialOperations::modularExponentiation(generator, productOfKeys, prime);
        std::ofstream outputFile(argv[2]);
        outputFile << ConversionOperations::convertDecimalToHexBigEndian(sharedKey) << std::endl;
        outputFile.close();
    }
    return 0;
}

LargeNumber LargeNumberConversion::copyLargeNumber(LargeNumber large_number) {
    LargeNumber result;
    result.is_negative = large_number.is_negative;
    for (int i = 0; i < 309; i++) result.digits[i] = large_number.digits[i];
    return result;
}

LargeNumber LargeNumberConversion::addLeadingDigit(LargeNumber large_number, int digit) {
    LargeNumber result;
    result.is_negative = large_number.is_negative;
    for (int i = 0; i < 309 - 1; i++) result.digits[i + 1] = large_number.digits[i];
    result.digits[0] = digit;
    return result;
}

bool LargeNumberConversion::isEqualToZero(LargeNumber large_number) {
    for (int i = 0; i < 309; i++) if (large_number.digits[i] != 0) return false;
    return true;
}

LargeNumber LargeNumberArithmetic::addLargeNumbers(LargeNumber first, LargeNumber second) {
    if (LargeNumberConversion::isEqualToZero(first)) return second;
    if (LargeNumberConversion::isEqualToZero(second)) return first;
    LargeNumber result;
    int val = 0, carry = 0;
    bool BothNegative = false;
    if (first.is_negative && second.is_negative) BothNegative = true;
    else if (first.is_negative) {
        first.is_negative = false;
        result = subtractLargeNumbers(second, first);
        return result;
    } else if (second.is_negative) {
        second.is_negative = false;
        result = subtractLargeNumbers(first, second);
        return result;
    }
    int i = 0;
    for (i; i < 309; i++) {
        val = (first.digits[i] + second.digits[i] + carry) % 100;
        carry = (first.digits[i] + second.digits[i] + carry) / 100;
        result.digits[i] = val;
    }
    if (carry != 0) result.digits[i] = carry;
    result.is_negative = BothNegative;
    return result;
}

LargeNumber LargeNumberArithmetic::subtractLargeNumbers(LargeNumber minuend, LargeNumber subtrahend) {
    if (LargeNumberConversion::isEqualToZero(subtrahend)) return minuend;
    if (LargeNumberConversion::isEqualToZero(minuend)) {
        subtrahend.is_negative = true;
        return subtrahend;
    }
    LargeNumber result, tempresult, first;
    first = LargeNumberConversion::copyLargeNumber(minuend);
    int val = 0, NextToMe = 0;
    bool LastNum = false;
    if (subtrahend.is_negative) {
        if (first.is_negative) {
            first.is_negative = false;
            subtrahend.is_negative = false;
            result = subtractLargeNumbers(subtrahend, first);
            return result;
        } else {
            subtrahend.is_negative = false;
            result = addLargeNumbers(first, subtrahend);
            return result;
        }
    } else {
        if (first.is_negative) {
            first.is_negative = false;
            subtrahend.is_negative = false;
            result = addLargeNumbers(first, subtrahend);
            result.is_negative = true;
            return result;
        }
    }
    int i = 0;
    for (i; i < 309; i++) {
        if (LastNum) break;
        if (first.digits[i] >= subtrahend.digits[i]) {
            val = first.digits[i] - subtrahend.digits[i];
            result.digits[i] = val;
        } else {
            if (i == 309) LastNum = true;
            int temp = i;
            while (temp < 309 - 1) {
                temp++;
                NextToMe++;
                if (first.digits[temp] != 0) {
                    first.digits[temp] -= 1;
                    first.digits[i] = first.digits[i] + 100;
                    NextToMe--;
                    temp = i + 1;
                    i--;
                    while (NextToMe != 0) {
                        first.digits[temp] = 99;
                        NextToMe--;
                        temp++;
                        ;
                    }
                    break;
                } else if (first.digits[temp] == 0 && temp == 309 - 1) LastNum = true;
            }
        }
    }
    if (LastNum == true) {
        result = subtractLargeNumbers(subtrahend, minuend);
        result.is_negative = true;
        return result;
    }
    return result;
}

LargeNumber LargeNumberArithmetic::multiplyLargeNumbers(LargeNumber first, LargeNumber second) {
    LargeNumber result;
    if (LargeNumberConversion::isEqualToZero(first)) return first;
    if (LargeNumberConversion::isEqualToZero(second)) return second;
    int val = 0, carry = 0;
    bool neg = false;
    if (first.is_negative && second.is_negative) {
        first.is_negative = false;
        second.is_negative = false;
    } else if (first.is_negative) {
        first.is_negative = false;
        neg = true;
    } else if (second.is_negative) {
        second.is_negative = false;
        neg = true;
    }
    int i, j;
    for (i = 0; i < 309; i++) {
        LargeNumber temp;
        for (j = 0; j < 309; j++) {
            val = ((first.digits[i] * second.digits[j]) + carry) % 100;
            carry = ((first.digits[i] * second.digits[j]) + carry) / 100;
            temp.digits[j] += val;
        }
        if (i != 0) {
            for (int k = 308; k != 0; k--) temp.digits[k] = temp.digits[k - i];
            for (int k = 0; k < i; k++) temp.digits[k] = 0;
        }
        result = addLargeNumbers(result, temp);
    }
    result.is_negative = neg;
    return result;
}

DivisionResult  LargeNumberArithmetic::divideBySmallNumber(LargeNumber dividend, LargeNumber divisor)
{
    DivisionResult result;
    if (LargeNumberConversion::isEqualToZero(dividend)) {
        result.quotient = dividend;
        result.remainder = dividend;
        return result;
    }
    LargeNumber countArray, One, tempresult = dividend;
    One.digits[0] = 1;
    bool CheckNeg = tempresult.is_negative;
    do {
        tempresult = subtractLargeNumbers(tempresult, divisor);
        CheckNeg = tempresult.is_negative;
        if (countArray.digits[0] != 99) countArray.digits[0] = countArray.digits[0] + 1;
        else countArray = addLargeNumbers(countArray, One);
    } while (!CheckNeg);
    if (countArray.digits[0] != 0) {
        countArray.digits[0] = countArray.digits[0] - 1;
        result.quotient = countArray;
    } else result.quotient = subtractLargeNumbers(countArray, One);
    result.remainder = addLargeNumbers(tempresult, divisor);
    return result;
}

DivisionResult  LargeNumberArithmetic::divideByLargeNumber(LargeNumber dividend, LargeNumber divisor)
{
    DivisionResult result, tempresult;
    LargeNumber partOffirst, test;
    if (LargeNumberConversion::isEqualToZero(dividend)) {
        result.quotient = dividend;
        result.remainder = dividend;
    }
    bool neg = false;
    if (dividend.is_negative && divisor.is_negative){
        dividend.is_negative = false;
        divisor.is_negative = false;
    } else if (dividend.is_negative) {
        dividend.is_negative = false;
        neg = true;
    } else if (divisor.is_negative) {
        divisor.is_negative = false;
        neg = true;
    }
    std::reverse(std::begin(dividend.digits), std::end(dividend.digits));
    int i = 0;
    while (i != 309) {
        do {
            partOffirst = LargeNumberConversion::addLeadingDigit(partOffirst, dividend.digits[i]);
            test = subtractLargeNumbers(partOffirst, divisor);
            i++;
        } while (test.is_negative && i != 309);
        tempresult = divideBySmallNumber(partOffirst, divisor);
        result.quotient.digits[309 - i] = tempresult.quotient.digits[0];
        partOffirst = tempresult.remainder;
    }
    result.remainder = partOffirst;
    result.remainder.is_negative = neg;
    result.quotient.is_negative = neg;
    return result;
}

LargeNumber LargeNumberSpecialOperations::modularExponentiation(LargeNumber base, LargeNumber exponent, LargeNumber modulus)
{
    if (LargeNumberConversion::isEqualToZero(base)) return base;
    if (LargeNumberConversion::isEqualToZero(exponent)) {
        LargeNumber hop;
        hop.digits[0] = 1;
        return hop;
    }
    LargeNumber first = LargeNumberConversion::copyLargeNumber(base);
    LargeNumber second = LargeNumberConversion::copyLargeNumber(exponent);
    std::stack<LargeNumberArray> memory;
    LargeNumberArray AoA;
    LargeNumber result, tempresult, count, tempcount, check, two;
    DivisionResult  DR;
    two.digits[0] = 2;
    tempcount.digits[0] = 1;
    tempresult = first;
    result = first;
    do {
        DR = LargeNumberArithmetic::divideByLargeNumber(tempresult, modulus);
        result = DR.remainder;
        count = tempcount;
        AoA.result = result;
        AoA.count = count;
        memory.push(AoA);
        tempcount = LargeNumberArithmetic::multiplyLargeNumbers(count, two);
        tempresult = LargeNumberArithmetic::multiplyLargeNumbers(result, result);
        check = LargeNumberArithmetic::subtractLargeNumbers(second, tempcount);
    } while (!check.is_negative);
    result = *new LargeNumber;
    result.digits[0] = 1;
    while (!LargeNumberConversion::isEqualToZero(second)) {
        AoA = memory.top();
        if (!LargeNumberArithmetic::subtractLargeNumbers(second, AoA.count).is_negative) {
            result = LargeNumberArithmetic::multiplyLargeNumbers(result, AoA.result);
            DR = LargeNumberArithmetic::divideByLargeNumber(result, modulus);
            result = DR.remainder;
            second = LargeNumberArithmetic::subtractLargeNumbers(second, AoA.count);
        }
        memory.pop();
    }
    return result;
}

std::string ConversionOperations::convertDecimalToHexBigEndian(LargeNumber decimal)
{
    std::string res = "";
    DivisionResult  DivisionResult;
    DivisionResult.quotient.digits[0] = 1;
    LargeNumber Sixteen;
    Sixteen.digits[0] = 16;
    while (!LargeNumberConversion::isEqualToZero(DivisionResult.quotient)) {
        DivisionResult = LargeNumberArithmetic::divideByLargeNumber(decimal, Sixteen);
        DivisionResult.remainder;
        if (DivisionResult.remainder.digits[0] == 10) res = "A" + res;
        else if (DivisionResult.remainder.digits[0] == 11) res = "B" + res;
        else if (DivisionResult.remainder.digits[0] == 12) res = "C" + res;
        else if (DivisionResult.remainder.digits[0] == 13) res = "D" + res;
        else if (DivisionResult.remainder.digits[0] == 14) res = "E" + res;
        else if (DivisionResult.remainder.digits[0] == 15) res = "F" + res;
        else res = std::to_string(DivisionResult.remainder.digits[0]) + res;
        decimal = LargeNumberConversion::copyLargeNumber(DivisionResult.quotient);
    }
    return res;
}

BigInteger::BigInteger(std::string &s)
{
    digits = "";
    int n = s.size();
    for (int i = n - 1; i >= 0; i--) {
        if (!isdigit(s[i])) throw("ERROR CONVERT");
        else digits.push_back(s[i] - '0');
    }
}

BigInteger::BigInteger(unsigned long long numConvert)
{
    do {
        digits.push_back(numConvert % 10);
        numConvert /= 10;
    } while (numConvert != 0);
}

BigInteger::BigInteger(const char *s)
{
    digits = "";
    for (int i = strlen(s) - 1; i >= 0; i--) {
        if (!isdigit(s[i])) throw("ERROR CONVERT");
        else digits.push_back(s[i] - '0');
    }
}

BigInteger::BigInteger(BigInteger &a)
{
    digits = a.digits;
}

bool isNull(const BigInteger &a)
{
    if (a.digits.size() == 1 && a.digits[0] == 0) return true;
    return false;
}

int getLength(const BigInteger &a)
{
    return a.digits.size();
}

int BigInteger::operator[](const int index) const
{
    if (digits.size() <= index || index < 0) throw("ERROR");
    return digits[index];
}

bool operator==(const BigInteger &a, const BigInteger &b)
{
    return a.digits == b.digits;
}

bool operator<(const BigInteger &a, const BigInteger &b)
{
    int n = getLength(a), m = getLength(b);
    if (n != m) return n < m;
    while (n--) if (a.digits[n] != b.digits[n]) return a.digits[n] < b.digits[n];
    return false;
}

bool operator>(const BigInteger &a, const BigInteger &b)
{
    return b < a;
}

bool operator<=(const BigInteger &a, const BigInteger &b)
{
    return !(a > b);
}

BigInteger &BigInteger::operator=(const BigInteger &a)
{
    digits = a.digits;
    return *this;
}

BigInteger &BigInteger::operator++()
{
    int i, n = digits.size();
    for (i = 0; i < n && digits[i] == 9; i++) digits[i] = 0;
    if (i == n) digits.push_back(1);
    else digits[i]++;
    return *this;
}

BigInteger BigInteger::operator++(int temp)
{
    BigInteger aux;
    aux = *this;
    ++(*this);
    return aux;
}

BigInteger &BigInteger::operator--()
{
    if (digits[0] == 0 && digits.size() == 1) throw("UNDERFLOW");
    int i, n = digits.size();
    for (i = 0; digits[i] == 0 && i < n; i++) digits[i] = 9;
    digits[i]--;
    if (n > 1 && digits[n - 1] == 0) digits.pop_back();
    return *this;
}

BigInteger BigInteger::operator--(int temp)
{
    BigInteger aux;
    aux = *this;
    --(*this);
    return aux;
}

BigInteger &operator+=(BigInteger &a, const BigInteger &b)
{
    int t = 0, s, i;
    int n = getLength(a), m = getLength(b);
    if (m > n) a.digits.append(m - n, 0);
    n = getLength(a);
    for (i = 0; i < n; i++) {
        if (i < m) s = (a.digits[i] + b.digits[i]) + t;
        else s = a.digits[i] + t;
        t = s / 10;
        a.digits[i] = (s % 10);
    }
    if (t) a.digits.push_back(t);
    return a;
}

BigInteger operator+(const BigInteger &a, const BigInteger &b)
{
    BigInteger temp;
    temp = a;
    temp += b;
    return temp;
}

BigInteger &operator-=(BigInteger &a, const BigInteger &b)
{
    if (a < b) throw("UNDERFLOW");
    int n = getLength(a), m = getLength(b);
    int i, t = 0, s;
    for (i = 0; i < n; i++) {
        if (i < m) s = a.digits[i] - b.digits[i] + t;
        else s = a.digits[i] + t;
        if (s < 0) s += 10, t = -1;
        else t = 0;
        a.digits[i] = s;
    }
    while (n > 1 && a.digits[n - 1] == 0) a.digits.pop_back(), n--;
    return a;
}

BigInteger operator-(const BigInteger &a, const BigInteger &b)
{
    BigInteger temp;
    temp = a;
    temp -= b;
    return temp;
}

BigInteger &operator*=(BigInteger &a, const BigInteger &b)
{
    if (isNull(a) || isNull(b)) {
        a = BigInteger();
        return a;
    }
    int n = a.digits.size(), m = b.digits.size();
    std::vector<int> v(n + m, 0);
    for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) v[i + j] += (a.digits[i]) * (b.digits[j]);
    n += m;
    a.digits.resize(v.size());
    for (int s, i = 0, t = 0; i < n; i++) {
        s = t + v[i];
        v[i] = s % 10;
        t = s / 10;
        a.digits[i] = v[i];
    }
    for (int i = n - 1; i >= 1 && !v[i]; i--) a.digits.pop_back();
    return a;
}

BigInteger operator*(const BigInteger &a, const BigInteger &b)
{
    BigInteger temp;
    temp = a;
    temp *= b;
    return temp;
}

BigInteger &operator/=(BigInteger &a, const BigInteger &b)
{
    if (isNull(b)) throw("Arithmetic Error: Division By 0");
    if (a < b) {
        a = BigInteger();
        return a;
    }
    if (a == b) {
        a = BigInteger(1);
        return a;
    }
    int i, lgcat = 0, cc;
    int n = getLength(a), m = getLength(b);
    std::vector<int> cat(n, 0);
    BigInteger t;
    for (i = n - 1; t * 10 + a.digits[i] < b; i--) {
        t *= 10;
        t += a.digits[i];
    }
    for (; i >= 0; i--) {
        t = t * 10 + a.digits[i];
        for (cc = 9; cc * b > t; cc--)
            ;
        t -= cc * b;
        cat[lgcat++] = cc;
    }
    a.digits.resize(cat.size());
    for (i = 0; i < lgcat; i++) a.digits[i] = cat[lgcat - i - 1];
    a.digits.resize(lgcat);
    return a;
}

BigInteger operator/(const BigInteger &a, const BigInteger &b)
{
    BigInteger temp;
    temp = a;
    temp /= b;
    return temp;
}

BigInteger &operator%=(BigInteger &a, const BigInteger &b)
{
    if (isNull(b)) throw("Arithmetic Error: Division By 0");
    if (a < b) return a;
    if (a == b) {
        a = BigInteger();
        return a;
    }
    int i, lgcat = 0, cc;
    int n = getLength(a), m = getLength(b);
    std::vector<int> cat(n, 0);
    BigInteger t;
    for (i = n - 1; t * 10 + a.digits[i] < b; i--) {
        t *= 10;
        t += a.digits[i];
    }
    for (; i >= 0; i--) {
        t = t * 10 + a.digits[i];
        for (cc = 9; cc * b > t; cc--)
            ;
        t -= cc * b;
        cat[lgcat++] = cc;
    }
    a = t;
    return a;
}

BigInteger operator%(const BigInteger &a, const BigInteger &b)
{
    BigInteger temp;
    temp = a;
    temp %= b;
    return temp;
}

BigInteger &operator^=(BigInteger &a, const BigInteger &b)
{
    BigInteger Exponent, Base(a);
    Exponent = b;
    a = 1;
    while (!isNull(Exponent)) {
        if (Exponent[0] & 1) a *= Base;
        Base *= Base;
        divide_by_2(Exponent);
    }
    return a;
}

BigInteger operator^(BigInteger &a, BigInteger &b)
{
    BigInteger temp(a);
    temp ^= b;
    return temp;
}

void divide_by_2(BigInteger &a)
{
    int add = 0;
    for (int i = a.digits.size() - 1; i >= 0; i--) {
        int digit = (a.digits[i] >> 1) + add;
        add = ((a.digits[i] & 1) * 5);
        a.digits[i] = digit;
    }
    while (a.digits.size() > 1 && !a.digits.back()) a.digits.pop_back();
}

std::string ConversionOperations::convertBigIntegerToString(BigInteger &bigInteger)
{
    std::string res = "";
    for (int i = bigInteger.digits.size() - 1; i >= 0; i--) res = res + std::to_string((short)bigInteger.digits[i]);
    return res;
}

LargeNumber ConversionOperations::convertStringToArray(std::string large_number) {
    LargeNumber final;
    bool neg = false;
    if (large_number[0] == '-') {
        neg = true;
        large_number.erase(0, 1);
    }
    reverse(begin(large_number), end(large_number));
    int val = 0, itr = 1, Out = 0;
    for (int i = 0; i < large_number.size(); i++) {
        val += (large_number[i] - '0') * itr;
        itr *= 10;
        if (itr == 100) {
            final.digits[Out] = val;
            Out++;
            itr = 1;
            val = 0;
        }
    }
    if (val != 0) final.digits[Out] = val;
    final.is_negative = neg;
    return final;
}

BigInteger ConversionOperations::convertHexadecimalToDecimal(std::string hexVal)
{
    std::reverse(hexVal.begin(), hexVal.end());
    int len = hexVal.size();
    BigInteger base("1");
    BigInteger dec_val("0");
    for (int i = 0; i < len; i++) {
        if (hexVal[i] >= '0' && hexVal[i] <= '9') {
            dec_val += (hexVal[i] - '0') * base;
            base = base * 16;
        } else if (hexVal[i] >= 'A' && hexVal[i] <= 'F') {
            dec_val += (hexVal[i] - '7') * base;
            base = base * 16;
        }
    }
    return dec_val;
}