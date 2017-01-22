Useful links:

- [Fastest way to do horizontal on x86](http://stackoverflow.com/questions/6996764/fastest-way-to-do-horizontal-float-vector-sum-on-x86/35270026#35270026)
- [MOVSHDUP](http://x86.renejeschke.de/html/file_module_x86_id_199.html)
- [http://www.felixcloutier.com/x86/MOVSHDUP.html](http://www.felixcloutier.com/x86/MOVSHDUP.html)
- [MOVHLPS](http://www.felixcloutier.com/x86/MOVHLPS.html)
- [MOVLPS](http://www.felixcloutier.com/x86/MOVLPS.html)
  - Cannot be used for reg -> reg or mem -> mem transports.
- [PSHUFLW](http://x86.renejeschke.de/html/file_module_x86_id_256.html)
- [UNPCKLPD](http://www.felixcloutier.com/x86/UNPCKLPD.html)
- [Using inline assembly to speed up Matrix multiplication](http://stackoverflow.com/questions/35145447/using-inline-assembly-to-speed-up-matrix-multiplication)
- [Miscellaneous Intrinsics Using Streaming SIMD Extensions](https://msdn.microsoft.com/en-us/library/dzs626wx(v=vs.71).aspx)
- [_mm256_srli_epi16/32/64](https://software.intel.com/en-us/node/523975)
- [Miscellaneous Intrinsics Using Streaming SIMD Extensions](https://www.cs.drexel.edu/~ma53/mirror/icc_manual/mergedProjects/intref_cls/common/intref_sse_misc.htm)
- [MOVD/MOVQ—Move Doubleword/Move Quadword](http://www.felixcloutier.com/x86/MOVD:MOVQ.html)
- [_mm_movehl_ps](https://msdn.microsoft.com/en-us/library/6h6ddxdw(v=vs.90).aspx)
- [UNPCKLPS—Unpack and Interleave Low Packed Single-Precision Floating-Point Values](http://www.felixcloutier.com/x86/UNPCKLPS.html)
- [implications of using _mm_shuffle_ps on integer vector](http://stackoverflow.com/questions/26983569/implications-of-using-mm-shuffle-ps-on-integer-vector)
- [_mm_blend_epi16](https://msdn.microsoft.com/cs-cz/library/bb531454(v=vs.100).aspx)
- 