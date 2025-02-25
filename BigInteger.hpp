#include <vector>
#include <string>
#include <cmath>
#include <iostream>

class BigInteger {
    private:
        const unsigned int LENGTH = 18;
        std::vector<unsigned long long> num;
        bool positive = true;

        unsigned long long length(unsigned long long n) const {
            if (n == 0) return 1;
            return ((long double) log2(n)*0.301029995663981)+1;
        }

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
        BigInteger(std::vector<unsigned long long>& n) {
            num = n;
        }

        BigInteger(std::string& n) {
            num = to_int(n);
        }

        /* BigInteger(std::string n) {
            num = to_int(n);
        } */

        BigInteger() = default;

        unsigned long long len() const {
            return ((num.size()-1)*LENGTH+length(num[num.size()-1]));
        }

        unsigned long long size() const {
            return num.size();
        }

        void push_back(unsigned long long n) {
            num.push_back(n);
        }

        void erase(unsigned long long n) {
            num.erase(num.begin()+n);
        }

        std::vector<unsigned long long>& getArray() {
            return num;
        }

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
            return r;
        }

        unsigned long long const& operator[](unsigned long long i) const {
            return num[i];
        }

        BigInteger operator+(BigInteger& a) {
            return add(a);
        }

        BigInteger operator+(std::string& a) {
            BigInteger temp(a);
            return *this+temp;
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

        BigInteger operator-(BigInteger& a) {
            bool positive = true;
            if (a > *this) {
                BigInteger temp = a;
                a = *this;
                *this = temp;
                positive = false;
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
            r.positive = false;
            return r;
        }

        BigInteger operator-(std::string& a) {
            BigInteger temp(a);
            return *this - temp;
        }

        void operator=(BigInteger a) {
            num = a.getArray();
        }

        void operator+=(BigInteger& a) {
            num = add(a).getArray();
        }
};
