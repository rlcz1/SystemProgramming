/*
 * 202102697 Jeon gyuri:
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* Copyright (C) 1991-2020 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 10.0.0.  Version 10.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2017, fifth edition, plus
   the following additions from Amendment 1 to the fifth edition:
   - 56 emoji characters
   - 285 hentaigana
   - 3 additional Zanabazar Square characters */
/* 
 * bitNor - ~(x|y) using only ~ and & 
 *   Example: bitNor(0x6, 0x5) = 0xFFFFFFF8
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
*/
int bitNor(int x, int y) {
	return (~x) & (~y);  // 드모르간의 법칙을 이용해 a nor b == a' and b'를 구현
}
/* 
 * allEvenBits - return 1 if all even-numbered bits in word set to 1
 *   Examples allEvenBits(0xFFFFFFFE) = 0, allEvenBits(0x55555555) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allEvenBits(int x) {
	int bit_mask = 0x55;  // 짝수를 비교할 0101 0101 bitmask
	int result;  // 결과값을 저장할 0101 0101 
	
	bit_mask = (bit_mask << 8) | bit_mask; // 0101 0101 0000 0000으로 만들고, or 연산으로 0101 0101 0101 0101이 되게 함
    bit_mask = (bit_mask << 16) | bit_mask; // 0101 0101 0101 0101 0000 0000 0000 0000으로 만들고, or연산으로 0101 0101 0101 0101 0101 0101 0101 0101이 되게 함

	result = x & bit_mask; // 홀수는 다 0으로 만듬
	result = result ^ bit_mask; // 다시 bitMask와 xor연산을 통해 비교해서 다 0이 되지 않으면 모든 짝수가 1이면 0000 0000 0000 0000 0000 0000 0000 0000 반환

	return !result; // 다 0이면 모든 짝수가 1인것이니 1반환, 다 0이 아니면 0반환
}
/* 
 * float_abs - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_abs(unsigned uf) {
	// 0XFF800000보다 작거나 같으면 NaN이나 무한대가 아닌것
	if (uf <= 0XFF800000) uf = uf & 0x7FFFFFFF;  // 부호비트를 제외한 모든 비트가 1인  0111 1111 1111 1111 1111 1111 1111 1111인 0x7FFFFFFF와의 and연산을 통해 입력값의 부호를 0으로 만들어 양수로 만든다.
	return uf;  
}
/* 
 * isGreater - if x > y  then return 1, else return 0 
 *   Example: isGreater(4,5) = 0, isGreater(5,4) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isGreater(int x, int y) {
	int diff = x + ~y;  // x와 y의 차이 값

    int diff_msb = diff >> 31;  // diff(x와 y차이)의 부호 저장(0이면 x가 y보다 크    거나 같을때, 1이면 x가 y보다 작을때)
    int x_msb = x >> 31;  // x의 부호 저장(양수일때 0, 음수일때 1)
    int y_msb = y >> 31;  // y의 부호 저장(양수일떄 0, 음수일때 1)

    int neg_x = x_msb & (!y_msb);  // x_msb는 x가 음수이면1, !y_msb는 y가 양수이>    면 1이다. &연산을 하면 x가 음수이고, y가 양수일때만 1이다.

    int check_msb = !(x_msb ^ y_msb);  // x와 y부호 비트를 비교해서 같으면 0 다르면 1을 반환하는데, !연산자를 통해 반대로 리턴한다. 즉, x와 y의 부호가 같아야 1이>    다.
    int x_bigger_0 = check_msb & diff_msb;  // check_msb는 x와 y의 부호가 같으면     1, diff_msb는 x와 y의 차이인 diff부호가 양수면 0이고 음수면 1이다. &연산을 통해 >    둘다 만족할때만 1이다. 즉, x_bigger_0은 부호가 양수면서 x가 y보다 클때 0이된다.
    // neg_x가 1이면 x가 음수이고 y가 양수이므로 x가 더 작은것
    // x_bigger가 1이면 둘다 양수면서 x가 y보다 큰 것.
    // neg_x=1, x_bigger=0이면 neg_x를 통해 x가 y보다 작음을 알 수 있음.
    // neg_x=0, x_bigger=1이면 x_bigger를 통해 x가 y보다 작음을 알 수 있음.
    // or연산자는 둘중 하나라도 참이면 참.
    // x가 y보다 클때 0을 반환하므로 값 반전.
    return !(neg_x | x_bigger_0);
}
/* 
 * addOK - Determine if can compute x+y without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int addOK(int x, int y) {
	// 덧셈에서 overflow가 발생할때
	// 1. 둘다 양수인데 결과값이 음수일때(x_msb = 0, y_msb = 0, x+y_msb = 1)
	// 2. 둘다 양수인데 결과값이 양수일때(x_msb = 1, y_msb = 1, x+y_msb = 0)
	int x_msb = x >> 31;  // x의 부호 저장(0일땐 양수, 1일떈 음수)
	int y_msb = y >> 31;  // y의 부호 저장(0일땐 양수, 1일땐 음수)

	int xpy_msb = ( x + y ) >> 31;  // x+y의 부호 저장(0일땐 양수, 1일땐 음수)

	int case1 = (!x_msb & !y_msb) & xpy_msb;  // 1번계산 
	int case2 = (x_msb & y_msb) & !xpy_msb;  // 2번게산
	
 	return !(case1 | case2);  // 둘다 overflow발생시 1반환하므로 !연산자로 반전
}
/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
    int minus_x = ~x + 1;  // x의 2의 보수
    int lsb = x & (minus_x);  // x와 x의 2의보수를 and연산을 하면, x의 최하위비트만 남고 나머지는 0이된다.
    int msb = ~lsb + 1;  // lsb의 2의 보수를 구한다.
    msb = ~(msb >> 31);  // lsb의 2의보수의 부호비트만 남기고, x의 부호비트를 반전시킨다. 0이라면 1111...1111이 된다.
    // 즉 x가 0이면 msb가 -1이 되고, 그 외에는 0이 됨. 

    return msb & 1;  // &1을 해서 최하위 비트만남겨서 0 또는 1만 반환
}
