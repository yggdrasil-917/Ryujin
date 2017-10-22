
#pragma once


#include "MathUtils.hpp"
#include "../Allocators/Memory.hpp"

#include "../Serialization/BinaryFilePupper.hpp"
#include "../Serialization/TextFilePupper.hpp"

namespace Ryujin
{
    namespace Math
    {
        template<typename T, int32 dimension>
        struct TVector
        {
            T data[dimension];
            
            FORCEINLINE T& operator[](const int32 i)
            {
                return data[i];
            }
            
            FORCEINLINE const T& operator[](const int32 i) const
            {
                return data[i];
            }
            
            FORCEINLINE TVector<T, dimension>& operator=(const TVector<T, dimension>& v)
            {
                for (int32 i = 0; i < dimension; ++i)
                {
                    data[i] = v.data[i];
                }
                
                return *this;
            }
            
            FORCEINLINE TVector<T, dimension> operator+(const TVector<T, dimension>& v) const
            {
                TVector<T, dimension> result;
                for (int32 i = 0; i < dimension; ++i)
                {
                    result.data[i] = data[i] + v.data[i];
                }
                
                return result;
            }
            
            FORCEINLINE TVector<T, dimension> operator-(const TVector<T, dimension>& v) const
            {
                TVector<T, dimension> result;
                for (int32 i = 0; i < dimension; ++i)
                {
                    result.data[i] = data[i] - v.data[i];
                }
                
                return result;
            }
            
            FORCEINLINE TVector<T, dimension> operator*(const real k) const
            {
                TVector<T, dimension> result;
                for (int32 i = 0; i < dimension; ++i)
                {
                    result.data[i] = data[i] * k;
                }
                
                return result;
            }
            
            FORCEINLINE TVector<T, dimension> operator*(const TVector<T, dimension>& v) const
            {
                TVector<T, dimension> result;
                for (int32 i = 0; i < dimension; ++i)
                {
                    result.data[i] = data[i] * v.data[i];
                }
                
                return result;
            }
            
            FORCEINLINE TVector<T, dimension>& operator+=(const TVector<T, dimension>& v)
            {
                for (int32 i = 0; i < dimension; ++i)
                {
                    data[i] += v.data[i];
                }
                
                return *this;
            }
            
            FORCEINLINE TVector<T, dimension>& operator-=(const TVector<T, dimension>& v)
            {
                for (int32 i = 0; i < dimension; ++i)
                {
                    data[i] -= v.data[i];
                }
                
                return *this;
            }
            
            FORCEINLINE TVector<T, dimension>& operator*=(const real k)
            {
                for (int32 i = 0; i < dimension; ++i)
                {
                    data[i] *= k;
                }
                
                return *this;
            }
        };
        
        
        template<typename T, int32 dimension>
        FORCEINLINE TVector<T, dimension> Min(const TVector<T, dimension>& a, const TVector<T, dimension>& b)
        {
            TVector<T, dimension> minV;
            for (int32 i = 0; i < dimension; ++i)
            {
                minV.data[i] = a.data[i] < b.data[i] ? a.data[i] : b.data[i];
            }
            return minV;
        }
        
        template<typename T, int32 dimension>
        FORCEINLINE TVector<T, dimension> Max(const TVector<T, dimension>& a, const TVector<T, dimension>& b)
        {
            TVector<T, dimension> maxV;
            for (int32 i = 0; i < dimension; ++i)
            {
                maxV.data[i] = a.data[i] > b.data[i] ? a.data[i] : b.data[i];
            }
            return maxV;
        }
        
        template<typename T, int32 dimension>
        FORCEINLINE TVector<T, dimension> Abs(const TVector<T, dimension>& v)
        {
            TVector<T, dimension> out;
            for (int32 i = 0; i < dimension; ++i)
            {
                out.data[i] = MathUtils::Abs(v.data[i]);
            }
            return out;
        }
        
        template<typename T, int32 dimension>
        FORCEINLINE TVector<T, dimension> Clamp(const TVector<T, dimension>& v, T clampMin, T clampMax)
        {
            TVector<T, dimension> out;
            for (int32 i = 0; i < dimension; ++i)
            {
                out.data[i] = MathUtils::Clamp(v.data[i], clampMin, clampMax);
            }
            return out;
        }
        
        template<typename T, int32 dimension>
        FORCEINLINE TVector<T, dimension> Lerp(const TVector<T, dimension>& start, const TVector<T, dimension>& end, real t)
        {
            TVector<T, dimension> out;
            for (int32 i = 0; i < dimension; ++i)
            {
                out.data[i] = start.data[i] + (end.data[i] - start.data[i]) * t;
            }
            return out;
        }
        
        template<typename T, int32 dimension>
        FORCEINLINE TVector<T, dimension> operator+(const TVector<T, dimension>& a, const TVector<T, dimension>& b)
        {
            TVector<T, dimension> result;
            for (int32 i = 0; i < dimension; ++i)
            {
                result.data[i] = a.data[i] + b.data[i];
            }
            
            return result;
        }
        
        template<typename T, int32 dimension>
        FORCEINLINE TVector<T, dimension> operator-(const TVector<T, dimension>& a, const TVector<T, dimension>& b)
        {
            TVector<T, dimension> result;
            for (int32 i = 0; i < dimension; ++i)
            {
                result.data[i] = a.data[i] - b.data[i];
            }
            
            return result;
        }
        
        template<typename T, int32 dimension>
        FORCEINLINE TVector<T, dimension> operator*(const TVector<T, dimension>& v, const real k)
        {
            TVector<T, dimension> result;
            for (int32 i = 0; i < dimension; ++i)
            {
                result.data[i] = v.data[i] * k;
            }
            
            return result;
        }
        
        template<typename T, int32 dimension>
        FORCEINLINE TVector<T, dimension> operator*(const real k, const TVector<T, dimension>& v)
        {
            TVector<T, dimension> result;
            for (int32 i = 0; i < dimension; ++i)
            {
                result.data[i] = v.data[i] * k;
            }
            
            return result;
        }
        
        template<typename T, int32 dimension>
        FORCEINLINE TVector<T, dimension> operator*(const TVector<T, dimension>& v1, const TVector<T, dimension>& v2)
        {
            TVector<T, dimension> result;
            for (int32 i = 0; i < dimension; ++i)
            {
                result.data[i] = v1.data[i] * v2.data[i];
            }
            
            return result;
        }
        
        template<typename T, int32 dimension>
        FORCEINLINE TVector<T, dimension> operator-(const TVector<T, dimension>& v)
        {
            TVector<T, dimension> result;
            for (int32 i = 0; i < dimension; ++i)
            {
                result.data[i] = -v.data[i];
            }
            
            return result;
        }
        
        template<typename T, int32 dimension>
        FORCEINLINE TVector<T, dimension>& operator+=(TVector<T, dimension>& v, const real k)
        {
            for (int32 i = 0; i < dimension; ++i)
            {
                v.data[i] += k;
            }
            
            return v;
        }
        
        template<typename T, int32 dimension>
        FORCEINLINE TVector<T, dimension>& operator+=(TVector<T, dimension>& left, const TVector<T, dimension>& right)
        {
            for (int32 i = 0; i < dimension; ++i)
            {
                left.data[i] += right.data[i];
            }
            
            return left;
        }
        
        template<typename T, int32 dimension>
        FORCEINLINE TVector<T, dimension>& operator-=(TVector<T, dimension>& v, const real k)
        {
            for (int32 i = 0; i < dimension; ++i)
            {
                v.data[i] -= k;
            }
            
            return v;
        }
        
        template<typename T, int32 dimension>
        FORCEINLINE TVector<T, dimension>& operator*=(TVector<T, dimension>& v, const real k)
        {
            for (int32 i = 0; i < dimension; ++i)
            {
                v.data[i] *= k;
            }
            
            return v;
        }
        
        template<typename T, int32 dimension>
        FORCEINLINE TVector<T, dimension>& operator*=(TVector<T, dimension>& left, const TVector<T, dimension>& right)
        {
            for (int32 i = 0; i < dimension; ++i)
            {
                left.data[i] *= right.data[i];
            }
            
            return left;
        }
        
        template<typename T, int32 dimension>
        FORCEINLINE real LengthSq(const TVector<T, dimension>& v)
        {
            real result = real(0);
            for (int32 i = 0; i < dimension; ++i)
            {
                result += v.data[i] * v.data[i];
            }
            
            return result;
        }
        
        template<typename T, int32 dimension>
        FORCEINLINE real Length(const TVector<T, dimension>& v)
        {
            return MathUtils::Sqrt(LengthSq(v));
        }
        
        template<typename T, int32 dimension>
        FORCEINLINE real Dot(const TVector<T, dimension>& a, const TVector<T, dimension>& b)
        {
            real result = real(0);
            for (int32 i = 0; i < dimension; ++i)
            {
                result += a.data[i] * b.data[i];
            }
            
            return result;
        }
        
        template<typename T, int32 dimension>
        FORCEINLINE TVector<T, dimension> Normalize(const TVector<T, dimension>& v)
        {
            TVector<T, dimension> result;
            real oneOverLength = real(1) / Length(v);
            for (int32 i = 0; i < dimension; ++i)
            {
                result.data[i] = v.data[i] * oneOverLength;
            }
            
            return result;
        }
        
        
        // Vector template specialization
        #include "VectorSpecialization.inl"
        
        // Relational vector ops
        template <int32 dimension>
        FORCEINLINE bool All(const TVector<bool, dimension>& v)
        {
            for (int32 i = 0; i < dimension; ++i)
            {
                if (v.data[i] == false)
                {
                    return false;
                }
            }
            return true;
        }
        
        template <int32 dimension>
        FORCEINLINE bool Any(const TVector<bool, dimension>& v)
        {
            for (int32 i = 0; i < dimension; ++i)
            {
                if (v.data[i] == true)
                {
                    return true;
                }
            }
            return false;
        }
        
        template<typename T, int32 dimension>
        FORCEINLINE TVector<bool, dimension> Equal(const TVector<T, dimension>& a, const TVector<T, dimension>& b)
        {
            TVector<bool, dimension> result;
            for (int32 i = 0; i < dimension; ++i)
            {
                result.data[i] = MathUtils::Equals(a.data[i], b.data[i]);
            }
            
            return result;
        }
        
        template<typename T, int32 dimension>
        FORCEINLINE TVector<bool, dimension> NotEqual(const TVector<T, dimension>& a, const TVector<T, dimension>& b)
        {
            TVector<bool, dimension> result;
            for (int32 i = 0; i < dimension; ++i)
            {
                result.data[i] = a.data[i] != b.data[i];
            }
            
            return result;
        }
        
        template<typename T, int32 dimension>
        FORCEINLINE TVector<bool, dimension> GreaterThan(const TVector<T, dimension>& a, const TVector<T, dimension>& b)
        {
            TVector<bool, dimension> result;
            for (int32 i = 0; i < dimension; ++i)
            {
                result.data[i] = a.data[i] > b.data[i];
            }
            
            return result;
        }
        
        template<typename T, int32 dimension>
        FORCEINLINE TVector<bool, dimension> GreaterThanEqual(const TVector<T, dimension>& a, const TVector<T, dimension>& b)
        {
            TVector<bool, dimension> result;
            for (int32 i = 0; i < dimension; ++i)
            {
                result.data[i] = a.data[i] >= b.data[i];
            }
            
            return result;
        }
        
        template<typename T, int32 dimension>
        FORCEINLINE TVector<bool, dimension> LessThan(const TVector<T, dimension>& a, const TVector<T, dimension>& b)
        {
            TVector<bool, dimension> result;
            for (int32 i = 0; i < dimension; ++i)
            {
                result.data[i] = a.data[i] < b.data[i];
            }
            
            return result;
        }
        
        template<typename T, int32 dimension>
        FORCEINLINE TVector<bool, dimension> LessThanEqual(const TVector<T, dimension>& a, const TVector<T, dimension>& b)
        {
            TVector<bool, dimension> result;
            for (int32 i = 0; i < dimension; ++i)
            {
                result.data[i] = a.data[i] <= b.data[i];
            }
            
            return result;
        }
        
        template<typename T, int32 dimension>
        FORCEINLINE TVector<bool, dimension> Not(const TVector<T, dimension>& v)
        {
            TVector<bool, dimension> result;
            for (int32 i = 0; i < dimension; ++i)
            {
                result.data[i] = !v.data[i];
            }
            
            return result;
        }

        
        //////////////////////// Matrix math
        template<typename T, int32 cols, int32 rows>
        struct TMatrix
        {
            TVector<T, rows> data[cols];
            
            FORCEINLINE TVector<T, rows>& operator[](int32 col)
            {
                return data[col];
            }
            
            FORCEINLINE const TVector<T, rows>& operator[](int32 col) const
            {
                return data[col];
            }
            
            FORCEINLINE TMatrix<T, cols, rows>& operator=(const TMatrix<T, cols, rows>& m)
            {
                for (int32 i = 0; i < cols; ++i)
                {
                    data[i] = m.data[i];
                }
                return *this;
            }
            
            FORCEINLINE TMatrix<T, cols, rows> operator*(const TMatrix<T, cols, rows>& m) const
            {
                TMatrix<T, cols, rows> out(0);
                
                for (int32 c = 0; c < cols; ++c)
                {
                    for (int32 r = 0; r < rows; ++r)
                    {
                        for (int32 c2 = 0; c2 < cols; ++c2)
                        {
                            out.data[c].data[r] += data[c2].data[r] * m.data[c].data[c2];
                        }
                    }
                }
                return out;
            }
            
            FORCEINLINE TMatrix<T, cols, rows> operator+(const T k) const
            {
                TMatrix<T, cols, rows> out;
                for (int32 i = 0; i < cols; ++i)
                {
                    out.data[i] = data[i] + k;
                }
                return out;
            }
            
            FORCEINLINE TMatrix<T, cols, rows> operator-(const T k) const
            {
                TMatrix<T, cols, rows> out;
                for (int32 i = 0; i < cols; ++i)
                {
                    out.data[i] = data[i] - k;
                }
                return out;
            }
            
            FORCEINLINE TMatrix<T, cols, rows> operator*(const T k) const
            {
                TMatrix<T, cols, rows> out;
                for (int32 i = 0; i < cols; ++i)
                {
                    out.data[i] = data[i] * k;
                }
                return out;
            }
        };
        
        
        
        template<typename T, int32 cols, int32 rows>
        FORCEINLINE TMatrix<T, cols, rows> operator*(const TMatrix<T, cols, rows>& m1, const TMatrix<T, cols, rows>& m2)
        {
            TMatrix<T, cols, rows> out(0);
            
            for (int32 c = 0; c < cols; ++c)
            {
                for (int32 r = 0; r < rows; ++r)
                {
                    for (int32 c2 = 0; c2 < cols; ++c2)
                    {
                        out.data.s1.data[c].data[r] += m1.data.s1.data[c2].data[r] * m2.data.s1.data[c].data[c2];
                    }
                }
            }
            return out;
        }
        
        template<typename T, int32 cols, int32 rows>
        FORCEINLINE TMatrix<T, cols, rows> operator+(const TMatrix<T, cols, rows>& m, const T k)
        {
            TMatrix<T, cols, rows> out;
            for (int32 i = 0; i < cols; ++i)
            {
                out.data[i] = m.data[i] + k;
            }
            return out;
        }
        
        template<typename T, int32 cols, int32 rows>
        FORCEINLINE TMatrix<T, cols, rows> operator+(const TMatrix<T, cols, rows>& m1, const TMatrix<T, cols, rows>& m2)
        {
            TMatrix<T, cols, rows> out;
            for (int32 i = 0; i < cols; ++i)
            {
                out.data[i] = m1.data[i] + m2.data[i];
            }
            return out;
        }
        
        template<typename T, int32 cols, int32 rows>
        FORCEINLINE TMatrix<T, cols, rows> operator-(const TMatrix<T, cols, rows>& m, const T k)
        {
            TMatrix<T, cols, rows> out;
            for (int32 i = 0; i < cols; ++i)
            {
                out.data[i] = m.data[i] - k;
            }
            return out;
        }
        
        template<typename T, int32 cols, int32 rows>
        FORCEINLINE TMatrix<T, cols, rows> operator-(const TMatrix<T, cols, rows>& m1, const TMatrix<T, cols, rows>& m2)
        {
            TMatrix<T, cols, rows> out;
            for (int32 i = 0; i < cols; ++i)
            {
                out.data[i] = m1.data[i] - m2.data[i];
            }
            return out;
        }
        
        template<typename T, int32 cols, int32 rows>
        FORCEINLINE TMatrix<T, cols, rows> operator*(const TMatrix<T, cols, rows>& m, const T k)
        {
            TMatrix<T, cols, rows> out;
            for (int32 i = 0; i < cols; ++i)
            {
                out.data.s1.data[i] = m.data.s1.data[i] * k;
            }
            return out;
        }
        
        template<typename T, int32 cols, int32 rows>
        FORCEINLINE TMatrix<T, cols, rows> operator-(const T k, const TMatrix<T, cols, rows>& m)
        {
            TMatrix<T, cols, rows> out;
            for (int32 i = 0; i < cols; ++i)
            {
                out.data.s1.data[i] = m.data.s1.data[i] * k;
            }
            return out;
        }
        
        template<typename T, int32 cols, int32 rows>
        FORCEINLINE TMatrix<T, rows - 1, cols - 1> Minor(const TMatrix<T, cols, rows>& m, int32 rowToMask, int32 colToMask)
        {
            TMatrix<T, rows - 1, cols - 1> out;
            int32 ii = 0;
            for (int32 i = 0; i < cols; ++i)
            {
                if (i == rowToMask)
                {
                    continue;
                }
                
                int32 jj = 0;
                for (int32 j = 0; j < cols; ++j)
                {
                    if (j == colToMask)
                    {
                        continue;
                    }
                    
                    out.data.s1.data[ii].data[jj] = m.data.s1.data[i].data[j];
                    ++jj;
                }
                ++ii;
            }
            
            return out;
        }
        
        template<typename T, int32 cols, int32 rows>
        FORCEINLINE TMatrix<T, cols, rows> Transpose(const TMatrix<T, cols, rows>& m)
        {
            TMatrix<T, cols, rows> out;
            for (int32 i = 0; i < cols; ++i)
            {
                for (int32 j = 0; j < cols; ++j)
                {
                    out.data.s1.data[j].data[i] = m.data.s1.data[i].data[j];
                }
            }
            
            return out;
        }
        
        
        #include "MatrixSpecialization.inl"
        
        //////////////////////// Quaternion
        struct Quaternion
        {
            TVector<real, 3> v;
            real w;

			Quaternion()
			:	v(TVector<real, 3>(real(0))),
				w(real(1))
			{

			}
            
            Quaternion(real inX, real inY, real inZ, real inW)
            :   v(TVector<real, 3>(inX, inY, inZ)),
                w(inW)
            {
                
            }
            
            Quaternion(const TVector<real, 3>& inV, const real inW)
            :   v(inV),
                w(inW)
            {
                
            }
            
            Quaternion(const Quaternion& q)
            :   v(q.v),
                w(q.w)
            {
                
            }
            
            FORCEINLINE Quaternion& operator=(const Quaternion& q)
            {
                v = q.v;
                w = q.w;
                return *this;
            }
            
            FORCEINLINE Quaternion operator+(const Quaternion& q) const
            {
                return Quaternion(v + q.v, w + q.w);
            }
            
            FORCEINLINE Quaternion operator-(const Quaternion& q) const
            {
                return Quaternion(v - q.v, w - q.w);
            }
            
            FORCEINLINE Quaternion operator*(const Quaternion& q)
            {
                Quaternion qq;
                qq.w = w * q.w - Dot(v, q.v);
                qq.v = q.v * w + v * q.w + Cross(v, q.v);
                
                return qq;
            }
            
            FORCEINLINE Quaternion operator*(const real scale)
            {
                Quaternion q;
                q.v.x = v.x * scale;
                q.v.y = v.y * scale;
                q.v.z = v.z * scale;
                q.w   = w * scale;
                return q;
            }
            
            FORCEINLINE real Pitch() const
            {
                return atan2((2) * (v.y * v.z + w * v.x), w * w - v.x * v.x - v.y * v.y + v.z * v.z);
            }
            
            FORCEINLINE real Yaw() const
            {
                return MathUtils::ArcSin(MathUtils::Clamp<real>(real(-2) * (v.x * v.z - w * v.y), real(-1), real(1)));
            }
            
            FORCEINLINE real Roll() const
            {
                return atan2(real(2) * (v.x * v.y + w * v.z), w * w + v.x * v.x - v.y * v.y - v.z * v.z);
            }
            
            FORCEINLINE real Angle() const
            {
                return MathUtils::ArcCos(w) * real(2);
            }
            
            FORCEINLINE TVector<real, 3> Axis() const
            {
                real temp = real(1) - w * w;
                if(temp <= real(0))
                    return TVector<real, 3>(0, 0, 1);
                
                temp = real(1) / MathUtils::Sqrt(temp);
                return TVector<real, 3>(v.x * temp, v.y * temp, v.z * temp);
            }
            
            friend void Pup(Pupper* p, Quaternion& obj, const VarInfo inVarInfo)
            {
                Pup(p, obj.v, inVarInfo.name + ".v");
                Pup(p, obj.w, inVarInfo.name + ".w");
            }
        };
        
        FORCEINLINE Quaternion operator+(const Quaternion& a, const Quaternion& b)
        {
            return Quaternion(a.v + b.v, a.w + b.w);
        }
        
        FORCEINLINE Quaternion operator-(const Quaternion& a, const Quaternion& b)
        {
            return Quaternion(a.v - b.v, a.w - b.w);
        }
        
        FORCEINLINE Quaternion operator*(const Quaternion& a, const Quaternion& b)
        {
            Quaternion qq;
            qq.w = a.w * b.w - Dot(a.v, b.v);
            qq.v = b.v * a.w + a.v * b.w + Cross(a.v, b.v);
            
            return qq;
        }
        
        FORCEINLINE TVector<real, 3> operator*(const Quaternion& a, const TVector<real, 3>& b)
        {
            TVector<real, 3> const quatVector(a.v.x, a.v.y, a.v.z);
            TVector<real, 3> const uv(Cross(quatVector, b));
            TVector<real, 3> const uuv(Cross(quatVector, uv));
            
            return b + ((uv * a.w) + uuv) * real(2);
//            return real(2) * Dot(a.v, b) * a.v + (a.w * a.w - Dot(a.v, a.v)) * b + real(2) * a.w * Cross(a.v, b);
        }
        
        FORCEINLINE Quaternion operator*(const Quaternion& a, const real scale)
        {
            Quaternion q;
            q.v.x = a.v.x * scale;
            q.v.y = a.v.y * scale;
            q.v.z = a.v.z * scale;
            q.w   = a.w * scale;
            return q;
        }
        
    
        FORCEINLINE Quaternion Conjugate(const Quaternion& q)
        {
            Quaternion qq;
            qq.v = -q.v;
            qq.w = q.w;
            return qq;
        }
        
        FORCEINLINE real LengthSq(const Quaternion& q)
        {
            return q.v.x * q.v.x + q.v.y * q.v.y + q.v.z * q.v.z + q.w * q.w;
        }
        
        FORCEINLINE real Length(const Quaternion& q)
        {
            return MathUtils::Sqrt(LengthSq(q));
        }
        
        FORCEINLINE Quaternion Normalize(const Quaternion& q)
        {
            Quaternion qq = q;
            real oneOverLength = real(1) / Length(q);
            qq.v.x *= oneOverLength;
            qq.v.y *= oneOverLength;
            qq.v.z *= oneOverLength;
            qq.w   *= oneOverLength;
            return qq;
        }
        
        FORCEINLINE real Dot(const Quaternion& a, const Quaternion& b)
        {
            return Dot(a.v, b.v) + a.w * b.w;
        }
        
        FORCEINLINE Quaternion Inverse(const Quaternion& q)
        {
            Quaternion inverse = Conjugate(q);
            real oneOverLengthSq = real(1) / LengthSq(q);
            inverse.v *= oneOverLengthSq;
            inverse.w *= oneOverLengthSq;
            
            return inverse;
        }
        
        FORCEINLINE TVector<real, 3> operator*(const TVector<real, 3>& b, const Quaternion& a)
        {
            return Inverse(a) * b;
        }
        
        FORCEINLINE Quaternion AngleAxis(const real& angle, const TVector<real, 3>& axis)
        {
            Quaternion q;
            real const s = MathUtils::Sin(angle * real(0.5));
            
            q.w = MathUtils::Cos(angle * real(0.5));
            q.v.x = axis.x * s;
            q.v.y = axis.y * s;
            q.v.z = axis.z * s;
            
            return q;
        }

        FORCEINLINE TMatrix<real, 4, 4> QuaternionToRotationMatrix(const Quaternion& q)
        {
            TMatrix<real, 4, 4> m;
            m.data.s1.data[0].x = real(1) - real(2) * q.v.y * q.v.y - real(2) * q.v.z * q.v.z;
            m.data.s1.data[0].y = real(2) * q.v.x * q.v.y - real(2) * q.v.z * q.w;
            m.data.s1.data[0].z = real(2) * q.v.x * q.v.z + real(2) * q.v.y * q.w;
            m.data.s1.data[0].w = real(0);
            
            m.data.s1.data[1].x = real(2) * q.v.x * q.v.y + real(2) * q.v.z * q.w;
            m.data.s1.data[1].y = real(1) - real(2) * q.v.x * q.v.x - real(2) * q.v.z * q.v.z;
            m.data.s1.data[1].z = real(2) * q.v.y * q.v.z - real(2) * q.v.x * q.w;
            m.data.s1.data[1].w = real(0);
            
            m.data.s1.data[2].x = real(2) * q.v.x * q.v.z - real(2) * q.v.y * q.w;
            m.data.s1.data[2].y = real(2) * q.v.y * q.v.z + real(2) * q.v.x * q.w;
            m.data.s1.data[2].z = real(1) - real(2) * q.v.x * q.v.x - real(2) * q.v.y * q.v.y;
            m.data.s1.data[2].w = real(0);
            
            m.data.s1.data[3].x = real(0);
            m.data.s1.data[3].y = real(0);
            m.data.s1.data[3].z = real(0);
            m.data.s1.data[3].w = real(1);
            
            return m;
        }
        
        FORCEINLINE Quaternion EulerToQuaternion(const TVector<real, 3>& euler)
        {
            Quaternion q;
            real c1 = MathUtils::Cos(euler.z * real(0.5));
            real c2 = MathUtils::Cos(euler.y * real(0.5));
            real c3 = MathUtils::Cos(euler.x * real(0.5));
            real s1 = MathUtils::Sin(euler.z * real(0.5));
            real s2 = MathUtils::Sin(euler.y * real(0.5));
            real s3 = MathUtils::Sin(euler.x * real(0.5));
            
            q.v.x = c1*c2*s3 - s1*s2*c3;
            q.v.y = c1*s2*c3 + s1*c2*s3;
            q.v.z = s1*c2*c3 - c1*s2*s3;
            q.w   = c1*c2*c3 + s1*s2*s3;
            
            return q;
        }

        FORCEINLINE Quaternion Slerp(const Quaternion& a, const Quaternion& b, real t)
        {
            real omega = MathUtils::ArcCos(MathUtils::Clamp<real>(Dot(a, b), -real(1), real(1)));
            if (MathUtils::Abs(omega) < real(1e-10))
            {
                omega = real(1e-10);
            }
            real som = MathUtils::Sin(omega);
            real st0 = MathUtils::Sin((real(1) - t) * omega) / som;
            real st1 = MathUtils::Sin(t * omega) / som;
            
            return Quaternion(a.v.x * st0 + b.v.x * st1,
                              a.v.y * st0 + b.v.y * st1,
                              a.v.z * st0 + b.v.z * st1,
                              a.w * st0 + b.w * st1);
        }
        
        template<int32 dimension>
        FORCEINLINE TVector<real, dimension> ExtractTranslation(const TMatrix<real, dimension+1, dimension+1>& m)
        {
            TVector<real, dimension> t;
            for (int32 d = 0; d < dimension; ++d)
            {
                t[d] = m[3][d];
            }
            return t;
        }
        
        template<int32 dimension>
        FORCEINLINE TVector<real, dimension> ExtractScale(const TMatrix<real, dimension+1, dimension+1>& m)
        {
            TVector<real, dimension> s;
            for (int32 d = 0; d < dimension; ++d)
            {
                real length = real(0);
                for (int32 dd = 0; dd < dimension; ++dd)
                {
                    length += m[d][dd] * m[d][dd];
                }
                length = MathUtils::Sqrt(length);
                s[d] = length;
            }
            return s;
        }
        
        template<int32 dimension>
        FORCEINLINE TMatrix<real, dimension, dimension> ExtractRotation(const TMatrix<real, dimension, dimension>& m)
        {
            TMatrix<real, dimension, dimension> r(0);
            TVector<real, dimension - 1> s = ExtractScale<dimension - 1>(m);
            for (int32 d = 0; d < dimension-1; ++d)
            {
                for (int32 dd = 0; dd < dimension-1; ++dd)
                {
                    r[dd][d] = m[dd][d] / s[dd];
                }
            }
            r[3][3] = real(1);
            
            return r;
        }
        
        
        FORCEINLINE TMatrix<real, 4, 4> LookAtRH(const TVector<real, 3>& eye, const TVector<real, 3>& center, const TVector<real, 3>& up)
        {
            TVector<real, 3> const f(Math::Normalize(center - eye));
            TVector<real, 3> const s(Math::Normalize(Math::Cross(f, up)));
            TVector<real, 3> const u(Math::Cross(s, f));
            
            TMatrix<real, 4, 4> result;
            result[0][0] = s.x;
            result[1][0] = s.y;
            result[2][0] = s.z;
            result[0][1] = u.x;
            result[1][1] = u.y;
            result[2][1] = u.z;
            result[0][2] = -f.x;
            result[1][2] = -f.y;
            result[2][2] = -f.z;
            result[3][0] = -Math::Dot(s, eye);
            result[3][1] = -Math::Dot(u, eye);
            result[3][2] = Math::Dot(f, eye);
            return result;
        }
        
        FORCEINLINE TMatrix<real, 4, 4> LookAtLH(const TVector<real, 3>& eye, const TVector<real, 3>& center, const TVector<real, 3>& up)
        {
            TVector<real, 3> const f(Math::Normalize(center - eye));
            TVector<real, 3> const s(Math::Normalize(Math::Cross(f, up)));
            TVector<real, 3> const u(Math::Cross(s, f));
            
            TMatrix<real, 4, 4> result;
            result[0][0] = s.x;
            result[1][0] = s.y;
            result[2][0] = s.z;
            result[0][1] = u.x;
            result[1][1] = u.y;
            result[2][1] = u.z;
            result[0][2] = f.x;
            result[1][2] = f.y;
            result[2][2] = f.z;
            result[3][0] = -Math::Dot(s, eye);
            result[3][1] = -Math::Dot(u, eye);
            result[3][2] = -Math::Dot(f, eye);
            return result;
        }
        
        FORCEINLINE TMatrix<real, 4, 4> LookAt(const TVector<real, 3>& eye, const TVector<real, 3>& center, const TVector<real, 3>& up)
        {
#if MATH_LEFT_HANDED
            return LookAtLH(eye, center, up);
#else
            return LookAtRH(eye, center, up);
#endif
        }
        
        FORCEINLINE TMatrix<real, 4, 4> PerspectiveInfiniteRH(const real fovy, const real aspect, const real nearZ)
        {
            const real range = MathUtils::Tan(fovy / static_cast<real>(2)) * nearZ;
            const real left = -range * aspect;
            const real right = range * aspect;
            const real bottom = -range;
            const real top = range;
            
            TMatrix<real, 4, 4> result(static_cast<real>(0));
            result[0][0] = (static_cast<real>(2) * nearZ) / (right - left);
            result[1][1] = (static_cast<real>(2) * nearZ) / (top - bottom);
            result[2][2] = -static_cast<real>(1);
            result[2][3] = -static_cast<real>(1);
            result[3][2] = -static_cast<real>(2) * nearZ;
            return result;
        }
        
        FORCEINLINE TMatrix<real, 4, 4> PerspectiveInfiniteLH(const real fovy, const real aspect, const real nearZ)
        {
            const real range = MathUtils::Tan(fovy / static_cast<real>(2)) * nearZ;
            const real left = -range * aspect;
            const real right = range * aspect;
            const real bottom = -range;
            const real top = range;
            
            TMatrix<real, 4, 4> result(static_cast<real>(0));
            result[0][0] = (static_cast<real>(2) * nearZ) / (right - left);
            result[1][1] = (static_cast<real>(2) * nearZ) / (top - bottom);
            result[2][2] = static_cast<real>(1);
            result[2][3] = static_cast<real>(1);
            result[3][2] = -static_cast<real>(2) * nearZ;
            return result;
        }
        
        FORCEINLINE TMatrix<real, 4, 4> PerspectiveInfinite(const real fovy, const real aspect, const real nearZ)
        {
#if MATH_LEFT_HANDED
            return PerspectiveInfiniteLH(fovy, aspect, nearZ);
#else
            return PerspectiveInfiniteRH(fovy, aspect, nearZ);
#endif
        }
        
        FORCEINLINE TMatrix<real, 4, 4> PerspectiveFovRH(const real fovy, const real aspect, const real nearZ, const real farZ)
        {
            real const rad = fovy;
            real const h = MathUtils::Cos(static_cast<real>(0.5) * rad) / MathUtils::Sin(static_cast<real>(0.5) * rad);
            real const w = h / aspect;// * height / width; ///todo max(width , Height) / min(width , Height)?
            
            TMatrix<real, 4, 4> result(static_cast<real>(0));
            result[0][0] = w;
            result[1][1] = h;
            result[2][3] = -static_cast<real>(1);
            
            result[2][2] = farZ / (nearZ - farZ);
            result[3][2] = -(farZ * nearZ) / (farZ - nearZ);
            
            return result;
        }
        
        FORCEINLINE TMatrix<real, 4, 4> PerspectiveFovLH(const real fovy, const real aspect, const real nearZ, const real farZ)
        {
            real const rad = fovy;
            real const h = MathUtils::Cos(static_cast<real>(0.5) * rad) / MathUtils::Sin(static_cast<real>(0.5) * rad);
            real const w = h / aspect;// * height / width; ///todo max(width , Height) / min(width , Height)?
            
            TMatrix<real, 4, 4> result(static_cast<real>(0));
            result[0][0] = w;
            result[1][1] = h;
            result[2][3] = static_cast<real>(1);
            
            result[2][2] = farZ / (nearZ - farZ);
            result[3][2] = -(farZ * nearZ) / (farZ - nearZ);
            
            return result;
        }
        
        // This assumes a symmetrical frustum
        FORCEINLINE TMatrix<real, 4, 4> PerspectiveFov(const real fovy, const real aspect, const real nearZ, const real farZ)
        {
#if MATH_LEFT_HANDED
            return PerspectiveFovLH(fovy, aspect, nearZ, farZ);
#else
            return PerspectiveFovRH(fovy, aspect, nearZ, farZ);
#endif
        }
        
        FORCEINLINE TMatrix<real, 4, 4> Orthographic(const real left, const real right, const real top, const real bottom, const real nearZ, const real farZ)
        {
            real zero = real(0);
            return TMatrix<real, 4, 4>(real(2) / (right - left), zero, zero, zero,
                                       zero, real(2) / (top - bottom), zero, zero,
                                       zero, zero, -real(2) / (farZ - nearZ), zero,
                                       -(right + left) / (right - left), -(top + bottom) / (top - bottom), -(farZ + nearZ) / (farZ - nearZ), real(1));
        }
        
    } // namespace Math
    
    
    
    using Vector2 = Math::TVector<real, 2>;
    using Vector3 = Math::TVector<real, 3>;
    using Vector4 = Math::TVector<real, 4>;
    using Vector2i = Math::TVector<int32, 2>;
    using Vector3i = Math::TVector<int32, 3>;
    using Vector4i = Math::TVector<int32, 4>;
    using Vector2ui = Math::TVector<uint32, 2>;
    using Vector3ui = Math::TVector<uint32, 3>;
    using Vector4ui = Math::TVector<uint32, 4>;
    using Vector2b = Math::TVector<bool, 2>;
    using Vector3b = Math::TVector<bool, 3>;
    using Vector4b = Math::TVector<bool, 4>;
    
    using Matrix33 = Math::TMatrix<real, 3, 3>;
    using Matrix44 = Math::TMatrix<real, 4, 4>;
    
    using Quaternion = Math::Quaternion;
    
    
    
    
    
    
    class Line2D
    {
    public:
        Vector2 origin;
        Vector2 dir;
        
        Line2D() : origin(Vector2()), dir(Vector2()) {}
        Line2D(const Vector2& o, const Vector2& d) : origin(o), dir(d) {}
        
        bool Intersects(const Line2D& line)
        {
            real denominator = dir.x * line.dir.y - dir.y * line.dir.x;
            return denominator > (real)(0.0000001f); // if they aren't parallel then they overlap or intersect
        }
        
        bool Intersects(Vector2& point, const Line2D& line)
        {
            real denominator = dir.x * line.dir.y - dir.y * line.dir.x;
            if (denominator < (real)(0.0000001f))
                return false;
            
            Vector2 v = origin - line.origin;
            // if t < 0 the intersection point is behind the line, t > 1 intersection is in front
            real t = line.dir.x * v.y - line.dir.y * v.x;
            
            point = origin + dir * t;
            
            return true;
        }
    };
}
