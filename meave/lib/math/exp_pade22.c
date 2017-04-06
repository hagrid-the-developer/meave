//   0:   8d 42 ff                lea    -0x1(%rdx),%eax
//   3:   85 d2                   test   %edx,%edx
//   5:   74 5f                   je     66 <exp_pade22+0x66>
//   7:   89 c2                   mov    %eax,%edx
//   9:   31 c0                   xor    %eax,%eax
//   b:   c5 d8 57 e4             vxorps %xmm4,%xmm4,%xmm4
//   f:   48 83 c2 01             add    $0x1,%rdx
//  13:   c5 fc 28 1d 00 00 00    vmovaps 0x0(%rip),%ymm3        # 1b <exp_pade22+0x1b>
//  1a:   00
//  1b:   c5 fc 28 2d 00 00 00    vmovaps 0x0(%rip),%ymm5        # 23 <exp_pade22+0x23>
//  22:   00
//  23:   48 c1 e2 05             shl    $0x5,%rdx
//  27:   66 0f 1f 84 00 00 00    nopw   0x0(%rax,%rax,1)
//  2e:   00 00
//  30:   c5 fc 28 04 06          vmovaps (%rsi,%rax,1),%ymm0
//  35:   c5 fc 28 d0             vmovaps %ymm0,%ymm2
//  39:   c5 fc 28 c8             vmovaps %ymm0,%ymm1
//  3d:   c4 e2 65 98 d0          vfmadd132ps %ymm0,%ymm3,%ymm2
//  42:   c4 e2 6d 98 cb          vfmadd132ps %ymm3,%ymm2,%ymm1
//  47:   c4 e2 6d 98 c5          vfmadd132ps %ymm5,%ymm2,%ymm0
//  4c:   c5 fc 53 c0             vrcpps %ymm0,%ymm0
//  50:   c4 e2 5d 98 c8          vfmadd132ps %ymm0,%ymm4,%ymm1
//  55:   c5 fc 29 0c 07          vmovaps %ymm1,(%rdi,%rax,1)
//  5a:   48 83 c0 20             add    $0x20,%rax
//  5e:   48 39 d0                cmp    %rdx,%rax
//  61:   75 cd                   jne    30 <exp_pade22+0x30>
//  63:   c5 f8 77                vzeroupper
//  66:   f3 c3                   repz retq

#include <immintrin.h>

void exp_pade22(__m256 *dst, const __m256 *src, unsigned len) {
	for (; len--; src++, dst++) {
		const __m256 x = *src;

		const __m256 plus6 = _mm256_set1_ps(+6.0);
		const __m256 minus6 = _mm256_set1_ps(-6.0);

		const __m256 a = _mm256_fmadd_ps(x, x, plus6);
		const __m256 a1 = _mm256_fmadd_ps(x, plus6, a);
		const __m256 a2 = _mm256_fmadd_ps(x, minus6, a);
		const __m256 r = _mm256_rcp_ps(a2);
		*dst = _mm256_fmadd_ps(a1, r, _mm256_setzero_ps());
	}
}
