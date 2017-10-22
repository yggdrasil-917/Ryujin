template<>
struct TMatrix<real, 2, 2>
{
    union u
    {
        struct _s1
        {
            TVector<real, 2> data[2];
        } s1;
        struct _s2
        {
            real m00, m01;
            real m10, m11;
        } s2;
        struct _s3
        {
            TVector<real, 2> x;
            TVector<real, 2> y;
        } s3;

        u() {}
        ~u() {}
    } data;

    TMatrix<real, 2, 2>()
    {
        data.s3.x = TVector<real, 2>(1.0f, 0.0f);
        data.s3.y = TVector<real, 2>(0.0f, 1.0f);
    }
    
    TMatrix<real, 2, 2>(const real v)
    {
        data.s3.x = TVector<real, 2>(v, v);
        data.s3.y = TVector<real, 2>(v, v);
    }

    TMatrix<real, 2, 2>(real m00, real m01,
                        real m10, real m11)
    {
        data.s3.x = TVector<real, 2>(m00, m01);
        data.s3.y = TVector<real, 2>(m10, m11);
    }

    TMatrix<real, 2, 2>(const TVector<real, 2>& inX, const TVector<real, 2>& inY)
    {
        data.s3.x = inX;
        data.s3.y = inY;
    }

    TMatrix<real, 2, 2>(const TMatrix<real, 2, 2>& m)
    {
        data.s3.x = m.data.s3.x;
        data.s3.y = m.data.s3.y;
    }

    FORCEINLINE TVector<real, 2>& operator[](int32 col)
    {
        return data.s1.data[col];
    }

    FORCEINLINE const TVector<real, 2>& operator[](int32 col) const
    {
        return data.s1.data[col];
    }
    
    friend void Pup(Pupper* p, TMatrix<real, 2, 2>& obj, const VarInfo inVarInfo)
    {
        Pup(p, obj.data.s1.data[0], inVarInfo.name + ".col0");
        Pup(p, obj.data.s1.data[1], inVarInfo.name + ".col1");
    }
};

FORCEINLINE real Determinant(const TMatrix<real, 2, 2>& m)
{
    return m.data.s1.data[0].x * m.data.s1.data[1].y - m.data.s1.data[0].y * m.data.s1.data[1].x;
}


template<>
struct TMatrix<real, 3, 3>
{
    union u
    {
        struct _s1
        {
            TVector<real, 3> data[3];
        } s1;
        struct _s2
        {
            real m00, m01, m02;
            real m10, m11, m12;
            real m20, m21, m22;
        } s2;
        struct _s3
        {
            TVector<real, 3> x;
            TVector<real, 3> y;
            TVector<real, 3> z;
        } s3;

        u() {}
        ~u() {}
    } data;
    
    TMatrix<real, 3, 3>()
    {
        data.s3.x = TVector<real, 3>(1.0f, 0.0f, 0.0f);
        data.s3.y = TVector<real, 3>(0.0f, 1.0f, 0.0f);
        data.s3.z = TVector<real, 3>(0.0f, 0.0f, 1.0f);
    }
    
    TMatrix<real, 3, 3>(const real v)
    {
        data.s3.x = TVector<real, 3>(v, v, v);
        data.s3.y = TVector<real, 3>(v, v, v);
        data.s3.z = TVector<real, 3>(v, v, v);
    }
    
    TMatrix<real, 3, 3>(real m00, real m01, real m02,
                        real m10, real m11, real m12,
                        real m20, real m21, real m22)
    {
        data.s3.x = TVector<real, 3>(m00, m01, m02);
        data.s3.y = TVector<real, 3>(m10, m11, m12);
        data.s3.z = TVector<real, 3>(m20, m21, m22);
    }
    
    TMatrix<real, 3, 3>(const TVector<real, 3>& inX, const TVector<real, 3>& inY, const TVector<real, 3>& inZ)
    {
        data.s3.x = inX;
        data.s3.y = inY;
        data.s3.z = inZ;
    }
    
    TMatrix<real, 3, 3>(const TMatrix<real, 3, 3>& m)
    {
        data.s3.x = m.data.s3.x;
        data.s3.y = m.data.s3.y;
        data.s3.z = m.data.s3.z;
    }
    
    FORCEINLINE TVector<real, 3>& operator[](int32 col)
    {
        return data.s1.data[col];
    }
    
    FORCEINLINE const TVector<real, 3>& operator[](int32 col) const
    {
        return data.s1.data[col];
    }
    
    friend void Pup(Pupper* p, TMatrix<real, 3, 3>& obj, const VarInfo inVarInfo)
    {
        Pup(p, obj.data.s1.data[0], inVarInfo.name + ".col0");
        Pup(p, obj.data.s1.data[1], inVarInfo.name + ".col1");
        Pup(p, obj.data.s1.data[2], inVarInfo.name + ".col2");
    }
};


FORCEINLINE real Determinant(const TMatrix<real, 3, 3>& m)
{
    return  m.data.s1.data[0].x * m.data.s1.data[1].y * m.data.s1.data[2].z +
            m.data.s1.data[0].y * m.data.s1.data[1].z * m.data.s1.data[2].x +
            m.data.s1.data[0].z * m.data.s1.data[1].x * m.data.s1.data[2].y -
            m.data.s1.data[0].z * m.data.s1.data[1].y * m.data.s1.data[2].x -
            m.data.s1.data[0].x * m.data.s1.data[1].z * m.data.s1.data[2].y -
            m.data.s1.data[0].y * m.data.s1.data[1].x * m.data.s1.data[2].z;
}

FORCEINLINE TMatrix<real, 3, 3> Inverse(const TMatrix<real, 3, 3>& m)
{
    TMatrix<real, 3, 3> out;
    real det = Determinant(m);
    if (det == 0.0f)
    {
        return out;
    }
    
    for (int32 c = 0; c < 3; ++c)
    {
        for (int32 r = 0; r < 3; ++r)
        {
            TMatrix<real, 2, 2> minor = Math::Minor(m, r, c);
            out.data.s1.data[r].data[c] = Determinant(minor);
        }
    }
    
    real oneOverDet = 1.0f / det;
    return out * oneOverDet;
}

template<>
struct TMatrix<real, 4, 4>
{
    union u
    {
        struct _s1
        {
            TVector<real, 4> data[4];
        } s1;
        struct _s2
        {
            real m00, m01, m02, m03;
            real m10, m11, m12, m13;
            real m20, m21, m22, m23;
            real m30, m31, m32, m33;
        } s2;
        struct _s3
        {
            TVector<real, 4> x;
            TVector<real, 4> y;
            TVector<real, 4> z;
            TVector<real, 4> w;
        } s3;

        u() {}
        ~u() {}
    } data;
    TMatrix<real, 4, 4>()
    {
        data.s3.x = TVector<real, 4>(1.0f, 0.0f, 0.0f, 0.0f);
        data.s3.y = TVector<real, 4>(0.0f, 1.0f, 0.0f, 0.0f);
        data.s3.z = TVector<real, 4>(0.0f, 0.0f, 1.0f, 0.0f);
        data.s3.w = TVector<real, 4>(0.0f, 0.0f, 0.0f, 1.0f);
    }
    
    TMatrix<real, 4, 4>(const real v)
    {
        data.s3.x = TVector<real, 4>(v, 0, 0, 0);
        data.s3.y = TVector<real, 4>(0, v, 0, 0);
        data.s3.z = TVector<real, 4>(0, 0, v, 0);
        data.s3.w = TVector<real, 4>(0, 0, 0, v);
    }
    
    TMatrix<real, 4, 4>(real m00, real m01, real m02, real m03,
                        real m10, real m11, real m12, real m13,
                        real m20, real m21, real m22, real m23,
                        real m30, real m31, real m32, real m33)
    {
        data.s3.x = TVector<real, 4>(m00, m01, m02, m03);
        data.s3.y = TVector<real, 4>(m10, m11, m12, m13);
        data.s3.z = TVector<real, 4>(m20, m21, m22, m23);
        data.s3.w = TVector<real, 4>(m30, m31, m32, m33);
    }
    
    TMatrix<real, 4, 4>(const TVector<real, 4>& inX, const TVector<real, 4>& inY, const TVector<real, 4>& inZ, const TVector<real, 4>& inW)
    {
        data.s3.x = inX;
        data.s3.y = inY;
        data.s3.z = inZ;
        data.s3.w = inW;
    }
    
    TMatrix<real, 4, 4>(const TMatrix<real, 4, 4>& m)
    {
        data.s3.x = m.data.s3.x;
        data.s3.y = m.data.s3.y;
        data.s3.z = m.data.s3.z;
        data.s3.w = m.data.s3.w;
    }
    
    FORCEINLINE TVector<real, 4>& operator[](int32 col)
    {
        return data.s1.data[col];
    }
    
    FORCEINLINE const TVector<real, 4>& operator[](int32 col) const
    {
        return data.s1.data[col];
    }
    
    FORCEINLINE TVector<real, 4> operator*(const TVector<real, 4>& v) const
    {
        TVector<real, 4> res;
        for (int32 i = 0; i < 4; ++i)
        {
            res.x += data.s1.data[i].x;
        }
        for (int32 i = 0; i < 4; ++i)
        {
            res.x += data.s1.data[i].y;
        }
        for (int32 i = 0; i < 4; ++i)
        {
            res.x += data.s1.data[i].z;
        }
        for (int32 i = 0; i < 4; ++i)
        {
            res.x += data.s1.data[i].w;
        }
        return res;
//        return TVector<real, 4>(Dot(data.s1.data[0], v), Dot(data.s1.data[1], v), Dot(data.s1.data[2], v), Dot(data.s1.data[3], v));
    }
    
    friend void Pup(Pupper* p, TMatrix<real, 4, 4>& obj, const VarInfo inVarInfo)
    {
        Pup(p, obj.data.s1.data[0], inVarInfo.name + ".col0");
        Pup(p, obj.data.s1.data[1], inVarInfo.name + ".col1");
        Pup(p, obj.data.s1.data[2], inVarInfo.name + ".col2");
        Pup(p, obj.data.s1.data[3], inVarInfo.name + ".col3");
    }
};




FORCEINLINE real Determinant(const TMatrix<real, 4, 4>& m)
{
    TMatrix<real, 3, 3> m0 = Math::Minor(m, 0, 0);
    TMatrix<real, 3, 3> m1 = Math::Minor(m, 0, 1);
    TMatrix<real, 3, 3> m2 = Math::Minor(m, 0, 2);
    TMatrix<real, 3, 3> m3 = Math::Minor(m, 0, 3);
    
    return m.data.s1.data[0].x * Determinant(m0) - m.data.s1.data[0].y * Determinant(m1) + m.data.s1.data[0].z * Determinant(m2) - m.data.s1.data[0].w * Determinant(m3);
}


FORCEINLINE TMatrix<real, 4, 4> Inverse(const TMatrix<real, 4, 4>& m)
{
    TMatrix<real, 4, 4> out;
    real det = Determinant(m);
    if (det == 0.0f)
    {
        return out;
    }
    
    for (int32 c = 0; c < 4; ++c)
    {
        for (int32 r = 0; r < 4; ++r)
        {
            TMatrix<real, 3, 3> minor = Math::Minor(m, c, r);
            out.data.s1.data[r].data[c] = Determinant(minor);
        }
    }
    
    real oneOverDet = 1.0f / det;
    return out * oneOverDet;
}

FORCEINLINE TVector<real, 4> operator*(TMatrix<real, 4, 4>& m, const TVector<real, 4>& v)
{
    return TVector<real, 4>(Dot(m[0], v), Dot(m[1], v), Dot(m[2], v), Dot(m[3], v));
}


