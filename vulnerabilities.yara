rule Vold_Dir_Traversal
{
    meta:
        description = "Checks vold binary for asec directory traversal"
        threat_level = 3
        in_the_wild = true
        binary = "/system/bin/vold"

    strings:
        $asec_id_check = "Invalid asec id "
        $is_vold_binary = "vold"

    condition:
       not $asec_id_check and $is_vold_binary
}

rule Gingerbreak
{
  meta:
      description = "Netlink unchecked"
      binary = "/system/bin/vold"

  strings:
      $a = "Invalid 'PARTN' value"
      $is_vold_binary = "vold"
      $is_vold_binary_2 = "/system/bin/mkfs.exfat"

  condition:
      not $a and $is_vold_binary and $is_vold_binary_2
}


rule ZergRush
{
  meta:
      description = "Use after free in libsysutils"
      binary = "/system/lib/libsysutils.so"

  strings:
      $command_length_check = "Command too long"
      $netlink_decode_string = "Error decoding NetlinkEvent"

  condition:
      not $command_length_check and $netlink_decode_string
}


rule RageAgainstTheCage
{
   meta:
      descrition = "Unchecked return value of setuid(2) call in adbd"
      binary = "/sbin/adbd"

/* unpatched htc one 2.1update1
.text:00008DE0 0D EB 00 01                 ADD.W           R1, SP, R0
.text:00008DE4 06 F0 8C EA                 BLX             sub_F300
.text:00008DE8 4F F4 FA 60                 MOV.W           R0, #0x7D0
.text:00008DEC 06 F0 78 EA                 BLX             sub_F2E0 // setuid(2000)
.text:00008DF0 4F F4 FA 60                 MOV.W           R0, #0x7D0
.text:00008DF4 06 F0 44 EA                 BLX             sub_F280 //setgid(2000)
*/
/* unpatched
.text:00008DE8 09 20                       MOVS            R0, #9
.text:00008DEA 06 F0 A2 EA                 BLX             sub_F330
.text:00008DEE 4F F4 FA 60                 MOV.W           R0, #0x7D0
.text:00008DF2 06 F0 8E EA                 BLX             sub_F310
.text:00008DF6 4F F4 FA 60                 MOV.W           R0, #0x7D0
.text:00008DFA 09 F0 5F F9                 BL              sub_120BC
*/

/* patched
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

*/

  strings:
      $vuln_setuid =         {
                              06 F0 ?? EA
                              4F F4 FA 60
                              06 F0 ?? EA
                              4F F4 FA 60
                              }

  condition:
    $vuln_setuid

}

rule Zimperlinch
{
   meta:
      descrition = "Unchecked return value of setuid(2) call in zygote"
      binary = "/system/lib/libdvm.so"
/* patched
.text:AD0556AE 04 98                       LDR             R0, [SP,#0x30+var_20]
.text:AD0556B0 FF F7 4A FF                 BL              sub_AD055548
.text:AD0556B4 00 28                       CMP             R0, #0
.text:AD0556B6 09 DA                       BGE             loc_AD0556CC
.text:AD0556B8 B9 F7 2E EA                 BLX             __errno
.text:AD0556BC 30 4A                       LDR             R2, =(aCannotSetrlimi - 0xAD081F50)
.text:AD0556BE A2 18                       ADDS            R2, R4, R2 ; "cannot setrlimit() errno: %d"
.text:AD0556C0 03 68                       LDR             R3, [R0]
.text:AD0556C2 29 48                       LDR             R0, =(aDalvikvm_0 - 0xAD081F50)
.text:AD0556C4 21 18                       ADDS            R1, R4, R0 ; "dalvikvm"
.text:AD0556C6 05 20                       MOVS            R0, #5
.text:AD0556C8 B8 F7 F2 EF                 BLX             __android_log_print
.text:AD0556CC
.text:AD0556CC             loc_AD0556CC                            ; CODE XREF: sub_AD0555F8+BEj
.text:AD0556CC 38 46                       MOV             R0, R7  ; gid
.text:AD0556CE B9 F7 84 EA                 BLX             setgid
.text:AD0556D2 00 28                       CMP             R0, #0
.text:AD0556D4 0B DA                       BGE             loc_AD0556EE
.text:AD0556D6 B9 F7 20 EA                 BLX             __errno
.text:AD0556DA 23 49                       LDR             R1, =(aDalvikvm_0 - 0xAD081F50)
.text:AD0556DC 29 4A                       LDR             R2, =(aCannotSetgidDE - 0xAD081F50)
.text:AD0556DE 61 18                       ADDS            R1, R4, R1 ; "dalvikvm"
.text:AD0556E0 A2 18                       ADDS            R2, R4, R2 ; "cannot setgid(%d) errno: %d"
.text:AD0556E2 03 68                       LDR             R3, [R0]
.text:AD0556E4 05 20                       MOVS            R0, #5
.text:AD0556E6 00 93                       STR             R3, [SP,#0x30+var_30]
.text:AD0556E8 3B 46                       MOV             R3, R7
.text:AD0556EA B8 F7 E2 EF                 BLX             __android_log_print
.text:AD0556EE
.text:AD0556EE             loc_AD0556EE                            ; CODE XREF: sub_AD0555F8+DCj
.text:AD0556EE 30 46                       MOV             R0, R6  ; uid
.text:AD0556F0 B9 F7 EC E8                 BLX             setuid
.text:AD0556F4 00 28                       CMP             R0, #0
.text:AD0556F6 0D DA                       BGE             loc_AD055714
.text:AD0556F8 B9 F7 0E EA                 BLX             __errno
.text:AD0556FC 1A 4B                       LDR             R3, =(aDalvikvm_0 - 0xAD081F50)
.text:AD0556FE E1 18                       ADDS            R1, R4, R3 ; "dalvikvm"
.text:AD055700 33 46                       MOV             R3, R6
.text:AD055702 D0 F8 00 C0                 LDR.W           R12, [R0]
.text:AD055706 20 48                       LDR             R0, =(aCannotSetuidDE - 0xAD081F50)
.text:AD055708 22 18                       ADDS            R2, R4, R0 ; "cannot setuid(%d) errno: %d"
.text:AD05570A 05 20                       MOVS            R0, #5
.text:AD05570C CD F8 00 C0                 STR.W           R12, [SP,#0x30+var_30]
.text:AD055710 B8 F7 CE EF                 BLX             __android_log_print
.text:AD055714
*/
      strings:
          $cannot_setuid = "cannot setuid("
          $dalvikvm = "dalvikvm"
      condition:
          not $cannot_setuid and $dalvikvm
}

rule Exploid
{
/* PATCHED:
.text:0000D868 B2 68                       LDR             R2, [R6,#8]
.text:0000D86A 01 2A                       CMP             R2, #1
.text:0000D86C E4 D1                       BNE             loc_D838
.text:0000D86E 73 68                       LDR             R3, [R6,#4]
.text:0000D870 00 2B                       CMP             R3, #0
.text:0000D872 E1 D1                       BNE             loc_D838
.text:0000D874 61 69                       LDR             R1, [R4,#0x14]
.text:0000D876 0B 29                       CMP             R1, #0xB
.text:0000D878 DE D9                       BLS             loc_D838
.text:0000D87A 23 69                       LDR             R3, [R4,#0x10]
.text:0000D87C 00 2B                       CMP             R3, #0
.text:0000D87E DB D0                       BEQ             loc_D838
.text:0000D880 9A 68                       LDR             R2, [R3,#8]
.text:0000D882 02 2A                       CMP             R2, #2
.text:0000D884 D8 D1                       BNE             loc_D838
.text:0000D886 1B 69                       LDR             R3, [R3,#0x10]
.text:0000D888 00 2B                       CMP             R3, #0
.text:0000D88A D5 D1                       BNE             loc_D838
.text:0000D88C 40 F2 FF 3C                 MOVW            R12, #0x3FF
.text:0000D890 60 45                       CMP             R0, R12
.text:0000D892 D1 DC                       BGT             loc_D838
.text:0000D894 3B 54                       STRB            R3, [R7,R0]
*/

/* Vulnerable
.text:0000A596
.text:0000A596             loc_A596                                ; CODE XREF: sub_A574+52j
.text:0000A596 B0 F5 80 6F                 CMP.W           R0, #0x400
.text:0000A59A 0C D0                       BEQ             loc_A5B6
.text:0000A59C 27 54                       STRB            R7, [R4,R0]
.text:0000A59E 20 18                       ADDS            R0, R4, R0
.text:0000A5A0 31 46                       MOV             R1, R6
.text:0000A5A2 47 70                       STRB            R7, [R0,#1]
.text:0000A5A4 20 46                       MOV             R0, R4
.text:0000A5A6 FF F7 91 FC                 BL              sub_9ECC
.text:0000A5AA 30 46                       MOV             R0, R6
.text:0000A5AC FF F7 C0 FD                 BL              sub_A130
.text:0000A5B0 30 46                       MOV             R0, R6
.text:0000A5B2 FF F7 BD FF                 BL              sub_A530
.text:0000A5B6
*/
   meta:
      descrition = "multicast Netlink message not validated from kernel"
      binary = "/init"
   strings:
      $overflow_check =  {B0 F5 80 6F ?? D0 27 54 20 18 31 46 47 70}
   condition:
      $overflow_check
}
