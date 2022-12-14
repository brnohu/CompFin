#pragma once

#include "../Utility/kMatrix.h"

namespace kXlUtils
{
    LPXLOPER12 getOper(int rows, int cols);

    bool checkDim(LPXLOPER12 in, int row, int col, string* error);

    bool getDbl(LPXLOPER12 in, int row, int col, double& num, string* error);
    bool getInt(LPXLOPER12 in, int row, int col, int& i, string* error);
    bool getStr(LPXLOPER12 in, int row, int col, string& str, string* error);

    void setDbl(int row, int col, double num, LPXLOPER12 out);
    void setStr(int row, int col, const string& str, LPXLOPER12 out);

    LPXLOPER12 setError(const string& error);

    bool getVector(LPXLOPER12 in, kVector<double>& out);
    bool getMatrix(LPXLOPER12 in, kMatrix<double>& out);

    void setVector(const kVector<double>& in, LPXLOPER12 out);
    void setMatrix(const kMatrix<double>& in, LPXLOPER12 out);
}

LPXLOPER12
kXlUtils::getOper(
    int rows,
    int cols)
{
    LPXLOPER12 out = TempXLOPER12();
    resize(out, rows, cols);
    return out;
}

bool
kXlUtils::checkDim(
    LPXLOPER12  in,
    int         row,
    int         col,
    string* error)
{
    if (row < 0 || (int)getRows(in) <= row || col < 0 || (int)getCols(in) <= col)
    {
        if (error) *error = "kXlUtils::checkDim() : dimension error";
        return false;
    }

    //  done
    return true;
}

bool
kXlUtils::getDbl(
    LPXLOPER12  in,
    int         row,
    int         col,
    double& num,
    string* error)
{
    if (!checkDim(in, row, col, error)) return false;
    double d = getNum(in, row, col);
    if (d == numeric_limits<double>::infinity())
    {
        if (error) *error = "kXlUtils::getDbl() : not able to get number";
        return false;
    }
    num = d;

    //  done
    return true;
}

bool
kXlUtils::getInt(
    LPXLOPER12  in,
    int         row,
    int         col,
    int& i,
    string* error)
{
    double d;
    if (!getDbl(in, row, col, d, error)) return d;
    i = (int)d;

    //  done
    return true;
}

bool
kXlUtils::getStr(
    LPXLOPER12  in,
    int         row,
    int         col,
    string& str,
    string* error)
{
    if (!checkDim(in, row, col, error)) return false;
    str = getString(in, row, col);
    if (!str.size())
    {
        if (error) *error = "kXlUtils::getStr() : no able to get string";
        return false;
    }

    //  done
    return true;
}

void
kXlUtils::setDbl(
    int         row,
    int         col,
    double      num,
    LPXLOPER12  out)
{
    if (!checkDim(out, row, col, nullptr)) return;
    setNum(out, num, row, col);

    //  done
    return;
}


void
kXlUtils::setStr(
    int             row,
    int             col,
    const string& str,
    LPXLOPER12      out)
{
    if (!checkDim(out, row, col, nullptr)) return;
    setString(out, str, row, col);

    //  done
    return;
}

LPXLOPER12
kXlUtils::setError(
    const string& error)
{
    //	declare, resize, fill
    LPXLOPER12 out = getOper(1, 1);
    setStr(0, 0, error, out);

    //  done
    return out;
}

bool kXlUtils::getVector(LPXLOPER12 in, kVector<double>& out)
{
    size_t m = getRows(in);

    out.resize(m);

    for(size_t i=0;i<m;++i)
    {
        double d = getNum(in, i, 0);
        if(d==numeric_limits<double>::infinity())
            return false;
        out((int)i) = d;
    }
    return true;
}

bool kXlUtils::getMatrix(LPXLOPER12 in, kMatrix<double>& out)
{
    size_t m = getRows(in);
    size_t n = getCols(in);
    out.resize(m, n);

    for(size_t i=0;i<m;++i)
    {
        for(size_t j=0;j<n;++j)
        {
            double d = getNum(in, i, j);
            if(d==numeric_limits<double>::infinity())
                return false;
            out((int)i, (int)j) = d;
        }
    }
    return true;
}

void kXlUtils::setVector(const kVector<double>& in, LPXLOPER12 out)
{
    size_t m = in.size();
    resize(out,m,1);
    for(size_t i=0;i<m;++i)
        setNum(out,in((int)i),i,0);
}

void kXlUtils::setMatrix(const kMatrix<double>& in, LPXLOPER12 out)
{
    size_t m = in.rows();
    size_t n = in.cols();
    resize(out,m,n);
    for(size_t i=0;i<m;++i)
        for(size_t j=0;j<n;++j)
            setNum(out,in((int)i,(int)j),i,j);
}
