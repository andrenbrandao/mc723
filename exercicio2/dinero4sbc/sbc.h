/*
 * 	sbc.h
 *
 *	Variables for SBC format for DineroIV
 * 	Written by MM
 *	Revision 1.1 Initial
 */


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_SIZE 30000

typedef unsigned int long long address_t;
typedef int long long offset_t;
typedef char unsigned byte1_t;
typedef short unsigned byte2_t;
typedef short unsigned stream_t;
typedef unsigned int instr_t;


typedef struct node_ref
{
  offset_t current_addr;
  offset_t data_stride;
  int long long rep_count;
  struct node_ref *next;
} node_ref;

typedef struct node_word
{
  char unsigned instr_type;
  struct node_word *next;
} node_word;


 //extern stream_t  	StreamIndex, StreamLen[MAX_SIZE];
 extern stream_t  	StreamLen[MAX_SIZE];
 extern address_t  	StreamStart[MAX_SIZE],CurrentDataAddr;
 extern byte2_t 	MaxStreamIndex;		/* number of streams in the table */


 extern node_word* 	WordList[MAX_SIZE];
 extern node_ref*  	RefList[MAX_SIZE];
 extern node_ref* 	NewRef;

 extern byte1_t		StartRef, StartStream;	/* flags for memory reference and stream start */
 extern byte1_t		TempType;		/* temporary instr type */
 extern int		dataNum;		/* number of the Dtrace */
 extern FILE		*fsbcd;			/* SBC D trace file */


