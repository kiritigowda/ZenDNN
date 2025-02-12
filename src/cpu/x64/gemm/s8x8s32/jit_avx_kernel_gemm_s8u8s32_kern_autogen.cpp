﻿/*******************************************************************************
* Modifications Copyright (c) 2021 Advanced Micro Devices, Inc. All rights reserved.
* Notified per clause 4(b) of the license.
*******************************************************************************/

/*******************************************************************************
* Copyright 2020 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#include "cpu/x64/jit_generator.hpp"

#include "cpu/x64/gemm/s8x8s32/common_u8.hpp"

namespace zendnn {
namespace impl {
namespace cpu {
namespace x64 {

jit_avx_kernel_gemm_s8u8s32_kern::jit_avx_kernel_gemm_s8u8s32_kern()
    : jit_generator(nullptr, S8U8S32_COMPUTE_KERNEL_CODE_SIZE) {}

void jit_avx_kernel_gemm_s8u8s32_kern::generate() {

#ifndef _WIN32

#define M rdi
#define N rsi
#define K rdx
#define A r8
#define B r9
#define C r10
#define LDC r11

#define AA rcx
#define I r12
#define J r13
#define H rax
#define AO r14
#define BO r15
#define CO1 rbx
#define CO2 rbp

#else

#define M rcx
#define N rdx
#define K r8
#define A rsi
#define B r9
#define C r10
#define LDC r11

#define AA rdi
#define I r12
#define J r13
#define H rax
#define AO r14
#define BO r15
#define CO1 rbx
#define CO2 rbp

#endif

#ifdef _WIN32
#define ARG_A (args_offset - 16) + rsp
#define ARG_B (args_offset - 8) + rsp
#endif
#define ARG_C ((args_offset + 0) + rsp)
#define ARG_LDC ((args_offset + 8) + rsp)

    inLocalLabel();
    {

        Xbyak::Label l102c;
        Xbyak::Label l1054;
        Xbyak::Label l1058;
        Xbyak::Label l1090;
        Xbyak::Label l10b8;
        Xbyak::Label l1140;
        Xbyak::Label l1154;
        Xbyak::Label l11dc;
        Xbyak::Label l1214;
        Xbyak::Label l1264;
        Xbyak::Label l12bc;
        Xbyak::Label l12f4;
        Xbyak::Label l1324;
        Xbyak::Label l1384;
        Xbyak::Label l1390;
        Xbyak::Label l13f0;
        Xbyak::Label l1418;
        Xbyak::Label l1458;
        Xbyak::Label l14a0;
        Xbyak::Label l14b8;
        Xbyak::Label l14bc;
        Xbyak::Label l14f4;
        Xbyak::Label l151c;
        Xbyak::Label l15a4;
        Xbyak::Label l15b8;
        Xbyak::Label l1640;
        Xbyak::Label l1678;
        Xbyak::Label l16c8;
        Xbyak::Label l1720;
        Xbyak::Label l1758;
        Xbyak::Label l1788;
        Xbyak::Label l17e8;
        Xbyak::Label l17f4;
        Xbyak::Label l1854;
        Xbyak::Label l187c;
        Xbyak::Label l18bc;
        Xbyak::Label l1904;
        Xbyak::Label l191c;
        Xbyak::Label l1920;
        Xbyak::Label l1958;
        Xbyak::Label l1980;
        Xbyak::Label l1a08;
        Xbyak::Label l1a1c;
        Xbyak::Label l1aa4;
        Xbyak::Label l1adc;
        Xbyak::Label l1b2c;
        Xbyak::Label l1b84;
        Xbyak::Label l1bbc;
        Xbyak::Label l1bec;
        Xbyak::Label l1c4c;
        Xbyak::Label l1c58;
        Xbyak::Label l1cb8;
        Xbyak::Label l1ce0;
        Xbyak::Label l1d20;
        Xbyak::Label l1d68;
        Xbyak::Label l1d80;
        Xbyak::Label l1d84;
        Xbyak::Label l240;
        Xbyak::Label l254;
        Xbyak::Label l3a0;
        Xbyak::Label l428;
        Xbyak::Label l4dc;
        Xbyak::Label l5b0;
        Xbyak::Label l668;
        Xbyak::Label l6b0;
        Xbyak::Label l788;
        Xbyak::Label l798;
        Xbyak::Label l870;
        Xbyak::Label l8bc;
        Xbyak::Label l934;
        Xbyak::Label l94;
        Xbyak::Label l9d0;
        Xbyak::Label la20;
        Xbyak::Label la34;
        Xbyak::Label la6c;
        Xbyak::Label la98;
        Xbyak::Label lb60;
        Xbyak::Label lb74;
        Xbyak::Label lbc;
        Xbyak::Label lc3c;
        Xbyak::Label lc90;
        Xbyak::Label ld00;
        Xbyak::Label ld80;
        Xbyak::Label lde4;
        Xbyak::Label le20;
        Xbyak::Label leac;
        Xbyak::Label lebc;
        Xbyak::Label lf4;
        Xbyak::Label lf48;
        Xbyak::Label lf7c;
        Xbyak::Label lfcc;

        auto stack_alloc_size = 32;
        auto args_offset = stack_alloc_size + get_size_of_abi_save_regs() + 8;
#ifdef _WIN32
        args_offset += 48;
#endif
        preamble();
        sub(rsp, stack_alloc_size);
#ifdef _WIN32
        mov(A, ptr[ARG_A]);
        mov(B, ptr[ARG_B]);
#endif

        mov(C, qword[ARG_C]);
        mov(LDC, qword[ARG_LDC]);
        sub(A, -128);
        sub(B, -128);
        mov(M, qword[M]);
        mov(N, qword[N]);
        mov(K, qword[K]);
        lea(LDC, ptr[LDC * 4 + 0x0]);
        vxorps(xmm8, xmm8, xmm8);
        vxorps(xmm9, xmm9, xmm9);
        vxorps(xmm10, xmm10, xmm10);
        vxorps(xmm11, xmm11, xmm11);
        vxorps(xmm12, xmm12, xmm12);
        vxorps(xmm13, xmm13, xmm13);
        vxorps(xmm14, xmm14, xmm14);
        vxorps(xmm15, xmm15, xmm15);
        mov(H, 0x10001);
        movq(xmm7, H);
        vpshufd(xmm7, xmm7, 0x0);
        mov(J, M);
        cmp(J, 0x10);
        jl(la34, T_NEAR);
        align(4);

        L(l94);
        mov(CO1, C);
        add(C, 0x40);
        mov(BO, B);
        mov(AA, K);
        shl(AA, 0x20);
        lea(AA, ptr[A + AA * 1 + 0x200]);
        mov(I, N);
        cmp(I, 0x2);
        jl(l668, T_NEAR);
        align(4);

        L(lbc);
        mov(AO, A);
        vmovdqu(xmm0, xword[AO - 0x80]);
        vmovdqu(xmm1, xword[AO - 0x70]);
        vmovdqu(xmm2, xword[AO - 0x60]);
        vmovdqu(xmm3, xword[AO - 0x50]);
        vmovdqu(xmm5, xword[BO - 0x80]);
        mov(H, K);
        sar(H, 0x3);
        jle(l3a0, T_NEAR);
        sub(H, 0x8);
        jle(l240, T_NEAR);
        align(4);

        L(lf4);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        prefetcht0(byte[AO + 0x180]);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm10, xmm10, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm2);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm12, xmm12, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm3);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm14, xmm14, xmm6);
        prefetcht0(byte[BO]);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm11, xmm11, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm2);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm13, xmm13, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm3);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm15, xmm15, xmm6);
        vmovdqu(xmm0, xword[AO - 0x40]);
        vmovdqu(xmm1, xword[AO - 0x30]);
        vmovdqu(xmm2, xword[AO - 0x20]);
        vmovdqu(xmm3, xword[AO - 0x10]);
        prefetcht0(byte[AO + 0x1c0]);
        vpshufd(xmm4, xmm5, 0xaa);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm10, xmm10, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm2);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm12, xmm12, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm3);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm14, xmm14, xmm6);
        vpshufd(xmm4, xmm5, 0xff);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm11, xmm11, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm2);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm13, xmm13, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm3);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm15, xmm15, xmm6);
        vmovdqu(xmm5, xword[BO - 0x70]);
        prefetcht1(byte[AA - 0x80]);
        vmovdqu(xmm0, xword[AO]);
        vmovdqu(xmm1, xword[AO + 0x10]);
        vmovdqu(xmm2, xword[AO + 0x20]);
        vmovdqu(xmm3, xword[AO + 0x30]);
        add(AA, 0x4);
        add(AO, 0x80);
        add(BO, 0x10);
        sub(H, 0x1);
        jg(lf4, T_NEAR);
        align(4);

        L(l240);
        prefetcht0(byte[CO1 + 0x3c]);
        prefetcht0(byte[CO1 + LDC * 1 + 0x3c]);
        add(H, 0x8);
        jle(l3a0, T_NEAR);
        align(4);

        L(l254);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        prefetcht0(byte[AO + 0x180]);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm10, xmm10, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm2);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm12, xmm12, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm3);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm14, xmm14, xmm6);
        prefetcht0(byte[BO]);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm11, xmm11, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm2);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm13, xmm13, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm3);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm15, xmm15, xmm6);
        vmovdqu(xmm0, xword[AO - 0x40]);
        vmovdqu(xmm1, xword[AO - 0x30]);
        vmovdqu(xmm2, xword[AO - 0x20]);
        vmovdqu(xmm3, xword[AO - 0x10]);
        prefetcht0(byte[AO + 0x1c0]);
        vpshufd(xmm4, xmm5, 0xaa);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm10, xmm10, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm2);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm12, xmm12, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm3);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm14, xmm14, xmm6);
        vpshufd(xmm4, xmm5, 0xff);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm11, xmm11, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm2);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm13, xmm13, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm3);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm15, xmm15, xmm6);
        vmovdqu(xmm5, xword[BO - 0x70]);
        prefetcht1(byte[AA - 0x80]);
        vmovdqu(xmm0, xword[AO]);
        vmovdqu(xmm1, xword[AO + 0x10]);
        vmovdqu(xmm2, xword[AO + 0x20]);
        vmovdqu(xmm3, xword[AO + 0x30]);
        add(AA, 0x4);
        add(AO, 0x80);
        add(BO, 0x10);
        sub(H, 0x1);
        jg(l254, T_NEAR);
        align(4);

        L(l3a0);
        mov(H, K);
        test(H, 0x4);
        je(l428, T_NEAR);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm10, xmm10, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm2);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm12, xmm12, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm3);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm14, xmm14, xmm6);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm11, xmm11, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm2);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm13, xmm13, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm3);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm15, xmm15, xmm6);
        add(AO, 0x40);
        add(BO, 0x8);
        align(4);

        L(l428);
        mov(H, K);
        test(H, 0x2);
        je(l4dc, T_NEAR);
        vxorps(xmm6, xmm6, xmm6);
        vmovdqu(xmm1, xword[AO - 0x80]);
        vpunpcklwd(xmm0, xmm1, xmm6);
        vpunpckhwd(xmm1, xmm1, xmm6);
        vmovdqu(xmm3, xword[AO - 0x70]);
        vpunpcklwd(xmm2, xmm3, xmm6);
        vpunpckhwd(xmm3, xmm3, xmm6);
        vbroadcastss(xmm5, dword[BO - 0x80]);
        vpunpcklwd(xmm5, xmm5, xmm5);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm10, xmm10, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm2);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm12, xmm12, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm3);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm14, xmm14, xmm6);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm11, xmm11, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm2);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm13, xmm13, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm3);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm15, xmm15, xmm6);
        add(AO, 0x20);
        add(BO, 0x4);
        align(4);

        L(l4dc);
        mov(H, K);
        test(H, 0x1);
        je(l5b0, T_NEAR);
        vxorps(xmm6, xmm6, xmm6);
        vbroadcastss(xmm0, dword[AO - 0x80]);
        vpunpcklbw(xmm0, xmm0, xmm6);
        vpunpcklwd(xmm0, xmm0, xmm6);
        vbroadcastss(xmm1, dword[AO - 0x7c]);
        vpunpcklbw(xmm1, xmm1, xmm6);
        vpunpcklwd(xmm1, xmm1, xmm6);
        vbroadcastss(xmm2, dword[AO - 0x78]);
        vpunpcklbw(xmm2, xmm2, xmm6);
        vpunpcklwd(xmm2, xmm2, xmm6);
        vbroadcastss(xmm3, dword[AO - 0x74]);
        vpunpcklbw(xmm3, xmm3, xmm6);
        vpunpcklwd(xmm3, xmm3, xmm6);
        vbroadcastss(xmm5, dword[BO - 0x80]);
        vpunpcklbw(xmm5, xmm5, xmm5);
        vpunpcklwd(xmm5, xmm5, xmm5);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm10, xmm10, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm2);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm12, xmm12, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm3);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm14, xmm14, xmm6);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm11, xmm11, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm2);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm13, xmm13, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm3);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm15, xmm15, xmm6);
        add(AO, 0x10);
        add(BO, 0x2);
        align(4);

        L(l5b0);
        vmovdqu(xmm0, xword[CO1]);
        vpaddd(xmm8, xmm8, xmm0);
        vmovdqu(xword[CO1], xmm8);
        vxorps(xmm8, xmm8, xmm8);
        vmovdqu(xmm0, xword[CO1 + 0x10]);
        vpaddd(xmm10, xmm10, xmm0);
        vmovdqu(xword[CO1 + 0x10], xmm10);
        vxorps(xmm10, xmm10, xmm10);
        vmovdqu(xmm0, xword[CO1 + 0x20]);
        vpaddd(xmm12, xmm12, xmm0);
        vmovdqu(xword[CO1 + 0x20], xmm12);
        vxorps(xmm12, xmm12, xmm12);
        vmovdqu(xmm0, xword[CO1 + 0x30]);
        vpaddd(xmm14, xmm14, xmm0);
        vmovdqu(xword[CO1 + 0x30], xmm14);
        vxorps(xmm14, xmm14, xmm14);
        vmovdqu(xmm0, xword[CO1 + LDC * 1]);
        vpaddd(xmm9, xmm9, xmm0);
        vmovdqu(xword[CO1 + LDC * 1], xmm9);
        vxorps(xmm9, xmm9, xmm9);
        vmovdqu(xmm0, xword[CO1 + LDC * 1 + 0x10]);
        vpaddd(xmm11, xmm11, xmm0);
        vmovdqu(xword[CO1 + LDC * 1 + 0x10], xmm11);
        vxorps(xmm11, xmm11, xmm11);
        vmovdqu(xmm0, xword[CO1 + LDC * 1 + 0x20]);
        vpaddd(xmm13, xmm13, xmm0);
        vmovdqu(xword[CO1 + LDC * 1 + 0x20], xmm13);
        vxorps(xmm13, xmm13, xmm13);
        vmovdqu(xmm0, xword[CO1 + LDC * 1 + 0x30]);
        vpaddd(xmm15, xmm15, xmm0);
        vmovdqu(xword[CO1 + LDC * 1 + 0x30], xmm15);
        vxorps(xmm15, xmm15, xmm15);
        lea(CO1, ptr[CO1 + LDC * 2]);
        sub(I, 0x2);
        cmp(I, 0x2);
        jge(lbc, T_NEAR);
        align(4);

        L(l668);
        test(I, 0x1);
        jle(la20, T_NEAR);
        mov(AO, A);
        vmovdqu(xmm0, xword[AO - 0x80]);
        vmovdqu(xmm1, xword[AO - 0x70]);
        vmovdqu(xmm2, xword[AO - 0x60]);
        vmovdqu(xmm3, xword[AO - 0x50]);
        vmovdqu(xmm5, xword[BO - 0x80]);
        mov(H, K);
        sar(H, 0x3);
        jle(l870, T_NEAR);
        sub(H, 0x8);
        jle(l788, T_NEAR);
        align(4);

        L(l6b0);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        prefetcht0(byte[AO + 0x180]);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm10, xmm10, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm2);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm12, xmm12, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm3);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm14, xmm14, xmm6);
        prefetcht0(byte[BO]);
        vmovdqu(xmm0, xword[AO - 0x40]);
        vmovdqu(xmm1, xword[AO - 0x30]);
        vmovdqu(xmm2, xword[AO - 0x20]);
        vmovdqu(xmm3, xword[AO - 0x10]);
        prefetcht0(byte[AO + 0x1c0]);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm10, xmm10, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm2);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm12, xmm12, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm3);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm14, xmm14, xmm6);
        vmovdqu(xmm5, xword[BO - 0x78]);
        prefetcht1(byte[AA - 0x80]);
        vmovdqu(xmm0, xword[AO]);
        vmovdqu(xmm1, xword[AO + 0x10]);
        vmovdqu(xmm2, xword[AO + 0x20]);
        vmovdqu(xmm3, xword[AO + 0x30]);
        add(AA, 0x4);
        add(AO, 0x80);
        add(BO, 0x8);
        sub(H, 0x1);
        jg(l6b0, T_NEAR);
        align(4);

        L(l788);
        prefetcht0(byte[CO1 + 0x3c]);
        add(H, 0x8);
        jle(l870, T_NEAR);
        align(4);

        L(l798);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        prefetcht0(byte[AO + 0x180]);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm10, xmm10, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm2);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm12, xmm12, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm3);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm14, xmm14, xmm6);
        prefetcht0(byte[BO]);
        vmovdqu(xmm0, xword[AO - 0x40]);
        vmovdqu(xmm1, xword[AO - 0x30]);
        vmovdqu(xmm2, xword[AO - 0x20]);
        vmovdqu(xmm3, xword[AO - 0x10]);
        prefetcht0(byte[AO + 0x1c0]);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm10, xmm10, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm2);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm12, xmm12, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm3);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm14, xmm14, xmm6);
        vmovdqu(xmm5, xword[BO - 0x78]);
        prefetcht1(byte[AA - 0x80]);
        vmovdqu(xmm0, xword[AO]);
        vmovdqu(xmm1, xword[AO + 0x10]);
        vmovdqu(xmm2, xword[AO + 0x20]);
        vmovdqu(xmm3, xword[AO + 0x30]);
        add(AA, 0x4);
        add(AO, 0x80);
        add(BO, 0x8);
        sub(H, 0x1);
        jg(l798, T_NEAR);
        align(4);

        L(l870);
        mov(H, K);
        test(H, 0x4);
        je(l8bc, T_NEAR);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm10, xmm10, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm2);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm12, xmm12, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm3);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm14, xmm14, xmm6);
        add(AO, 0x40);
        add(BO, 0x4);
        align(4);

        L(l8bc);
        mov(H, K);
        test(H, 0x2);
        je(l934, T_NEAR);
        vxorps(xmm6, xmm6, xmm6);
        vmovdqu(xmm1, xword[AO - 0x80]);
        vpunpcklwd(xmm0, xmm1, xmm6);
        vpunpckhwd(xmm1, xmm1, xmm6);
        vmovdqu(xmm3, xword[AO - 0x70]);
        vpunpcklwd(xmm2, xmm3, xmm6);
        vpunpckhwd(xmm3, xmm3, xmm6);
        vbroadcastss(xmm5, dword[BO - 0x80]);
        vpunpcklwd(xmm5, xmm5, xmm5);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm10, xmm10, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm2);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm12, xmm12, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm3);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm14, xmm14, xmm6);
        add(AO, 0x20);
        add(BO, 0x2);
        align(4);

        L(l934);
        mov(H, K);
        test(H, 0x1);
        je(l9d0, T_NEAR);
        vxorps(xmm6, xmm6, xmm6);
        vbroadcastss(xmm0, dword[AO - 0x80]);
        vpunpcklbw(xmm0, xmm0, xmm6);
        vpunpcklwd(xmm0, xmm0, xmm6);
        vbroadcastss(xmm1, dword[AO - 0x7c]);
        vpunpcklbw(xmm1, xmm1, xmm6);
        vpunpcklwd(xmm1, xmm1, xmm6);
        vbroadcastss(xmm2, dword[AO - 0x78]);
        vpunpcklbw(xmm2, xmm2, xmm6);
        vpunpcklwd(xmm2, xmm2, xmm6);
        vbroadcastss(xmm3, dword[AO - 0x74]);
        vpunpcklbw(xmm3, xmm3, xmm6);
        vpunpcklwd(xmm3, xmm3, xmm6);
        vbroadcastss(xmm5, dword[BO - 0x80]);
        vpunpcklbw(xmm5, xmm5, xmm5);
        vpunpcklwd(xmm5, xmm5, xmm5);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm10, xmm10, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm2);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm12, xmm12, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm3);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm14, xmm14, xmm6);
        add(AO, 0x10);
        add(BO, 0x1);
        align(4);

        L(l9d0);
        vmovdqu(xmm0, xword[CO1]);
        vpaddd(xmm8, xmm8, xmm0);
        vmovdqu(xword[CO1], xmm8);
        vxorps(xmm8, xmm8, xmm8);
        vmovdqu(xmm0, xword[CO1 + 0x10]);
        vpaddd(xmm10, xmm10, xmm0);
        vmovdqu(xword[CO1 + 0x10], xmm10);
        vxorps(xmm10, xmm10, xmm10);
        vmovdqu(xmm0, xword[CO1 + 0x20]);
        vpaddd(xmm12, xmm12, xmm0);
        vmovdqu(xword[CO1 + 0x20], xmm12);
        vxorps(xmm12, xmm12, xmm12);
        vmovdqu(xmm0, xword[CO1 + 0x30]);
        vpaddd(xmm14, xmm14, xmm0);
        vmovdqu(xword[CO1 + 0x30], xmm14);
        vxorps(xmm14, xmm14, xmm14);
        lea(CO1, ptr[CO1 + LDC * 1]);
        align(4);

        L(la20);
        mov(A, AO);
        sub(J, 0x10);
        cmp(J, 0x10);
        jge(l94, T_NEAR);
        align(4);

        L(la34);
        test(J, 0x8);
        jle(l1058, T_NEAR);
        mov(CO1, C);
        add(C, 0x20);
        mov(BO, B);
        mov(AA, K);
        shl(AA, 0x10);
        lea(AA, ptr[A + AA * 1 + 0x200]);
        mov(I, N);
        cmp(I, 0x2);
        jl(lde4, T_NEAR);
        align(4);

        L(la6c);
        mov(AO, A);
        vmovdqu(xmm0, xword[AO - 0x80]);
        vmovdqu(xmm1, xword[AO - 0x70]);
        vmovdqu(xmm5, xword[BO - 0x80]);
        mov(H, K);
        sar(H, 0x3);
        jle(lc3c, T_NEAR);
        sub(H, 0x8);
        jle(lb60, T_NEAR);
        align(4);

        L(la98);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        prefetcht0(byte[AO + 0x180]);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm10, xmm10, xmm6);
        prefetcht0(byte[BO]);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm11, xmm11, xmm6);
        vmovdqu(xmm0, xword[AO - 0x60]);
        vmovdqu(xmm1, xword[AO - 0x50]);
        prefetcht0(byte[AO + 0x1c0]);
        vpshufd(xmm4, xmm5, 0xaa);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm10, xmm10, xmm6);
        vpshufd(xmm4, xmm5, 0xff);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm11, xmm11, xmm6);
        vmovdqu(xmm5, xword[BO - 0x70]);
        prefetcht1(byte[AA - 0x80]);
        vmovdqu(xmm0, xword[AO - 0x40]);
        vmovdqu(xmm1, xword[AO - 0x30]);
        add(AA, 0x4);
        add(AO, 0x40);
        add(BO, 0x10);
        sub(H, 0x1);
        jg(la98, T_NEAR);
        align(4);

        L(lb60);
        prefetcht0(byte[CO1 + 0x3c]);
        prefetcht0(byte[CO1 + LDC * 1 + 0x3c]);
        add(H, 0x8);
        jle(lc3c, T_NEAR);
        align(4);

        L(lb74);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        prefetcht0(byte[AO + 0x180]);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm10, xmm10, xmm6);
        prefetcht0(byte[BO]);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm11, xmm11, xmm6);
        vmovdqu(xmm0, xword[AO - 0x60]);
        vmovdqu(xmm1, xword[AO - 0x50]);
        prefetcht0(byte[AO + 0x1c0]);
        vpshufd(xmm4, xmm5, 0xaa);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm10, xmm10, xmm6);
        vpshufd(xmm4, xmm5, 0xff);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm11, xmm11, xmm6);
        vmovdqu(xmm5, xword[BO - 0x70]);
        prefetcht1(byte[AA - 0x80]);
        vmovdqu(xmm0, xword[AO - 0x40]);
        vmovdqu(xmm1, xword[AO - 0x30]);
        add(AA, 0x4);
        add(AO, 0x40);
        add(BO, 0x10);
        sub(H, 0x1);
        jg(lb74, T_NEAR);
        align(4);

        L(lc3c);
        mov(H, K);
        test(H, 0x4);
        je(lc90, T_NEAR);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm10, xmm10, xmm6);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm11, xmm11, xmm6);
        add(AO, 0x20);
        add(BO, 0x8);
        align(4);

        L(lc90);
        mov(H, K);
        test(H, 0x2);
        je(ld00, T_NEAR);
        vxorps(xmm6, xmm6, xmm6);
        vmovdqu(xmm1, xword[AO - 0x80]);
        vpunpcklwd(xmm0, xmm1, xmm6);
        vpunpckhwd(xmm1, xmm1, xmm6);
        vbroadcastss(xmm5, dword[BO - 0x80]);
        vpunpcklwd(xmm5, xmm5, xmm5);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm10, xmm10, xmm6);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm11, xmm11, xmm6);
        add(AO, 0x10);
        add(BO, 0x4);
        align(4);

        L(ld00);
        mov(H, K);
        test(H, 0x1);
        je(ld80, T_NEAR);
        vxorps(xmm6, xmm6, xmm6);
        vbroadcastss(xmm0, dword[AO - 0x80]);
        vpunpcklbw(xmm0, xmm0, xmm6);
        vpunpcklwd(xmm0, xmm0, xmm6);
        vbroadcastss(xmm1, dword[AO - 0x7c]);
        vpunpcklbw(xmm1, xmm1, xmm6);
        vpunpcklwd(xmm1, xmm1, xmm6);
        vbroadcastss(xmm5, dword[BO - 0x80]);
        vpunpcklbw(xmm5, xmm5, xmm5);
        vpunpcklwd(xmm5, xmm5, xmm5);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm10, xmm10, xmm6);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm11, xmm11, xmm6);
        add(AO, 0x8);
        add(BO, 0x2);
        align(4);

        L(ld80);
        vmovdqu(xmm0, xword[CO1]);
        vpaddd(xmm8, xmm8, xmm0);
        vmovdqu(xword[CO1], xmm8);
        vxorps(xmm8, xmm8, xmm8);
        vmovdqu(xmm0, xword[CO1 + 0x10]);
        vpaddd(xmm10, xmm10, xmm0);
        vmovdqu(xword[CO1 + 0x10], xmm10);
        vxorps(xmm10, xmm10, xmm10);
        vmovdqu(xmm0, xword[CO1 + LDC * 1]);
        vpaddd(xmm9, xmm9, xmm0);
        vmovdqu(xword[CO1 + LDC * 1], xmm9);
        vxorps(xmm9, xmm9, xmm9);
        vmovdqu(xmm0, xword[CO1 + LDC * 1 + 0x10]);
        vpaddd(xmm11, xmm11, xmm0);
        vmovdqu(xword[CO1 + LDC * 1 + 0x10], xmm11);
        vxorps(xmm11, xmm11, xmm11);
        lea(CO1, ptr[CO1 + LDC * 2]);
        sub(I, 0x2);
        cmp(I, 0x2);
        jge(la6c, T_NEAR);
        align(4);

        L(lde4);
        test(I, 0x1);
        jle(l1054, T_NEAR);
        mov(AO, A);
        vmovdqu(xmm0, xword[AO - 0x80]);
        vmovdqu(xmm1, xword[AO - 0x70]);
        vmovdqu(xmm5, xword[BO - 0x80]);
        mov(H, K);
        sar(H, 0x3);
        jle(lf48, T_NEAR);
        sub(H, 0x8);
        jle(leac, T_NEAR);
        align(4);

        L(le20);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        prefetcht0(byte[AO + 0x180]);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm10, xmm10, xmm6);
        prefetcht0(byte[BO]);
        vmovdqu(xmm0, xword[AO - 0x60]);
        vmovdqu(xmm1, xword[AO - 0x50]);
        prefetcht0(byte[AO + 0x1c0]);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm10, xmm10, xmm6);
        vmovdqu(xmm5, xword[BO - 0x78]);
        prefetcht1(byte[AA - 0x80]);
        vmovdqu(xmm0, xword[AO - 0x40]);
        vmovdqu(xmm1, xword[AO - 0x30]);
        add(AA, 0x4);
        add(AO, 0x40);
        add(BO, 0x8);
        sub(H, 0x1);
        jg(le20, T_NEAR);
        align(4);

        L(leac);
        prefetcht0(byte[CO1 + 0x3c]);
        add(H, 0x8);
        jle(lf48, T_NEAR);
        align(4);

        L(lebc);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        prefetcht0(byte[AO + 0x180]);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm10, xmm10, xmm6);
        prefetcht0(byte[BO]);
        vmovdqu(xmm0, xword[AO - 0x60]);
        vmovdqu(xmm1, xword[AO - 0x50]);
        prefetcht0(byte[AO + 0x1c0]);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm10, xmm10, xmm6);
        vmovdqu(xmm5, xword[BO - 0x78]);
        prefetcht1(byte[AA - 0x80]);
        vmovdqu(xmm0, xword[AO - 0x40]);
        vmovdqu(xmm1, xword[AO - 0x30]);
        add(AA, 0x4);
        add(AO, 0x40);
        add(BO, 0x8);
        sub(H, 0x1);
        jg(lebc, T_NEAR);
        align(4);

        L(lf48);
        mov(H, K);
        test(H, 0x4);
        je(lf7c, T_NEAR);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm10, xmm10, xmm6);
        add(AO, 0x20);
        add(BO, 0x4);
        align(4);

        L(lf7c);
        mov(H, K);
        test(H, 0x2);
        je(lfcc, T_NEAR);
        vxorps(xmm6, xmm6, xmm6);
        vmovdqu(xmm1, xword[AO - 0x80]);
        vpunpcklwd(xmm0, xmm1, xmm6);
        vpunpckhwd(xmm1, xmm1, xmm6);
        vbroadcastss(xmm5, dword[BO - 0x80]);
        vpunpcklwd(xmm5, xmm5, xmm5);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm10, xmm10, xmm6);
        add(AO, 0x10);
        add(BO, 0x2);
        align(4);

        L(lfcc);
        mov(H, K);
        test(H, 0x1);
        je(l102c, T_NEAR);
        vxorps(xmm6, xmm6, xmm6);
        vbroadcastss(xmm0, dword[AO - 0x80]);
        vpunpcklbw(xmm0, xmm0, xmm6);
        vpunpcklwd(xmm0, xmm0, xmm6);
        vbroadcastss(xmm1, dword[AO - 0x7c]);
        vpunpcklbw(xmm1, xmm1, xmm6);
        vpunpcklwd(xmm1, xmm1, xmm6);
        vbroadcastss(xmm5, dword[BO - 0x80]);
        vpunpcklbw(xmm5, xmm5, xmm5);
        vpunpcklwd(xmm5, xmm5, xmm5);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpmaddubsw(xmm6, xmm4, xmm1);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm10, xmm10, xmm6);
        add(AO, 0x8);
        add(BO, 0x1);
        align(4);

        L(l102c);
        vmovdqu(xmm0, xword[CO1]);
        vpaddd(xmm8, xmm8, xmm0);
        vmovdqu(xword[CO1], xmm8);
        vxorps(xmm8, xmm8, xmm8);
        vmovdqu(xmm0, xword[CO1 + 0x10]);
        vpaddd(xmm10, xmm10, xmm0);
        vmovdqu(xword[CO1 + 0x10], xmm10);
        vxorps(xmm10, xmm10, xmm10);
        lea(CO1, ptr[CO1 + LDC * 1]);
        align(4);

        L(l1054);
        mov(A, AO);
        align(4);

        L(l1058);
        test(J, 0x4);
        jle(l14bc, T_NEAR);
        mov(CO1, C);
        add(C, 0x10);
        mov(BO, B);
        mov(AA, K);
        shl(AA, 0x8);
        lea(AA, ptr[A + AA * 1 + 0x200]);
        mov(I, N);
        cmp(I, 0x2);
        jl(l12f4, T_NEAR);
        align(4);

        L(l1090);
        mov(AO, A);
        vmovdqu(xmm0, xword[AO - 0x80]);
        vmovdqu(xmm5, xword[BO - 0x80]);
        mov(H, K);
        sar(H, 0x3);
        jle(l11dc, T_NEAR);
        sub(H, 0x8);
        jle(l1140, T_NEAR);
        align(4);

        L(l10b8);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        prefetcht0(byte[AO + 0x180]);
        prefetcht0(byte[BO]);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        vmovdqu(xmm0, xword[AO - 0x70]);
        prefetcht0(byte[AO + 0x1c0]);
        vpshufd(xmm4, xmm5, 0xaa);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpshufd(xmm4, xmm5, 0xff);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        vmovdqu(xmm5, xword[BO - 0x70]);
        prefetcht1(byte[AA - 0x80]);
        vmovdqu(xmm0, xword[AO - 0x60]);
        add(AA, 0x4);
        add(AO, 0x20);
        add(BO, 0x10);
        sub(H, 0x1);
        jg(l10b8, T_NEAR);
        align(4);

        L(l1140);
        prefetcht0(byte[CO1 + 0x3c]);
        prefetcht0(byte[CO1 + LDC * 1 + 0x3c]);
        add(H, 0x8);
        jle(l11dc, T_NEAR);
        align(4);

        L(l1154);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        prefetcht0(byte[AO + 0x180]);
        prefetcht0(byte[BO]);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        vmovdqu(xmm0, xword[AO - 0x70]);
        prefetcht0(byte[AO + 0x1c0]);
        vpshufd(xmm4, xmm5, 0xaa);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpshufd(xmm4, xmm5, 0xff);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        vmovdqu(xmm5, xword[BO - 0x70]);
        prefetcht1(byte[AA - 0x80]);
        vmovdqu(xmm0, xword[AO - 0x60]);
        add(AA, 0x4);
        add(AO, 0x20);
        add(BO, 0x10);
        sub(H, 0x1);
        jg(l1154, T_NEAR);
        align(4);

        L(l11dc);
        mov(H, K);
        test(H, 0x4);
        je(l1214, T_NEAR);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        add(AO, 0x10);
        add(BO, 0x8);
        align(4);

        L(l1214);
        mov(H, K);
        test(H, 0x2);
        je(l1264, T_NEAR);
        vxorps(xmm6, xmm6, xmm6);
        vmovdqu(xmm1, xword[AO - 0x80]);
        vpunpcklwd(xmm0, xmm1, xmm6);
        vbroadcastss(xmm5, dword[BO - 0x80]);
        vpunpcklwd(xmm5, xmm5, xmm5);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        add(AO, 0x8);
        add(BO, 0x4);
        align(4);

        L(l1264);
        mov(H, K);
        test(H, 0x1);
        je(l12bc, T_NEAR);
        vxorps(xmm6, xmm6, xmm6);
        vbroadcastss(xmm0, dword[AO - 0x80]);
        vpunpcklbw(xmm0, xmm0, xmm6);
        vpunpcklwd(xmm0, xmm0, xmm6);
        vbroadcastss(xmm5, dword[BO - 0x80]);
        vpunpcklbw(xmm5, xmm5, xmm5);
        vpunpcklwd(xmm5, xmm5, xmm5);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        add(AO, 0x4);
        add(BO, 0x2);
        align(4);

        L(l12bc);
        vmovdqu(xmm0, xword[CO1]);
        vpaddd(xmm8, xmm8, xmm0);
        vmovdqu(xword[CO1], xmm8);
        vxorps(xmm8, xmm8, xmm8);
        vmovdqu(xmm0, xword[CO1 + LDC * 1]);
        vpaddd(xmm9, xmm9, xmm0);
        vmovdqu(xword[CO1 + LDC * 1], xmm9);
        vxorps(xmm9, xmm9, xmm9);
        lea(CO1, ptr[CO1 + LDC * 2]);
        sub(I, 0x2);
        cmp(I, 0x2);
        jge(l1090, T_NEAR);
        align(4);

        L(l12f4);
        test(I, 0x1);
        jle(l14b8, T_NEAR);
        mov(AO, A);
        vmovdqu(xmm0, xword[AO - 0x80]);
        vmovdqu(xmm5, xword[BO - 0x80]);
        mov(H, K);
        sar(H, 0x3);
        jle(l13f0, T_NEAR);
        sub(H, 0x8);
        jle(l1384, T_NEAR);
        align(4);

        L(l1324);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        prefetcht0(byte[AO + 0x180]);
        prefetcht0(byte[BO]);
        vmovdqu(xmm0, xword[AO - 0x70]);
        prefetcht0(byte[AO + 0x1c0]);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vmovdqu(xmm5, xword[BO - 0x78]);
        prefetcht1(byte[AA - 0x80]);
        vmovdqu(xmm0, xword[AO - 0x60]);
        add(AA, 0x4);
        add(AO, 0x20);
        add(BO, 0x8);
        sub(H, 0x1);
        jg(l1324, T_NEAR);
        align(4);

        L(l1384);
        prefetcht0(byte[CO1 + 0x3c]);
        add(H, 0x8);
        jle(l13f0, T_NEAR);
        align(4);

        L(l1390);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        prefetcht0(byte[AO + 0x180]);
        prefetcht0(byte[BO]);
        vmovdqu(xmm0, xword[AO - 0x70]);
        prefetcht0(byte[AO + 0x1c0]);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vmovdqu(xmm5, xword[BO - 0x78]);
        prefetcht1(byte[AA - 0x80]);
        vmovdqu(xmm0, xword[AO - 0x60]);
        add(AA, 0x4);
        add(AO, 0x20);
        add(BO, 0x8);
        sub(H, 0x1);
        jg(l1390, T_NEAR);
        align(4);

        L(l13f0);
        mov(H, K);
        test(H, 0x4);
        je(l1418, T_NEAR);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        add(AO, 0x10);
        add(BO, 0x4);
        align(4);

        L(l1418);
        mov(H, K);
        test(H, 0x2);
        je(l1458, T_NEAR);
        vxorps(xmm6, xmm6, xmm6);
        vmovdqu(xmm1, xword[AO - 0x80]);
        vpunpcklwd(xmm0, xmm1, xmm6);
        vbroadcastss(xmm5, dword[BO - 0x80]);
        vpunpcklwd(xmm5, xmm5, xmm5);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        add(AO, 0x8);
        add(BO, 0x2);
        align(4);

        L(l1458);
        mov(H, K);
        test(H, 0x1);
        je(l14a0, T_NEAR);
        vxorps(xmm6, xmm6, xmm6);
        vbroadcastss(xmm0, dword[AO - 0x80]);
        vpunpcklbw(xmm0, xmm0, xmm6);
        vpunpcklwd(xmm0, xmm0, xmm6);
        vbroadcastss(xmm5, dword[BO - 0x80]);
        vpunpcklbw(xmm5, xmm5, xmm5);
        vpunpcklwd(xmm5, xmm5, xmm5);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        add(AO, 0x4);
        add(BO, 0x1);
        align(4);

        L(l14a0);
        vmovdqu(xmm0, xword[CO1]);
        vpaddd(xmm8, xmm8, xmm0);
        vmovdqu(xword[CO1], xmm8);
        vxorps(xmm8, xmm8, xmm8);
        lea(CO1, ptr[CO1 + LDC * 1]);
        align(4);

        L(l14b8);
        mov(A, AO);
        align(4);

        L(l14bc);
        test(J, 0x2);
        jle(l1920, T_NEAR);
        mov(CO1, C);
        add(C, 0x8);
        mov(BO, B);
        mov(AA, K);
        shl(AA, 0x4);
        lea(AA, ptr[A + AA * 1 + 0x200]);
        mov(I, N);
        cmp(I, 0x2);
        jl(l1758, T_NEAR);
        align(4);

        L(l14f4);
        mov(AO, A);
        vmovdqu(xmm0, xword[AO - 0x80]);
        vmovdqu(xmm5, xword[BO - 0x80]);
        mov(H, K);
        sar(H, 0x3);
        jle(l1640, T_NEAR);
        sub(H, 0x8);
        jle(l15a4, T_NEAR);
        align(4);

        L(l151c);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        prefetcht0(byte[AO + 0x180]);
        prefetcht0(byte[BO]);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        vmovdqu(xmm0, xword[AO - 0x78]);
        prefetcht0(byte[AO + 0x1c0]);
        vpshufd(xmm4, xmm5, 0xaa);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpshufd(xmm4, xmm5, 0xff);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        vmovdqu(xmm5, xword[BO - 0x70]);
        prefetcht1(byte[AA - 0x80]);
        vmovdqu(xmm0, xword[AO - 0x70]);
        add(AA, 0x4);
        add(AO, 0x10);
        add(BO, 0x10);
        sub(H, 0x1);
        jg(l151c, T_NEAR);
        align(4);

        L(l15a4);
        prefetcht0(byte[CO1 + 0x3c]);
        prefetcht0(byte[CO1 + LDC * 1 + 0x3c]);
        add(H, 0x8);
        jle(l1640, T_NEAR);
        align(4);

        L(l15b8);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        prefetcht0(byte[AO + 0x180]);
        prefetcht0(byte[BO]);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        vmovdqu(xmm0, xword[AO - 0x78]);
        prefetcht0(byte[AO + 0x1c0]);
        vpshufd(xmm4, xmm5, 0xaa);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpshufd(xmm4, xmm5, 0xff);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        vmovdqu(xmm5, xword[BO - 0x70]);
        prefetcht1(byte[AA - 0x80]);
        vmovdqu(xmm0, xword[AO - 0x70]);
        add(AA, 0x4);
        add(AO, 0x10);
        add(BO, 0x10);
        sub(H, 0x1);
        jg(l15b8, T_NEAR);
        align(4);

        L(l1640);
        mov(H, K);
        test(H, 0x4);
        je(l1678, T_NEAR);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        add(AO, 0x8);
        add(BO, 0x8);
        align(4);

        L(l1678);
        mov(H, K);
        test(H, 0x2);
        je(l16c8, T_NEAR);
        vxorps(xmm6, xmm6, xmm6);
        vmovdqu(xmm1, xword[AO - 0x80]);
        vpunpcklwd(xmm0, xmm1, xmm6);
        vbroadcastss(xmm5, dword[BO - 0x80]);
        vpunpcklwd(xmm5, xmm5, xmm5);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        add(AO, 0x4);
        add(BO, 0x4);
        align(4);

        L(l16c8);
        mov(H, K);
        test(H, 0x1);
        je(l1720, T_NEAR);
        vxorps(xmm6, xmm6, xmm6);
        vbroadcastss(xmm0, dword[AO - 0x80]);
        vpunpcklbw(xmm0, xmm0, xmm6);
        vpunpcklwd(xmm0, xmm0, xmm6);
        vbroadcastss(xmm5, dword[BO - 0x80]);
        vpunpcklbw(xmm5, xmm5, xmm5);
        vpunpcklwd(xmm5, xmm5, xmm5);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        add(AO, 0x2);
        add(BO, 0x2);
        align(4);

        L(l1720);
        vmovsd(xmm0, qword[CO1]);
        vpaddd(xmm8, xmm8, xmm0);
        vmovlps(qword[CO1], xmm8);
        vxorps(xmm8, xmm8, xmm8);
        vmovsd(xmm0, qword[CO1 + LDC * 1]);
        vpaddd(xmm9, xmm9, xmm0);
        vmovlps(qword[CO1 + LDC * 1], xmm9);
        vxorps(xmm9, xmm9, xmm9);
        lea(CO1, ptr[CO1 + LDC * 2]);
        sub(I, 0x2);
        cmp(I, 0x2);
        jge(l14f4, T_NEAR);
        align(4);

        L(l1758);
        test(I, 0x1);
        jle(l191c, T_NEAR);
        mov(AO, A);
        vmovdqu(xmm0, xword[AO - 0x80]);
        vmovdqu(xmm5, xword[BO - 0x80]);
        mov(H, K);
        sar(H, 0x3);
        jle(l1854, T_NEAR);
        sub(H, 0x8);
        jle(l17e8, T_NEAR);
        align(4);

        L(l1788);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        prefetcht0(byte[AO + 0x180]);
        prefetcht0(byte[BO]);
        vmovdqu(xmm0, xword[AO - 0x78]);
        prefetcht0(byte[AO + 0x1c0]);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vmovdqu(xmm5, xword[BO - 0x78]);
        prefetcht1(byte[AA - 0x80]);
        vmovdqu(xmm0, xword[AO - 0x70]);
        add(AA, 0x4);
        add(AO, 0x10);
        add(BO, 0x8);
        sub(H, 0x1);
        jg(l1788, T_NEAR);
        align(4);

        L(l17e8);
        prefetcht0(byte[CO1 + 0x3c]);
        add(H, 0x8);
        jle(l1854, T_NEAR);
        align(4);

        L(l17f4);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        prefetcht0(byte[AO + 0x180]);
        prefetcht0(byte[BO]);
        vmovdqu(xmm0, xword[AO - 0x78]);
        prefetcht0(byte[AO + 0x1c0]);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vmovdqu(xmm5, xword[BO - 0x78]);
        prefetcht1(byte[AA - 0x80]);
        vmovdqu(xmm0, xword[AO - 0x70]);
        add(AA, 0x4);
        add(AO, 0x10);
        add(BO, 0x8);
        sub(H, 0x1);
        jg(l17f4, T_NEAR);
        align(4);

        L(l1854);
        mov(H, K);
        test(H, 0x4);
        je(l187c, T_NEAR);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        add(AO, 0x8);
        add(BO, 0x4);
        align(4);

        L(l187c);
        mov(H, K);
        test(H, 0x2);
        je(l18bc, T_NEAR);
        vxorps(xmm6, xmm6, xmm6);
        vmovdqu(xmm1, xword[AO - 0x80]);
        vpunpcklwd(xmm0, xmm1, xmm6);
        vbroadcastss(xmm5, dword[BO - 0x80]);
        vpunpcklwd(xmm5, xmm5, xmm5);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        add(AO, 0x4);
        add(BO, 0x2);
        align(4);

        L(l18bc);
        mov(H, K);
        test(H, 0x1);
        je(l1904, T_NEAR);
        vxorps(xmm6, xmm6, xmm6);
        vbroadcastss(xmm0, dword[AO - 0x80]);
        vpunpcklbw(xmm0, xmm0, xmm6);
        vpunpcklwd(xmm0, xmm0, xmm6);
        vbroadcastss(xmm5, dword[BO - 0x80]);
        vpunpcklbw(xmm5, xmm5, xmm5);
        vpunpcklwd(xmm5, xmm5, xmm5);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        add(AO, 0x2);
        add(BO, 0x1);
        align(4);

        L(l1904);
        vmovsd(xmm0, qword[CO1]);
        vpaddd(xmm8, xmm8, xmm0);
        vmovlps(qword[CO1], xmm8);
        vxorps(xmm8, xmm8, xmm8);
        lea(CO1, ptr[CO1 + LDC * 1]);
        align(4);

        L(l191c);
        mov(A, AO);
        align(4);

        L(l1920);
        test(J, 0x1);
        jle(l1d84, T_NEAR);
        mov(CO1, C);
        add(C, 0x4);
        mov(BO, B);
        mov(AA, K);
        shl(AA, 0x2);
        lea(AA, ptr[A + AA * 1 + 0x200]);
        mov(I, N);
        cmp(I, 0x2);
        jl(l1bbc, T_NEAR);
        align(4);

        L(l1958);
        mov(AO, A);
        vmovdqu(xmm0, xword[AO - 0x80]);
        vmovdqu(xmm5, xword[BO - 0x80]);
        mov(H, K);
        sar(H, 0x3);
        jle(l1aa4, T_NEAR);
        sub(H, 0x8);
        jle(l1a08, T_NEAR);
        align(4);

        L(l1980);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        prefetcht0(byte[AO + 0x180]);
        prefetcht0(byte[BO]);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        vmovdqu(xmm0, xword[AO - 0x7c]);
        prefetcht0(byte[AO + 0x1c0]);
        vpshufd(xmm4, xmm5, 0xaa);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpshufd(xmm4, xmm5, 0xff);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        vmovdqu(xmm5, xword[BO - 0x70]);
        prefetcht1(byte[AA - 0x80]);
        vmovdqu(xmm0, xword[AO - 0x78]);
        add(AA, 0x4);
        add(AO, 0x8);
        add(BO, 0x10);
        sub(H, 0x1);
        jg(l1980, T_NEAR);
        align(4);

        L(l1a08);
        prefetcht0(byte[CO1 + 0x3c]);
        prefetcht0(byte[CO1 + LDC * 1 + 0x3c]);
        add(H, 0x8);
        jle(l1aa4, T_NEAR);
        align(4);

        L(l1a1c);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        prefetcht0(byte[AO + 0x180]);
        prefetcht0(byte[BO]);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        vmovdqu(xmm0, xword[AO - 0x7c]);
        prefetcht0(byte[AO + 0x1c0]);
        vpshufd(xmm4, xmm5, 0xaa);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpshufd(xmm4, xmm5, 0xff);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        vmovdqu(xmm5, xword[BO - 0x70]);
        prefetcht1(byte[AA - 0x80]);
        vmovdqu(xmm0, xword[AO - 0x78]);
        add(AA, 0x4);
        add(AO, 0x8);
        add(BO, 0x10);
        sub(H, 0x1);
        jg(l1a1c, T_NEAR);
        align(4);

        L(l1aa4);
        mov(H, K);
        test(H, 0x4);
        je(l1adc, T_NEAR);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        add(AO, 0x4);
        add(BO, 0x8);
        align(4);

        L(l1adc);
        mov(H, K);
        test(H, 0x2);
        je(l1b2c, T_NEAR);
        vxorps(xmm6, xmm6, xmm6);
        vmovdqu(xmm1, xword[AO - 0x80]);
        vpunpcklwd(xmm0, xmm1, xmm6);
        vbroadcastss(xmm5, dword[BO - 0x80]);
        vpunpcklwd(xmm5, xmm5, xmm5);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        add(AO, 0x2);
        add(BO, 0x4);
        align(4);

        L(l1b2c);
        mov(H, K);
        test(H, 0x1);
        je(l1b84, T_NEAR);
        vxorps(xmm6, xmm6, xmm6);
        vbroadcastss(xmm0, dword[AO - 0x80]);
        vpunpcklbw(xmm0, xmm0, xmm6);
        vpunpcklwd(xmm0, xmm0, xmm6);
        vbroadcastss(xmm5, dword[BO - 0x80]);
        vpunpcklbw(xmm5, xmm5, xmm5);
        vpunpcklwd(xmm5, xmm5, xmm5);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm9, xmm9, xmm6);
        add(AO, 0x1);
        add(BO, 0x2);
        align(4);

        L(l1b84);
        vmovss(xmm0, dword[CO1]);
        vpaddd(xmm8, xmm8, xmm0);
        vmovss(dword[CO1], xmm8);
        vxorps(xmm8, xmm8, xmm8);
        vmovss(xmm0, dword[CO1 + LDC * 1]);
        vpaddd(xmm9, xmm9, xmm0);
        vmovss(dword[CO1 + LDC * 1], xmm9);
        vxorps(xmm9, xmm9, xmm9);
        lea(CO1, ptr[CO1 + LDC * 2]);
        sub(I, 0x2);
        cmp(I, 0x2);
        jge(l1958, T_NEAR);
        align(4);

        L(l1bbc);
        test(I, 0x1);
        jle(l1d80, T_NEAR);
        mov(AO, A);
        vmovdqu(xmm0, xword[AO - 0x80]);
        vmovdqu(xmm5, xword[BO - 0x80]);
        mov(H, K);
        sar(H, 0x3);
        jle(l1cb8, T_NEAR);
        sub(H, 0x8);
        jle(l1c4c, T_NEAR);
        align(4);

        L(l1bec);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        prefetcht0(byte[AO + 0x180]);
        prefetcht0(byte[BO]);
        vmovdqu(xmm0, xword[AO - 0x7c]);
        prefetcht0(byte[AO + 0x1c0]);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vmovdqu(xmm5, xword[BO - 0x78]);
        prefetcht1(byte[AA - 0x80]);
        vmovdqu(xmm0, xword[AO - 0x78]);
        add(AA, 0x4);
        add(AO, 0x8);
        add(BO, 0x8);
        sub(H, 0x1);
        jg(l1bec, T_NEAR);
        align(4);

        L(l1c4c);
        prefetcht0(byte[CO1 + 0x3c]);
        add(H, 0x8);
        jle(l1cb8, T_NEAR);
        align(4);

        L(l1c58);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        prefetcht0(byte[AO + 0x180]);
        prefetcht0(byte[BO]);
        vmovdqu(xmm0, xword[AO - 0x7c]);
        prefetcht0(byte[AO + 0x1c0]);
        vpshufd(xmm4, xmm5, 0x55);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        vmovdqu(xmm5, xword[BO - 0x78]);
        prefetcht1(byte[AA - 0x80]);
        vmovdqu(xmm0, xword[AO - 0x78]);
        add(AA, 0x4);
        add(AO, 0x8);
        add(BO, 0x8);
        sub(H, 0x1);
        jg(l1c58, T_NEAR);
        align(4);

        L(l1cb8);
        mov(H, K);
        test(H, 0x4);
        je(l1ce0, T_NEAR);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        add(AO, 0x4);
        add(BO, 0x4);
        align(4);

        L(l1ce0);
        mov(H, K);
        test(H, 0x2);
        je(l1d20, T_NEAR);
        vxorps(xmm6, xmm6, xmm6);
        vmovdqu(xmm1, xword[AO - 0x80]);
        vpunpcklwd(xmm0, xmm1, xmm6);
        vbroadcastss(xmm5, dword[BO - 0x80]);
        vpunpcklwd(xmm5, xmm5, xmm5);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        add(AO, 0x2);
        add(BO, 0x2);
        align(4);

        L(l1d20);
        mov(H, K);
        test(H, 0x1);
        je(l1d68, T_NEAR);
        vxorps(xmm6, xmm6, xmm6);
        vbroadcastss(xmm0, dword[AO - 0x80]);
        vpunpcklbw(xmm0, xmm0, xmm6);
        vpunpcklwd(xmm0, xmm0, xmm6);
        vbroadcastss(xmm5, dword[BO - 0x80]);
        vpunpcklbw(xmm5, xmm5, xmm5);
        vpunpcklwd(xmm5, xmm5, xmm5);
        vpshufd(xmm4, xmm5, 0x0);
        vpmaddubsw(xmm6, xmm4, xmm0);
        vpmaddwd(xmm6, xmm7, xmm6);
        vpaddd(xmm8, xmm8, xmm6);
        add(AO, 0x1);
        add(BO, 0x1);
        align(4);

        L(l1d68);
        vmovss(xmm0, dword[CO1]);
        vpaddd(xmm8, xmm8, xmm0);
        vmovss(dword[CO1], xmm8);
        vxorps(xmm8, xmm8, xmm8);
        lea(CO1, ptr[CO1 + LDC * 1]);
        align(4);

        L(l1d80);
        mov(A, AO);
        align(4);

        L(l1d84);
        add(rsp, stack_alloc_size);
        postamble();
    }
    outLocalLabel();

#undef M
#undef N
#undef K
#undef A
#undef B
#undef C
#undef LDC
#undef AA
#undef I
#undef J
#undef H
#undef AO
#undef BO
#undef CO1
#undef CO2
#ifdef _WIN32
#undef ARG_A
#undef ARG_B
#endif
#undef ARG_C
#undef ARG_LDC
}

} // namespace x64
} // namespace cpu
} // namespace impl
} // namespace zendnn
