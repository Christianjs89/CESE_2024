/*
 * asm.S
 *
 *  Created on: May 16, 2024
 *      Author: chris
 */


// Ejemplo de cómo se realiza un archivo para escribir código en Assembly.
// Los comentarios son iguales que en C, y además se puede utilizar @:
@ Un comentario

// Directivas del compilador: Son aquellas que comienzan con un "."
// Para que nuestro código funcione, necesitamos:
.text 	// Sección de texto: se indica que en esta sección hay código para ejecutar

// Otras opcionales, pero muy recomendadas, son (no importa el orden):
.syntax unified		// Uso del UAL
.thumb		 	// El ISA que se utilizará es Thumb-2

//Otra directiva que es mandatoria es .global. Sirve para que nuestra función sea "visible" por los demás archivos. No olvidar colocar el prototipo de la función en el respectivo archivo .h

// Exportar todas las funciones aquí (.global + nombre_función)
.global asm_sum
.global asm_svc
.global asm_stack
.global asm_zeros
.global bitfield_clear
.global productoEscalar32
.global productoEscalar12

// Se recomienda colocar el prototipo de la función en C, como ayuda:
// Prototipo en archivo .h:
// uint32_t asm_sum(uint32_t primerOperando, uint32_t segundoOperando)

// Parámetros recibidos:
// r0: primerOperando
// r1: segundoOperando

// Valor de retorno:
// r0: resultado de la suma entre los parámetros pasados
// Se recomienda respetar la identación: ayuda a leer mejor el código

.thumb_func			// Se le indica al Ensamblador la función
	asm_sum:		// Nombre de la función. Los ":" sirven para indicar etiquetas
		add	r0, r1	// r0 = r0 + r1

		bx lr		// Vuelve adonde fue llamada (específicamente, si "pc"
                    // es la dirección, en el código del programa, de la
                    // instrucción "bl asm_sum" usada para saltar a
                    // esta función, entonces "lr" contiene ese "pc" + 4).
                    // el valor de retorno debe quedar en el registro r0.

// Prototipo en archivo .h:
// void asm_svc()

// Parámetros recibidos: void

// Valor de retorno: void
.thumb_func
	asm_svc:
		svc 0

		bx lr

// Prototipo en archivo .h:
// void asm_stack(uint32_t uno, uint32_t dos, uint32_t tres, uint32_t cuatro, uint32_t cinco, uint32_t seis)

// Parámetros recibidos: r0 a r5

// Valor de retorno: void
.thumb_func
	asm_stack:
		// Resguardamos el valor de los registros (5 registros)
		push {r4 - r8}
		// Se recupera el 5° parámetro
		ldr r4, [SP, 20] // → 20 = 4(32 bits)*5(registros: r4-r8)
		// Se recupera el 6° parámetro
		ldr r5, [SP, 24]

		// Recuperamos los valores
		pop {r4 - r8}
		// Volvemos al main
		bx lr

// Prototipo en archivo.h:
// void asm_zeros(uint32_t * vector, uint32_t longitud)

// Parámetros recibidos:
//   r0: dirección del vector
//   r1: longitud

// Valor de retorno: Nada

.thumb_func
asm_zeros:
		// Guarda de seguridad
		cbz r0, exit1	// Salir si la dirección del vector es nula
		mov r2, #0 // luego se usa r2 para asignar 0 a cada elemento
	loop1:
		// Se recorre el vector del último al primer elemento
		sub r1, #1	// Restar 1 antes de recorrer el vector
		// El shift se hace por dos, por lo que se incrementa de a 4 bytes
		str r2, [r0, r1, LSL #2]
		cbz r1, exit1
		b loop1
	exit1:
       	bx lr

// Prototipo en archivo .h:
// uint32_t bitfield_clear(uint32_t dato, uint32_t ancho, uint32_t inicio)

// Parámetros recibidos:
//   r0: dato a procesar
//   r1: ancho del campo de bits
//   r2: posición del bit menos significativo del campo de bits

// Valor de retorno: valor procesado

.thumb_func
bitfield_clear:
		// Guarda de seguridad
		cbz r0, _exit5	// Salir si el dato es cero
		cbz r1, _exit5	// Salir si el ancho del campo es cero

		mov r3, #1	// Se coloca un 1 en el lsb de r3
		lsl r3, r1	// Se desplaza el 1 según el ancho del campo
		sub r3, #1	// Se genera la máscara. Ej: 001000 - 1 = 000111
		lsl r3, r2	// Se desplaza la máscara según inicio. Ej: 011100
		mvn r3, r3	// Se niega la máscara. Ej: 100011
		and r0, r3	// Se aplica and bit a bit. Se devuelve el resultado en r0

	_exit5:
		bx lr


/* -------------- PRODUCTO ESCALAR 32 -------------- */
// prototipo en header:
// void productoEscalar32(uint32_t *vectorIn, uint32_t *vectorOut, uint32_t longitud,uint16_t escalar);

// Parametros entrada:
// 	R0 pointer al vector de entrada
// 	R1 pointer al vector de salida
// 	R2 longitud del vector
// 	R3 escalar multiplicador

// Parametros salida:
// 	R0 pointer al vectorOut

.thumb_func
productoEscalar32:
	// Verificar direcciones de vectores de entrada y salida
	CBZ R0, exit // compare and branch if zero: si pointer de vector es NULL salir
	CBZ R1, exit // idem

	// R3 ya tiene al escalar, no requiere el MOV

	// Guardar elemento del vector en un registro nuevo
	PUSH {R4} // hacer backup de lo que haya en el registro R4 a la stack en RAM

	loop:
		// Recorrer al vector de N-1 a 0
		SUB R2, #1 // decrementar -1 al contador. Comienza en R2=N -longitud-
		// Cargar elementos array al registro
		LDR R4, [R0, R2, LSL #2] // R4 = *(R0+(R2<<2))= *(R0+R2x4) >> 1er valor es R4=[R0+(N-1)x4]
		// Realizar operacion entre registros
		MUL R4, R3 // R4=R4 x R3
		// Almacenar operacion en memoria
		STR R4, [R1, R2, LSL #2] // *(R1+(R2<<2))=*(R1+(R2+4)=R4 >> almacenar en array de salida el resultado
		// checkear contador
		CBZ R2, exit // ir a exit si el contador R2=0
		// repetir loop si R2 !=0
		B loop

	exit:
		POP {R4} // traer de vuelta lo que habia en R4 antes de volver al main
		BX LR // retornar a main

/* -------------- PRODUCTO ESCALAR 32 -------------- */


/* -------------- PRODUCTO ESCALAR 12 SATURADO -------------- */
// void productoEscalar12(uint16_t *vectorIn, uint16_t *vectorOut, uint32_t longitud, uint16_t escalar);
// Parametros entrada:
// 	R0 pointer al vector de entrada
// 	R1 pointer al vector de salida
// 	R2 longitud del vector
// 	R3 escalar multiplicador

// Parametros salida:
// 	R0 pointer al vectorOut

.thumb_func
productoEscalar12:
	// Verificar direcciones de vectores de entrada y salida
	CBZ R0, exit2 // compare and branch if zero: si pointer de vector es NULL salir
	CBZ R1, exit2 // idem

	// R3 ya tiene al escalar, no requiere el MOV

	// Guardar elemento del vector en un registro nuevo
	PUSH {R4} // hacer backup de lo que haya en el registro R4 a la stack en RAM

	loop2:
		// Recorrer al vector de N-1 a 0
		SUB R2, #1 // decrementar -1 al contador. Comienza en R2=N -longitud-
		// Cargar elementos array al registro, ahora 16 bits Half word
		LDRH R4, [R0, R2, LSL #1] // R4 = *(R0+(R2<<1))= *(R0+R2x2) >> 1er valor es R4=[R0+(N-1)x4]
		// Realizar operacion entre registros
		MUL R4, R3 // R4=R4 x R3
		// Saturar en 12 bits
		USAT R4, #12, R4 // saturar el resultado en R4 y guardar en mismo registro
		// Almacenar operacion en memoria
		STRH R4, [R1, R2, LSL #1] // *(R1+(R2<<2))=*(R1+(R2+4)=R4 >> almacenar en array de salida el resultado
		// checkear contador
		CBZ R2, exit2 // ir a exit si el contador R2=0
		// repetir loop si R2 !=0
		B loop2

	exit2:
		POP {R4} // traer de vuelta lo que habia en R4 antes de volver al main
		BX LR // retornar a main



/* -------------- PRODUCTO ESCALAR 12 SATURADO -------------- */


