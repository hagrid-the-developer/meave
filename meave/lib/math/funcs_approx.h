#ifndef MEAVE_MATH_FUNCS_APPROX_H_INCLUDED
#	define MEAVE_MATH_FUNCS_APPROX_H_INCLUDED

void exp_pade22(float *dst, const float *src, const unsigned len);
void exp2_taylor(float *dst, const float *src, const unsigned len);


typedef float v8sf __attribute__((vector_size(32)));

v8sf exp2_taylor_one(const v8sf $);

// Functions from avx2_math
v8sf exp256_ps(v8sf);

#endif // MEAVE_MATH_FUNCS_APPROX_H_INCLUDED
