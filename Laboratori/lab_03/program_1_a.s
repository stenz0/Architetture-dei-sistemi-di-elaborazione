			.data
v1:			.double 1.11, 2.22, 3.33, 4.44, 5.55, 6.66, 7.77, 8.88, 9.99, 10.10, 11.11, 12.12, 13.13, 14.14, 15.15, 16.16
			.double 1.11, 2.22, 3.33, 4.44, 5.55, 6.66, 7.77, 8.88, 9.99, 10.10, 11.11, 12.12, 13.13, 14.14, 15.15, 16.16
			.double 1.11, 2.22, 3.33, 4.44, 5.55, 6.66, 7.77, 8.88, 9.99, 10.10, 11.11, 12.12, 13.13, 14.14, 15.15, 16.16
			.double 1.11, 2.22, 3.33, 4.44, 5.55, 6.66, 7.77, 8.88, 9.99, 10.10, 11.11, 12.12, 13.13, 14.14, 15.15, 16.16
		
v2:			.double 91.11, 81.22, 7.33, 65.44, 15.55, 5.66, 7.77, 27.88, 13.99, 90.10, 36.11, 4.12, 3.13, 4.14, 77.15, 6.16
			.double 92.11, 82.22, 3.33, 4.44, 50.55, 66.66, 37.77, 78.88, 19.99, 9.10, 35.11, 2.12, 5.13, 6.14, 75.15, 8.16
			.double 93.11, 83.22, 75.33, 54.44, 45.55, 4.66, 57.77, 25.88, 1.99, 29.10, 1.11, 48.12, 33.13, 14.14, 7.15, 66.16
			.double 94.11, 84.22, 33.33, 67.44, 56.55, 6.66, 39.77, 22.88, 9.99, 20.10, 31.11, 42.12, 53.13, 64.14, 5.15, 80.16

v3:			.double 1.11, 2.22, 3.33, 10.32, 5.55, 6.66, 7.77, 8.88, 9.99, 10.10, 11.11, 35.90, 13.13, 14.14, 15.15, 16.16
			.double 1.11, 2.22, 3.33, 4.44, 5.55, 6.66, 7.77, 8.88, 9.99, 10.10, 11.11, 12.12, 13.13, 14.14, 15.15, 16.16
			.double 93.11, 83.22, 75.33, 54.44, 45.55, 4.66, 57.77, 25.88, 1.99, 29.10, 1.11, 48.12, 33.13, 14.14, 7.15, 66.16
			.double 94.11, 84.22, 33.33, 67.44, 56.55, 6.66, 39.77, 22.88, 9.99, 20.10, 31.11, 0.00, 53.13, 64.14, 5.15, 80.16
		
v4:			.double 93.11, 83.22, 75.33, 54.44, 45.55, 4.66, 57.77, 25.88, 1.99, 29.10, 1.11, 48.12, 33.13, 14.14, 7.15, 66.16
			.double 53.11, 89.22, 75.53, 94.47, 41.11, 47.60, 52.27, 90.80, 1.00, 2500, 5.00, 40.10, 3.03, 10.14, 70.15, 60.06
			.double 1.11, 2.22, 3.33, 4.44, 5.55, 6.66, 7.77, 8.88, 9.99, 10.10, 11.11, 12.12, 13.13, 14.14, 15.15, 16.16
			.double 94.11, 84.22, 33.33, 67.44, 56.55, 6.66, 39.77, 22.88, 9.99, 20.10, 31.11, 42.12, 53.13, 64.14, 5.15, 80.16
		
v5:			.space 512
v6:			.space 512
v7:			.space 512

			.text
		
main: 		dadd R8, R0, R0 	#contatore che deve arrivare a 512
			daddi R3, R0, 1		#maschera
			dadd R2, R0, R0 	#contatore ciclo
			daddi R9, R0, 512
			daddi R1, R0, 1		#valore di m
			daddi R4, R0, 2		#potenza di 2
			daddi R6, R0, 2		#numero due per potenza
			add.d F8, F0, F0 	#valore di p
			add.d F9, F0, F0	#valore di k
		
			#r1 m
			#r2 contatore ciclo
			#r3 maschera(1)
			#r4 potenza di 2
			#r5 risultato and/sll/variabile per calcoli
			#r6 numero per elevare potenza
			#r7
			#r8 indice vettori
			#r9 supporto calcoli
			#r10
			#r11 supporto calcoli
			
			#f1 v1[i]
			#f2 v2[i]
			#f3 v3[i]
			#f4 v4[i]
			#f5 v5[i]
			#f6 v6[i]
			#f7 v7[i]
			#f8 p
			#f9 k
			#f10 supporto calcoli
			#f11 supporto calcoli
			#f12
		
		
ciclo:		and R5, R2, R3
			l.d F1, v1(R8)
			l.d F2, v2(R8)
			l.d F3, v3(R8)
			l.d F4, v4(R8)
			beq R5, R3, dispari
			
			dsrlv R5, R2, R3
			dsllv R1, R1, R5
			dsllv R1, R1, R5
			mtc1 R1, F10
			cvt.d.l F10, F11
			mul.d F8, F1, F11	#nuovo valore di p
			add.d F11, F8, F0
			cvt.l.d F12, F11
			mfc1 R1, F12
			j oltre
			
dispari:	dmul R5, R1, R2		#m per i
			cvt.l.d F12, F4
			mtc1 R5, F10		#sposto un valore int su un float
			mfc1 R11, F12
			cvt.d.l F10, F11	#converto l'int ad un float
			ddiv R5, R11, R4		#v4[i]/2^i
			dsllv R4, R4, R6		#potenza di due per il prossimo ciclo
			div.d F8, F1, F11	#v1[i]/ m*i
			mtc1 R5, F10		#sposto un valore int su un float
			cvt.d.l F10, F9		#converto l'int ad un float
		
oltre:		mul.d F5, F8, F2
			daddi R2, R2, 1
			add.d F6, F9, F1
			add.d F5, F5, F3
			add.d F5, F5, F4
			s.d F5, v5(R8)
			div.d F6, F5, F6
			s.d F6, v6(R8)
			add.d F7, F2, F3
			mul.d F7, F6, F7
			s.d F7, v7(R8)
			daddi R8, R8, 8
			bne R8, R9, ciclo
HALT