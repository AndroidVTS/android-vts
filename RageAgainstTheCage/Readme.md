
--------------------- Vulnerable ----------------------------
.text:00008DB0             loc_8DB0                                ; CODE XREF: sub_8C94+1C8j
.text:00008DB0 4F F0 00 08                 MOV.W           R8, #0
.text:00008DB4 01 21                       MOVS            R1, #1
.text:00008DB6 08 20                       MOVS            R0, #8
.text:00008DB8 42 46                       MOV             R2, R8
.text:00008DBA 43 46                       MOV             R3, R8
.text:00008DBC CD F8 00 80                 STR.W           R8, [SP,#0xC0+var_C0]
.text:00008DC0 06 F0 AE EA                 BLX             sub_F320
.text:00008DC4 3A 48                       LDR             R0, =(unk_224D8 - 0x2857C)
.text:00008DC6 0D F1 08 0E                 ADD.W           LR, SP, #0xC0+var_B8
.text:00008DCA 04 EB 00 0C                 ADD.W           R12, R4, R0 ; unk_224D8
.text:00008DCE BC E8 0F 00                 LDMIA.W         R12!, {R0-R3}
.text:00008DD2 AE E8 0F 00                 STMIA.W         LR!, {R0-R3}
.text:00008DD6 9C E8 0F 00                 LDMIA.W         R12, {R0-R3}
.text:00008DDA 8E E8 0F 00                 STMIA.W         LR, {R0-R3}
.text:00008DDE 08 20                       MOVS            R0, #8
.text:00008DE0 0D EB 00 01                 ADD.W           R1, SP, R0
.text:00008DE4 06 F0 8C EA                 BLX             sub_F300
.text:00008DE8 4F F4 FA 60                 MOV.W           R0, #0x7D0
.text:00008DEC 06 F0 78 EA                 BLX             sub_F2E0
.text:00008DF0 4F F4 FA 60                 MOV.W           R0, #0x7D0
.text:00008DF4 06 F0 44 EA                 BLX             sub_F280
.text:00008DF8 0A A9                       ADD             R1, SP, #0xC0+var_98
.text:00008DFA 40 F2 30 33                 MOVW            R3, #0x330
.text:00008DFE 0D A8                       ADD             R0, SP, #0xC0+var_8C
.text:00008E00 4F F4 80 02                 MOV.W           R2, #0x400000
.text:00008E04 C1 F6 98 13                 MOVT.W          R3, #0x1998
.text:00008E08 CD F8 38 80                 STR.W           R8, [SP,#0xC0+var_88]
.text:00008E0C 0D 93                       STR             R3, [SP,#0xC0+var_8C]
.text:00008E0E 0B 92                       STR             R2, [SP,#0xC0+var_94]
.text:00008E10 0A 92                       STR             R2, [SP,#0xC0+var_98]
.text:00008E12 CD F8 30 80                 STR.W           R8, [SP,#0xC0+var_90]
.text:00008E16 06 F0 9C EA                 BLX             sub_F350
.text:00008E1A DF F8 98 C0                 LDR.W           R12, =(off_285D4 - 0x2857C)
.text:00008E1E 54 F8 0C 10                 LDR.W           R1, [R4,R12] ; unk_2F840
.text:00008E22 08 68                       LDR             R0, [R1]
.text:00008E24 10 F0 01 0F                 TST.W           R0, #1
.text:00008E28 3F F4 6C AF                 BEQ.W           loc_8D04
.text:00008E2C DF F8 70 C0                 LDR.W           R12, =(off_285CC - 0x2857C)
.text:00008E30 21 4A                       LDR             R2, =(aLocalPort5037D - 0x2857C)
.text:00008E32 54 F8 0C 10                 LDR.W           R1, [R4,R12] ; unk_28788
.text:00008E36 A0 18                       ADDS            R0, R4, R2 ; "Local port 5037 disabled\n"
.text:00008E38 19 22                       MOVS            R2, #0x19
.text:00008E3A 11 F1 A8 03                 ADDS.W          R3, R1, #0xA8
.text:00008E3E 01 21                       MOVS            R1, #1
.text:00008E40 09 F0 E6 F9                 BL              sub_12210
.text:00008E44 5E E7                       B               loc_8D04
.text:00008E46             ; ---------------------------------------------------------------------------

------------------------------ Not Vuln -------------------------------
.text:0000B79E             loc_B79E                                ; CODE XREF: sub_B668+1F6j
.text:0000B79E 00 22                       MOVS            R2, #0
.text:0000B7A0 08 20                       MOVS            R0, #8
.text:0000B7A2 01 21                       MOVS            R1, #1
.text:0000B7A4 13 46                       MOV             R3, R2
.text:0000B7A6 00 92                       STR             R2, [SP,#0xE8+var_E8]
.text:0000B7A8 FC F7 E2 EC                 BLX             sub_8170
.text:0000B7AC 00 28                       CMP             R0, #0
.text:0000B7AE 58 D1                       BNE             loc_B862
.text:0000B7B0 DF F8 F8 C0                 LDR.W           R12, =(unk_1E568 - 0xB7BC)
.text:0000B7B4 0D F1 0C 0E                 ADD.W           LR, SP, #0xE8+var_DC
.text:0000B7B8 FC 44                       ADD             R12, PC ; unk_1E568
.text:0000B7BA BC E8 0F 00                 LDMIA.W         R12!, {R0-R3}
.text:0000B7BE AE E8 0F 00                 STMIA.W         LR!, {R0-R3}
.text:0000B7C2 BC E8 0F 00                 LDMIA.W         R12!, {R0-R3}
.text:0000B7C6 AE E8 0F 00                 STMIA.W         LR!, {R0-R3}
.text:0000B7CA DC F8 00 10                 LDR.W           R1, [R12]
.text:0000B7CE 09 20                       MOVS            R0, #9
.text:0000B7D0 CE F8 00 10                 STR.W           R1, [LR]
.text:0000B7D4 03 A9                       ADD             R1, SP, #0xE8+var_DC
.text:0000B7D6 FC F7 BC EC                 BLX             sub_8150
.text:0000B7DA 00 28                       CMP             R0, #0
.text:0000B7DC 41 D1                       BNE             loc_B862
.text:0000B7DE 4F F4 FA 60                 MOV.W           R0, #0x7D0
.text:0000B7E2 FC F7 A6 EC                 BLX             sub_8130
.text:0000B7E6 00 28                       CMP             R0, #0
.text:0000B7E8 3B D1                       BNE             loc_B862
.text:0000B7EA 4F F4 FA 60                 MOV.W           R0, #0x7D0
.text:0000B7EE 06 F0 6D FA                 BL              sub_11CCC
.text:0000B7F2 00 28                       CMP             R0, #0
.text:0000B7F4 35 D1                       BNE             loc_B862
.text:0000B7F6 4F F4 80 01                 MOV.W           R1, #0x400000
.text:0000B7FA 10 90                       STR             R0, [SP,#0xE8+var_A8]
.text:0000B7FC 0D 91                       STR             R1, [SP,#0xE8+var_B4]
.text:0000B7FE 40 F2 30 33                 MOVW            R3, #0x330
.text:0000B802 0C 91                       STR             R1, [SP,#0xE8+var_B8]
.text:0000B804 0C A9                       ADD             R1, SP, #0xE8+var_B8
.text:0000B806 0E 90                       STR             R0, [SP,#0xE8+var_B0]
.text:0000B808 0F A8                       ADD             R0, SP, #0xE8+var_AC
.text:0000B80A C1 F6 98 13                 MOVT.W          R3, #0x1998
.text:0000B80E 0F 93                       STR             R3, [SP,#0xE8+var_AC]
.text:0000B810 FC F7 C6 EC                 BLX             sub_81A0
.text:0000B814 26 48                       LDR             R0, =0xFFFFFF6C
.text:0000B816 22 58                       LDR             R2, [R4,R0]
.text:0000B818 13 68                       LDR             R3, [R2]
.text:0000B81A DA 07                       LSLS            R2, R3, #0x1F
.text:0000B81C 00 D1                       BNE             loc_B820
.text:0000B81E 60 E7                       B               loc_B6E2
.text:0000B820             ; ---------------------------------------------------------------------------


 ---------------- Not Vulnerable ----------------------
.text:00009822             loc_9822                                ; CODE XREF: .text:00009970j
.text:00009822 DF F8 3C C2                 LDR.W           R12, =(unk_28818 - 0x982E)
.text:00009826 0D F1 10 0E                 ADD.W           LR, SP, #0x10
.text:0000982A FC 44                       ADD             R12, PC ; unk_28818
.text:0000982C BC E8 0F 00                 LDMIA.W         R12!, {R0-R3}
.text:00009830 AE E8 0F 00                 STMIA.W         LR!, {R0-R3}
.text:00009834 BC E8 0F 00                 LDMIA.W         R12!, {R0-R3}
.text:00009838 AE E8 0F 00                 STMIA.W         LR!, {R0-R3}
.text:0000983C 9C E8 07 00                 LDMIA.W         R12, {R0-R2}
.text:00009840 8E E8 07 00                 STMIA.W         LR, {R0-R2}
.text:00009844 0B 20                       MOVS            R0, #0xB
.text:00009846 04 A9                       ADD             R1, SP, #0x10
.text:00009848 0F F0 7C EB                 BLX             sub_18F44
.text:0000984C 00 28                       CMP             R0, #0
.text:0000984E 40 F0 AF 80                 BNE.W           loc_99B0
.text:00009852 4F F4 FA 60                 MOV.W           R0, #0x7D0
.text:00009856 0F F0 56 EB                 BLX             sub_18F04
.text:0000985A 00 28                       CMP             R0, #0
.text:0000985C 40 F0 A8 80                 BNE.W           loc_99B0
.text:00009860 4F F4 FA 60                 MOV.W           R0, #0x7D0
.text:00009864 16 F0 71 FB                 BL              sub_1FF4A
.text:00009868 00 28                       CMP             R0, #0
.text:0000986A 40 F0 A1 80                 BNE.W           loc_99B0
.text:0000986E 5F 4A                       LDR             R2, =0xFFFFFF64
.text:00009870 B4 58                       LDR             R4, [R6,R2]
.text:00009872 23 68                       LDR             R3, [R4]
.text:00009874 DA 07                       LSLS            R2, R3, #0x1F
.text:00009876 7F F5 BD AE                 BPL.W           loc_95F4
.text:0000987A 0C F0 EA FD                 BL              sub_16452
.text:0000987E DF F8 E4 A1                 LDR.W           R10, =(unk_3149C - 0x9886)
.text:00009882 FA 44                       ADD             R10, PC ; unk_3149C
.text:00009884 D0 F8 00 90                 LDR.W           R9, [R0]
.text:00009888 50 46                       MOV             R0, R10
.text:0000988A 17 F0 84 EF                 BLX             sub_21794
.text:0000988E 5E 48                       LDR             R0, =0xFFFFFF60
.text:00009890 75 49                       LDR             R1, =(aSS - 0x989A)
.text:00009892 76 4A                       LDR             R2, =(aSystemCoreAdbA - 0x989E)
.text:00009894 35 58                       LDR             R5, [R6,R0]
.text:00009896 79 44                       ADD             R1, PC  ; "%s::%s():"
.text:00009898 75 4B                       LDR             R3, =(aAdb_main - 0x98A2)
.text:0000989A 7A 44                       ADD             R2, PC  ; "system/core/adb/adb.c"
.text:0000989C A8 35                       ADDS            R5, #0xA8
.text:0000989E 7B 44                       ADD             R3, PC  ; "adb_main"
.text:000098A0 28 46                       MOV             R0, R5
.text:000098A2 11 F0 93 F9                 BL              sub_1ABCC
.text:000098A6 0C F0 D4 FD                 BL              sub_16452
.text:000098AA 01 21                       MOVS            R1, #1
.text:000098AC 14 22                       MOVS            R2, #0x14
.text:000098AE 2B 46                       MOV             R3, R5
.text:000098B0 C0 F8 00 90                 STR.W           R9, [R0]
.text:000098B4 6F 48                       LDR             R0, =(aLocalPortDisab - 0x98BA)
.text:000098B6 78 44                       ADD             R0, PC  ; "Local port disabled\n"
.text:000098B8 0C F0 D4 FE                 BL              sub_16664
.text:000098BC 28 46                       MOV             R0, R5
.text:000098BE 11 F0 87 F8                 BL              sub_1A9D0
.text:000098C2 50 46                       MOV             R0, R10
.text:000098C4 17 F0 E8 EF                 BLX             sub_21898
.text:000098C8 0C F0 C3 FD                 BL              sub_16452
.text:000098CC C0 F8 00 90                 STR.W           R9, [R0]
.text:000098D0 90 E6                       B               loc_95F4


