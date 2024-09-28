	// file header
	.arch armv6           		// armv6 architecture
	.arm		      		// arm 32-bit instruction set
	.fpu vfp	      		// floating point co-processor
	.syntax unified	      		// modern syntax

	// .data // uncomment if needed

	.text 				// start of text segment
	// ======
	

        .global asm_doCol
	// ====== DO NOT EDIT ANYTHING ABOVE THIS LINE ======
        // !!! SET FP_OFFSET TO THE NUMBER OF (SAVED REGISTERS -1 * 4)
        // definitions that apply to the entire source file
        .equ 	FP_OFFSET, 24 // TODO	// (# of saved regs - 1) * 4
	
	// asm_doCol(belem *dest,	// r0
	//           belem *src,	// r1
	//           size_t rows,	// r2
	//           size_t cols)	// r3

asm_doCol:	
	push {r4-r8, fp, lr}			// TODO save preserved registers as needed
	add fp, sp, #FP_OFFSET		// DO NOT EDIT THIS LINE

	// TODO Implement doCol here
	MOV r4, #2
.Lfor:
	cmp r4, r2					// for (i=2; i<rows; i++) {
	bge .Lendfor				//

	mov r6, #0					// 	count = 0

	mov r5, r1					// 	row = src
	sub r5, r5, #1				// 	row--
	ldrb r7, [r5, #0]			//	count += row[0]
	add r6, r6, r7				//
	ldrb r7, [r5, #2]			//	count += row[2]
	add r6, r6, r7				//

	mov r5, r1					// 	row = src-cols
	sub r5, r5, r3				//
	sub r5, r5, #1				// 	row--
	ldrb r7, [r5, #0]			//	count += row[0]
	add r6, r6, r7				//
	ldrb r7, [r5, #1]			//	count += row[1]
	add r6, r6, r7				//
	ldrb r7, [r5, #2]			//	count += row[2]
	add r6, r6, r7				//

	mov r5, r1					// 	row = src+cols
	add r5, r5, r3				//
	sub r5, r5, #1				// 	row--
	ldrb r7, [r5, #0]			//	count += row[0]
	add r6, r6, r7				//
	ldrb r7, [r5, #1]			//	count += row[1]
	add r6, r6, r7				//
	ldrb r7, [r5, #2]			//	count += row[2]
	add r6, r6, r7				//

	mov r8, #0
	strb r8, [r0]				// *dest = 0

	cmp r6, #3					// if (count == 3) {
	bne .Lelse					//
	mov r8, #1					//
	strb r8, [r0]				// 	*dest = 1
	b .Lout_if					//}
.Lelse:							//
	cmp r6, #2					// if (count == 2 && (*src) == 1) {
	bne .Lout_if				//
	ldrb r8, [r1]				//
	cmp r8, #1					// 	
	bne .Lout_if				//
	mov r8, #1					//
	strb r8, [r0]				// 		*dest = 1
.Lout_if:						// }
	
	add r4, r4, #1				//
	
	add r1, r1, r3				//  src += cols;
	add r0, r0, r3				//  dest += cols;

	b .Lfor						// }
.Lendfor:


exit:	
	sub sp, fp, #FP_OFFSET		// DO NOT EDIT THIS LINE
	pop {r4-r8, fp, lr}// TODO restore preserved registers as needed
	bx lr				// DO NOT EDIT THIS LINE
	
