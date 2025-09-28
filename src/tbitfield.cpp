// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
    if (len <= 0)
        throw "Некорректная длина поля";

    BitLen = len;
    MemLen = (len + sizeof(TELEM)*8 - 1) / (sizeof(TELEM)*8);
    pMem = new TELEM[MemLen];

    for (int i = 0; i < MemLen; ++i){
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[bf.MemLen];

    for (int i = 0; i < bf.MemLen; ++i){
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete [] pMem;
    pMem = nullptr;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n >= BitLen)
        throw "Индекс бита вне поля";
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n >= BitLen)
        throw "Индекс бита вне поля";
    return 1 << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    int index_n = GetMemIndex(n);
    TELEM maska = GetMemMask(n);
    pMem[index_n] |= maska;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    int index_n = GetMemIndex(n);
    TELEM maska = GetMemMask(n);
    pMem[index_n] &= ~maska;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    int index_n = GetMemIndex(n);
    TELEM maska = GetMemMask(n);
    return (pMem[index_n] & maska) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf){return *this;}

    BitLen = bf.BitLen;
    if (MemLen != bf.MemLen){
        MemLen = bf.MemLen;
        delete [] pMem;
        pMem = new TELEM[bf.MemLen];
    }
    for (int i = 0; i < MemLen; ++i){
        pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 0;

    for (int i = 0; i < MemLen; ++i)
        if (pMem[i] != bf.pMem[i])
            return 0;

    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 1;

    for (int i = 0; i < MemLen; ++i)
        if (pMem[i] == bf.pMem[i])
            return 0;

    return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int max_Len = max(BitLen , bf.BitLen);
    TBitField res(max_Len);

    for (int i = 0; i < MemLen; ++i){
        res.pMem[i] = pMem[i];
    }
    for (int i = 0; i < bf.MemLen; ++i){
        res.pMem[i] |= bf.pMem[i];
    }
    return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int max_Len = max(BitLen , bf.BitLen), min_Size = min(MemLen, bf.MemLen);
    TBitField res(max_Len);

    for (int i = 0; i < min_Size; ++i){
        res.pMem[i] = pMem[i] & bf.pMem[i];
    }

    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(BitLen);

    for (int i = 0; i < MemLen; ++i){
        res.pMem[i] = ~pMem[i];
    }

    int bitsInLast = BitLen % (sizeof(TELEM) * 8);
    if (bitsInLast != 0) {
        TELEM mask = (1 << bitsInLast) - 1;
        res.pMem[MemLen - 1] &= mask;
    }

    return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    for (int i = 0; i < bf.BitLen; ++i){
        int bit;
        istr >> bit;

        if (bit == 1) bf.SetBit(i);
        else if (bit == 0) bf.ClrBit(i);
        else break;
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; ++i){
        ostr << bf.GetBit(i);
    }
    return ostr;
}
