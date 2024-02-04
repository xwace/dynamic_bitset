#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
using namespace std; 

class bitset_ex
{
public:
    bitset_ex(const int bit_size1) :bit_size(bit_size1),cluster_output(8)
    {
        long_size = (bit_size + (long_bit_size - 1)) / long_bit_size;
        data = new long[long_size]();
        memset(data, 0, sizeof(long)*long_size);
    }
    ~bitset_ex()
    {
        delete[]data;
    }
public:
    size_t size()    {
        return bit_size;
    }
    bool any()
    {
        long ret = 0;
        for (int i = 0; i < long_size; ++i)
            ret |= data[i];
        return (ret != 0);
    }
    bool none()
    {
        long ret = 0;
        for (int i = 0; i < long_size; ++i)
            ret = data[i];
        return (ret == 0);
    }
    
public:
    bool test(const int bit_pos) const
    {
        return data[bit_pos / long_bit_size] & (1 << (bit_pos % long_bit_size));
    }
    const bool operator[](const int bit_pos) const
    {
        return data[bit_pos / long_bit_size] & (1 << (bit_pos % long_bit_size));
    }
    void set(const int bit_pos = -1, const bool set_value = true)
    {
        if (bit_pos == -1)
        {
            memset(data, 0XFF, sizeof(long)*long_size);
            return;
        }
        //---------------------------------------------------------------------
        if (set_value)
            data[bit_pos / long_bit_size] |= (1 << (bit_pos % long_bit_size));
        else
            data[bit_pos / long_bit_size] &= (~(1 << (bit_pos % long_bit_size)));
    }
    void reset(const int bit_pos = -1)
    {
        if (bit_pos == -1)  //  if bit_pos == -1, reset all bit to zero
        {
            memset(data, 0, sizeof(long)*long_size);
        }
        else // otherwise, only set the given bit_pos to zero
        {
            data[bit_pos / long_bit_size] &= (~(1 << (bit_pos % long_bit_size)));
        }
    }

    void print()
    {
        for (int i = bit_size - 1; i >= 0; --i)
        {
            if ((*this)[i])
                cout << 1;
            else
                cout << 0;
            if (i % cluster_output == 0)
                cout << " ";
        }
        cout << endl;
    }
public:
    class delegator
    {
        public:
            delegator(bitset_ex& bs, const int bitpos) :p(&bs), bit_pos(bitpos){}
        public:
            delegator& operator = (const bool value) // a special design
            {
                p->set(bit_pos, value);
                return (*this);
            }
            delegator& operator=(const delegator& db)  //bitset_ex b1(..),b2(..);   b1[i] = b2[j];
            {    // assign delegator to element
                p->set(bit_pos, bool(db));
                return (*this);
            }
            bool operator ~() const
            {    // return complemented element
                return (!p->test(bit_pos));
            }
            explicit operator bool() const                     // if(bi[i]) 
            {    // return element
                return (p->test(bit_pos));
            }
        protected:
            bitset_ex * p;
            int         bit_pos;
    };

    bitset_ex::delegator operator[](const int bit_pos)
    {   
        return bitset_ex::delegator(*this,bit_pos);
    }

public:
    void set_output_format(const int output_cluster_digits = 8) // 如： 111101 00110011 11110000， 即多少个bit后输出空格分开！
    {
        if (output_cluster_digits <= 0)
        {
            cluster_output = 8;   // 非法输入，强制设置为8-bit连续输出，从低位到高位！
            return;
        }
        cluster_output = output_cluster_digits;
    }

protected:
    long*  data;
    int    long_size;         // 多少位整数，内部使用！！！    
    int    bit_size;          // 位数
    int    cluster_output;    // 连续多少位输出后加分隔空格，如 1111 0000, 或者 11110000 00110011 ，或者 11 1111100000 0011001100
    const static int long_bit_size = sizeof(long) * 8; //64
}; 
