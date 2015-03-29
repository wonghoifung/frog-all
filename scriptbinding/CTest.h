#ifndef CTEST_HEADER
#define CTEST_HEADER

class CTest
{
public:
    CTest();
    virtual ~CTest();
    int Add(int x, int y);
    
    void setx(int x) { m_x = x; }
    int getx() { return m_x; }
    
    int m_x;
};

#endif


