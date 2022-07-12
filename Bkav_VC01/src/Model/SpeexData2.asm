.section .rodata
.align 4

.global g_uSpeexData2Begin
.global g_uSpeexData2End

g_uSpeexData2Begin:
.incbin "../src/Model/Megkel_voice_command.spxpack"
g_uSpeexData2End:
