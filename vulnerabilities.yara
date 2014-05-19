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

  condition:
      not $a and $is_vold_binary
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

/* unpatched
.text:00008DE0 0D EB 00 01                 ADD.W           R1, SP, R0
.text:00008DE4 06 F0 8C EA                 BLX             sub_F300
.text:00008DE8 4F F4 FA 60                 MOV.W           R0, #0x7D0
.text:00008DEC 06 F0 78 EA                 BLX             sub_F2E0
.text:00008DF0 4F F4 FA 60                 MOV.W           R0, #0x7D0
.text:00008DF4 06 F0 44 EA                 BLX             sub_F280
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
      $vuln_setuid =         {0D EB 00 01
                              06 F0 8C EA
                              4F F4 FA 60
                              06 F0 78 EA
                              4F F4 FA 60
                              06 F0 44 EA}

  condition:
    $vuln_setuid

}
