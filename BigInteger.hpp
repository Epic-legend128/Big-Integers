#include <vector>
#include <string>
#include <cmath>
#include <iostream>

class BigInteger {
    private:
        // number of digits allowed per cell in the vector
        const unsigned int LENGTH = 18;
        
        // vector holding the BigInteger
        std::vector<unsigned long long> num;
        
        // stores the sign of the number(positive or negative)
        bool positive = true;

        // returns number of digits of an unsigned long long, using log10(n)+1
        unsigned long long length(unsigned long long n) const {
            if (n == 0) return 1;
            return ((long double) log2(n)*0.301029995663981)+1;
        }

        // converts string number into vector of unsigned long long
        std::vector<unsigned long long> to_int(std::string& n) {
            std::vector<unsigned long long> r;
            r.reserve(ceil((long double)n.length()/LENGTH));
            std::string temp;
            for (int i = 0; i<ceil((long double)n.length()/LENGTH); i++) {
                temp = "";
                for (int j = n.length()-i*LENGTH-1; j>(long long)n.length()-(i+1)*LENGTH-1 && j>=0; j--) {
                    temp = n[j] + temp;
                }
                if (temp == "" && i == 0) r.push_back(0);
                else r.push_back(std::stoull(temp));
            }
            return r;
        }

        // adds a BigInteger to the current BigInteger
        BigInteger add(BigInteger& a) {
            bool changeSign = false;
            if (!this->positive && !a.positive) {
                changeSign = true;
            }
            else if (!this->positive) {
                this->positive = true;
                BigInteger temp = *this - a;
                this->positive = false;
                return temp;
            }
            else if (!a.positive) {
                a.positive = true;
                BigInteger temp = *this - a;
                a.positive = false;
                return temp;
            }
            const unsigned long long d = (unsigned long long)1e18;
            BigInteger r;
            r.num.reserve(std::max((size_t)a.size(), num.size())+1);
            unsigned long long carry = 0;
            int mn = (a.size() > num.size() ? num.size() : a.size());
            unsigned long long i;
            for (i = 0; i<mn; i++) {
                unsigned long long temp = a[i] + num[i] + carry;
                carry = temp - temp%d;
                r.push_back(temp-carry);
                carry /= d;
            }
            while (i < a.size()) {
                unsigned long long temp = a[i] + carry;
                carry = temp - temp%d;
                r.push_back(temp-carry);
                carry /= d;
                i++;
            }

            while (i < num.size()) {
                unsigned long long temp = num[i] + carry;
                carry = temp - temp%d;
                r.push_back(temp-carry);
                carry /= d;
                i++;
            }
            if (carry > 0) {
                r.push_back(carry);
            }
            if (changeSign) r.positive = false;
            return r;
        }

    public:
        // overloaded constructors
        BigInteger(std::vector<unsigned long long>& n) {
            num = n;
        }

        BigInteger(std::string& n) {
            if (n[0] == '-') {
                positive = false;
                std::string temp = n.substr(1);
                num = to_int(temp);
            }
            else num = to_int(n);
        }

        BigInteger() = default;

        // number of digits of the BigInteger
        unsigned long long len() const {
            return ((num.size()-1)*LENGTH+length(num[num.size()-1]));
        }

        // number of cells in the vector
        unsigned long long size() const {
            return num.size();
        }

        // adds cell to the vector
        void push_back(unsigned long long n) {
            num.push_back(n);
        }

        // deletes the last number
        void erase(unsigned long long n) {
            num.erase(num.begin()+n);
        }

        //fast power, exp <= 2^64-1
        BigInteger power(BigInteger& base, unsigned long long exp) {
            if (exp == 0) {
                std::string temp = "1";
                return temp;
            }
            if (exp == 1) return base;
            if (exp&1) return power(base, exp-1) * base;
            BigInteger half = power(base, exp/2);
            return half * half;
        }

        //factorial of f
        BigInteger factorial() {
            BigInteger r;
            r << "1";
            BigInteger i;
            for (i << "1"; i<=*this; i++) {
                r *= i;
            }
            return r;
        }

        // returns array representation of the BigInteger
        std::vector<unsigned long long>& getArray() {
            return num;
        }

        // returns string representation of BigInteger
        std::string value() {
            std::string r = "";
            r.reserve(num.size()*LENGTH);
            for (int i = 0; i<num.size(); i++) {
                if (i == num.size()-1) r = std::to_string(num[i]) + r;
                else {
                    int l = length(num[i]);
                    r = std::to_string(num[i]) + r;
                    int a = LENGTH - l;
                    if (a > 0) r = std::string(a, '0') + r;
                }
            }
            return (positive ? "" : "-") + r;
        }

        // overloaded operators
        BigInteger operator+(BigInteger& a) {
            return add(a);
        }
        
        BigInteger operator+(std::string& a) {
            BigInteger temp(a);
            return *this+temp;
        }

        BigInteger operator*(BigInteger& a) {
            //todo
            return *this;
        }

        BigInteger operator-(BigInteger& a) {
            bool pos = true;
            if (a > *this) {
                BigInteger temp = a;
                a = *this;
                *this = temp;
                pos = false;
            }

            const long long d = (long long)1e18;
            BigInteger r;
            r.num.reserve(num.size());
            int carry = 0;
            int mn = a.size();
            long long i;
            for (i = 0; i<mn; i++) {
                long long temp = num[i] - a[i] - carry;
                if (temp < 0) {
                    temp += d;
                    carry = 1;
                }
                else {
                    carry = 0;
                }
                r.push_back(temp);
            }
            while (i < num.size()) {
                long long temp = num[i] - carry;
                if (temp == 0 && i == num.size()-1) break;
                if (temp < 0) {
                    temp += d;
                    carry = 1;
                }
                else {
                    carry = 0;
                }
                r.push_back(temp);
                i++;
            }
            if (carry > 0) {
                //nothing
            }
            if (!pos) {
                r.positive = false;
                BigInteger temp = a;
                a = *this;
                *this = temp;
            }
            return r;
        }

        BigInteger operator-(std::string& a) {
            BigInteger temp(a);
            return *this - temp;
        }

        void operator=(BigInteger a) {
            num = a.getArray();
            positive = a.positive;
        }

        //directly assigning value to b without the need of a variable
        void operator<<(std::string n) {
            if (n[0] == '-') {
                positive = false;
                std::string temp = n.substr(1);
                num = to_int(temp);
            }
            else num = to_int(n);
        }

        void operator*=(BigInteger& a) {
            *this = *this * a;
        }

        void operator+=(BigInteger& a) {
            *this = *this + a;
        }

        BigInteger operator++(int a) {
            std::string temp = "1";
            *this = *this + temp;
            return *this;
        }

        BigInteger operator--(int a) {
            std::string temp = "1";
            *this = *this - temp;
            return *this;
        }

        void operator-=(BigInteger& a) {
            *this = *this - a;
        }

        unsigned long long const& operator[](unsigned long long i) const {
            return num[i];
        }

        bool operator>(BigInteger& a) {
            if (num.size() != a.size()) return num.size() > a.size();
            for (int i = num.size()-1; i>=0; i--) {
                if (num[i] != a[i]) {
                    return num[i] > a[i];
                }
            }
            return false;
        }

        bool operator<(BigInteger& a) {
            if (num.size() != a.size()) return num.size() < a.size();
            for (int i = num.size()-1; i>=0; i--) {
                if (num[i] != a[i]) {
                    return num[i] < a[i];
                }
            }
            return false;
        }

        bool operator==(BigInteger& a) {
            if (num.size() != a.size()) return false;
            for (int i = num.size()-1; i>=0; i--) {
                if (num[i] != a[i]) return false;
            }
            return true;
        }

        bool operator!=(BigInteger& a) {
            return !(*this == a);
        }

        bool operator<=(BigInteger& a) {
            return *this == a || *this < a;
        }

        bool operator>=(BigInteger& a) {
            return *this == a || *this > a;
        }
};

//used for printing in the cout
std::ostream& operator<<(std::ostream& os, BigInteger& a) {
    os << a.value();
    return os;
}
