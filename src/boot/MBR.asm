; Разработка MBR для ОС: Vortex
; Версия: 1.001 - UnStable
; Fixed: 
; 1) Поддержка дозагрузки секторов при поиске файла
; 2) Реализовать подгрузку FAT таблицы. След. за 32 сектором.
;-------- Память -----------
;   00000-005FF        используется BIOS ( 1536 )
;   00600-07BFF        свободно ( 30208 )
;   07C00-07DFF        программа начальной загрузки ( 512 )
;   07E00-9FFFF        свободно ( 623104 )
;   A0000-FFFFF        используется BIOS ( 393216 )
;---------------------------
use16
org 7c5ah
	jmp Loader
printstr: 				; страница 0
		mov ah, 05H
		mov al, 0h
		int 10h
		mov ah, 0eH
		mov al, 0
	@@:	
		lodsb
		test al, al
		jz @f
		int 10h
		jmp @b
	@@: 
		ret
Error:
	mov si, error_log
	call printstr
	hlt
	jmp $

;-------fix---------
ReadEDD:
	push ax dx si
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	mov ax, 0x0e2e ;Progress
	int 0x10
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>
.RConfig:
	mov ax, 0
	mov dl, 80h
	int 13h

.Chance2:
;=== LBA EXd === 
	mov si, LDB
	mov ah, 42h
	mov dl, 80h
	int 13h
	jc .RConfig
;===============

.end:
	pop si dx ax
ret

Loader: 	
	mov si, Loading
	call printstr
cld

;Fat read
	mov word[buffer], 600h
	call ReadEDD

	; Search kluster
	movzx ax, byte[fat_sector_to_klaster]
	mul word[fat_size_sector]
	mov [SizeKluster], ax

	;Search RootDir
	mov ax, word[fat32_bigsektorpearlfat]
	mov dx, word[fat32_bigsektorpearlfat+2]
	movzx cx, byte[fat_copys]
	mul cx
	add ax, word[fat_reserv_sectors]
	mov word[RootDir], ax

	;Search FatBP
	;mov word[FatBP], 32	;

;Search File
	mov word[Ldb_bq], ax
	mov [buffer], 0x700
@@:	
	mov si, 0x700
	


END_:
	;# RETURN
	;#600h Fat table
	jmp 0:7e00h
	

;---------------
LDB:	
		db 10h,0
num		dw 01h
buffer	dw 7e00h
		dw 0x0
Ldb_bq	dq 0
;---------------

SizeKluster dw ? ; it is processed
FatBP 		dw 32 ; it is processed 


Loading 	db 'Loading',0
error_log	db 'Fatal Error',0

End_clus dd 0x0FFFFFFF

RootDir 		dd	?

boot_next 		db 'BOOT       '

virtual at 800h
	data_t db 512 dup (?)
end virtual

;FirstDataSector = fat_reserv_sectors + (fat_copys * fat32_bigsektorpearlfat) + RootDirSectors; Корневой каталог

virtual at 600h
							db 3 dup (?)
	fat_OEM 				db 	8 dup (?)
	fat_size_sector 		dw 	?
	fat_sector_to_klaster 	db 	?
	fat_reserv_sectors 		dw	?
	fat_copys 				db 	?
	RDE						dw	?
	fat_bigtotalsektor		dw	?
	fat_diskid 				db 	?
	fat_sektorperfat		dw	?
	fat_sektorpertrack		dw	?
	fat_head				dw	?
	fat_hidesektor			dd	?
	fat_bigtotalsektors		dd	?; размер в секторах
	fat32_bigsektorpearlfat	dd	?
	fat32_flags				dw	?
	fat32_fsver				dw	?
	fat32root_cul			dd	?
	fat32_infosector		dw	?
	fat32_BackBootSector	dw	?
							db	12 dup (?)
	fat32_Drivenum			db	?
	fat32_reserv1			db	?
	fat32_bootsig			db	?
	fat32_voiid				dd	?
	fat_volumelabel			db	11 dup (?);11
	fat_SysID				db	8 dup (?)
end virtual

virtual at 0x700 ;Size: 32
	Dir_name			db 11 dup (?)
	Dir_attr			db ?
	Dir_NTresrv			db 0
	Dir_CtrTimeTenth	db ?
	Dir_FstClusHI		dw ?;99 91
	Dir_WrtTime			dw ?
	Dir_wrtDate			dw ?
	Dir_FstClusLO		dw ?
	Dir_Filesize		dd ?
end virtual

;---------------------------
	rb 512-90 -($-7c5ah)-2
	db 0x55, 0xAA
