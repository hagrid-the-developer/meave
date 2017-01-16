
./test-rothash:     file format elf64-x86-64


Disassembly of section .init:

0000000000400df0 <_init>:
  400df0:	48 83 ec 08          	sub    $0x8,%rsp
  400df4:	48 8b 05 fd 21 20 00 	mov    0x2021fd(%rip),%rax        # 602ff8 <_DYNAMIC+0x1f0>
  400dfb:	48 85 c0             	test   %rax,%rax
  400dfe:	74 05                	je     400e05 <_init+0x15>
  400e00:	e8 3b 00 00 00       	callq  400e40 <__gmon_start__@plt>
  400e05:	48 83 c4 08          	add    $0x8,%rsp
  400e09:	c3                   	retq   

Disassembly of section .plt:

0000000000400e10 <abort@plt-0x10>:
  400e10:	ff 35 f2 21 20 00    	pushq  0x2021f2(%rip)        # 603008 <_GLOBAL_OFFSET_TABLE_+0x8>
  400e16:	ff 25 f4 21 20 00    	jmpq   *0x2021f4(%rip)        # 603010 <_GLOBAL_OFFSET_TABLE_+0x10>
  400e1c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000400e20 <abort@plt>:
  400e20:	ff 25 f2 21 20 00    	jmpq   *0x2021f2(%rip)        # 603018 <_GLOBAL_OFFSET_TABLE_+0x18>
  400e26:	68 00 00 00 00       	pushq  $0x0
  400e2b:	e9 e0 ff ff ff       	jmpq   400e10 <_init+0x20>

0000000000400e30 <clock_gettime@plt>:
  400e30:	ff 25 ea 21 20 00    	jmpq   *0x2021ea(%rip)        # 603020 <_GLOBAL_OFFSET_TABLE_+0x20>
  400e36:	68 01 00 00 00       	pushq  $0x1
  400e3b:	e9 d0 ff ff ff       	jmpq   400e10 <_init+0x20>

0000000000400e40 <__gmon_start__@plt>:
  400e40:	ff 25 e2 21 20 00    	jmpq   *0x2021e2(%rip)        # 603028 <_GLOBAL_OFFSET_TABLE_+0x28>
  400e46:	68 02 00 00 00       	pushq  $0x2
  400e4b:	e9 c0 ff ff ff       	jmpq   400e10 <_init+0x20>

0000000000400e50 <_ZdlPv@plt>:
  400e50:	ff 25 da 21 20 00    	jmpq   *0x2021da(%rip)        # 603030 <_GLOBAL_OFFSET_TABLE_+0x30>
  400e56:	68 03 00 00 00       	pushq  $0x3
  400e5b:	e9 b0 ff ff ff       	jmpq   400e10 <_init+0x20>

0000000000400e60 <__assert_fail@plt>:
  400e60:	ff 25 d2 21 20 00    	jmpq   *0x2021d2(%rip)        # 603038 <_GLOBAL_OFFSET_TABLE_+0x38>
  400e66:	68 04 00 00 00       	pushq  $0x4
  400e6b:	e9 a0 ff ff ff       	jmpq   400e10 <_init+0x20>

0000000000400e70 <_ZNSt8ios_base4InitC1Ev@plt>:
  400e70:	ff 25 ca 21 20 00    	jmpq   *0x2021ca(%rip)        # 603040 <_GLOBAL_OFFSET_TABLE_+0x40>
  400e76:	68 05 00 00 00       	pushq  $0x5
  400e7b:	e9 90 ff ff ff       	jmpq   400e10 <_init+0x20>

0000000000400e80 <__libc_start_main@plt>:
  400e80:	ff 25 c2 21 20 00    	jmpq   *0x2021c2(%rip)        # 603048 <_GLOBAL_OFFSET_TABLE_+0x48>
  400e86:	68 06 00 00 00       	pushq  $0x6
  400e8b:	e9 80 ff ff ff       	jmpq   400e10 <_init+0x20>

0000000000400e90 <__cxa_atexit@plt>:
  400e90:	ff 25 ba 21 20 00    	jmpq   *0x2021ba(%rip)        # 603050 <_GLOBAL_OFFSET_TABLE_+0x50>
  400e96:	68 07 00 00 00       	pushq  $0x7
  400e9b:	e9 70 ff ff ff       	jmpq   400e10 <_init+0x20>

0000000000400ea0 <__vsnprintf_chk@plt>:
  400ea0:	ff 25 b2 21 20 00    	jmpq   *0x2021b2(%rip)        # 603058 <_GLOBAL_OFFSET_TABLE_+0x58>
  400ea6:	68 08 00 00 00       	pushq  $0x8
  400eab:	e9 60 ff ff ff       	jmpq   400e10 <_init+0x20>

0000000000400eb0 <_ZNSt8ios_base4InitD1Ev@plt>:
  400eb0:	ff 25 aa 21 20 00    	jmpq   *0x2021aa(%rip)        # 603060 <_GLOBAL_OFFSET_TABLE_+0x60>
  400eb6:	68 09 00 00 00       	pushq  $0x9
  400ebb:	e9 50 ff ff ff       	jmpq   400e10 <_init+0x20>

0000000000400ec0 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt>:
  400ec0:	ff 25 a2 21 20 00    	jmpq   *0x2021a2(%rip)        # 603068 <_GLOBAL_OFFSET_TABLE_+0x68>
  400ec6:	68 0a 00 00 00       	pushq  $0xa
  400ecb:	e9 40 ff ff ff       	jmpq   400e10 <_init+0x20>

0000000000400ed0 <_ZNSsC1EmcRKSaIcE@plt>:
  400ed0:	ff 25 9a 21 20 00    	jmpq   *0x20219a(%rip)        # 603070 <_GLOBAL_OFFSET_TABLE_+0x70>
  400ed6:	68 0b 00 00 00       	pushq  $0xb
  400edb:	e9 30 ff ff ff       	jmpq   400e10 <_init+0x20>

0000000000400ee0 <_ZNSs4_Rep10_M_destroyERKSaIcE@plt>:
  400ee0:	ff 25 92 21 20 00    	jmpq   *0x202192(%rip)        # 603078 <_GLOBAL_OFFSET_TABLE_+0x78>
  400ee6:	68 0c 00 00 00       	pushq  $0xc
  400eeb:	e9 20 ff ff ff       	jmpq   400e10 <_init+0x20>

0000000000400ef0 <__cxa_call_unexpected@plt>:
  400ef0:	ff 25 8a 21 20 00    	jmpq   *0x20218a(%rip)        # 603080 <_GLOBAL_OFFSET_TABLE_+0x80>
  400ef6:	68 0d 00 00 00       	pushq  $0xd
  400efb:	e9 10 ff ff ff       	jmpq   400e10 <_init+0x20>

0000000000400f00 <strtol@plt>:
  400f00:	ff 25 82 21 20 00    	jmpq   *0x202182(%rip)        # 603088 <_GLOBAL_OFFSET_TABLE_+0x88>
  400f06:	68 0e 00 00 00       	pushq  $0xe
  400f0b:	e9 00 ff ff ff       	jmpq   400e10 <_init+0x20>

0000000000400f10 <__cxa_allocate_exception@plt>:
  400f10:	ff 25 7a 21 20 00    	jmpq   *0x20217a(%rip)        # 603090 <_GLOBAL_OFFSET_TABLE_+0x90>
  400f16:	68 0f 00 00 00       	pushq  $0xf
  400f1b:	e9 f0 fe ff ff       	jmpq   400e10 <_init+0x20>

0000000000400f20 <_ZNSt9exceptionD2Ev@plt>:
  400f20:	ff 25 72 21 20 00    	jmpq   *0x202172(%rip)        # 603098 <_GLOBAL_OFFSET_TABLE_+0x98>
  400f26:	68 10 00 00 00       	pushq  $0x10
  400f2b:	e9 e0 fe ff ff       	jmpq   400e10 <_init+0x20>

0000000000400f30 <munmap@plt>:
  400f30:	ff 25 6a 21 20 00    	jmpq   *0x20216a(%rip)        # 6030a0 <_GLOBAL_OFFSET_TABLE_+0xa0>
  400f36:	68 11 00 00 00       	pushq  $0x11
  400f3b:	e9 d0 fe ff ff       	jmpq   400e10 <_init+0x20>

0000000000400f40 <__cxa_throw@plt>:
  400f40:	ff 25 62 21 20 00    	jmpq   *0x202162(%rip)        # 6030a8 <_GLOBAL_OFFSET_TABLE_+0xa8>
  400f46:	68 12 00 00 00       	pushq  $0x12
  400f4b:	e9 c0 fe ff ff       	jmpq   400e10 <_init+0x20>

0000000000400f50 <_ZNSo9_M_insertImEERSoT_@plt>:
  400f50:	ff 25 5a 21 20 00    	jmpq   *0x20215a(%rip)        # 6030b0 <_GLOBAL_OFFSET_TABLE_+0xb0>
  400f56:	68 13 00 00 00       	pushq  $0x13
  400f5b:	e9 b0 fe ff ff       	jmpq   400e10 <_init+0x20>

0000000000400f60 <_ZNSo9_M_insertIdEERSoT_@plt>:
  400f60:	ff 25 52 21 20 00    	jmpq   *0x202152(%rip)        # 6030b8 <_GLOBAL_OFFSET_TABLE_+0xb8>
  400f66:	68 14 00 00 00       	pushq  $0x14
  400f6b:	e9 a0 fe ff ff       	jmpq   400e10 <_init+0x20>

0000000000400f70 <_ZNSs6resizeEmc@plt>:
  400f70:	ff 25 4a 21 20 00    	jmpq   *0x20214a(%rip)        # 6030c0 <_GLOBAL_OFFSET_TABLE_+0xc0>
  400f76:	68 15 00 00 00       	pushq  $0x15
  400f7b:	e9 90 fe ff ff       	jmpq   400e10 <_init+0x20>

0000000000400f80 <_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_@plt>:
  400f80:	ff 25 42 21 20 00    	jmpq   *0x202142(%rip)        # 6030c8 <_GLOBAL_OFFSET_TABLE_+0xc8>
  400f86:	68 16 00 00 00       	pushq  $0x16
  400f8b:	e9 80 fe ff ff       	jmpq   400e10 <_init+0x20>

0000000000400f90 <rand@plt>:
  400f90:	ff 25 3a 21 20 00    	jmpq   *0x20213a(%rip)        # 6030d0 <_GLOBAL_OFFSET_TABLE_+0xd0>
  400f96:	68 17 00 00 00       	pushq  $0x17
  400f9b:	e9 70 fe ff ff       	jmpq   400e10 <_init+0x20>

0000000000400fa0 <__gxx_personality_v0@plt>:
  400fa0:	ff 25 32 21 20 00    	jmpq   *0x202132(%rip)        # 6030d8 <_GLOBAL_OFFSET_TABLE_+0xd8>
  400fa6:	68 18 00 00 00       	pushq  $0x18
  400fab:	e9 60 fe ff ff       	jmpq   400e10 <_init+0x20>

0000000000400fb0 <_Unwind_Resume@plt>:
  400fb0:	ff 25 2a 21 20 00    	jmpq   *0x20212a(%rip)        # 6030e0 <_GLOBAL_OFFSET_TABLE_+0xe0>
  400fb6:	68 19 00 00 00       	pushq  $0x19
  400fbb:	e9 50 fe ff ff       	jmpq   400e10 <_init+0x20>

0000000000400fc0 <_ZNSs12_M_leak_hardEv@plt>:
  400fc0:	ff 25 22 21 20 00    	jmpq   *0x202122(%rip)        # 6030e8 <_GLOBAL_OFFSET_TABLE_+0xe8>
  400fc6:	68 1a 00 00 00       	pushq  $0x1a
  400fcb:	e9 40 fe ff ff       	jmpq   400e10 <_init+0x20>

0000000000400fd0 <_ZNSs4swapERSs@plt>:
  400fd0:	ff 25 1a 21 20 00    	jmpq   *0x20211a(%rip)        # 6030f0 <_GLOBAL_OFFSET_TABLE_+0xf0>
  400fd6:	68 1b 00 00 00       	pushq  $0x1b
  400fdb:	e9 30 fe ff ff       	jmpq   400e10 <_init+0x20>

0000000000400fe0 <mmap@plt>:
  400fe0:	ff 25 12 21 20 00    	jmpq   *0x202112(%rip)        # 6030f8 <_GLOBAL_OFFSET_TABLE_+0xf8>
  400fe6:	68 1c 00 00 00       	pushq  $0x1c
  400feb:	e9 20 fe ff ff       	jmpq   400e10 <_init+0x20>

Disassembly of section .text:

0000000000400ff0 <main>:
  400ff0:	55                   	push   %rbp
  400ff1:	ba 0a 00 00 00       	mov    $0xa,%edx
  400ff6:	48 89 e5             	mov    %rsp,%rbp
  400ff9:	41 57                	push   %r15
  400ffb:	41 56                	push   %r14
  400ffd:	41 55                	push   %r13
  400fff:	41 54                	push   %r12
  401001:	53                   	push   %rbx
  401002:	48 83 e4 e0          	and    $0xffffffffffffffe0,%rsp
  401006:	48 81 ec b0 00 00 00 	sub    $0xb0,%rsp
  40100d:	48 8b 7e 08          	mov    0x8(%rsi),%rdi
  401011:	31 f6                	xor    %esi,%esi
  401013:	e8 e8 fe ff ff       	callq  400f00 <strtol@plt>
  401018:	45 31 c9             	xor    %r9d,%r9d
  40101b:	31 ff                	xor    %edi,%edi
  40101d:	41 b8 ff ff ff ff    	mov    $0xffffffff,%r8d
  401023:	b9 22 00 00 00       	mov    $0x22,%ecx
  401028:	ba 03 00 00 00       	mov    $0x3,%edx
  40102d:	be 00 2d 31 01       	mov    $0x1312d00,%esi
  401032:	4c 63 e0             	movslq %eax,%r12
  401035:	e8 a6 ff ff ff       	callq  400fe0 <mmap@plt>
  40103a:	48 83 f8 ff          	cmp    $0xffffffffffffffff,%rax
  40103e:	48 89 c3             	mov    %rax,%rbx
  401041:	0f 84 98 05 00 00    	je     4015df <main+0x5ef>
  401047:	45 31 ed             	xor    %r13d,%r13d
  40104a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
  401050:	e8 3b ff ff ff       	callq  400f90 <rand@plt>
  401055:	99                   	cltd   
  401056:	c1 ea 19             	shr    $0x19,%edx
  401059:	01 d0                	add    %edx,%eax
  40105b:	83 e0 7f             	and    $0x7f,%eax
  40105e:	29 d0                	sub    %edx,%eax
  401060:	42 88 04 2b          	mov    %al,(%rbx,%r13,1)
  401064:	49 83 c5 01          	add    $0x1,%r13
  401068:	49 81 fd 00 2d 31 01 	cmp    $0x1312d00,%r13
  40106f:	75 df                	jne    401050 <main+0x60>
  401071:	e8 9a 07 00 00       	callq  401810 <_ZN5meave12_GLOBAL__N_111getrealtimeEv>
  401076:	49 81 fc 00 2d 31 01 	cmp    $0x1312d00,%r12
  40107d:	c5 fb 11 44 24 28    	vmovsd %xmm0,0x28(%rsp)
  401083:	0f 87 c1 01 00 00    	ja     40124a <main+0x25a>
  401089:	49 8d 44 24 e0       	lea    -0x20(%r12),%rax
  40108e:	45 31 ff             	xor    %r15d,%r15d
  401091:	31 ff                	xor    %edi,%edi
  401093:	48 89 c2             	mov    %rax,%rdx
  401096:	4d 89 e6             	mov    %r12,%r14
  401099:	48 c1 ea 05          	shr    $0x5,%rdx
  40109d:	48 89 d1             	mov    %rdx,%rcx
  4010a0:	31 d2                	xor    %edx,%edx
  4010a2:	48 c1 e1 05          	shl    $0x5,%rcx
  4010a6:	48 01 da             	add    %rbx,%rdx
  4010a9:	48 29 c8             	sub    %rcx,%rax
  4010ac:	48 89 44 24 10       	mov    %rax,0x10(%rsp)
  4010b1:	48 8d 41 20          	lea    0x20(%rcx),%rax
  4010b5:	48 89 44 24 08       	mov    %rax,0x8(%rsp)
  4010ba:	31 c0                	xor    %eax,%eax
  4010bc:	f6 c2 03             	test   $0x3,%dl
  4010bf:	75 16                	jne    4010d7 <main+0xe7>
  4010c1:	eb 4b                	jmp    40110e <main+0x11e>
  4010c3:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
  4010c8:	0f b6 34 32          	movzbl (%rdx,%rsi,1),%esi
  4010cc:	48 c1 e1 03          	shl    $0x3,%rcx
  4010d0:	83 c0 01             	add    $0x1,%eax
  4010d3:	d3 e6                	shl    %cl,%esi
  4010d5:	31 f7                	xor    %esi,%edi
  4010d7:	48 89 c1             	mov    %rax,%rcx
  4010da:	89 c6                	mov    %eax,%esi
  4010dc:	83 e1 03             	and    $0x3,%ecx
  4010df:	75 03                	jne    4010e4 <main+0xf4>
  4010e1:	c1 c7 0d             	rol    $0xd,%edi
  4010e4:	49 39 f4             	cmp    %rsi,%r12
  4010e7:	77 df                	ja     4010c8 <main+0xd8>
  4010e9:	4b 8d 04 26          	lea    (%r14,%r12,1),%rax
  4010ed:	41 31 ff             	xor    %edi,%r15d
  4010f0:	48 3d 00 2d 31 01    	cmp    $0x1312d00,%rax
  4010f6:	0f 87 54 01 00 00    	ja     401250 <main+0x260>
  4010fc:	4c 89 f2             	mov    %r14,%rdx
  4010ff:	31 ff                	xor    %edi,%edi
  401101:	49 89 c6             	mov    %rax,%r14
  401104:	48 01 da             	add    %rbx,%rdx
  401107:	31 c0                	xor    %eax,%eax
  401109:	f6 c2 03             	test   $0x3,%dl
  40110c:	75 c9                	jne    4010d7 <main+0xe7>
  40110e:	49 83 fc 1f          	cmp    $0x1f,%r12
  401112:	0f 86 20 01 00 00    	jbe    401238 <main+0x248>
  401118:	48 8b 7c 24 08       	mov    0x8(%rsp),%rdi
  40111d:	4c 89 64 24 18       	mov    %r12,0x18(%rsp)
  401122:	48 89 d0             	mov    %rdx,%rax
  401125:	31 c9                	xor    %ecx,%ecx
  401127:	45 31 c9             	xor    %r9d,%r9d
  40112a:	45 31 db             	xor    %r11d,%r11d
  40112d:	31 f6                	xor    %esi,%esi
  40112f:	45 31 c0             	xor    %r8d,%r8d
  401132:	45 31 d2             	xor    %r10d,%r10d
  401135:	45 31 ed             	xor    %r13d,%r13d
  401138:	48 01 d7             	add    %rdx,%rdi
  40113b:	48 89 7c 24 20       	mov    %rdi,0x20(%rsp)
  401140:	31 ff                	xor    %edi,%edi
  401142:	4c 8b 64 24 20       	mov    0x20(%rsp),%r12
  401147:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
  40114e:	00 00 
  401150:	41 c1 c5 0d          	rol    $0xd,%r13d
  401154:	41 c1 c2 0d          	rol    $0xd,%r10d
  401158:	48 83 c0 20          	add    $0x20,%rax
  40115c:	41 c1 c0 0d          	rol    $0xd,%r8d
  401160:	c1 c6 0d             	rol    $0xd,%esi
  401163:	44 33 68 e0          	xor    -0x20(%rax),%r13d
  401167:	41 c1 c3 0d          	rol    $0xd,%r11d
  40116b:	44 33 50 e4          	xor    -0x1c(%rax),%r10d
  40116f:	41 c1 c1 0d          	rol    $0xd,%r9d
  401173:	44 33 40 e8          	xor    -0x18(%rax),%r8d
  401177:	c1 c7 0d             	rol    $0xd,%edi
  40117a:	33 70 ec             	xor    -0x14(%rax),%esi
  40117d:	c1 c1 0d             	rol    $0xd,%ecx
  401180:	44 33 58 f0          	xor    -0x10(%rax),%r11d
  401184:	44 33 48 f4          	xor    -0xc(%rax),%r9d
  401188:	33 78 f8             	xor    -0x8(%rax),%edi
  40118b:	33 48 fc             	xor    -0x4(%rax),%ecx
  40118e:	4c 39 e0             	cmp    %r12,%rax
  401191:	75 bd                	jne    401150 <main+0x160>
  401193:	4c 8b 64 24 18       	mov    0x18(%rsp),%r12
  401198:	45 31 eb             	xor    %r13d,%r11d
  40119b:	45 31 d1             	xor    %r10d,%r9d
  40119e:	48 8b 44 24 10       	mov    0x10(%rsp),%rax
  4011a3:	44 31 c7             	xor    %r8d,%edi
  4011a6:	31 f1                	xor    %esi,%ecx
  4011a8:	48 83 f8 0f          	cmp    $0xf,%rax
  4011ac:	76 28                	jbe    4011d6 <main+0x1e6>
  4011ae:	41 c1 c3 0d          	rol    $0xd,%r11d
  4011b2:	4c 89 e6             	mov    %r12,%rsi
  4011b5:	41 c1 c1 0d          	rol    $0xd,%r9d
  4011b9:	c1 c7 0d             	rol    $0xd,%edi
  4011bc:	48 29 c6             	sub    %rax,%rsi
  4011bf:	c1 c1 0d             	rol    $0xd,%ecx
  4011c2:	48 01 d6             	add    %rdx,%rsi
  4011c5:	48 83 e8 10          	sub    $0x10,%rax
  4011c9:	44 33 1e             	xor    (%rsi),%r11d
  4011cc:	44 33 4e 04          	xor    0x4(%rsi),%r9d
  4011d0:	33 7e 08             	xor    0x8(%rsi),%edi
  4011d3:	33 4e 0c             	xor    0xc(%rsi),%ecx
  4011d6:	44 31 df             	xor    %r11d,%edi
  4011d9:	44 31 c9             	xor    %r9d,%ecx
  4011dc:	48 83 f8 07          	cmp    $0x7,%rax
  4011e0:	76 18                	jbe    4011fa <main+0x20a>
  4011e2:	c1 c7 0d             	rol    $0xd,%edi
  4011e5:	4c 89 e6             	mov    %r12,%rsi
  4011e8:	c1 c1 0d             	rol    $0xd,%ecx
  4011eb:	48 29 c6             	sub    %rax,%rsi
  4011ee:	48 83 e8 08          	sub    $0x8,%rax
  4011f2:	48 01 d6             	add    %rdx,%rsi
  4011f5:	33 3e                	xor    (%rsi),%edi
  4011f7:	33 4e 04             	xor    0x4(%rsi),%ecx
  4011fa:	31 cf                	xor    %ecx,%edi
  4011fc:	48 83 f8 03          	cmp    $0x3,%rax
  401200:	76 10                	jbe    401212 <main+0x222>
  401202:	4c 89 e1             	mov    %r12,%rcx
  401205:	c1 c7 0d             	rol    $0xd,%edi
  401208:	48 29 c1             	sub    %rax,%rcx
  40120b:	48 83 e8 04          	sub    $0x4,%rax
  40120f:	33 3c 0a             	xor    (%rdx,%rcx,1),%edi
  401212:	48 85 c0             	test   %rax,%rax
  401215:	0f 84 ce fe ff ff    	je     4010e9 <main+0xf9>
  40121b:	b9 c0 1f 40 00       	mov    $0x401fc0,%ecx
  401220:	ba 56 00 00 00       	mov    $0x56,%edx
  401225:	be 18 1e 40 00       	mov    $0x401e18,%esi
  40122a:	bf 88 1e 40 00       	mov    $0x401e88,%edi
  40122f:	e8 2c fc ff ff       	callq  400e60 <__assert_fail@plt>
  401234:	0f 1f 40 00          	nopl   0x0(%rax)
  401238:	4c 89 e0             	mov    %r12,%rax
  40123b:	31 c9                	xor    %ecx,%ecx
  40123d:	31 ff                	xor    %edi,%edi
  40123f:	45 31 c9             	xor    %r9d,%r9d
  401242:	45 31 db             	xor    %r11d,%r11d
  401245:	e9 5e ff ff ff       	jmpq   4011a8 <main+0x1b8>
  40124a:	45 31 ff             	xor    %r15d,%r15d
  40124d:	0f 1f 00             	nopl   (%rax)
  401250:	e8 bb 05 00 00       	callq  401810 <_ZN5meave12_GLOBAL__N_111getrealtimeEv>
  401255:	be 8d 1e 40 00       	mov    $0x401e8d,%esi
  40125a:	bf 20 31 60 00       	mov    $0x603120,%edi
  40125f:	c5 fb 5c 5c 24 28    	vsubsd 0x28(%rsp),%xmm0,%xmm3
  401265:	c4 c1 f9 7e dd       	vmovq  %xmm3,%r13
  40126a:	e8 51 fc ff ff       	callq  400ec0 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt>
  40126f:	48 8b 10             	mov    (%rax),%rdx
  401272:	4c 89 e6             	mov    %r12,%rsi
  401275:	48 89 c7             	mov    %rax,%rdi
  401278:	48 8b 4a e8          	mov    -0x18(%rdx),%rcx
  40127c:	48 c7 44 08 10 08 00 	movq   $0x8,0x10(%rax,%rcx,1)
  401283:	00 00 
  401285:	48 89 c1             	mov    %rax,%rcx
  401288:	48 03 4a e8          	add    -0x18(%rdx),%rcx
  40128c:	8b 51 18             	mov    0x18(%rcx),%edx
  40128f:	80 e2 4f             	and    $0x4f,%dl
  401292:	80 ca 80             	or     $0x80,%dl
  401295:	89 51 18             	mov    %edx,0x18(%rcx)
  401298:	e8 b3 fc ff ff       	callq  400f50 <_ZNSo9_M_insertImEERSoT_@plt>
  40129d:	be 94 1e 40 00       	mov    $0x401e94,%esi
  4012a2:	48 89 c7             	mov    %rax,%rdi
  4012a5:	e8 16 fc ff ff       	callq  400ec0 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt>
  4012aa:	44 89 fe             	mov    %r15d,%esi
  4012ad:	48 89 c7             	mov    %rax,%rdi
  4012b0:	e8 9b fc ff ff       	callq  400f50 <_ZNSo9_M_insertImEERSoT_@plt>
  4012b5:	be 9e 1e 40 00       	mov    $0x401e9e,%esi
  4012ba:	48 89 c7             	mov    %rax,%rdi
  4012bd:	e8 fe fb ff ff       	callq  400ec0 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt>
  4012c2:	c4 c1 f9 6e c5       	vmovq  %r13,%xmm0
  4012c7:	48 89 c7             	mov    %rax,%rdi
  4012ca:	e8 91 fc ff ff       	callq  400f60 <_ZNSo9_M_insertIdEERSoT_@plt>
  4012cf:	be a1 1e 40 00       	mov    $0x401ea1,%esi
  4012d4:	48 89 c7             	mov    %rax,%rdi
  4012d7:	e8 e4 fb ff ff       	callq  400ec0 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt>
  4012dc:	48 89 c7             	mov    %rax,%rdi
  4012df:	e8 9c fc ff ff       	callq  400f80 <_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_@plt>
  4012e4:	e8 27 05 00 00       	callq  401810 <_ZN5meave12_GLOBAL__N_111getrealtimeEv>
  4012e9:	49 81 fc 00 2d 31 01 	cmp    $0x1312d00,%r12
  4012f0:	c5 fb 11 44 24 28    	vmovsd %xmm0,0x28(%rsp)
  4012f6:	0f 87 d3 02 00 00    	ja     4015cf <main+0x5df>
  4012fc:	4d 89 e6             	mov    %r12,%r14
  4012ff:	45 31 ed             	xor    %r13d,%r13d
  401302:	31 c0                	xor    %eax,%eax
  401304:	eb 10                	jmp    401316 <main+0x326>
  401306:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  40130d:	00 00 00 
  401310:	4c 89 f0             	mov    %r14,%rax
  401313:	49 89 d6             	mov    %rdx,%r14
  401316:	48 8d 3c 03          	lea    (%rbx,%rax,1),%rdi
  40131a:	ba 0d 00 00 00       	mov    $0xd,%edx
  40131f:	4c 89 e6             	mov    %r12,%rsi
  401322:	0f 18 17             	prefetcht1 (%rdi)
  401325:	0f 18 94 03 80 00 00 	prefetcht1 0x80(%rbx,%rax,1)
  40132c:	00 
  40132d:	0f 18 94 03 a0 00 00 	prefetcht1 0xa0(%rbx,%rax,1)
  401334:	00 
  401335:	0f 18 94 03 c0 00 00 	prefetcht1 0xc0(%rbx,%rax,1)
  40133c:	00 
  40133d:	e8 ae 08 00 00       	callq  401bf0 <rothash_kernel_avx2>
  401342:	4b 8d 14 26          	lea    (%r14,%r12,1),%rdx
  401346:	41 31 c5             	xor    %eax,%r13d
  401349:	48 81 fa 00 2d 31 01 	cmp    $0x1312d00,%rdx
  401350:	76 be                	jbe    401310 <main+0x320>
  401352:	e8 b9 04 00 00       	callq  401810 <_ZN5meave12_GLOBAL__N_111getrealtimeEv>
  401357:	be 8d 1e 40 00       	mov    $0x401e8d,%esi
  40135c:	bf 20 31 60 00       	mov    $0x603120,%edi
  401361:	c5 fb 5c 64 24 28    	vsubsd 0x28(%rsp),%xmm0,%xmm4
  401367:	c4 c1 f9 7e e6       	vmovq  %xmm4,%r14
  40136c:	e8 4f fb ff ff       	callq  400ec0 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt>
  401371:	48 8b 10             	mov    (%rax),%rdx
  401374:	4c 89 e6             	mov    %r12,%rsi
  401377:	48 89 c7             	mov    %rax,%rdi
  40137a:	48 8b 4a e8          	mov    -0x18(%rdx),%rcx
  40137e:	48 c7 44 08 10 08 00 	movq   $0x8,0x10(%rax,%rcx,1)
  401385:	00 00 
  401387:	48 89 c1             	mov    %rax,%rcx
  40138a:	48 03 4a e8          	add    -0x18(%rdx),%rcx
  40138e:	8b 51 18             	mov    0x18(%rcx),%edx
  401391:	80 e2 4f             	and    $0x4f,%dl
  401394:	80 ca 80             	or     $0x80,%dl
  401397:	89 51 18             	mov    %edx,0x18(%rcx)
  40139a:	e8 b1 fb ff ff       	callq  400f50 <_ZNSo9_M_insertImEERSoT_@plt>
  40139f:	be a9 1e 40 00       	mov    $0x401ea9,%esi
  4013a4:	48 89 c7             	mov    %rax,%rdi
  4013a7:	e8 14 fb ff ff       	callq  400ec0 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt>
  4013ac:	44 89 ee             	mov    %r13d,%esi
  4013af:	48 89 c7             	mov    %rax,%rdi
  4013b2:	e8 99 fb ff ff       	callq  400f50 <_ZNSo9_M_insertImEERSoT_@plt>
  4013b7:	be 9e 1e 40 00       	mov    $0x401e9e,%esi
  4013bc:	48 89 c7             	mov    %rax,%rdi
  4013bf:	e8 fc fa ff ff       	callq  400ec0 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt>
  4013c4:	c4 c1 f9 6e c6       	vmovq  %r14,%xmm0
  4013c9:	48 89 c7             	mov    %rax,%rdi
  4013cc:	e8 8f fb ff ff       	callq  400f60 <_ZNSo9_M_insertIdEERSoT_@plt>
  4013d1:	be a1 1e 40 00       	mov    $0x401ea1,%esi
  4013d6:	48 89 c7             	mov    %rax,%rdi
  4013d9:	e8 e2 fa ff ff       	callq  400ec0 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt>
  4013de:	48 89 c7             	mov    %rax,%rdi
  4013e1:	e8 9a fb ff ff       	callq  400f80 <_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_@plt>
  4013e6:	e8 25 04 00 00       	callq  401810 <_ZN5meave12_GLOBAL__N_111getrealtimeEv>
  4013eb:	49 81 fc 00 2d 31 01 	cmp    $0x1312d00,%r12
  4013f2:	c5 fb 11 44 24 28    	vmovsd %xmm0,0x28(%rsp)
  4013f8:	0f 87 d9 01 00 00    	ja     4015d7 <main+0x5e7>
  4013fe:	4c 89 e6             	mov    %r12,%rsi
  401401:	4d 89 e3             	mov    %r12,%r11
  401404:	4c 89 e2             	mov    %r12,%rdx
  401407:	83 e6 1f             	and    $0x1f,%esi
  40140a:	49 83 fc 1f          	cmp    $0x1f,%r12
  40140e:	c5 e8 57 d2          	vxorps %xmm2,%xmm2,%xmm2
  401412:	49 0f 46 f4          	cmovbe %r12,%rsi
  401416:	41 b8 10 00 00 00    	mov    $0x10,%r8d
  40141c:	45 31 ed             	xor    %r13d,%r13d
  40141f:	45 31 f6             	xor    %r14d,%r14d
  401422:	4c 8d 56 f0          	lea    -0x10(%rsi),%r10
  401426:	49 29 f3             	sub    %rsi,%r11
  401429:	4c 8d 4c 24 70       	lea    0x70(%rsp),%r9
  40142e:	eb 19                	jmp    401449 <main+0x459>
  401430:	41 31 c5             	xor    %eax,%r13d
  401433:	4a 8d 04 22          	lea    (%rdx,%r12,1),%rax
  401437:	48 3d 00 2d 31 01    	cmp    $0x1312d00,%rax
  40143d:	0f 87 c8 00 00 00    	ja     40150b <main+0x51b>
  401443:	49 89 d6             	mov    %rdx,%r14
  401446:	48 89 c2             	mov    %rax,%rdx
  401449:	49 01 de             	add    %rbx,%r14
  40144c:	31 c0                	xor    %eax,%eax
  40144e:	41 f6 c6 03          	test   $0x3,%r14b
  401452:	75 dc                	jne    401430 <main+0x440>
  401454:	49 83 fc 1f          	cmp    $0x1f,%r12
  401458:	c5 fc 29 54 24 30    	vmovaps %ymm2,0x30(%rsp)
  40145e:	0f 87 ca 01 00 00    	ja     40162e <main+0x63e>
  401464:	48 83 fe 0f          	cmp    $0xf,%rsi
  401468:	0f 87 20 02 00 00    	ja     40168e <main+0x69e>
  40146e:	48 89 f0             	mov    %rsi,%rax
  401471:	48 8b 4c 24 38       	mov    0x38(%rsp),%rcx
  401476:	48 31 4c 24 30       	xor    %rcx,0x30(%rsp)
  40147b:	48 83 f8 07          	cmp    $0x7,%rax
  40147f:	76 2e                	jbe    4014af <main+0x4bf>
  401481:	c5 fd 6f 44 24 30    	vmovdqa 0x30(%rsp),%ymm0
  401487:	4c 89 e1             	mov    %r12,%rcx
  40148a:	48 29 c1             	sub    %rax,%rcx
  40148d:	48 83 e8 08          	sub    $0x8,%rax
  401491:	c5 f5 72 d0 13       	vpsrld $0x13,%ymm0,%ymm1
  401496:	c5 fd 72 f0 0d       	vpslld $0xd,%ymm0,%ymm0
  40149b:	c5 fd eb c1          	vpor   %ymm1,%ymm0,%ymm0
  40149f:	c4 c2 7d 59 0c 0e    	vpbroadcastq (%r14,%rcx,1),%ymm1
  4014a5:	c5 fd ef c1          	vpxor  %ymm1,%ymm0,%ymm0
  4014a9:	c5 fd 7f 44 24 30    	vmovdqa %ymm0,0x30(%rsp)
  4014af:	8b 4c 24 34          	mov    0x34(%rsp),%ecx
  4014b3:	31 4c 24 30          	xor    %ecx,0x30(%rsp)
  4014b7:	48 83 f8 03          	cmp    $0x3,%rax
  4014bb:	76 2e                	jbe    4014eb <main+0x4fb>
  4014bd:	c5 fd 6f 44 24 30    	vmovdqa 0x30(%rsp),%ymm0
  4014c3:	4c 89 e1             	mov    %r12,%rcx
  4014c6:	48 29 c1             	sub    %rax,%rcx
  4014c9:	48 83 e8 04          	sub    $0x4,%rax
  4014cd:	c5 f5 72 d0 13       	vpsrld $0x13,%ymm0,%ymm1
  4014d2:	c5 fd 72 f0 0d       	vpslld $0xd,%ymm0,%ymm0
  4014d7:	c5 fd eb c1          	vpor   %ymm1,%ymm0,%ymm0
  4014db:	c4 c2 7d 18 0c 0e    	vbroadcastss (%r14,%rcx,1),%ymm1
  4014e1:	c5 fd ef c1          	vpxor  %ymm1,%ymm0,%ymm0
  4014e5:	c5 fd 7f 44 24 30    	vmovdqa %ymm0,0x30(%rsp)
  4014eb:	48 85 c0             	test   %rax,%rax
  4014ee:	0f 85 1e 01 00 00    	jne    401612 <main+0x622>
  4014f4:	8b 44 24 30          	mov    0x30(%rsp),%eax
  4014f8:	41 31 c5             	xor    %eax,%r13d
  4014fb:	4a 8d 04 22          	lea    (%rdx,%r12,1),%rax
  4014ff:	48 3d 00 2d 31 01    	cmp    $0x1312d00,%rax
  401505:	0f 86 38 ff ff ff    	jbe    401443 <main+0x453>
  40150b:	c5 f8 77             	vzeroupper 
  40150e:	e8 fd 02 00 00       	callq  401810 <_ZN5meave12_GLOBAL__N_111getrealtimeEv>
  401513:	be 8d 1e 40 00       	mov    $0x401e8d,%esi
  401518:	bf 20 31 60 00       	mov    $0x603120,%edi
  40151d:	c5 fb 5c 6c 24 28    	vsubsd 0x28(%rsp),%xmm0,%xmm5
  401523:	c4 c1 f9 7e ee       	vmovq  %xmm5,%r14
  401528:	e8 93 f9 ff ff       	callq  400ec0 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt>
  40152d:	48 8b 10             	mov    (%rax),%rdx
  401530:	4c 89 e6             	mov    %r12,%rsi
  401533:	48 89 c7             	mov    %rax,%rdi
  401536:	48 8b 4a e8          	mov    -0x18(%rdx),%rcx
  40153a:	48 c7 44 08 10 08 00 	movq   $0x8,0x10(%rax,%rcx,1)
  401541:	00 00 
  401543:	48 89 c1             	mov    %rax,%rcx
  401546:	48 03 4a e8          	add    -0x18(%rdx),%rcx
  40154a:	8b 51 18             	mov    0x18(%rcx),%edx
  40154d:	80 e2 4f             	and    $0x4f,%dl
  401550:	80 ca 80             	or     $0x80,%dl
  401553:	89 51 18             	mov    %edx,0x18(%rcx)
  401556:	e8 f5 f9 ff ff       	callq  400f50 <_ZNSo9_M_insertImEERSoT_@plt>
  40155b:	be b7 1e 40 00       	mov    $0x401eb7,%esi
  401560:	48 89 c7             	mov    %rax,%rdi
  401563:	e8 58 f9 ff ff       	callq  400ec0 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt>
  401568:	44 89 ee             	mov    %r13d,%esi
  40156b:	48 89 c7             	mov    %rax,%rdi
  40156e:	e8 dd f9 ff ff       	callq  400f50 <_ZNSo9_M_insertImEERSoT_@plt>
  401573:	be 9e 1e 40 00       	mov    $0x401e9e,%esi
  401578:	48 89 c7             	mov    %rax,%rdi
  40157b:	e8 40 f9 ff ff       	callq  400ec0 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt>
  401580:	c4 c1 f9 6e c6       	vmovq  %r14,%xmm0
  401585:	48 89 c7             	mov    %rax,%rdi
  401588:	e8 d3 f9 ff ff       	callq  400f60 <_ZNSo9_M_insertIdEERSoT_@plt>
  40158d:	be a1 1e 40 00       	mov    $0x401ea1,%esi
  401592:	48 89 c7             	mov    %rax,%rdi
  401595:	e8 26 f9 ff ff       	callq  400ec0 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc@plt>
  40159a:	48 89 c7             	mov    %rax,%rdi
  40159d:	e8 de f9 ff ff       	callq  400f80 <_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_@plt>
  4015a2:	bf 20 31 60 00       	mov    $0x603120,%edi
  4015a7:	e8 d4 f9 ff ff       	callq  400f80 <_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_@plt>
  4015ac:	48 85 db             	test   %rbx,%rbx
  4015af:	74 0d                	je     4015be <main+0x5ce>
  4015b1:	be 00 2d 31 01       	mov    $0x1312d00,%esi
  4015b6:	48 89 df             	mov    %rbx,%rdi
  4015b9:	e8 72 f9 ff ff       	callq  400f30 <munmap@plt>
  4015be:	48 8d 65 d8          	lea    -0x28(%rbp),%rsp
  4015c2:	31 c0                	xor    %eax,%eax
  4015c4:	5b                   	pop    %rbx
  4015c5:	41 5c                	pop    %r12
  4015c7:	41 5d                	pop    %r13
  4015c9:	41 5e                	pop    %r14
  4015cb:	41 5f                	pop    %r15
  4015cd:	5d                   	pop    %rbp
  4015ce:	c3                   	retq   
  4015cf:	45 31 ed             	xor    %r13d,%r13d
  4015d2:	e9 7b fd ff ff       	jmpq   401352 <main+0x362>
  4015d7:	45 31 ed             	xor    %r13d,%r13d
  4015da:	e9 2f ff ff ff       	jmpq   40150e <main+0x51e>
  4015df:	bf 10 00 00 00       	mov    $0x10,%edi
  4015e4:	e8 27 f9 ff ff       	callq  400f10 <__cxa_allocate_exception@plt>
  4015e9:	ba 00 2d 31 01       	mov    $0x1312d00,%edx
  4015ee:	be f0 1d 40 00       	mov    $0x401df0,%esi
  4015f3:	48 89 c7             	mov    %rax,%rdi
  4015f6:	48 89 c3             	mov    %rax,%rbx
  4015f9:	31 c0                	xor    %eax,%eax
  4015fb:	e8 e0 04 00 00       	callq  401ae0 <_ZN5meave5ErrorC1EPKcz>
  401600:	ba 10 1a 40 00       	mov    $0x401a10,%edx
  401605:	be e0 20 40 00       	mov    $0x4020e0,%esi
  40160a:	48 89 df             	mov    %rbx,%rdi
  40160d:	e8 2e f9 ff ff       	callq  400f40 <__cxa_throw@plt>
  401612:	b9 e0 1e 40 00       	mov    $0x401ee0,%ecx
  401617:	ba 60 00 00 00       	mov    $0x60,%edx
  40161c:	be 48 1e 40 00       	mov    $0x401e48,%esi
  401621:	bf 88 1e 40 00       	mov    $0x401e88,%edi
  401626:	c5 f8 77             	vzeroupper 
  401629:	e8 32 f8 ff ff       	callq  400e60 <__assert_fail@plt>
  40162e:	4b 8d 0c 26          	lea    (%r14,%r12,1),%rcx
  401632:	4c 89 f0             	mov    %r14,%rax
  401635:	c5 fd 6f 44 24 30    	vmovdqa 0x30(%rsp),%ymm0
  40163b:	48 83 c0 20          	add    $0x20,%rax
  40163f:	48 89 cf             	mov    %rcx,%rdi
  401642:	c5 f5 72 d0 13       	vpsrld $0x13,%ymm0,%ymm1
  401647:	c5 fd 72 f0 0d       	vpslld $0xd,%ymm0,%ymm0
  40164c:	c5 fd eb c9          	vpor   %ymm1,%ymm0,%ymm1
  401650:	c5 ff f0 40 e0       	vlddqu -0x20(%rax),%ymm0
  401655:	48 29 c7             	sub    %rax,%rdi
  401658:	48 83 ff 1f          	cmp    $0x1f,%rdi
  40165c:	c5 f5 ef c0          	vpxor  %ymm0,%ymm1,%ymm0
  401660:	c5 fd 7f 44 24 30    	vmovdqa %ymm0,0x30(%rsp)
  401666:	0f 86 f8 fd ff ff    	jbe    401464 <main+0x474>
  40166c:	eb c7                	jmp    401635 <main+0x645>
  40166e:	48 85 db             	test   %rbx,%rbx
  401671:	49 89 c4             	mov    %rax,%r12
  401674:	74 6d                	je     4016e3 <main+0x6f3>
  401676:	be 00 2d 31 01       	mov    $0x1312d00,%esi
  40167b:	48 89 df             	mov    %rbx,%rdi
  40167e:	c5 f8 77             	vzeroupper 
  401681:	e8 aa f8 ff ff       	callq  400f30 <munmap@plt>
  401686:	4c 89 e7             	mov    %r12,%rdi
  401689:	e8 22 f9 ff ff       	callq  400fb0 <_Unwind_Resume@plt>
  40168e:	31 c0                	xor    %eax,%eax
  401690:	4c 89 cf             	mov    %r9,%rdi
  401693:	4c 89 c1             	mov    %r8,%rcx
  401696:	f3 ab                	rep stos %eax,%es:(%rdi)
  401698:	c5 fd 6f 44 24 30    	vmovdqa 0x30(%rsp),%ymm0
  40169e:	4c 89 d0             	mov    %r10,%rax
  4016a1:	c5 f5 72 d0 13       	vpsrld $0x13,%ymm0,%ymm1
  4016a6:	c5 fd 72 f0 0d       	vpslld $0xd,%ymm0,%ymm0
  4016ab:	c5 fd eb c1          	vpor   %ymm1,%ymm0,%ymm0
  4016af:	c4 81 7b f0 0c 1e    	vlddqu (%r14,%r11,1),%xmm1
  4016b5:	c5 f9 7f 4c 24 70    	vmovdqa %xmm1,0x70(%rsp)
  4016bb:	c5 fd ef 44 24 70    	vpxor  0x70(%rsp),%ymm0,%ymm0
  4016c1:	c5 fd 7f 44 24 30    	vmovdqa %ymm0,0x30(%rsp)
  4016c7:	e9 a5 fd ff ff       	jmpq   401471 <main+0x481>
  4016cc:	48 83 fa ff          	cmp    $0xffffffffffffffff,%rdx
  4016d0:	48 89 c7             	mov    %rax,%rdi
  4016d3:	74 1b                	je     4016f0 <main+0x700>
  4016d5:	48 83 c2 01          	add    $0x1,%rdx
  4016d9:	74 0d                	je     4016e8 <main+0x6f8>
  4016db:	c5 f8 77             	vzeroupper 
  4016de:	e8 cd f8 ff ff       	callq  400fb0 <_Unwind_Resume@plt>
  4016e3:	c5 f8 77             	vzeroupper 
  4016e6:	eb 9e                	jmp    401686 <main+0x696>
  4016e8:	c5 f8 77             	vzeroupper 
  4016eb:	e8 00 f8 ff ff       	callq  400ef0 <__cxa_call_unexpected@plt>
  4016f0:	c5 f8 77             	vzeroupper 
  4016f3:	e8 f8 f7 ff ff       	callq  400ef0 <__cxa_call_unexpected@plt>
  4016f8:	48 89 c7             	mov    %rax,%rdi
  4016fb:	eb d8                	jmp    4016d5 <main+0x6e5>
  4016fd:	0f 1f 00             	nopl   (%rax)

0000000000401700 <_GLOBAL__sub_I__ZN5meave10str_printfEPKcP13__va_list_tag>:
  401700:	48 83 ec 08          	sub    $0x8,%rsp
  401704:	bf c1 32 60 00       	mov    $0x6032c1,%edi
  401709:	e8 62 f7 ff ff       	callq  400e70 <_ZNSt8ios_base4InitC1Ev@plt>
  40170e:	ba 08 31 60 00       	mov    $0x603108,%edx
  401713:	be c1 32 60 00       	mov    $0x6032c1,%esi
  401718:	bf b0 0e 40 00       	mov    $0x400eb0,%edi
  40171d:	48 83 c4 08          	add    $0x8,%rsp
  401721:	e9 6a f7 ff ff       	jmpq   400e90 <__cxa_atexit@plt>

0000000000401726 <_start>:
  401726:	31 ed                	xor    %ebp,%ebp
  401728:	49 89 d1             	mov    %rdx,%r9
  40172b:	5e                   	pop    %rsi
  40172c:	48 89 e2             	mov    %rsp,%rdx
  40172f:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
  401733:	50                   	push   %rax
  401734:	54                   	push   %rsp
  401735:	49 c7 c0 b0 1d 40 00 	mov    $0x401db0,%r8
  40173c:	48 c7 c1 40 1d 40 00 	mov    $0x401d40,%rcx
  401743:	48 c7 c7 f0 0f 40 00 	mov    $0x400ff0,%rdi
  40174a:	e8 31 f7 ff ff       	callq  400e80 <__libc_start_main@plt>
  40174f:	f4                   	hlt    

0000000000401750 <deregister_tm_clones>:
  401750:	b8 17 31 60 00       	mov    $0x603117,%eax
  401755:	55                   	push   %rbp
  401756:	48 2d 10 31 60 00    	sub    $0x603110,%rax
  40175c:	48 83 f8 0e          	cmp    $0xe,%rax
  401760:	48 89 e5             	mov    %rsp,%rbp
  401763:	77 02                	ja     401767 <deregister_tm_clones+0x17>
  401765:	5d                   	pop    %rbp
  401766:	c3                   	retq   
  401767:	b8 00 00 00 00       	mov    $0x0,%eax
  40176c:	48 85 c0             	test   %rax,%rax
  40176f:	74 f4                	je     401765 <deregister_tm_clones+0x15>
  401771:	5d                   	pop    %rbp
  401772:	bf 10 31 60 00       	mov    $0x603110,%edi
  401777:	ff e0                	jmpq   *%rax
  401779:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000401780 <register_tm_clones>:
  401780:	b8 10 31 60 00       	mov    $0x603110,%eax
  401785:	55                   	push   %rbp
  401786:	48 2d 10 31 60 00    	sub    $0x603110,%rax
  40178c:	48 c1 f8 03          	sar    $0x3,%rax
  401790:	48 89 e5             	mov    %rsp,%rbp
  401793:	48 89 c2             	mov    %rax,%rdx
  401796:	48 c1 ea 3f          	shr    $0x3f,%rdx
  40179a:	48 01 d0             	add    %rdx,%rax
  40179d:	48 d1 f8             	sar    %rax
  4017a0:	75 02                	jne    4017a4 <register_tm_clones+0x24>
  4017a2:	5d                   	pop    %rbp
  4017a3:	c3                   	retq   
  4017a4:	ba 00 00 00 00       	mov    $0x0,%edx
  4017a9:	48 85 d2             	test   %rdx,%rdx
  4017ac:	74 f4                	je     4017a2 <register_tm_clones+0x22>
  4017ae:	5d                   	pop    %rbp
  4017af:	48 89 c6             	mov    %rax,%rsi
  4017b2:	bf 10 31 60 00       	mov    $0x603110,%edi
  4017b7:	ff e2                	jmpq   *%rdx
  4017b9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

00000000004017c0 <__do_global_dtors_aux>:
  4017c0:	80 3d f9 1a 20 00 00 	cmpb   $0x0,0x201af9(%rip)        # 6032c0 <completed.6973>
  4017c7:	75 11                	jne    4017da <__do_global_dtors_aux+0x1a>
  4017c9:	55                   	push   %rbp
  4017ca:	48 89 e5             	mov    %rsp,%rbp
  4017cd:	e8 7e ff ff ff       	callq  401750 <deregister_tm_clones>
  4017d2:	5d                   	pop    %rbp
  4017d3:	c6 05 e6 1a 20 00 01 	movb   $0x1,0x201ae6(%rip)        # 6032c0 <completed.6973>
  4017da:	f3 c3                	repz retq 
  4017dc:	0f 1f 40 00          	nopl   0x0(%rax)

00000000004017e0 <frame_dummy>:
  4017e0:	48 83 3d 18 16 20 00 	cmpq   $0x0,0x201618(%rip)        # 602e00 <__JCR_END__>
  4017e7:	00 
  4017e8:	74 1e                	je     401808 <frame_dummy+0x28>
  4017ea:	b8 00 00 00 00       	mov    $0x0,%eax
  4017ef:	48 85 c0             	test   %rax,%rax
  4017f2:	74 14                	je     401808 <frame_dummy+0x28>
  4017f4:	55                   	push   %rbp
  4017f5:	bf 00 2e 60 00       	mov    $0x602e00,%edi
  4017fa:	48 89 e5             	mov    %rsp,%rbp
  4017fd:	ff d0                	callq  *%rax
  4017ff:	5d                   	pop    %rbp
  401800:	e9 7b ff ff ff       	jmpq   401780 <register_tm_clones>
  401805:	0f 1f 00             	nopl   (%rax)
  401808:	e9 73 ff ff ff       	jmpq   401780 <register_tm_clones>
  40180d:	0f 1f 00             	nopl   (%rax)

0000000000401810 <_ZN5meave12_GLOBAL__N_111getrealtimeEv>:
  401810:	55                   	push   %rbp
  401811:	31 ff                	xor    %edi,%edi
  401813:	48 89 e5             	mov    %rsp,%rbp
  401816:	48 83 e4 e0          	and    $0xffffffffffffffe0,%rsp
  40181a:	48 83 ec 20          	sub    $0x20,%rsp
  40181e:	48 89 e6             	mov    %rsp,%rsi
  401821:	e8 0a f6 ff ff       	callq  400e30 <clock_gettime@plt>
  401826:	83 f8 ff             	cmp    $0xffffffff,%eax
  401829:	74 1b                	je     401846 <_ZN5meave12_GLOBAL__N_111getrealtimeEv+0x36>
  40182b:	c4 e1 f3 2a 4c 24 08 	vcvtsi2sdq 0x8(%rsp),%xmm1,%xmm1
  401832:	c4 e1 fb 2a 04 24    	vcvtsi2sdq (%rsp),%xmm0,%xmm0
  401838:	c5 f3 59 0d 08 09 00 	vmulsd 0x908(%rip),%xmm1,%xmm1        # 402148 <_ZTVN5meave5ErrorE+0x28>
  40183f:	00 
  401840:	c9                   	leaveq 
  401841:	c5 f3 58 c0          	vaddsd %xmm0,%xmm1,%xmm0
  401845:	c3                   	retq   
  401846:	e8 d5 f5 ff ff       	callq  400e20 <abort@plt>
  40184b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000401850 <_ZN5meave10str_printfEPKcP13__va_list_tag>:
  401850:	41 56                	push   %r14
  401852:	49 89 f0             	mov    %rsi,%r8
  401855:	48 c7 c1 ff ff ff ff 	mov    $0xffffffffffffffff,%rcx
  40185c:	41 55                	push   %r13
  40185e:	49 89 f6             	mov    %rsi,%r14
  401861:	31 f6                	xor    %esi,%esi
  401863:	41 54                	push   %r12
  401865:	55                   	push   %rbp
  401866:	48 89 d5             	mov    %rdx,%rbp
  401869:	53                   	push   %rbx
  40186a:	48 89 fb             	mov    %rdi,%rbx
  40186d:	31 ff                	xor    %edi,%edi
  40186f:	48 83 ec 20          	sub    $0x20,%rsp
  401873:	48 8b 02             	mov    (%rdx),%rax
  401876:	4c 8d 4c 24 08       	lea    0x8(%rsp),%r9
  40187b:	48 89 44 24 08       	mov    %rax,0x8(%rsp)
  401880:	48 8b 42 08          	mov    0x8(%rdx),%rax
  401884:	48 89 44 24 10       	mov    %rax,0x10(%rsp)
  401889:	48 8b 42 10          	mov    0x10(%rdx),%rax
  40188d:	ba 01 00 00 00       	mov    $0x1,%edx
  401892:	48 89 44 24 18       	mov    %rax,0x18(%rsp)
  401897:	e8 04 f6 ff ff       	callq  400ea0 <__vsnprintf_chk@plt>
  40189c:	ba 7e 00 00 00       	mov    $0x7e,%edx
  4018a1:	48 89 df             	mov    %rbx,%rdi
  4018a4:	44 8d 60 01          	lea    0x1(%rax),%r12d
  4018a8:	41 89 c5             	mov    %eax,%r13d
  4018ab:	48 8d 4c 24 07       	lea    0x7(%rsp),%rcx
  4018b0:	4d 63 e4             	movslq %r12d,%r12
  4018b3:	4c 89 e6             	mov    %r12,%rsi
  4018b6:	e8 15 f6 ff ff       	callq  400ed0 <_ZNSsC1EmcRKSaIcE@plt>
  4018bb:	48 8b 3b             	mov    (%rbx),%rdi
  4018be:	8b 57 f8             	mov    -0x8(%rdi),%edx
  4018c1:	85 d2                	test   %edx,%edx
  4018c3:	78 0b                	js     4018d0 <_ZN5meave10str_printfEPKcP13__va_list_tag+0x80>
  4018c5:	48 89 df             	mov    %rbx,%rdi
  4018c8:	e8 f3 f6 ff ff       	callq  400fc0 <_ZNSs12_M_leak_hardEv@plt>
  4018cd:	48 8b 3b             	mov    (%rbx),%rdi
  4018d0:	49 89 e9             	mov    %rbp,%r9
  4018d3:	ba 01 00 00 00       	mov    $0x1,%edx
  4018d8:	4d 89 f0             	mov    %r14,%r8
  4018db:	48 c7 c1 ff ff ff ff 	mov    $0xffffffffffffffff,%rcx
  4018e2:	4c 89 e6             	mov    %r12,%rsi
  4018e5:	e8 b6 f5 ff ff       	callq  400ea0 <__vsnprintf_chk@plt>
  4018ea:	48 8b 13             	mov    (%rbx),%rdx
  4018ed:	48 8b 42 e8          	mov    -0x18(%rdx),%rax
  4018f1:	48 8d 68 ff          	lea    -0x1(%rax),%rbp
  4018f5:	8b 42 f8             	mov    -0x8(%rdx),%eax
  4018f8:	85 c0                	test   %eax,%eax
  4018fa:	78 0b                	js     401907 <_ZN5meave10str_printfEPKcP13__va_list_tag+0xb7>
  4018fc:	48 89 df             	mov    %rbx,%rdi
  4018ff:	e8 bc f6 ff ff       	callq  400fc0 <_ZNSs12_M_leak_hardEv@plt>
  401904:	48 8b 13             	mov    (%rbx),%rdx
  401907:	80 3c 2a 00          	cmpb   $0x0,(%rdx,%rbp,1)
  40190b:	75 1d                	jne    40192a <_ZN5meave10str_printfEPKcP13__va_list_tag+0xda>
  40190d:	49 63 f5             	movslq %r13d,%rsi
  401910:	48 89 df             	mov    %rbx,%rdi
  401913:	31 d2                	xor    %edx,%edx
  401915:	e8 56 f6 ff ff       	callq  400f70 <_ZNSs6resizeEmc@plt>
  40191a:	48 83 c4 20          	add    $0x20,%rsp
  40191e:	48 89 d8             	mov    %rbx,%rax
  401921:	5b                   	pop    %rbx
  401922:	5d                   	pop    %rbp
  401923:	41 5c                	pop    %r12
  401925:	41 5d                	pop    %r13
  401927:	41 5e                	pop    %r14
  401929:	c3                   	retq   
  40192a:	b9 a0 20 40 00       	mov    $0x4020a0,%ecx
  40192f:	ba 15 00 00 00       	mov    $0x15,%edx
  401934:	be c8 1d 40 00       	mov    $0x401dc8,%esi
  401939:	bf 78 1e 40 00       	mov    $0x401e78,%edi
  40193e:	e8 1d f5 ff ff       	callq  400e60 <__assert_fail@plt>
  401943:	66 66 66 66 2e 0f 1f 	data32 data32 data32 nopw %cs:0x0(%rax,%rax,1)
  40194a:	84 00 00 00 00 00 

0000000000401950 <_ZN5meave10str_printfEPKcz>:
  401950:	53                   	push   %rbx
  401951:	48 89 fb             	mov    %rdi,%rbx
  401954:	48 81 ec d0 00 00 00 	sub    $0xd0,%rsp
  40195b:	84 c0                	test   %al,%al
  40195d:	48 89 54 24 30       	mov    %rdx,0x30(%rsp)
  401962:	48 89 4c 24 38       	mov    %rcx,0x38(%rsp)
  401967:	4c 89 44 24 40       	mov    %r8,0x40(%rsp)
  40196c:	4c 89 4c 24 48       	mov    %r9,0x48(%rsp)
  401971:	74 3f                	je     4019b2 <_ZN5meave10str_printfEPKcz+0x62>
  401973:	c5 f8 29 44 24 50    	vmovaps %xmm0,0x50(%rsp)
  401979:	c5 f8 29 4c 24 60    	vmovaps %xmm1,0x60(%rsp)
  40197f:	c5 f8 29 54 24 70    	vmovaps %xmm2,0x70(%rsp)
  401985:	c5 f8 29 9c 24 80 00 	vmovaps %xmm3,0x80(%rsp)
  40198c:	00 00 
  40198e:	c5 f8 29 a4 24 90 00 	vmovaps %xmm4,0x90(%rsp)
  401995:	00 00 
  401997:	c5 f8 29 ac 24 a0 00 	vmovaps %xmm5,0xa0(%rsp)
  40199e:	00 00 
  4019a0:	c5 f8 29 b4 24 b0 00 	vmovaps %xmm6,0xb0(%rsp)
  4019a7:	00 00 
  4019a9:	c5 f8 29 bc 24 c0 00 	vmovaps %xmm7,0xc0(%rsp)
  4019b0:	00 00 
  4019b2:	48 8d 54 24 08       	lea    0x8(%rsp),%rdx
  4019b7:	48 89 df             	mov    %rbx,%rdi
  4019ba:	c7 44 24 08 10 00 00 	movl   $0x10,0x8(%rsp)
  4019c1:	00 
  4019c2:	48 8d 84 24 e0 00 00 	lea    0xe0(%rsp),%rax
  4019c9:	00 
  4019ca:	c7 44 24 0c 30 00 00 	movl   $0x30,0xc(%rsp)
  4019d1:	00 
  4019d2:	48 89 44 24 10       	mov    %rax,0x10(%rsp)
  4019d7:	48 8d 44 24 20       	lea    0x20(%rsp),%rax
  4019dc:	48 89 44 24 18       	mov    %rax,0x18(%rsp)
  4019e1:	e8 6a fe ff ff       	callq  401850 <_ZN5meave10str_printfEPKcP13__va_list_tag>
  4019e6:	48 81 c4 d0 00 00 00 	add    $0xd0,%rsp
  4019ed:	48 89 d8             	mov    %rbx,%rax
  4019f0:	5b                   	pop    %rbx
  4019f1:	c3                   	retq   
  4019f2:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  4019f9:	00 00 00 
  4019fc:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000401a00 <_ZNK5meave5Error4whatEv>:
  401a00:	48 8b 47 08          	mov    0x8(%rdi),%rax
  401a04:	c3                   	retq   
  401a05:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  401a0c:	00 00 00 
  401a0f:	90                   	nop

0000000000401a10 <_ZN5meave5ErrorD1Ev>:
  401a10:	53                   	push   %rbx
  401a11:	48 89 fb             	mov    %rdi,%rbx
  401a14:	48 83 ec 10          	sub    $0x10,%rsp
  401a18:	48 c7 07 30 21 40 00 	movq   $0x402130,(%rdi)
  401a1f:	48 8b 47 08          	mov    0x8(%rdi),%rax
  401a23:	48 8d 78 e8          	lea    -0x18(%rax),%rdi
  401a27:	48 81 ff a0 32 60 00 	cmp    $0x6032a0,%rdi
  401a2e:	75 0e                	jne    401a3e <_ZN5meave5ErrorD1Ev+0x2e>
  401a30:	48 89 df             	mov    %rbx,%rdi
  401a33:	e8 e8 f4 ff ff       	callq  400f20 <_ZNSt9exceptionD2Ev@plt>
  401a38:	48 83 c4 10          	add    $0x10,%rsp
  401a3c:	5b                   	pop    %rbx
  401a3d:	c3                   	retq   
  401a3e:	48 8d 57 10          	lea    0x10(%rdi),%rdx
  401a42:	b9 00 00 00 00       	mov    $0x0,%ecx
  401a47:	48 85 c9             	test   %rcx,%rcx
  401a4a:	74 17                	je     401a63 <_ZN5meave5ErrorD1Ev+0x53>
  401a4c:	83 c8 ff             	or     $0xffffffff,%eax
  401a4f:	f0 0f c1 02          	lock xadd %eax,(%rdx)
  401a53:	85 c0                	test   %eax,%eax
  401a55:	7f d9                	jg     401a30 <_ZN5meave5ErrorD1Ev+0x20>
  401a57:	48 8d 74 24 0f       	lea    0xf(%rsp),%rsi
  401a5c:	e8 7f f4 ff ff       	callq  400ee0 <_ZNSs4_Rep10_M_destroyERKSaIcE@plt>
  401a61:	eb cd                	jmp    401a30 <_ZN5meave5ErrorD1Ev+0x20>
  401a63:	8b 50 f8             	mov    -0x8(%rax),%edx
  401a66:	8d 4a ff             	lea    -0x1(%rdx),%ecx
  401a69:	89 48 f8             	mov    %ecx,-0x8(%rax)
  401a6c:	89 d0                	mov    %edx,%eax
  401a6e:	eb e3                	jmp    401a53 <_ZN5meave5ErrorD1Ev+0x43>

0000000000401a70 <_ZN5meave5ErrorD0Ev>:
  401a70:	53                   	push   %rbx
  401a71:	48 89 fb             	mov    %rdi,%rbx
  401a74:	48 83 ec 10          	sub    $0x10,%rsp
  401a78:	48 c7 07 30 21 40 00 	movq   $0x402130,(%rdi)
  401a7f:	48 8b 47 08          	mov    0x8(%rdi),%rax
  401a83:	48 8d 78 e8          	lea    -0x18(%rax),%rdi
  401a87:	48 81 ff a0 32 60 00 	cmp    $0x6032a0,%rdi
  401a8e:	75 16                	jne    401aa6 <_ZN5meave5ErrorD0Ev+0x36>
  401a90:	48 89 df             	mov    %rbx,%rdi
  401a93:	e8 88 f4 ff ff       	callq  400f20 <_ZNSt9exceptionD2Ev@plt>
  401a98:	48 89 df             	mov    %rbx,%rdi
  401a9b:	e8 b0 f3 ff ff       	callq  400e50 <_ZdlPv@plt>
  401aa0:	48 83 c4 10          	add    $0x10,%rsp
  401aa4:	5b                   	pop    %rbx
  401aa5:	c3                   	retq   
  401aa6:	48 8d 57 10          	lea    0x10(%rdi),%rdx
  401aaa:	b9 00 00 00 00       	mov    $0x0,%ecx
  401aaf:	48 85 c9             	test   %rcx,%rcx
  401ab2:	74 17                	je     401acb <_ZN5meave5ErrorD0Ev+0x5b>
  401ab4:	83 c8 ff             	or     $0xffffffff,%eax
  401ab7:	f0 0f c1 02          	lock xadd %eax,(%rdx)
  401abb:	85 c0                	test   %eax,%eax
  401abd:	7f d1                	jg     401a90 <_ZN5meave5ErrorD0Ev+0x20>
  401abf:	48 8d 74 24 0f       	lea    0xf(%rsp),%rsi
  401ac4:	e8 17 f4 ff ff       	callq  400ee0 <_ZNSs4_Rep10_M_destroyERKSaIcE@plt>
  401ac9:	eb c5                	jmp    401a90 <_ZN5meave5ErrorD0Ev+0x20>
  401acb:	8b 50 f8             	mov    -0x8(%rax),%edx
  401ace:	8d 4a ff             	lea    -0x1(%rdx),%ecx
  401ad1:	89 48 f8             	mov    %ecx,-0x8(%rax)
  401ad4:	89 d0                	mov    %edx,%eax
  401ad6:	eb e3                	jmp    401abb <_ZN5meave5ErrorD0Ev+0x4b>
  401ad8:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
  401adf:	00 

0000000000401ae0 <_ZN5meave5ErrorC1EPKcz>:
  401ae0:	53                   	push   %rbx
  401ae1:	48 89 fb             	mov    %rdi,%rbx
  401ae4:	48 81 ec e0 00 00 00 	sub    $0xe0,%rsp
  401aeb:	84 c0                	test   %al,%al
  401aed:	48 89 54 24 40       	mov    %rdx,0x40(%rsp)
  401af2:	48 89 4c 24 48       	mov    %rcx,0x48(%rsp)
  401af7:	4c 89 44 24 50       	mov    %r8,0x50(%rsp)
  401afc:	4c 89 4c 24 58       	mov    %r9,0x58(%rsp)
  401b01:	74 42                	je     401b45 <_ZN5meave5ErrorC1EPKcz+0x65>
  401b03:	c5 f8 29 44 24 60    	vmovaps %xmm0,0x60(%rsp)
  401b09:	c5 f8 29 4c 24 70    	vmovaps %xmm1,0x70(%rsp)
  401b0f:	c5 f8 29 94 24 80 00 	vmovaps %xmm2,0x80(%rsp)
  401b16:	00 00 
  401b18:	c5 f8 29 9c 24 90 00 	vmovaps %xmm3,0x90(%rsp)
  401b1f:	00 00 
  401b21:	c5 f8 29 a4 24 a0 00 	vmovaps %xmm4,0xa0(%rsp)
  401b28:	00 00 
  401b2a:	c5 f8 29 ac 24 b0 00 	vmovaps %xmm5,0xb0(%rsp)
  401b31:	00 00 
  401b33:	c5 f8 29 b4 24 c0 00 	vmovaps %xmm6,0xc0(%rsp)
  401b3a:	00 00 
  401b3c:	c5 f8 29 bc 24 d0 00 	vmovaps %xmm7,0xd0(%rsp)
  401b43:	00 00 
  401b45:	48 8d 84 24 f0 00 00 	lea    0xf0(%rsp),%rax
  401b4c:	00 
  401b4d:	48 c7 03 30 21 40 00 	movq   $0x402130,(%rbx)
  401b54:	48 89 44 24 20       	mov    %rax,0x20(%rsp)
  401b59:	48 8d 54 24 18       	lea    0x18(%rsp),%rdx
  401b5e:	48 8d 44 24 30       	lea    0x30(%rsp),%rax
  401b63:	48 c7 43 08 b8 32 60 	movq   $0x6032b8,0x8(%rbx)
  401b6a:	00 
  401b6b:	48 8d 7c 24 10       	lea    0x10(%rsp),%rdi
  401b70:	48 89 44 24 28       	mov    %rax,0x28(%rsp)
  401b75:	c7 44 24 18 10 00 00 	movl   $0x10,0x18(%rsp)
  401b7c:	00 
  401b7d:	c7 44 24 1c 30 00 00 	movl   $0x30,0x1c(%rsp)
  401b84:	00 
  401b85:	e8 c6 fc ff ff       	callq  401850 <_ZN5meave10str_printfEPKcP13__va_list_tag>
  401b8a:	48 8d 7b 08          	lea    0x8(%rbx),%rdi
  401b8e:	48 8d 74 24 10       	lea    0x10(%rsp),%rsi
  401b93:	e8 38 f4 ff ff       	callq  400fd0 <_ZNSs4swapERSs@plt>
  401b98:	48 8b 44 24 10       	mov    0x10(%rsp),%rax
  401b9d:	48 8d 78 e8          	lea    -0x18(%rax),%rdi
  401ba1:	48 81 ff a0 32 60 00 	cmp    $0x6032a0,%rdi
  401ba8:	75 09                	jne    401bb3 <_ZN5meave5ErrorC1EPKcz+0xd3>
  401baa:	48 81 c4 e0 00 00 00 	add    $0xe0,%rsp
  401bb1:	5b                   	pop    %rbx
  401bb2:	c3                   	retq   
  401bb3:	48 8d 57 10          	lea    0x10(%rdi),%rdx
  401bb7:	b9 00 00 00 00       	mov    $0x0,%ecx
  401bbc:	48 85 c9             	test   %rcx,%rcx
  401bbf:	74 17                	je     401bd8 <_ZN5meave5ErrorC1EPKcz+0xf8>
  401bc1:	83 c8 ff             	or     $0xffffffff,%eax
  401bc4:	f0 0f c1 02          	lock xadd %eax,(%rdx)
  401bc8:	85 c0                	test   %eax,%eax
  401bca:	7f de                	jg     401baa <_ZN5meave5ErrorC1EPKcz+0xca>
  401bcc:	48 8d 74 24 0f       	lea    0xf(%rsp),%rsi
  401bd1:	e8 0a f3 ff ff       	callq  400ee0 <_ZNSs4_Rep10_M_destroyERKSaIcE@plt>
  401bd6:	eb d2                	jmp    401baa <_ZN5meave5ErrorC1EPKcz+0xca>
  401bd8:	8b 50 f8             	mov    -0x8(%rax),%edx
  401bdb:	8d 4a ff             	lea    -0x1(%rdx),%ecx
  401bde:	89 48 f8             	mov    %ecx,-0x8(%rax)
  401be1:	89 d0                	mov    %edx,%eax
  401be3:	eb e3                	jmp    401bc8 <_ZN5meave5ErrorC1EPKcz+0xe8>
  401be5:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  401bec:	00 00 00 
  401bef:	90                   	nop

0000000000401bf0 <rothash_kernel_avx2>:
  401bf0:	c5 ec 57 d2          	vxorps %ymm2,%ymm2,%ymm2
  401bf4:	c5 dc 57 e4          	vxorps %ymm4,%ymm4,%ymm4
  401bf8:	c5 cc 57 f6          	vxorps %ymm6,%ymm6,%ymm6
  401bfc:	c4 41 3c 57 c0       	vxorps %ymm8,%ymm8,%ymm8
  401c01:	48 89 f2             	mov    %rsi,%rdx
  401c04:	48 c1 ea 07          	shr    $0x7,%rdx
  401c08:	74 5a                	je     401c64 <rothash_kernel_avx2+0x74>
  401c0a:	c5 e5 72 f2 0d       	vpslld $0xd,%ymm2,%ymm3
  401c0f:	c5 d5 72 f4 0d       	vpslld $0xd,%ymm4,%ymm5
  401c14:	c5 c5 72 f6 0d       	vpslld $0xd,%ymm6,%ymm7
  401c19:	c4 c1 35 72 f0 0d    	vpslld $0xd,%ymm8,%ymm9
  401c1f:	c5 ed 72 d2 1d       	vpsrld $0x1d,%ymm2,%ymm2
  401c24:	c5 dd 72 d4 1d       	vpsrld $0x1d,%ymm4,%ymm4
  401c29:	c5 cd 72 d6 1d       	vpsrld $0x1d,%ymm6,%ymm6
  401c2e:	c4 c1 3d 72 d0 1d    	vpsrld $0x1d,%ymm8,%ymm8
  401c34:	c5 e5 eb d2          	vpor   %ymm2,%ymm3,%ymm2
  401c38:	c5 d5 eb e4          	vpor   %ymm4,%ymm5,%ymm4
  401c3c:	c5 c5 eb f6          	vpor   %ymm6,%ymm7,%ymm6
  401c40:	c4 41 35 eb c0       	vpor   %ymm8,%ymm9,%ymm8
  401c45:	c5 ec 57 17          	vxorps (%rdi),%ymm2,%ymm2
  401c49:	c5 dc 57 67 20       	vxorps 0x20(%rdi),%ymm4,%ymm4
  401c4e:	c5 cc 57 77 40       	vxorps 0x40(%rdi),%ymm6,%ymm6
  401c53:	c5 3c 57 47 60       	vxorps 0x60(%rdi),%ymm8,%ymm8
  401c58:	48 8d bf 80 00 00 00 	lea    0x80(%rdi),%rdi
  401c5f:	48 ff ca             	dec    %rdx
  401c62:	75 a6                	jne    401c0a <rothash_kernel_avx2+0x1a>
  401c64:	66 0f 7e d0          	movd   %xmm2,%eax
  401c68:	c3                   	retq   
  401c69:	48 89 f2             	mov    %rsi,%rdx
  401c6c:	48 c1 ea 05          	shr    $0x5,%rdx
  401c70:	74 1b                	je     401c8d <rothash_kernel_avx2+0x9d>
  401c72:	c5 e5 72 f2 0d       	vpslld $0xd,%ymm2,%ymm3
  401c77:	c5 ed 72 d2 1d       	vpsrld $0x1d,%ymm2,%ymm2
  401c7c:	c5 e5 eb d2          	vpor   %ymm2,%ymm3,%ymm2
  401c80:	c5 ec 57 17          	vxorps (%rdi),%ymm2,%ymm2
  401c84:	48 8d 7f 20          	lea    0x20(%rdi),%rdi
  401c88:	48 ff ca             	dec    %rdx
  401c8b:	75 e5                	jne    401c72 <rothash_kernel_avx2+0x82>
  401c8d:	66 0f 7e d0          	movd   %xmm2,%eax
  401c91:	c3                   	retq   
  401c92:	c4 e3 7d 19 d3 01    	vextractf128 $0x1,%ymm2,%xmm3
  401c98:	c5 e0 57 d2          	vxorps %xmm2,%xmm3,%xmm2
  401c9c:	0f 12 da             	movhlps %xmm2,%xmm3
  401c9f:	c5 e0 57 d2          	vxorps %xmm2,%xmm3,%xmm2
  401ca3:	c5 e1 73 d2 20       	vpsrlq $0x20,%xmm2,%xmm3
  401ca8:	c5 e0 57 d2          	vxorps %xmm2,%xmm3,%xmm2
  401cac:	66 0f 7e d0          	movd   %xmm2,%eax
  401cb0:	c3                   	retq   
  401cb1:	48 83 e6 1f          	and    $0x1f,%rsi
  401cb5:	48 89 f2             	mov    %rsi,%rdx
  401cb8:	48 c1 ea 04          	shr    $0x4,%rdx
  401cbc:	74 1a                	je     401cd8 <rothash_kernel_avx2+0xe8>
  401cbe:	c5 e1 72 f2 0d       	vpslld $0xd,%xmm2,%xmm3
  401cc3:	c5 e9 72 d2 1d       	vpsrld $0x1d,%xmm2,%xmm2
  401cc8:	c5 e1 eb d2          	vpor   %xmm2,%xmm3,%xmm2
  401ccc:	c5 e8 57 17          	vxorps (%rdi),%xmm2,%xmm2
  401cd0:	48 8d 7f 10          	lea    0x10(%rdi),%rdi
  401cd4:	48 83 ee 10          	sub    $0x10,%rsi
  401cd8:	0f 12 da             	movhlps %xmm2,%xmm3
  401cdb:	c5 e0 57 d2          	vxorps %xmm2,%xmm3,%xmm2
  401cdf:	48 89 f2             	mov    %rsi,%rdx
  401ce2:	48 c1 ea 03          	shr    $0x3,%rdx
  401ce6:	74 1e                	je     401d06 <rothash_kernel_avx2+0x116>
  401ce8:	c5 e1 72 f2 0d       	vpslld $0xd,%xmm2,%xmm3
  401ced:	c5 e9 72 d2 1d       	vpsrld $0x1d,%xmm2,%xmm2
  401cf2:	c5 e1 eb d2          	vpor   %xmm2,%xmm3,%xmm2
  401cf6:	f3 0f 7e 1f          	movq   (%rdi),%xmm3
  401cfa:	c5 e8 57 d3          	vxorps %xmm3,%xmm2,%xmm2
  401cfe:	48 8d 7f 08          	lea    0x8(%rdi),%rdi
  401d02:	48 83 ee 08          	sub    $0x8,%rsi
  401d06:	c5 e1 73 d2 20       	vpsrlq $0x20,%xmm2,%xmm3
  401d0b:	c5 e0 57 d2          	vxorps %xmm2,%xmm3,%xmm2
  401d0f:	48 89 f2             	mov    %rsi,%rdx
  401d12:	48 c1 ea 02          	shr    $0x2,%rdx
  401d16:	74 1e                	je     401d36 <rothash_kernel_avx2+0x146>
  401d18:	c5 e1 72 f2 0d       	vpslld $0xd,%xmm2,%xmm3
  401d1d:	c5 e9 72 d2 1d       	vpsrld $0x1d,%xmm2,%xmm2
  401d22:	c5 e1 eb d2          	vpor   %xmm2,%xmm3,%xmm2
  401d26:	66 0f 6e 1f          	movd   (%rdi),%xmm3
  401d2a:	c5 e8 57 d3          	vxorps %xmm3,%xmm2,%xmm2
  401d2e:	48 8d 7f 04          	lea    0x4(%rdi),%rdi
  401d32:	48 83 ee 04          	sub    $0x4,%rsi
  401d36:	66 0f 7e d0          	movd   %xmm2,%eax
  401d3a:	c3                   	retq   
  401d3b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000401d40 <__libc_csu_init>:
  401d40:	41 57                	push   %r15
  401d42:	41 89 ff             	mov    %edi,%r15d
  401d45:	41 56                	push   %r14
  401d47:	49 89 f6             	mov    %rsi,%r14
  401d4a:	41 55                	push   %r13
  401d4c:	49 89 d5             	mov    %rdx,%r13
  401d4f:	41 54                	push   %r12
  401d51:	4c 8d 25 90 10 20 00 	lea    0x201090(%rip),%r12        # 602de8 <__frame_dummy_init_array_entry>
  401d58:	55                   	push   %rbp
  401d59:	48 8d 2d 98 10 20 00 	lea    0x201098(%rip),%rbp        # 602df8 <__init_array_end>
  401d60:	53                   	push   %rbx
  401d61:	4c 29 e5             	sub    %r12,%rbp
  401d64:	31 db                	xor    %ebx,%ebx
  401d66:	48 c1 fd 03          	sar    $0x3,%rbp
  401d6a:	48 83 ec 08          	sub    $0x8,%rsp
  401d6e:	e8 7d f0 ff ff       	callq  400df0 <_init>
  401d73:	48 85 ed             	test   %rbp,%rbp
  401d76:	74 1e                	je     401d96 <__libc_csu_init+0x56>
  401d78:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
  401d7f:	00 
  401d80:	4c 89 ea             	mov    %r13,%rdx
  401d83:	4c 89 f6             	mov    %r14,%rsi
  401d86:	44 89 ff             	mov    %r15d,%edi
  401d89:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
  401d8d:	48 83 c3 01          	add    $0x1,%rbx
  401d91:	48 39 eb             	cmp    %rbp,%rbx
  401d94:	75 ea                	jne    401d80 <__libc_csu_init+0x40>
  401d96:	48 83 c4 08          	add    $0x8,%rsp
  401d9a:	5b                   	pop    %rbx
  401d9b:	5d                   	pop    %rbp
  401d9c:	41 5c                	pop    %r12
  401d9e:	41 5d                	pop    %r13
  401da0:	41 5e                	pop    %r14
  401da2:	41 5f                	pop    %r15
  401da4:	c3                   	retq   
  401da5:	66 66 2e 0f 1f 84 00 	data32 nopw %cs:0x0(%rax,%rax,1)
  401dac:	00 00 00 00 

0000000000401db0 <__libc_csu_fini>:
  401db0:	f3 c3                	repz retq 

Disassembly of section .fini:

0000000000401db4 <_fini>:
  401db4:	48 83 ec 08          	sub    $0x8,%rsp
  401db8:	48 83 c4 08          	add    $0x8,%rsp
  401dbc:	c3                   	retq   
