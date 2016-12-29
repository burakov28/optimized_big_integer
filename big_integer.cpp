#include "big_integer.h"

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cassert>


#define puba push_back
#define LL long long

int const dec_base = (int) 1e9;
int const dec_base_log = 9;
int const base_log = 30;
int const base = (1 << 30);

const int MAX_MEM = (int) 240 * 1024 * 1024;

int mpos, max_mpos;
char memory[MAX_MEM];

inline void * operator new (size_t n) {
    char *res = memory + mpos;
    mpos += n;
    assert(mpos <= MAX_MEM);
    return (void *)res; 
}

inline void operator delete (void *) { }




void del_zeros(big_integer & a) {
    while (a.line.size() > 1 && a.line.back() == 0) a.line.pop_back();
    if (a.line.size() == 1 && a.line[0] == 0) a.sign = false;
}

void normalize(big_integer & a) {
    
    del_zeros(a);

    for (int i = 0; i < (int) a.line.size(); ++i) {
        if (a.line[i] < 0) continue;
        if (i + 1 == (int) a.line.size()) {
            if (a.line[i] == 0) break;
            a.line.puba(0);
        }
        a.line[i + 1] += a.line[i] / base;
        a.line[i] %= base;
    }

    if (a.line.back() < 0) {
        a.sign = !a.sign;
        for (int i = 0; i < (int) a.line.size(); ++i) {
            a.line[i] = -a.line[i];
        }
    }
   

    a.line.back() -= 1;
    for (int i = 0; i + 1 < (int) a.line.size(); ++i) {
        a.line[i] += base - 1;
    }
    a.line[0] += 1;

    for (int i = 0; i < (int) a.line.size(); ++i) {
        if (i + 1 == (int) a.line.size()) {
            if (a.line[i] == 0) break;
            a.line.puba(0);
        }
        a.line[i + 1] += a.line[i] / base;
        a.line[i] %= base;
    }
    del_zeros(a);
}

void short_mul(big_integer & a, int b) {
    if (b < 0) {
        a.sign = !a.sign;
        b = -b;
    }

    a.line.puba(0);
    a.line.puba(0);
    a.line.puba(0);
    LL cur = 0;
    for (int i = 0; i < (int) a.line.size(); ++i) {
        cur += ((LL) a.line[i]) * b;        
        a.line[i] = cur % base; 
        cur /= base;
    }
    del_zeros(a);
}

int short_div(big_integer & a, int b) {
    if (b < 0) {
        a.sign = !a.sign;
        b = -b;
    }

    LL cur = 0;
    for (int i = (int) a.line.size() - 1; i >= 0; --i) {
        cur *= base;
        cur += a.line[i];
        a.line[i] = cur / b;
        cur = cur % b;  
    }
    del_zeros(a);
    return cur;
}

int neg(int a) {
    int ret = 0;
    for (int i = 0; i < base_log; ++i) {
        if ((a & (1 << i)) == 0) {
            ret += (1 << i);    
        }
    }
    return ret;
}

void to_bin_form(big_integer & a, int sz) {
    while ((int) a.line.size() < sz + 1) a.line.puba(0);
    if (a.sign) {
        for (int i = 0; i < sz; ++i) {
            a.line[i] = neg(a.line[i]);
        }
        a.sign = false;
        a += 1;
    }
}

std::string to_bin_string(big_integer const& a) {
    std::string ret = "";
    for (int i = (int) a.line.size() - 1; i >= 0; --i) {
        int cur = a.line[i];
        for (int j = base_log - 1; j >= 0; --j) {
            if (cur & (1 << j)) {
                ret += '1';
            }
            else {
                ret += '0';
            }
        }
    }
    return ret;
}

void to_base_form(big_integer & a) {
    if (a.line.back() != 0) {
        a.sign = false;
        a -= 1;
        a.sign = true;
        
        for (int i = 0; i < (int) a.line.size(); ++i) {
            a.line[i] = neg(a.line[i]);
        }
        //a += 1;
    }
    else {
        a.sign = false;
    }
    del_zeros(a);
}

bool uLess(big_integer const & a, big_integer const & b) {
    if (a.line.size() < b.line.size()) return true;
    if (a.line.size() > b.line.size()) return false;

    for (int i = (int) a.line.size() - 1; i >= 0; --i) {
        if (a.line[i] < b.line[i]) return true;
        if (a.line[i] > b.line[i]) return false;
    }
    return false;
}

big_integer::big_integer()
{
    //line.clear();
    line.puba(0);
    sign = false;
}

big_integer::big_integer(big_integer const& other) : line(other.line), sign(other.sign)
{
    //std::cout << "constructor started" << std::endl;
    //std::cout << "hey" << std::endl;
    //line = other.line;

    //std::cout << "constructor break" << std::endl;
    //sign = other.sign;
}

big_integer::big_integer(int a)
{
    sign = false;
    /*
    if (a < 0) {
        sign = true;
        a = -a;
    }
    */
    //line.clear();
    line.puba(a % base);
    line.puba(a / base);
    
    //std::cout << a << std::endl;
    del_zeros(*this);
    if (line.back() < 0) {
        for (int i = 0; i < (int) line.size(); ++i) {
            line[i] = - line[i];
        }
        sign = true;
    }
}

big_integer::big_integer(std::string const& str)
{
    //line.clear();
    line.puba(0);
    sign = false;

    int sz = str.size();
    if (str[0] == '-') {
        //sign = true;
        --sz;
    }

    int pos = 0;
    if (str[0] == '-') {
        ++pos;
    }
    for (int i = 0; i < sz / dec_base_log; ++i) {
        int cur = 0;        
        for (int j = 0; j < dec_base_log; ++j) {
            cur *= 10;
            cur += (str[pos] - '0');
            ++pos;            
        }

        //std::cout << cur << std::endl;
        short_mul(*this, dec_base);
        //std::cout << *this << std::endl;
        /*
        for (int j = 0; j < (int) line.size(); ++j) {
            std::cout << line[j] << " ";
        }
        std::cout << std::endl;
        */

        (*this) += cur;
        
        /*for (int j = 0; j < (int) line.size(); ++j) {
            std::cout << line[j] << " ";
        }

        std::cout << std::endl;
        */
    }
    int cur = 0;
    int pw = 1;
    for (int i = pos; i < (int) str.size(); ++i) {
        cur *= 10;
        cur += (str[i] - '0');
        pw *= 10;
    }
    //std::cout << pw << " " << cur << std::endl;
    short_mul(*this, pw);
    (*this) += cur;
    if (str[0] == '-') sign = true;
    del_zeros(*this);
    /*
    std::cout << "well done!" << " ";
    for (int i = 0; i < (int) line.size(); ++i) {
        std::cout << line[i] << " ";
    }
    std::cout << std::endl;
    */
}


big_integer::~big_integer() {}

big_integer& big_integer::operator=(big_integer const& other)
{
    line = other.line;
    sign = other.sign;
    return *this;
}

big_integer& big_integer::operator+=(big_integer const& rhs)
{
    //std::cout << "loL" << std::endl;
    while (line.size() < rhs.line.size()) line.puba(0);
    /*
    for (int i = 0; i < (int) line.size(); ++i) {
        std::cout << line[i] << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < (int) line.size(); ++i) {
        std::cout << rhs.line[i] << " ";
    }
    std::cout << std::endl;
    */
    if (sign != rhs.sign) {
        //std::cout << "hell" << std::endl;
        for (int i = 0; i < (int) rhs.line.size(); ++i) {
            line[i] -= rhs.line[i];
        }
    }
    else {
        for (int i = 0; i < (int) rhs.line.size(); ++i) {
            line[i] += rhs.line[i];
        }
    }
    /*
    for (int i = 0; i < (int) line.size(); ++i) {
        std::cout << line[i] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
    */
    del_zeros(*this);

    if (line.back() < 0) {
        sign = !sign;
        for (int i = 0; i < (int) line.size(); ++i) {
            line[i] = -line[i];
        }
    }

    for (int i = 0; i + 1 < (int) line.size(); ++i) {
        while (line[i] < 0) {
            line[i] += base;
            line[i + 1] -= 1;
        }
    }

    for (int i = 0; ; ++i) {
        if (i + 1 == (int) line.size()) {
            if (line[i] == 0) break;
            else line.puba(0);
        }
        line[i + 1] += line[i] / base;
        line[i] %= base;
    }

    del_zeros(*this);
    /*
    std::cout << "@ ";
    for (int i = 0; i < (int) line.size(); ++i) {
        std::cout << line[i] << " ";
    }
    std::cout << std::endl;
    */
    /*
    std::cout << "# ";
    for (int i = 0; i < (int) line.size(); ++i) {
        std::cout << line[i] << " ";
    }
    std::cout << std::endl;
    */
    return *this;
}

big_integer& big_integer::operator-=(big_integer const& rhs)
{
    sign = !sign;
    (*this) += rhs;
    sign = !sign;
    del_zeros(*this);
    return *this;
}

big_integer& big_integer::operator*=(big_integer const& rhs)
{
    big_integer tmp = (*this);

    sign = sign ^ rhs.sign;
    line.resize(line.size() + rhs.line.size() + 2);
    for (int i = 0; i < (int) line.size(); ++i) {
        line[i] = 0;
    }

    for (int i = 0; i < (int) rhs.line.size(); ++i) {
        LL b = rhs.line[i];
        for (int j = (int) tmp.line.size() - 1; j >= 0; --j) {
            LL cur = b * tmp.line[j];
            line[i + j] += cur % base;

            if (line[i + j] >= base) {
                line[i + j + 1] += 1;
                line[i + j] -= base;
            }

            line[i + j + 1] += (cur >> base_log); 
        }
        for (int j = 0; j + 1 < (int) line.size(); ++j) {
            while (line[j] > base) {
                line[j] -= base;
                line[j + 1] += 1;
            }
        }

    }
    del_zeros(*this);
    return *this;
}

big_integer& big_integer::operator/=(big_integer const& rhs)
{
    if (uLess(*this, rhs)) {
        *this = 0;
        return *this;
    }

    sign ^= rhs.sign;
    
    if (rhs.line.size() == 1) {
        short_div(*this, rhs.line[0]);
        return *this;    
    }
    
    big_integer ta = *this;
    ta.sign = false;

    for (int i = 0; i < (int) line.size(); ++i) {
        line[i] = 0;
    }
        
    int sz = line.size();
    sz -= (int) rhs.line.size();
    LL cur = 0;
    int rsz = rhs.line.size();
    optimized_vector < LL > tmp (rhs.line.size());
    for (int i = 0; i < sz + 1; ++i) {
        //std::cout << cur << std::endl;
        int pos = line.size() - i - 1;
        cur *= base;        
        cur += ta.line[pos];

        LL l = 0, r = base + 2;
        //std::cout << cur << std::endl;
        while (true) {
            LL b = 0;
            if (r - l > 1) {
                b = (r + l) / 2;
            }
            else {
                b = l;
            }

            //std::cout << b << " b" << std::endl;
            
            LL dcur = 0;
            for (int j = 0; j < rsz; ++j) {
                dcur += b * rhs.line[j];
                if (j + 1 != rsz) {
                    tmp[j] = dcur % base;
                    dcur /= base;
                }
                else {
                    tmp[j] = dcur;
                }
            }

            bool flag = true;
            for (int j = rsz - 1; j >= 0; --j) {
                if (j + 1 != rsz) {
                    if (ta.line[sz + j - i] < tmp[j]) {
                        flag = false;
                        break;
                    }
                    if (ta.line[sz + j - i] > tmp[j]) {
                        break;
                    }
                }
                else {
                    if (cur < tmp[j]) {
                        flag = false;
                        break;
                    }
                    if (cur > tmp[j]) {
                        break;
                    }
                }
            }

            if (r - l <= 1) break;
            if (flag) {
                l = b;
            }
            else {
                r = b;
            }
            
        }

        line[sz - i] = l;

        //std::cout << l << " l" << std::endl;

        for (int j = 0; j < rsz; ++j) {
            if (j + 1 != rsz) {
                ta.line[sz + j - i] -= tmp[j];
            }
            else {
                cur -= tmp[j];
            }
        }
        ta.line[pos] = cur;

        for (int j = 0; j + 1 < (int) ta.line.size(); ++j) {
            if (ta.line[j] < 0) {
                ta.line[j] += base;
                ta.line[j + 1] -= 1;
            }
        }

        cur = ta.line[pos];

        //del_zeros(ta);
    }
    //std::cerr << "lol" << std::endl;
    del_zeros(*this);
    return *this;
}

big_integer& big_integer::operator%=(big_integer const& rhs)
{
    big_integer tmp = *this;
    tmp /= rhs;
    tmp *= rhs;
    *this -= tmp;
    //del_zeros(*this);
    return *this;
}

big_integer& big_integer::operator&=(big_integer const& rhs)
{
    to_bin_form(*this, std::max(line.size(), rhs.line.size()) + 1);
    big_integer tmp = rhs;
    to_bin_form(tmp, std::max(line.size(), rhs.line.size()) + 1);
    for (int i = 0; i < (int) line.size(); ++i) {
        line[i] &= tmp.line[i];
    }
    to_base_form(*this);
    return *this;
}

big_integer& big_integer::operator|=(big_integer const& rhs)
{
    to_bin_form(*this, std::max(line.size(), rhs.line.size()) + 1);
    big_integer tmp = rhs;
    to_bin_form(tmp, std::max(line.size(), rhs.line.size()) + 1);
    for (int i = 0; i < (int) line.size(); ++i) {
        line[i] |= tmp.line[i];
    }
    to_base_form(*this);
    return *this;

}

big_integer& big_integer::operator^=(big_integer const& rhs)
{
    to_bin_form(*this, std::max(line.size(), rhs.line.size()) + 1);
    big_integer tmp = rhs;
    to_bin_form(tmp, std::max(line.size(), rhs.line.size()) + 1);
    for (int i = 0; i < (int) line.size(); ++i) {
        line[i] ^= tmp.line[i];
    }
    to_base_form(*this);
    return *this;
}

big_integer& big_integer::operator<<=(int rhs)
{
    int sh = rhs / base_log;
    line.reverse();
    //std::reverse(line.begin(), line.end());
    for (int i = 0; i < sh; ++i) line.puba(0);
    line.reverse();
    //std::reverse(line.begin(), line.end());
    
    del_zeros(*this);
    short_mul(*this, 1 << (rhs % base_log));
    return *this;
}

big_integer& big_integer::operator>>=(int rhs)
{
    
    bool sg = sign;
    if (sg) {
        to_bin_form(*this, line.size() + rhs / base_log + 2);
    }
    //std::cout << to_bin_string(*this) << std::endl;
    int sh = rhs / base_log;
    line.reverse();
    //std::reverse(line.begin(), line.end());
    for (int i = 0; i < sh && line.size() > 0; ++i) line.pop_back();
    if (line.size() == 0) {
        line.puba(0);
        sign = false;
        return *this;
    }

    line.reverse();
    //std::reverse(line.begin(), line.end()); 
    //del_zeros(*this);


    short_div(*this, 1 << (rhs % base_log));
    if (sg) {
        for (int i = 0; i < rhs % base_log; ++i) {
            line.back() += (1 << (base_log - 1 - i));
        }
        to_base_form(*this);
    }
    //std::cout << to_bin_string(*this) << std::endl;
    
    //std::cout << to_bin_string(*this) << std::endl;
    return *this;
}

big_integer big_integer::operator+() const
{
    return *this;
}

big_integer big_integer::operator-() const
{
    big_integer r = (*this);
    r.sign = !sign;
    del_zeros(r);
    return r;
}

big_integer big_integer::operator~() const
{
    big_integer ret = *this;
    ret += 1;
    return -ret;
}

big_integer& big_integer::operator++()
{
    (*this) += 1;
    return *this;
}

big_integer big_integer::operator++(int)
{
    big_integer r = *this;
    ++*this;
    return r;
}

big_integer& big_integer::operator--()
{
    (*this) -= 1;
    return *this;
}

big_integer big_integer::operator--(int)
{
    big_integer r = *this;
    --*this;
    return r;
}

big_integer operator+(big_integer a, big_integer const& b)
{
    return a += b;
}

big_integer operator-(big_integer a, big_integer const& b)
{
    return a -= b;
}

big_integer operator*(big_integer a, big_integer const& b)
{
    return a *= b;
}

big_integer operator/(big_integer a, big_integer const& b)
{
    return a /= b;
}

big_integer operator%(big_integer a, big_integer const& b)
{
    return a %= b;
}

big_integer operator&(big_integer a, big_integer const& b)
{
    return a &= b;
}

big_integer operator|(big_integer a, big_integer const& b)
{
    return a |= b;
}

big_integer operator^(big_integer a, big_integer const& b)
{
    return a ^= b;
}

big_integer operator<<(big_integer a, int b)
{
    return a <<= b;
}

big_integer operator>>(big_integer a, int b)
{
    return a >>= b;
}

bool operator==(big_integer const& a, big_integer const& b)
{
    if (a.sign != b.sign || a.line.size() != b.line.size()) return false;
    for (int i = 0; i < (int) a.line.size(); ++i) {
        if (a.line[i] != b.line[i]) return false;
    }
    return true;   
}

bool operator!=(big_integer const& a, big_integer const& b)
{
    return !(a == b);
}

bool operator<(big_integer const& a, big_integer const& b)
{
    if (a.sign == b.sign) {
        if (a.sign) {
            return (uLess(b, a));
        }
        else {
            return (uLess(a, b));
        }
    }
    else {
        return a.sign;
    }
}

bool operator>(big_integer const& a, big_integer const& b)
{
    return (b < a);
}

bool operator<=(big_integer const& a, big_integer const& b)
{
    return !(a > b);
}

bool operator>=(big_integer const& a, big_integer const& b)
{
    return !(a < b);
}

std::string to_string(big_integer const& a)
{
    //std::cout << "start" << std::endl;
    if (a == 0) {
        return "0";
    }
    //std::cout << "1" << std::endl;
    big_integer tmp = a;
    //std::cout << "2" << std::endl;
    bool sg = tmp.sign;
    tmp.sign = false;
    optimized_vector < int > ans;
    //std::cout << "1" << std::endl;
    //std::cout << "before reversing" << std::endl;
    while (tmp > 0) {
        ans.puba(short_div(tmp, dec_base));
    }
    ans.reverse();
    //std::cout << "has been reversed" << std::endl;
    //std::reverse(ans.begin(), ans.end());
    std::string ret = "";
    if (sg) ret += '-';
    for (int i = 0; i < (int) ans.size(); ++i) {
        int cur = ans[i];
        std::string cs = "";
        while (cur > 0) {
            cs += (cur % 10) + '0';
            cur /= 10;
        }
        if (i != 0) {
            while ((int) cs.size() < dec_base_log) cs += '0';
        }
        std::reverse(cs.begin(), cs.end());
        ret += cs;
    }
    return ret;
}
                     
std::ostream& operator<<(std::ostream& s, big_integer const& a)
{
    return (s << to_string(a));
}