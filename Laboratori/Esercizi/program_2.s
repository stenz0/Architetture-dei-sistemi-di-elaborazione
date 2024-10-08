		.data
		
i:		.double 2.22, 9.42, 8.23, 7.45, 0.87, 1.89, 2.52, 347.86, 46.32, 63.67, 4.44, 2.45, 8.53, 6.98, 1.34
		.double 0.09, 3.45, 6.32, 6.31, 2.89, 5.56, 3.15, 7.35, 9.42, 67.43, 32.64, 7.97, 5.32, 6.32, 8.90
		
w:		.double 1.90, 6.42, 79.76, 2.34, 7.98, 5.23, 8.65, 9.09, 7.42, 1.23, 6.43, 7.43, 8.45, 8.88, 7.77
		.double 3.23, 5.67, 44.67, 56.67, 4.21, 5.43, 0.98, 0.67, 0.34, 6.54, 2.34, 45.67, 78.88, 5.55, 2.22

b:		.word 0x4065600000000000

a: 		.word 0x7ff

ris:	.double 0

		.text
		
main: 	dadd R8, R0, R0 	#contatore che deve arrivare a 232
		add.d F3, F0, F0
		daddi R9, R0, 232
		ld R6, a(R0)
		
ciclo:	l.d F1, i(R8)
		l.d F2, w(R8)
		mul.d F1, F1, F2
		add.d F3, F3, F1
		daddi R8, R8, 8
		bne R8, R9, ciclo
		
		l.d F1, b(R0)
		add.d F3, F3, F1
		add.d F1, F0, F3
		mfc1 R1, F3
		dsll R1, R1, 1
		dsrl R1, R1, 31
		dsrl R1, R1, 22
		beq R1, R6, fine
		s.d F1, ris(R0)
fine:	HALT