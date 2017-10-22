
template<>
struct TVector<real, 2>
{
    union
    {
        real data[2];
        struct
        {
            real x;
            real y;
        };
    };
    
    TVector<real, 2>()
    {
        x = y = 0.0f;
    }
    
    TVector<real, 2>(real value)
    {
        x = y = value;
    }
    
    TVector<real, 2>(real inX, real inY)
    :   x(inX),
        y(inY)
    {
        
    }
    
    TVector<real, 2>(const TVector<real, 2>& v)
    :   x(v.x),
        y(v.y)
    {
        
    }
    
    FORCEINLINE real& operator[](const int32 i)
    {
        return data[i];
    }
    
    FORCEINLINE const real& operator[](const int32 i) const
    {
        return data[i];
    }
    
    friend void Pup(Pupper* p, TVector<real, 2>& obj, const VarInfo inVarInfo)
    {
        Pup(p, obj.x, inVarInfo.name + ".x");
        Pup(p, obj.y, inVarInfo.name + ".y");
    }
};

template<>
struct TVector<real, 3>
{
    union
    {
        real data[3];
        struct
        {
            real x;
            real y;
            real z;
        };
    };
    
    TVector<real, 3>()
    {
        x = y = z = 0.0f;
    }
    
    TVector<real, 3>(real value)
    {
        x = y = z = value;
    }
    
    TVector<real, 3>(real inX, real inY, real inZ)
    :   x(inX),
        y(inY),
        z(inZ)
    {
        
    }
    
    TVector<real, 3>(const TVector<real, 3>& v)
    :   x(v.x),
        y(v.y),
        z(v.z)
    {
        
    }
    
    FORCEINLINE real& operator[](const int32 i)
    {
        return data[i];
    }
    
    FORCEINLINE const real& operator[](const int32 i) const
    {
        return data[i];
    }
    
    friend void Pup(Pupper* p, TVector<real, 3>& obj, const VarInfo inVarInfo)
    {
        Pup(p, obj.x, inVarInfo.name + ".x");
        Pup(p, obj.y, inVarInfo.name + ".y");
        Pup(p, obj.z, inVarInfo.name + ".z");
    }
};

FORCEINLINE TVector<real, 3> Cross(const TVector<real, 3>& a, const TVector<real, 3>& b)
{
    TVector<real, 3> v;
    v.x = a.y * b.z - a.z * b.y;
    v.y = a.z * b.x - a.x * b.z;
    v.z = a.x * b.y - a.y * b.x;
    return v;
}

template<>
struct TVector<real, 4>
{
    union
    {
        real data[4];
        struct
        {
            real x;
            real y;
            real z;
            real w;
        };
        struct
        {
            real r;
            real g;
            real b;
            real a;
        };
    };
    
    TVector<real, 4>()
    {
        x = y = z = w = 0.0f;
    }
    
    TVector<real, 4>(real value)
    {
        x = y = z = w = value;
    }
    
    TVector<real, 4>(real inX, real inY, real inZ, real inW)
    :   x(inX),
        y(inY),
        z(inZ),
        w(inW)
    {
        
    }
    
    TVector<real, 4>(const TVector<real, 4>& v)
    :   x(v.x),
        y(v.y),
        z(v.z),
        w(v.w)
    {
        
    }
    
    TVector<real, 4>(const TVector<real, 3>& v, float w)
    :   x(v.x),
        y(v.y),
        z(v.z),
        w(w)
    {
        
    }
    
    FORCEINLINE real& operator[](const int32 i)
    {
        return data[i];
    }
    
    FORCEINLINE const real& operator[](const int32 i) const
    {
        return data[i];
    }
    
    friend void Pup(Pupper* p, TVector<real, 4>& obj, const VarInfo inVarInfo)
    {
        Pup(p, obj.x, inVarInfo.name + ".x");
        Pup(p, obj.y, inVarInfo.name + ".y");
        Pup(p, obj.z, inVarInfo.name + ".z");
        Pup(p, obj.w, inVarInfo.name + ".w");
    }
};








