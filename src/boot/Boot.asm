; Разработка Загрузчика для ОС: Vortex
; Версия: 0.1 - Tested
;------- Описание ----------
; Файловая система: 		FAT32
; Конфигурационный файл: 	Load.conf
;---- В Разработке ---------
; Прочитать Конфиг -
; проверить, что процессор не ниже 80286
; Проверить не загружено ли ядро
; Загрузить Ядра
; Подготовить среду
; Стартовать
;-------- Память -----------
;   00000-005FF        используется BIOS ( 1536 )
;   00600-07BFF        свободно ( 30208 )
;   07C00-07DFF        программа начальной загрузки ( 512 )
;   07E00-9FFFF        свободно ( 623104 )
;   A0000-FFFFF        используется BIOS ( 393216 )
;----- Служебная памать ----
;   01600-0....        Буфер 
;---------------------------
use16
org 0x7e00
	jmp Start
	
	include '../Include/x16/assembly/sysmacros'
	
gdt32:
	dq 0                  ; NULL - 0
	dq 0x00CF9A000000FFFF ; CODE - 8
	dq 0x00CF92000000FFFF ; DATA - 10	
gdtr32:
	dw $ - gdt32 - 1
	dd gdt32	

Start:
; Подготовка к загрузке Ядра на адрес 0x100000
	 

	mov si, OK
	call printstr	
	;call clrscr
	
	cli
	
	push ds
	
	lgdt fword[gdtr32]
	
	mov eax, cr0
	or al, 1
	mov cr0, eax
	
	jmp $+2
	
	mov bx, 08h
	mov ds, bx
	
	and al, 0xFE
	mov cr0, eax
	
	pop ds
	sti
	
	mov eax, 0x1000000
	mov si, core_pach
	LoadFile			;get in edx size kernel in byte
;Получаем карту памяти
	clc

	
	cli
	
	in al, 0x70
	or al, 0x80
	out 0x70, al
	
	mov eax, cr0
	or al, 1
	mov cr0, eax
	

	
	;jmp $
	
	jmp 0x8:Start32


;-----------------------------
core_pach db 'KERNEL     $'
OK db '[OK]',13,10,0
use32
	;include '../Include/x32/Assembly/sysfunc'
	
Start32:
							; Получаем карту памяти от Bios
	;mov eax, 0x12345678 	; Ключ
	;----------------
	; Размер ядра в edx
	;----------------
	jmp 0x8:0x1000000
	hlt
jmp $

