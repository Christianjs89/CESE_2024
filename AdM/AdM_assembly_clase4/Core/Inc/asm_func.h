/*
 * asm_func.h
 *
 *  Created on: May 16, 2024
 *      Author: chris
 */

#ifndef INC_ASM_FUNC_H_
#define INC_ASM_FUNC_H_

    /* Clase 3*/
    uint32_t asm_sum(uint32_t primerOperando, uint32_t segundoOperando);
    void asm_svc();

    /* Clase 4*/
    void asm_stack(uint32_t uno, uint32_t dos, uint32_t tres, uint32_t cuatro, uint32_t cinco, uint32_t seis);
    void asm_zeros(uint32_t *vec, uint32_t longitud);
    uint32_t bitfield_clear(uint32_t dato, uint32_t ancho, uint32_t inicio);

    /* Practica restante */
    void productoEscalar32(uint32_t *vectorIn, uint32_t *vectorOut, uint32_t longitud,uint16_t escalar);
    void productoEscalar12(uint16_t *vectorIn, uint16_t *vectorOut, uint32_t longitud, uint16_t escalar);

#endif /* INC_ASM_FUNC_H_ */
