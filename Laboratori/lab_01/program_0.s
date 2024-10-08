			.data
v1:			.byte 1,2,3,4,5,6,7,8,7,6,5,4,3,2,1
v2: 		.byte 1,2,3,4,5,6,7,8,9,1,2,3,4,5,6
v3:    		.byte 0,0,0,0,0,0,0,10,0,0,0,0,0,0,0
b1:			.space 1
b2:    		.space 1
b3:			.space 1
v4:			.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	
			.text

main:		daddi R3, R0, v1  	#R3 ha l'indirizzo del vettore analizzato
			dadd R10, R0, R0	#R10 tiene conto di quale vettore viene anallizato
			dadd R6, R0, R0	
			
loop:		dadd R1, R0, R0		#R1 è l'offset partendo da sx
			daddi R10, R10, 1	#Incremento del numero del vettore
			daddi R2, R0, 14	#R2 è l'offset partendo da dx
			dadd R6, R0, R0		#Flag che conterrà 1 se palindromo
			
			
loop2:		dadd R7, R1, R3		#R7 ha lindirizzo del byte corrente sx(offset compreso)
			lb R4, 0(R7)
			dadd R8, R2, R3		#R8 come R7 ma dal fondo
			lb R5, 0(R8)
			daddi R1, R1, 1		#incremento offset
			daddi R2, R2, -1	#decremento offset
			bne R4, R5, no		#se avviene il salto il vettore non è palindromo
			beq R1, R2, ok		#se avviene il salto è palindromo
			j loop2				#ciclo
			
ok:			daddi R6, R0, 1		#flag a 1
			
no:			daddi R9, R0, 2
			beq R10, R9, secondo
			daddi R9, R0, 3
			beq R10, R9, terzo
			sb R6, b1(R0)
			dadd R16, R6, R0	#flag v1
			daddi R3, R0, v2	#ricomincia loop con v2
			j loop
			
secondo:	sb R6, b2(R0)
			dadd R17, R0, R6	#flag v2
			daddi R3, R0, v3	#ricomincia loop con v3
			j loop
			
terzo:		sb R6, b3(R0)		#R6 flag v3

			daddi R1, R0, -1
			daddi R2, R0, 15
			dadd R18, R0, R0 #v1
			dadd R19, R0, R0 #v2
			dadd R20, R0, R0 #v3
loop3:		daddi R1, R1, 1
			daddi R2, R2, -1
			beq R16, R0, s1
			lb R18, v1(R1)
s1:			beq R17, R0, s2
			lb R19, v2(R1)
s2:			beq R6, R0, s3
			lb R20, v3(R1)
s3:			dadd R21, R18, R19
			dadd R21, R21, R20
			sb R21, v4(R1)
			sb R21, v4(R2)
			bne R1, R2, loop3
HALT