/*
 * SBC format handling for Dinero IV (SBC is a compressed format)
 * with 64-bit addresses
 *
 *  Written by MM (modified binfmt,c from Dinero suite)
 *
 *  Note: buffered records
 *
 *  Rev. 1.2 - 	read stream table with instruction words,
 *		just skip them
 *
 *  Rev. 1.0 - initial
 */

#include <stddef.h>
#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "d4.h"
#include "cmdd4.h"
#include "tracein.h"
#include "cmdargs.h"
#include "sbc.h"

#define READ_SIZE 8192
//#define READ_SIZE 4096

/* This format has STF, SBIT, and STF files */

/* global variables for SBC */

 stream_t  	StreamLen[MAX_SIZE];
 address_t  	StreamStart[MAX_SIZE], CurrentDataAddr;
 byte2_t 	MaxStreamIndex;		/* number of streams in the table */

 node_word* 	WordList[MAX_SIZE];
 node_ref*  	RefList[MAX_SIZE];
 node_ref* 	NewRef;

 byte1_t	StartRef, StartStream;	/* flags for memory reference and stream start */
 byte1_t	TempType;		/* temporary instr type */
 int		dataNum;		/* number of the Dtrace */
 FILE		*fsbcd;			/* SBC D trace file */


void my_alloc(void);
void read_table(void);
void read_data(void);

/************************************************************************/
/* my_alloc */

void my_alloc(void)
{
    node_word* NewWord;
    static node_word* PrevWord = NULL;
    static node_ref* PrevRef = NULL;

    /* alloc new word */
    NewWord = malloc(sizeof(struct node_word));
    NewWord->next = NULL;
    NewWord->instr_type = TempType;

    if (StartStream)
    {   WordList[MaxStreamIndex] = NewWord;
        StartStream = 0;
    }
    else
    {   PrevWord->next=NewWord;
    }
    PrevWord = NewWord;

    /* alloc new mem ref */
    if (TempType !=2)
    {
        NewRef = malloc(sizeof(struct node_ref));
	NewRef->next = NULL;
	NewRef->current_addr = 0;
	NewRef->data_stride = 0;
	NewRef->rep_count = 0;

	if (StartRef)
	{   RefList[MaxStreamIndex] = NewRef;
	    StartRef = 0;
	}
	else
	{     PrevRef->next=NewRef;
	}
	PrevRef = NewRef;
    }
}

/*************************************************************/
/* read sbc table */

void read_table(void)
{
    int     	i, k;			/* loop index */
    char 	sys_command[80]; 	/* sys command buffer */
    FILE 	*ftable_b; 		/* binary sbc table file */
    byte1_t 	InstrType;		/* instruction type byte */
    instr_t	InstrWord;		/* instruction word */

    sprintf(sys_command, "gzip -cd tblID.%s.I.bin.gz", tracename);

    if ( (ftable_b = popen(sys_command, "r")) == NULL)
    {
	   fprintf(stderr, "error popen\n");
	   exit(1);
    }

    MaxStreamIndex = 1;

    while ( fread(&StreamLen[MaxStreamIndex],sizeof(stream_t),1,ftable_b) == 1 )
    {
        fread(&StreamStart[MaxStreamIndex],sizeof(address_t),1,ftable_b);
        StartRef = 1;
	StartStream = 1;

        for (k=1;k<=StreamLen[MaxStreamIndex]/4;++k)
	{
	    fread(&InstrType,sizeof(byte1_t),1,ftable_b);
	    for (i=3;i>=0;--i)
	    {
	        TempType=InstrType>>(2*i);
	        TempType = TempType & 0x03;

		my_alloc();
	    }
	}

	if (StreamLen[MaxStreamIndex]%4 != 0)
	   fread(&InstrType,sizeof(byte1_t),1,ftable_b);

	for (k=StreamLen[MaxStreamIndex]%4;k>=1;--k)
	{
	    TempType=InstrType>>(2*(k-1));
	    TempType = TempType & 0x03;

	    my_alloc();
	}

	for(i=1;i<=StreamLen[MaxStreamIndex];++i)
	{
	    fread(&InstrWord,sizeof(instr_t),1,ftable_b);
	}
	++MaxStreamIndex;
    } /*end read table*/


    if ( pclose(ftable_b) == -1 )
    {	fprintf(stderr,"error pclose table\n");
	exit(1);
    }
}

/***********************************************************/
/* read data */

void read_data(void)
{
    static unsigned char 	inbufD[READ_SIZE];	/* input buffer */
    static int 			inptrD = 0;		/* input buffer pointer */
    static int 			hiwater = 0;		/* shows the end of buffer */
    byte1_t 			HeaderByte;		/* header byte */
    char 			h_addr_offset,		/* header fields */
    				h_data_stride, h_rep_count;
    char 			sys_command[80]; 	/* sys command buffer */
    int 			nread;			/* number of bytes read */
    int 			recordSize;		/* size of variable record */
    int				i;			/* loop index */
    int 			temp;			/* temporary storage */

    /* variables for different field lengths */
    char 	tchar;
    short 	tshort;
    int 	tint;
    int long long tllint;

    if (NewRef->rep_count == 0)
    {
	NewRef->current_addr = NewRef->current_addr - NewRef->data_stride; //bug corrected
	
	/* first check the buffer */
	if (inptrD > hiwater - 1)	/* need at least 1 byte for header */
	{
	    /* read from sbc data file */
	    inptrD = 0;
	    nread = fread (&inbufD[inptrD], 1, sizeof(inbufD), fsbcd);
	    hiwater = nread;

	    if (nread < 0)
	    die ("binary input error: %s\n", strerror (errno));

	    while (nread == 0)
	    {
		if ( pclose(fsbcd) == -1 )
		    die("pclose error\n");

		/* open new one */

		++dataNum;
		fprintf(stderr, "reading sbc D trace #%d\n", dataNum);
		sprintf(sys_command, "gzip -cd C3.%s.D.%d.bin.gz", tracename, dataNum);
  	 	if ( (fsbcd = popen(sys_command, "r")) == NULL)
		    die("error popen sbcd\n");

		/* read */
		inptrD = 0;
	 	nread = fread (&inbufD[inptrD], 1, sizeof(inbufD), fsbcd);
	    	hiwater = nread;
	    } /* end while nread = 0 */
	} /* end if reached end of input buffer*/

	/* read header from buf */
	HeaderByte = inbufD[inptrD++];
	h_addr_offset = HeaderByte & 0x03;
	h_data_stride = (HeaderByte>>2) & 0x07;
	h_rep_count = (HeaderByte>>5) & 0x07;

	/* calculate record size */
	recordSize = 0;

	temp = 1;
	for (i=1; i<=h_addr_offset; ++i)
	    temp = temp << 1;
	recordSize += temp;

	temp = 1;
	if (h_data_stride < 5)
	{
	    for (i=2; i<=h_data_stride; ++i)
	        temp = temp << 1;
	    if (h_data_stride > 0)
	        recordSize += temp;
	}

        temp = 1;
	if (h_rep_count < 5)
	{
	    for (i=2; i<=h_rep_count; ++i)
	        temp = temp << 1;
	    if (h_rep_count > 0)
	        recordSize += temp;
	}

	if (inptrD > hiwater - recordSize)
	{
	    if (hiwater > inptrD)
	    {
	    	memcpy (inbufD, &inbufD[inptrD], hiwater-inptrD);
		inptrD = hiwater - inptrD;
	    }
	    else
		inptrD = 0;

	    nread = fread(&inbufD[inptrD], 1, sizeof(inbufD)-inptrD, fsbcd);
	    if (nread <= 0) /* impossible here */
	        die("error reading sbcd\n");
	    hiwater = nread + inptrD;
	    inptrD = 0;
	}

	/* now read fields from buffer */
	switch (h_addr_offset)
	{
	    case 0:
		tchar = inbufD[inptrD++];
		NewRef->current_addr = NewRef->current_addr + tchar;
		break;
            case 1:
	   	tshort = inbufD[inptrD+1];
		tshort = (tshort << 8)|inbufD[inptrD];
		inptrD += 2;
		NewRef->current_addr = NewRef->current_addr + tshort;
		break;
 	    case 2:
	        tint = inbufD[inptrD + 3];
		for (i=2;i>=0;--i)
		    tint = (tint<<8) | inbufD[inptrD+i];
                inptrD += 4;
		NewRef->current_addr = NewRef->current_addr + tint;
		break;
            case 3:
	    	tllint = inbufD[inptrD + 7];
		for (i=6;i>=0;--i)
		    tllint = (tllint<<8) | inbufD[inptrD+i];
		inptrD += 8;
		NewRef->current_addr = NewRef->current_addr + tllint;
		break;
  	    default:
       		die("should not be here\n");
  		break;
 	}

	switch(h_data_stride)
 	{
  	    case 0:
		NewRef->data_stride = 0;
		break;
  	    case 1:
		tchar = inbufD[inptrD++];
		NewRef->data_stride = tchar;
		break;
            case 2:
		tshort = inbufD[inptrD+1];
		tshort = (tshort << 8)|inbufD[inptrD];
		inptrD += 2;
		NewRef->data_stride = tshort;
		break;
  	    case 3:
	        tint = inbufD[inptrD + 3];
		for (i=2;i>=0;--i)
		    tint = (tint<<8) | inbufD[inptrD+i];
                inptrD += 4;
		NewRef->data_stride = tint;
		break;
  	    case 4:
	    	tllint = inbufD[inptrD + 7];
		for (i=6;i>=0;--i)
		    tllint = (tllint<<8) | inbufD[inptrD+i];
		inptrD += 8;
		NewRef->data_stride = tllint;
		break;
	    case 5:
		NewRef->data_stride = 1;
		break;
	    case 6:
		NewRef->data_stride = 4;
		break;
	    case 7:
		NewRef->data_stride = 8;
		break;
            default:
       	        fprintf(stderr, "should not be here\n");
		exit(1);
  		break;
 	}

	switch(h_rep_count)
 	{
  	    case 0:
                NewRef->rep_count = 0;
		break;
  	    case 1:
		tchar = inbufD[inptrD++];
		NewRef->rep_count = tchar;
		break;
            case 2:
		tshort = inbufD[inptrD+1];
		tshort = (tshort << 8)|inbufD[inptrD];
		inptrD += 2;
		NewRef->rep_count = tshort;
		break;
  	    case 3:
		tint = inbufD[inptrD + 3];
		for (i=2;i>=0;--i)
		    tint = (tint<<8) | inbufD[inptrD+i];
                inptrD += 4;
		NewRef->rep_count = tint;
		break;
  	    case 4:
	    	tllint = inbufD[inptrD + 7];
		for (i=6;i>=0;--i)
		    tllint = (tllint<<8) | inbufD[inptrD+i];
		inptrD += 8;
		NewRef->rep_count = tllint;
		break;
	    case 5:
		NewRef->rep_count = 1;
		break;
            default:
       	        fprintf(stderr, "should not be here\n");
		exit(1);
  		break;
 	}
	++NewRef->rep_count;


    } /* end if rep_count = 0 */

}

/********************************************************************************/
d4memref
tracein_sbc()
{
	static unsigned char 	inbufI[READ_SIZE];	/* input buffer */
	static int 		inptrI = READ_SIZE;	/* input buffer pointer */
	static int 		hiwater = READ_SIZE;	/* shows the end of buffer */
	d4memref 	r;			/* record */
	static char 	firstTrace = 1;		/* first trace flag */
	static FILE	*fsbci;			/* SBC I trace file */
	static int	instNum = 1;		/* number of the Itrace */
	char 		sys_command[80]; 	/* sys command buffer */
	int 		nread;			/* number of bytes read */
	static	int	needData = 0;		/* we want a data address */
	static	int	tempLen = 0;		/* stream len left to process */
	static	address_t	CurrentInstAddr = 0;	/* current instruction address */
	static node_word* 	NewWord = NULL;		/* instruction to process */
	stream_t  	StreamIndex;		/* index of a current stream */


        if (!needData) /* an instruction address */
	{
	    if (tempLen == 0) /* end of stream - need a new one */
	    {
		if (inptrI > (hiwater - sizeof(stream_t))) /* need to fill inbuf */
	        {
	            if (firstTrace)
		    {
		        firstTrace = 0;

		    	read_table();

		    	fprintf(stderr, "reading sbc I trace #%d\n", instNum);

		    	sprintf(sys_command, "gzip -cd T1.%s.I.%d.bin.gz", tracename, instNum);
                    	if ( (fsbci = popen(sys_command, "r")) == NULL)
		            die("error popen sbci\n");

			dataNum = 1;
			fprintf(stderr, "reading sbc data trace #%d\n", dataNum);
    	    		sprintf(sys_command, "gzip -cd C3.%s.D.%d.bin.gz", tracename, dataNum);

    	    		if ( (fsbcd = popen(sys_command, "r")) == NULL)
			    die("error popen sbcd\n");

	  	    } /* end if first trace */

	            inptrI = 0;
		    nread = fread (&inbufI[inptrI], 1, sizeof(inbufI), fsbci);
		    hiwater = nread;

		    if (nread < 0)
			die ("binary input error: %s\n", strerror (errno));

		    if (nread == 0)
		    {
		        if ( pclose(fsbci) == -1 )
			    die("pclose error\n");

		        if (instNum == maxtrace) /* end of trace */
		        {
			    if ( pclose(fsbcd) == -1 )
			    die("pclose error\n");

			    r.accesstype = D4TRACE_END;
			    r.address = 0;
			    r.size = 0;
			    return r;
		        }
		        /* else open new one */
		        ++instNum;
		    	fprintf(stderr, "reading sbc I trace #%d\n", instNum);
		    	sprintf(sys_command, "gzip -cd T1.%s.I.%d.bin.gz", tracename, instNum);
                    	if ( (fsbci = popen(sys_command, "r")) == NULL)
		            die("error popen sbci\n");

			inptrI = 0;
		        nread = fread (&inbufI[inptrI], 1, sizeof(inbufI), fsbci);
			if (nread <= 0) die("no more I traces\n");
		        hiwater = nread;
		    } /* end if nread = 0 */
	        } /* end if reached end of input buffer*/

		StreamIndex = inbufI[inptrI+1];
		StreamIndex = (StreamIndex << 8) | inbufI[inptrI];

		inptrI += 2;
		CurrentInstAddr = StreamStart[StreamIndex];
		tempLen = StreamLen[StreamIndex];
	        NewWord = WordList[StreamIndex];
	        NewRef = RefList[StreamIndex];

	    } /* end if tempLen = 0 */

	        r.size = 4;
		r.accesstype = D4XINSTRN;
		r.address = CurrentInstAddr;

		--tempLen;
		CurrentInstAddr = CurrentInstAddr + 4;
		if ( NewWord->instr_type != 2) /* next is a data address */
		{
		    TempType = NewWord->instr_type;
		    needData = 1;
		}
		NewWord = NewWord->next;
	}/* end if instr address needed */
	else
	{
	    needData = 0;
	    read_data();
	    r.address = NewRef->current_addr;
	    r.accesstype = TempType;
	    r.size = 4; /* TODO check this  - approximation ? */

	    --NewRef->rep_count;
	    NewRef->current_addr = NewRef->current_addr + NewRef->data_stride;
            NewRef = NewRef->next;

	}

	return r;
}

