/*
 * mm-implicit.c - an empty malloc package
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 *
 * @id : 202102697 
 * @name : JeonGyuri
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "mm.h"
#include "memlib.h"

/* If you want debugging output, use the following macro.  When you hand
 * in, remove the #define DEBUG line. */
#define DEBUG
#ifdef DEBUG
# define dbg_printf(...) printf(__VA_ARGS__)
#else
# define dbg_printf(...)
#endif


/* do not change the following! */
#ifdef DRIVER
/* create aliases for driver tests */
#define malloc mm_malloc
#define free mm_free
#define realloc mm_realloc
#define calloc mm_calloc
#endif /* def DRIVER */

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8
#define WSIZE 4 //word크기
#define DSIZE 8 //double word크기 - 최소 payload크기
#define CHUNKSIZE (1<<12) //초기 heap크기 설정
#define OVERHEAD 8 //heder + footer의 크기(실제 데이터가저장되는공간x 오버헤드가됨)
#define MAX(x,y) ((x) > (y) ? (x) : (y)) //x와 y중에서 더 큰 값
#define PACK(size, alloc) ((size) | (alloc)) //PACK매크로를 사용하여 size와 alloc의 값을 하나의 word로.
#define GET(p) (*(unsigned int*)(p)) //포인터가 가리키는 위치에서 word크기의 값을 읽는다
#define PUT(p, val) (*(unsigned int *)(p) = (val)) // 포인터가가리키는곳에 word크기의 val값을 쓴다
#define GET_SIZE(p) (GET(p) & ~0x7) //header에서 block size를 읽는 것
#define GET_ALLOC(p) (GET(p) & 0x1) // block의 할당 여부 false=0, true = 1
#define HDRP(bp) ((char *)(bp) - WSIZE) // 주어진 포인터 bp의 header주소 계산
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE) //주어진 bp포인터의footer주소 계산
#define NEXT_BLKP(bp) ((char*)(bp) + GET_SIZE((char*)(bp)-WSIZE)) //다음 block의 주소 계산
#define PREV_BLKP(bp) ((char*)(bp) - GET_SIZE((char*)(bp)-DSIZE)) //다이전 block의 주소 ㅖ계

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(p) (((size_t)(p) + (ALIGNMENT-1)) & ~0x7)

/*
 * Initialize: return -1 on error, 0 on success.
 */

static char *last_bp;

static char *heap_listp = 0;
static void *find_fit(size_t asize);
static void *coalesce(void *bp);
static void *extend_heap(size_t words);
static void place(void *bp, size_t asize);

int mm_init(void) {
	if ((heap_listp = mem_sbrk(4*WSIZE)) == NULL) return -1;

	PUT(heap_listp, 0);
	PUT(heap_listp+WSIZE, PACK(OVERHEAD, 1));
	PUT(heap_listp+DSIZE, PACK(OVERHEAD, 1));
	PUT(heap_listp+WSIZE+DSIZE, PACK(0, 1));
	heap_listp += DSIZE;

	if (extend_heap(CHUNKSIZE/WSIZE) == NULL) return -1;
    
	return 0;
}

static void *find_fit(size_t asize) {
	char *bp = last_bp;

	for (; GET_SIZE(HDRP(last_bp)) > 0; last_bp  = NEXT_BLKP(last_bp)) {
		if (!GET_ALLOC(HDRP(last_bp)) && GET_SIZE(HDRP(last_bp)) >= asize) {
			return last_bp;
		}
	}

	for (last_bp = heap_listp; last_bp < bp; last_bp = NEXT_BLKP(last_bp)) {
		if (!GET_ALLOC(HDRP(last_bp)) && (GET_SIZE(HDRP(last_bp)) >= asize)) {
			return last_bp;
		}
	}

	return NULL;
}

/*
 * malloc
 */
void *malloc (size_t size) {
    size_t asize;
	size_t extendsize;
	char *bp;

	if (size <= 0) return NULL;

	if (size <= DSIZE) asize = DSIZE * 2;
	else asize = DSIZE * ((size + DSIZE + (DSIZE-1)) / DSIZE);

	if ((bp = find_fit(asize)) != NULL) { // find fit
		//위에서 찾은 block을 place하수를 이용하여 가용블록 표시 및, 사이즈 표시
		place(bp, asize);
		last_bp = bp;
		return bp;
	}

	extendsize = MAX(asize, CHUNKSIZE);
	// find_fit함수로 블록을 찾지 못한 경우, 남은 heap에 가용공간이 없으므로 extend_heapㄹ을 호출하여 반환
	if ((bp = extend_heap(extendsize/WSIZE)) == NULL) return NULL;
	// extend_heap 호출로 반환된 block에 place함수를 호출
	place(bp, asize);

	last_bp = bp;
	return bp;
}

/*
 * free
 */
void free (void *ptr) {
    if(!ptr || ptr == 0) return;

	size_t size = GET_SIZE(HDRP(ptr)); //ptr의 헤더에서 block size를 읽어온다.

	// 실제로 데이터를 지우는 것이 아닌, header와 footer의 최하위 1bit(1, 할당된 상태)를 수정하여 간단하게 free가 가능
	PUT(HDRP(ptr),PACK(size,0)); // ptr의 header에 block size와 alloc = 0을 저장
	PUT(FTRP(ptr),PACK(size,0)); // ptr의 footer에 block size와 alloc = 0을 저

	coalesce(ptr); //ㅇ주위에 빈 블록이 있을 시 병합한다.
}

static void *coalesce(void *bp) {
	
	size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
	// 이전 블럭의 할당 여부 0 = No, 1 = YES
	size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
	// 다음 블럭의 할다 ㅇ여부
	size_t size = GET_SIZE(HDRP(bp));
	//현재 블럭의 크기
	
	// case1: 이전 블럭, 다음 블럭 최하위 bit가 둘다 1인 경우 (할당) 블럭 병합 없이 bp return
	if (prev_alloc && next_alloc) {
		last_bp = bp;
		return bp;
	}
	// case2: 이전 ㅡ블럭 최하위비트가 1이고(할당), 다음 블럭 최하위 bit가 0인 경우(비할당) 다음 블럭과 병합한 뒤 bp return
	if (prev_alloc && !next_alloc) {
		size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
		PUT(HDRP(bp),PACK(size,0));
		PUT(FTRP(bp), PACK(size,0));
	// case3: 이전 블럭 최하위 bit가 0이고(비할당), 다음블럭 최하위 비트가 1인경ㅇ우(할당)
	// 이전 블럭과 병ㅎ합한 뒤 새로운 bp return
	}else if(!prev_alloc && next_alloc) {
		size += GET_SIZE(HDRP(PREV_BLKP(bp)));
		PUT( FTRP(bp), PACK(size,0) );
		PUT( HDRP(PREV_BLKP(bp)), PACK(size, 0) );
		bp = PREV_BLKP(bp);
	// case4: 이전 블럭 최하위 bit가 0(비할당), 다음 블럭 최하위 비트가 0인경우(비할당)
	// 이전블럭, 현재블럭, 다음 블럭을 ㅂ모두 병합한 뒤 새롱누 bp return;
	} else {
		size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(FTRP(NEXT_BLKP(bp)));
		PUT(HDRP(PREV_BLKP(bp)), PACK(size,0));
		PUT(FTRP(NEXT_BLKP(bp)), PACK(size,0));

		bp = PREV_BLKP(bp);
	}
	last_bp = bp;
	/*
	if (heap_cur > bp) heap_cur = bp;
	*/
	return bp;

}

/*
 * realloc - you may want to look at mm-naive.c
 */
void *realloc(void *oldptr, size_t size) {
	size_t oldsize;
	void* newptr;

	if (!oldptr) return malloc(size);

	if (size == 0) {
		free(oldptr);
		return 0;
	}

	newptr = malloc(size);

	if (!newptr) return 0;

	oldsize = GET_SIZE(HDRP(oldptr));
	if (size < oldsize) oldsize = size;

	memcpy(newptr, oldptr, oldsize);

	free(oldptr);

    return newptr;
}

static void place(void *bp, size_t asize) {
	
	size_t current_size  = GET_SIZE(HDRP(bp));

	if ((current_size - asize) >= (DSIZE + OVERHEAD)) {
		PUT(HDRP(bp), PACK(asize, 1));
		PUT(FTRP(bp), PACK(asize, 1));

		bp = NEXT_BLKP(bp);

		PUT(HDRP(bp), PACK(current_size - asize, 0));
		PUT(FTRP(bp), PACK(current_size - asize, 0));
	} else {
		PUT(HDRP(bp), PACK(current_size ,1));
		PUT(FTRP(bp), PACK(current_size, 1));
	}
}


/*
 * calloc - you may want to look at mm-naive.c
 * This function is not tested by mdriver, but it is
 * needed to run the traces.
 */
void *calloc (size_t nmemb, size_t size) {
    return NULL;
}

static void *extend_heap(size_t words) {
	  char *bp;
      size_t size = (words % 2) ? ((words + 1)*WSIZE) : (words *WSIZE);

      if ((long)(bp = mem_sbrk(size)) == -1) return NULL;

      PUT(HDRP(bp), PACK(size,0));
      PUT(FTRP(bp), PACK(size,0));
      PUT(HDRP(NEXT_BLKP(bp)), PACK(0,1));

     return coalesce(bp);
}

/*
 * Return whether the pointer is in the heap.
 * May be useful for debugging.
 */
static int in_heap(const void *p) {
    return p < mem_heap_hi() && p >= mem_heap_lo();
}

/*
 * Return whether the pointer is aligned.
 * May be useful for debugging.
 */
static int aligned(const void *p) {
    return (size_t)ALIGN(p) == (size_t)p;
}

/*
 * mm_checkheap
 */
void mm_checkheap(int verbose) {
}
