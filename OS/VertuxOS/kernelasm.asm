; Разработка Ядра для ОС: Vortex
; Версия: 0.1 - Started
;------- Описание ----------
; Файловая система: 		FAT32
; Конфигурационный файл: 	Load.conf
; Разрядность: 			x86
;---- В Разработке ---------
;1.Не допустит выполнение программы, если процессор не 32-разрядный (i286 или XT).
;2.Произведёт поиск локального APIC в процессоре
;-------- Память -----------
;   00000-005FF        используется BIOS ( 1536 )
;   00600-07BFF        свободно ( 30208 )
;   07C00-07DFF        программа начальной загрузки ( 512 )
;   07E00-9FFFF        свободно ( 623104 )
;   A0000-FFFFF        используется BIOS ( 393216 )
;----- Служебная памать ----
;   01600-0....        Буфер 
;---------------------------
format ELF

public BGMM
public Start
public _Stack_down

extrn kernel_main




	
section ".text" executable
Start:
	mov [BGMM], eax
	;----------------
	; Размер ядра в edx
	;----------------
	push edx
	call kernel_main
	hlt
	jmp $

	;include 'Include/x32/Assembly/sysfunc'
	;include 'Include/Header/IRQT'
	;include 'Include/Header/INIT'

section ".data" writable
	;include 'Include/x32/Assembly/sysfunc.data'
	BGMM dd ?

section ".bss" writable
_Stack:
	db 0x1000 dup (?)
_Stack_down:


