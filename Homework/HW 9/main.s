	////////////////////////////
	// version 1.3 11/22/2021 //
	////////////////////////////
	.arch armv6				// armv6 architecture
	.arm					// arm 32-bit instruction set
	.fpu vfp				// floating point co-processor
	.syntax unified				// modern sytax

	// function import
	.extern encrypt
	.extern decrypt
	.extern stderr
	.extern fprintf
	.extern fclose
	.extern setup
	.extern encryptdelete

	// global constants
	.include "encrypter.h"

	.section .rodata
.Lmsg1:	.string "Write failed on output\n"
.Lmsg2:	.string "Bookfile is too short for message\n"
	.text

	//////////////////////////////////////////////////////
	// int main(int argc, char **argv)                  //
	// encrypter [-d | -e] -b bookfile encryption_file  //
	//////////////////////////////////////////////////////

	.global main				// main global for linking to
	.type	main, %function			// define as a function
	.equ	FP_OFF,		32		// fp offset in main stack frame
	.equ 	BUFSZ,		1024		// max for assignment is 4096 min is 1024

	//////////////////////////////////////////////////////////////////////////////
	// automatics (local variable) frame layout
	// NOTICE! odd # of regs pushed, Not 8-byte aligned at FP_OFF; add 4 bytes pad
	// 
	// local stack frame name are used with fp as base
	// format is .equ VAR_NAME, NAME_OF_PREVIOUS_VARIABLE + <size of variable>
	// first variable should use  FP_OFF as the previous variable
	//////////////////////////////////////////////////////////////////////////////
	.equ	FPBOOK,		FP_OFF+4	// FILE * to book file
	.equ	FPIN,		FPBOOK+4	// FILE * to input file
	.equ	FPOUT,		FPIN+4		// FILE * to output file
	.equ	MODE,		FPOUT+4		// decrypt or encrypt mode
	.equ	IOBUF,		MODE+BUFSZ	// buffer for input file
	.equ	BOOKBUF,	IOBUF+BUFSZ	// buffer for book file
	// add local variables here: Then adjust PAD or comment out pad line as needed 
	.equ    READCNT, 	BOOKBUF+4	// readcnt local variable
	.equ 	POS,		READCNT+4	// pos local variable
	.equ	TOREAD,		POS + 4		//toread local variable
	.equ	BYTES,		TOREAD + 4	//bytes local variable
	.equ	PAD,		BYTES+4		// Stack frame PAD if needed goes here
	.equ	OUT6,		PAD+4		// output arg6
	.equ	OUT5,		OUT6+4		// output arg5 must be at bottom
	.equ	FRAMESZ,	OUT5-FP_OFF	// total space for automatics
	//////////////////////////////////////////////////////////////////////////////
	// make sure that FRAMESZ + FP_OFF + 4 divides by 8 EVENLY!
	//////////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////////
	// passed arg offsets used with sp as the base
	//////////////////////////////////////////////////////////////////////////////
	.equ	OARG6,		4		// Outgoing arg 6		
	.equ	OARG5,		0		// Outgoing arg 5		

main:
	// function prologue
	push	{r4-r10, fp, lr}		// WARNING! odd count register save!
	add	fp, sp, FP_OFF			// set frame pointer to frame base
	ldr r4, =FRAMESZ			// if frame size is too big, use pseudo ldr
	sub sp, sp, r4 			    // allocate space for locals and passed args 

	//initial some important stack variable.
	mov r5, 0
	//FPBOOK=0
	ldr r4, =FPBOOK	
	sub r4, fp, r4	
	str r5, [r4]
	//FPIN=0
	ldr r4, =FPIN	
	sub r4, fp, r4	
	str r5, [r4]
	//FPOUT=0
	ldr r4, =FPOUT	
	sub r4, fp, r4	
	str r5, [r4]

	// call int setup(argc, argv, &mode, &FPBOOK, &FPIN, &FPOUT)
	// set up for main loop
	// r2 = &mode
	ldr r2, =MODE				
	sub r2, fp, r2				
	// r3 = &FPBOOK
	ldr r3, =FPBOOK				
	sub r3, fp, r3				
	// push(&FPOUT)
	ldr r4, =FPOUT				
	sub r4, fp, r4				
	push {r4}
	// push(&FPIN)
	ldr r4, =FPIN				
	sub r4, fp, r4
	push {r4}
	bl setup

	//setup failed check
	cmp r0, EXIT_OK
	bne .Ldone

	// main loop
.Lloop:							
	// read the input 
	// r0 = IOBUF
	ldr r0, =IOBUF				
	sub r0, fp, r0				
	mov r0, r0
	//r1 = 1
	mov r1, 1		
	//r2 = BUFSZ			
	ldr r2, =BUFSZ	
	//r3 = FPIN			
	ldr r3, =FPIN				
	sub r3, fp, r3
	ldr r3, [r3]
	bl fread

	//readcnt = r0
	ldr r4, =READCNT	
	sub r4, fp, r4	
	str r0, [r4]	
	
	//compare readcnt and 0
	cmp r0, 0
	beq .Ldone

	//pos = 0
	ldr r4, =POS	
	sub r4, fp, r4	
	mov r5, 0
	str r5, [r4]

	//toread = readcnt, r0 is still readcnt now.
	ldr r4, =TOREAD	
	sub r4, fp, r4	
	str r0, [r4]

	// now read the book the same number of chars
.Lloop1:
	//r0 = &bookbuf[pos]
	ldr r5, =BOOKBUF				
	sub r5, fp, r5					
	ldr r4, =POS					
	sub r4, fp, r4
	ldr r4, [r4]
	add r5, r5, r4
	mov r0, r5
	//r1 = 1
	mov r1, 1
	//r2 = toread						
	ldr r2, =TOREAD					
	sub r2, fp, r2				
	ldr r2, [r2]
	//r3 = FPBOOK
	ldr r3, =FPBOOK
	sub r3, fp, r3				
	ldr r3, [r3]
	bl fread

	//bytes = r0
	ldr r4, =BYTES			
	sub r4, fp, r4	
	str r0, [r4]

	//check bytes >0, r0 is still bytes
	cmp r0, 0
	beq .Ldone1

	//pos += bytes, r0 is still bytes
	ldr r4, =POS
	sub r4, fp, r4	
	ldr r5, [r4]
	add r0, r0, r5 
	str r0, [r4]

	//if pos == readcnt, r0 is still pos
	ldr r4, =READCNT			
	sub r4, fp, r4	
	ldr r4, [r4]
	cmp r0, r4
	beq	.Ldone1
	
	//toread -=  bytes
	ldr r4, =TOREAD			
	sub r4, fp, r4
	ldr r6, [r4]
	ldr r5, =BYTES			
	sub r5, fp, r5
	ldr r5, [r5]
	sub r6, r6, r5
	str r6, [r4]
	b .Lloop1
.Ldone1:
	//if bytes == 0:
	ldr r4, =BYTES			
	sub r4, fp, r4
	ldr r4, [r4]
	cmp r4, 0
	bne .Lcontinue_process
	
	//fprintf(stderr, "Bookfile is too short for message\n");
	ldr r0, =stderr
	ldr r0, [r0]
	ldr r1, =.Lmsg2
	bl fprintf
	//exit with EXIT_FAIL
	ldr r0, =EXIT_FAIL 
	b .Ldone

.Lcontinue_process:
	// Both buffers are full, process the input
	//r10 = readcnt
	ldr r10, =READCNT			
	sub r10, fp, r10	
	ldr r10, [r10]
	//r9 = MODE
	ldr r9, =MODE			
	sub r9, fp, r9	
	ldr r9, [r9]
	//r8 = j = 0
	mov r8, 0

	// based on mode: either encrypt the input or decrypt the input
.Lprocesloop:
	//if r10 == 0
	cmp r10, 0
	beq .Lprocesend

	//r0 = iobuf[j]
	ldr r0, =IOBUF			
	sub r0, fp, r0
	add r0, r8
	ldrb r0, [r0]
	//r1 = bookbuf[j]
	ldr r1, =BOOKBUF			
	sub r1, fp, r1
	add r1, r8
	ldrb r1, [r1]
	
	//DECRYPT_MODE:0
	//ENCRYPT_MODE:1
	cmp r9, DECRYPT_MODE
	bne .Lencrypt
	bl decrypt
	b .Lprocesloopcontinue
.Lencrypt:
	bl encrypt
.Lprocesloopcontinue:

	//store the result.
	ldr r4, =IOBUF			
	sub r4, fp, r4
	add r4, r8
	strb r0, [r4]

	//r10--
	sub r10, r10, 1
	//r8++
	add r8, r8, 1
	b .Lprocesloop
.Lprocesend:
	// write out the i/o buffer as all chars are processed
	
	//r0 = iobuf
	ldr r0, =IOBUF			
	sub r0, fp, r0
	//r1 = 1
	mov r1, 1
	//r2 = r4 = readcnt
	ldr r4, =READCNT			
	sub r4, fp, r4
	ldr r4, [r4]
	mov r2, r4
	//r3 = FPOUT
	ldr r3, =FPOUT			
	sub r3, fp, r3
	ldr r3, [r3]
	bl fwrite
	//r0 == readcnt
	cmp r0, r4
	beq .Lloop
	//fprintf(stderr, ""Write failed on output\n");
	ldr r0, =stderr
	ldr r1, =.Lmsg1
	bl fprintf
	//exit with EXIT_FAIL
	ldr r0, =EXIT_FAIL 
	b .Ldone
	// end of loop
.Ldone:
	//back up r0
	mov r10, r0

	// close the files using fclose()
	// fclose if not empty
.LcloseFPBOOK:
	ldr r0, =FPBOOK	
	sub r0, fp, r0	
	ldr r0, [r0]
	cmp r0, 0
	beq .LcloseFPIN
	bl fclose
.LcloseFPIN:
	ldr r0, =FPIN	
	sub r0, fp, r0	
	ldr r0, [r0]
	cmp r0, 0
	beq .LcloseFPOUT
	bl fclose
.LcloseFPOUT:
	ldr r0, =FPOUT	
	sub r0, fp, r0	
	ldr r0, [r0]
	cmp r0, 0
	beq .Lout_close
	bl fclose
.Lout_close:

	//restore r0
	mov r0, r10

	// if encrypt failed to finish all input remove the incomplete encrypt file
	cmp r0, 0
	beq .Lout_encryptdelete
	
	//check now is encrypt mode
	ldr r0, =MODE	
	sub r0, fp, r0	
	ldr r0, [r0]
	ldr r1, =DECRYPT_MODE
	cmp r1, r0
	beq .Lout_encryptdelete
	bl encryptdelete
.Lout_encryptdelete:

	//restore r0 agin
	mov r0, r10
	// function epilogue
	sub	sp, fp, FP_OFF			// restore stack frame top
	pop	{r4-r10,fp,lr}			// remove frame and restore
	bx	lr				// return to caller

	// function footer
	.size	main, (. - main)		// set size for function

	// file footer
	.section .note.GNU-stack,"",%progbits // set executable (linker)
.end
