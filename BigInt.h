#include <iostream>
#include <cstring>
#include <sstream>
#include <bitset>
#include <iomanip>

class BigInt {
    static const int MaxDigits = 60;  // Adjust this based on the maximum number of digits needed
    int arr[MaxDigits];

public:
    BigInt(long long val = 0);
    BigInt(const char* text);
    BigInt(const BigInt& copy);

    BigInt operator+(const BigInt& val) const;
    BigInt operator-(const BigInt& val) const;
    BigInt operator*(const BigInt& val) const;
    BigInt operator%(const BigInt& val) const;
    BigInt operator^(int exp) const;

    BigInt& operator+=(const BigInt& rhs);
    BigInt& operator-=(const BigInt& rhs);
    BigInt& operator*=(const BigInt& rhs);
    BigInt& operator%=(const BigInt& rhs);
    BigInt& operator^=(int exp);

    BigInt& operator++();    // Pre-increment
    BigInt operator++(int);  // Post-increment
    BigInt& operator--();    // Pre-decrement
    BigInt operator--(int);  // Post-decrement

    bool operator==(const BigInt& val) const;
    bool operator!=(const BigInt& val) const;
    bool operator<(const BigInt& val) const;
    bool operator<=(const BigInt& val) const;
    bool operator>(const BigInt& val) const;
    bool operator>=(const BigInt& val) const;
    BigInt operator/(const BigInt& val) const;
    BigInt& operator/=(const BigInt& rhs);

    void display();

    // Conversion Operator
    operator std::string();
    operator long long();

    // Additional conversion functions
    static BigInt fromString(const std::string& str);
    static BigInt fromBinary(const std::string& binary);
    static BigInt fromHex(const std::string& hex);
    std::string toString() const;
    std::string toBinary() const;
    std::string toHex() const;

    friend std::ostream& operator<<(std::ostream& output, const BigInt& val);
    friend std::istream& operator>>(std::istream& input, BigInt& val);
};

BigInt& BigInt::operator++() {
    // Pre-increment
    *this += 1;
    return *this;
}

BigInt BigInt::operator++(int) {
    // Post-increment
    BigInt temp(*this);
    *this += 1;
    return temp;
}

BigInt& BigInt::operator--() {
    // Pre-decrement
    *this -= 1;
    return *this;
}

BigInt BigInt::operator--(int) {
    // Post-decrement
    BigInt temp(*this);
    *this -= 1;
    return temp;
}

std::string BigInt::toString() const {
    std::string str;
    int i = MaxDigits - 1;
    while (i >= 0 && arr[i] == 0) {
        --i;
    }

    if (i == -1) {
        str.push_back('0');
    }
    else {
        for (; i >= 0; --i) {
            if (i == MaxDigits - 1) {
                str += std::to_string(arr[i]);
            }
            else {
                // Ensure each group has four digits
                str += std::to_string(arr[i] + 10000).substr(1);
            }
        }
    }

    return str;
}

BigInt::BigInt(long long val) {
    memset(arr, 0, sizeof(arr));

    int i = 0;
    while (val > 0) {
        arr[i++] = val % 10000;  // Use a group of four digits
        val /= 10000;
    }
}

BigInt::BigInt(const char* text) {
    memset(arr, 0, sizeof(arr));

    int len = strlen(text);
    int idx = 0;

    for (int i = len - 1; i >= 0; i -= 4) {
        int group = 0;
        for (int j = max(0, i - 3); j <= i; ++j) {
            if (j >= 0) {
                group = group * 10 + (text[j] - '0');
            }
        }
        arr[idx++] = group;
    }
}

BigInt::BigInt(const BigInt& copy) {
    memcpy(arr, copy.arr, sizeof(arr));
}

BigInt BigInt::operator+(const BigInt& val) const {
    BigInt result;
    int carry = 0;

    for (int i = 0; i < MaxDigits; ++i) {
        result.arr[i] = arr[i] + val.arr[i] + carry;
        carry = result.arr[i] / 10000;
        result.arr[i] %= 10000;
    }

    return result;
}

BigInt BigInt::operator-(const BigInt& val) const {
    BigInt result;
    int borrow = 0;

    for (int i = 0; i < MaxDigits; ++i) {
        result.arr[i] = arr[i] - val.arr[i] - borrow;
        if (result.arr[i] < 0) {
            result.arr[i] += 10;
            borrow = 1;
        }
        else {
            borrow = 0;
        }
    }

    // Handle negative result
    if (result.arr[MaxDigits - 1] < 0) {
        for (int i = 0; i < MaxDigits; ++i) {
            result.arr[i] = -result.arr[i];
        }
    }

    return result;
}

BigInt BigInt::operator*(const BigInt& val) const {
    BigInt result;

    for (int i = 0; i < MaxDigits; ++i) {
        for (int j = 0; j < MaxDigits - i; ++j) {
            result.arr[i + j] += arr[i] * val.arr[j];
            result.arr[i + j + 1] += result.arr[i + j] / 10000;
            result.arr[i + j] %= 10000;
        }
    }

    return result;
}

BigInt BigInt::operator%(const BigInt& val) const {
    BigInt quotient = *this / val;
    BigInt remainder = *this - quotient * val;
    return remainder;
}

BigInt BigInt::operator^(int exp) const {
    BigInt result = 1;
    BigInt base(*this);
    int currentExp = exp;

    while (currentExp > 0) {
        if (currentExp % 2 == 1) {
            result *= base;
        }
        base *= base;
        currentExp /= 2;
    }

    return result;
}

BigInt& BigInt::operator+=(const BigInt& rhs) {
    *this = *this + rhs;
    return *this;
}

BigInt& BigInt::operator-=(const BigInt& rhs) {
    *this = *this - rhs;
    return *this;
}

BigInt& BigInt::operator*=(const BigInt& rhs) {
    *this = *this * rhs;
    return *this;
}

BigInt& BigInt::operator%=(const BigInt& rhs) {
    *this = *this % rhs;
    return *this;
}

BigInt& BigInt::operator^=(int exp) {
    *this = *this ^ exp;
    return *this;
}

bool BigInt::operator==(const BigInt& val) const {
    for (int i = 0; i < MaxDigits; ++i) {
        if (arr[i] != val.arr[i]) {
            return false;
        }
    }
    return true;
}

bool BigInt::operator!=(const BigInt& val) const {
    return !(*this == val);
}

bool BigInt::operator<(const BigInt& val) const {
    for (int i = MaxDigits - 1; i >= 0; --i) {
        if (arr[i] < val.arr[i]) {
            return true;
        }
        else if (arr[i] > val.arr[i]) {
            return false;
        }
    }
    return false;
}

bool BigInt::operator<=(const BigInt& val) const {
    return (*this < val) || (*this == val);
}

bool BigInt::operator>(const BigInt& val) const {
    return !(*this <= val);
}

bool BigInt::operator>=(const BigInt& val) const {
    return !(*this < val);
}

void BigInt::display() {
    int i = MaxDigits - 1;
    while (i >= 0 && arr[i] == 0) {
        --i;
    }

    if (i == -1) {
        std::cout << "0";
    }
    else {
        for (; i >= 0; --i) {
            std::cout << arr[i];
        }
    }

    std::cout << std::endl;
}

BigInt::operator std::string() {
    std::string str;
    int i = MaxDigits - 1;
    while (i >= 0 && arr[i] == 0) {
        --i;
    }

    if (i == -1) {
        str.push_back('0');
    }
    else {
        for (; i >= 0; --i) {
            if (i == MaxDigits - 1) {
                str += std::to_string(arr[i]);
            }
            else {
                // Ensure each group has four digits
                str += std::to_string(arr[i] + 10000).substr(1);
            }
        }
    }

    return str;
}

BigInt::operator long long() {
    long long result = 0;
    long long multiplier = 1;
    for (int i = 0; i < MaxDigits; ++i) {
        result += static_cast<long long>(arr[i]) * multiplier;
        multiplier *= 10000;
    }
    return result;
}

BigInt BigInt::fromString(const std::string& str) {
    return BigInt(str.c_str());
}

BigInt BigInt::fromBinary(const std::string& binary) {
    BigInt result;
    int index = 0;

    for (int i = binary.length() - 1; i >= 0; --i) {
        if (binary[i] == '1') {
            result += (BigInt(2) ^ index);
        }
        ++index;
    }

    return result;
}

BigInt BigInt::fromHex(const std::string& hex) {
    BigInt result;

    std::istringstream iss(hex);
    iss >> std::hex >> result;

    return result;
}

std::string BigInt::toBinary() const {
    std::string binary;
    BigInt temp(*this);

    while (temp > 0) {
        binary = (temp % BigInt(2)).toString() + binary;
        temp /= 2;
    }

    if (binary.empty()) {
        binary = "0";
    }

    return binary;
}

std::string BigInt::toHex() const {
    std::stringstream ss;
    ss << std::hex << *this;
    return ss.str();
}

std::ostream& operator<<(std::ostream& output, const BigInt& val) {
    int i = BigInt::MaxDigits - 1;
    while (i >= 0 && val.arr[i] == 0) {
        --i;
    }

    if (i == -1) {
        output << "0";
    }
    else {
        for (; i >= 0; --i) {
            output << val.arr[i];
        }
    }

    return output;
}
BigInt BigInt::operator/(const BigInt& val = "0") const {
    BigInt z("0");

    if (val == z) {
        return val;
    }

    BigInt result;
    BigInt remainder = *this;

    for (int i = MaxDigits - 1; i >= 0; --i) {
        result.arr[i] = 0;
        while (remainder >= val) {
            remainder -= val;
            result.arr[i]++;
        }
    }

    return result;
}

BigInt& BigInt::operator/=(const BigInt& rhs) {
    *this = *this / rhs;
    return *this;
}
std::istream& operator>>(std::istream& input, BigInt& val) {
    std::string str;
    input >> str;
    val = BigInt::fromString(str);
    return input;
}
