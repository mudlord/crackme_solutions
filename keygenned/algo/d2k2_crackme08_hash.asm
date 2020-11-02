.686p
.mmx                        ; enable MMX instructions
.xmm                        ; enable SSE instructions
.model flat, stdcall
option casemap:none

.data 

havalb1 dd 0
haval1 dd 0
haval2 dd 0
haval_buf db 100h dup(0)
haval_buf2 db 100h dup(0)
haval_buf3 db 100h dup(0)
.data?

.code

option prologue:none
option epilogue:none

; Start of selected range: 0x00403E2C
d2k2_crackme08_hash PROC x:DWORD,y:DWORD,z:DWORD
push ebp
mov ebp,esp
pushad 
mov ecx,dword ptr [ebp+0Ch]
mov esi,dword ptr [ebp+10h]
mov dword ptr [haval1],ecx
mov byte ptr [havalb1],1h
mov dword ptr [haval2],ecx
mov edi,offset haval_buf2
mov dword ptr [haval_buf],344F7A98h
mov dword ptr [haval_buf+4h],85A308D3h
mov dword ptr [haval_buf+8h],13198A2Eh
mov dword ptr [haval_buf+12],3707344h
mov dword ptr [haval_buf+16],0A4093822h
mov dword ptr [haval_buf+20],299F31D0h
mov dword ptr [haval_buf+24],82EFA98h
mov dword ptr [haval_buf+28],0EC4E6C89h
mov dword ptr [edi],243F6A88h
mov dword ptr [edi+4h],85A308D3h
mov dword ptr [edi+8h],13198A2Eh
mov dword ptr [edi+0Ch],3707344h
mov dword ptr [edi+10h],0A4093822h
mov dword ptr [edi+14h],299F31D0h
mov dword ptr [edi+18h],82EFA98h
mov dword ptr [edi+1Ch],0EC4E6C89h

LABEL_0x004010A9:
cmp dword ptr [haval2],80h
jb LABEL_0x0040242F ; => 0x0040242F
mov ecx,dword ptr [edi+14h]
mov eax,dword ptr [edi+10h]
and ecx,dword ptr [edi+4h]
mov ebx,dword ptr [edi+18h]
xor ecx,eax
and ebx,dword ptr [edi]
xor eax,dword ptr [edi+0Ch]
xor ebx,ecx
and eax,dword ptr [edi+8h]
xor eax,ebx
ror eax,7h
ror dword ptr [edi+1Ch],0Bh
add eax,dword ptr [esi]
add dword ptr [edi+1Ch],eax
mov ecx,dword ptr [edi+10h]
mov eax,dword ptr [edi+0Ch]
and ecx,dword ptr [edi]
mov ebx,dword ptr [edi+14h]
xor ecx,eax
and ebx,dword ptr [edi+1Ch]
xor eax,dword ptr [edi+8h]
xor ebx,ecx
and eax,dword ptr [edi+4h]
xor eax,ebx
ror eax,7h
ror dword ptr [edi+18h],0Bh
add eax,dword ptr [esi+4h]
add dword ptr [edi+18h],eax
mov ecx,dword ptr [edi+0Ch]
mov eax,dword ptr [edi+8h]
and ecx,dword ptr [edi+1Ch]
mov ebx,dword ptr [edi+10h]
xor ecx,eax
and ebx,dword ptr [edi+18h]
xor eax,dword ptr [edi+4h]
xor ebx,ecx
and eax,dword ptr [edi]
xor eax,ebx
ror eax,7h
ror dword ptr [edi+14h],0Bh
add eax,dword ptr [esi+8h]
add dword ptr [edi+14h],eax
mov ecx,dword ptr [edi+8h]
mov eax,dword ptr [edi+4h]
and ecx,dword ptr [edi+18h]
mov ebx,dword ptr [edi+0Ch]
xor ecx,eax
and ebx,dword ptr [edi+14h]
xor eax,dword ptr [edi]
xor ebx,ecx
and eax,dword ptr [edi+1Ch]
xor eax,ebx
ror eax,7h
ror dword ptr [edi+10h],0Bh
add eax,dword ptr [esi+0Ch]
add dword ptr [edi+10h],eax
mov ecx,dword ptr [edi+4h]
mov eax,dword ptr [edi]
and ecx,dword ptr [edi+14h]
mov ebx,dword ptr [edi+8h]
xor ecx,eax
and ebx,dword ptr [edi+10h]
xor eax,dword ptr [edi+1Ch]
xor ebx,ecx
and eax,dword ptr [edi+18h]
xor eax,ebx
ror eax,7h
ror dword ptr [edi+0Ch],0Bh
add eax,dword ptr [esi+10h]
add dword ptr [edi+0Ch],eax
mov ecx,dword ptr [edi]
mov eax,dword ptr [edi+1Ch]
and ecx,dword ptr [edi+10h]
mov ebx,dword ptr [edi+4h]
xor ecx,eax
and ebx,dword ptr [edi+0Ch]
xor eax,dword ptr [edi+18h]
xor ebx,ecx
and eax,dword ptr [edi+14h]
xor eax,ebx
ror eax,7h
ror dword ptr [edi+8h],0Bh
add eax,dword ptr [esi+14h]
add dword ptr [edi+8h],eax
mov ecx,dword ptr [edi+1Ch]
mov eax,dword ptr [edi+18h]
and ecx,dword ptr [edi+0Ch]
mov ebx,dword ptr [edi]
xor ecx,eax
and ebx,dword ptr [edi+8h]
xor eax,dword ptr [edi+14h]
xor ebx,ecx
and eax,dword ptr [edi+10h]
xor eax,ebx
ror eax,7h
ror dword ptr [edi+4h],0Bh
add eax,dword ptr [esi+18h]
add dword ptr [edi+4h],eax
mov ecx,dword ptr [edi+18h]
mov eax,dword ptr [edi+14h]
and ecx,dword ptr [edi+8h]
mov ebx,dword ptr [edi+1Ch]
xor ecx,eax
and ebx,dword ptr [edi+4h]
xor eax,dword ptr [edi+10h]
xor ebx,ecx
and eax,dword ptr [edi+0Ch]
xor eax,ebx
ror eax,7h
ror dword ptr [edi],0Bh
add eax,dword ptr [esi+1Ch]
add dword ptr [edi],eax
mov ecx,dword ptr [edi+14h]
mov eax,dword ptr [edi+10h]
and ecx,dword ptr [edi+4h]
mov ebx,dword ptr [edi+18h]
xor ecx,eax
and ebx,dword ptr [edi]
xor eax,dword ptr [edi+0Ch]
xor ebx,ecx
and eax,dword ptr [edi+8h]
xor eax,ebx
ror eax,7h
ror dword ptr [edi+1Ch],0Bh
add eax,dword ptr [esi+20h]
add dword ptr [edi+1Ch],eax
mov ecx,dword ptr [edi+10h]
mov eax,dword ptr [edi+0Ch]
and ecx,dword ptr [edi]
mov ebx,dword ptr [edi+14h]
xor ecx,eax
and ebx,dword ptr [edi+1Ch]
xor eax,dword ptr [edi+8h]
xor ebx,ecx
and eax,dword ptr [edi+4h]
xor eax,ebx
ror eax,7h
ror dword ptr [edi+18h],0Bh
add eax,dword ptr [esi+24h]
add dword ptr [edi+18h],eax
mov ecx,dword ptr [edi+0Ch]
mov eax,dword ptr [edi+8h]
and ecx,dword ptr [edi+1Ch]
mov ebx,dword ptr [edi+10h]
xor ecx,eax
and ebx,dword ptr [edi+18h]
xor eax,dword ptr [edi+4h]
xor ebx,ecx
and eax,dword ptr [edi]
xor eax,ebx
ror eax,7h
ror dword ptr [edi+14h],0Bh
add eax,dword ptr [esi+28h]
add dword ptr [edi+14h],eax
mov ecx,dword ptr [edi+8h]
mov eax,dword ptr [edi+4h]
and ecx,dword ptr [edi+18h]
mov ebx,dword ptr [edi+0Ch]
xor ecx,eax
and ebx,dword ptr [edi+14h]
xor eax,dword ptr [edi]
xor ebx,ecx
and eax,dword ptr [edi+1Ch]
xor eax,ebx
ror eax,7h
ror dword ptr [edi+10h],0Bh
add eax,dword ptr [esi+2Ch]
add dword ptr [edi+10h],eax
mov ecx,dword ptr [edi+4h]
mov eax,dword ptr [edi]
and ecx,dword ptr [edi+14h]
mov ebx,dword ptr [edi+8h]
xor ecx,eax
and ebx,dword ptr [edi+10h]
xor eax,dword ptr [edi+1Ch]
xor ebx,ecx
and eax,dword ptr [edi+18h]
xor eax,ebx
ror eax,7h
ror dword ptr [edi+0Ch],0Bh
add eax,dword ptr [esi+30h]
add dword ptr [edi+0Ch],eax
mov ecx,dword ptr [edi]
mov eax,dword ptr [edi+1Ch]
and ecx,dword ptr [edi+10h]
mov ebx,dword ptr [edi+4h]
xor ecx,eax
and ebx,dword ptr [edi+0Ch]
xor eax,dword ptr [edi+18h]
xor ebx,ecx
and eax,dword ptr [edi+14h]
xor eax,ebx
ror eax,7h
ror dword ptr [edi+8h],0Bh
add eax,dword ptr [esi+34h]
add dword ptr [edi+8h],eax
mov ecx,dword ptr [edi+1Ch]
mov eax,dword ptr [edi+18h]
and ecx,dword ptr [edi+0Ch]
mov ebx,dword ptr [edi]
xor ecx,eax
and ebx,dword ptr [edi+8h]
xor eax,dword ptr [edi+14h]
xor ebx,ecx
and eax,dword ptr [edi+10h]
xor eax,ebx
ror eax,7h
ror dword ptr [edi+4h],0Bh
add eax,dword ptr [esi+38h]
add dword ptr [edi+4h],eax
mov ecx,dword ptr [edi+18h]
mov eax,dword ptr [edi+14h]
and ecx,dword ptr [edi+8h]
mov ebx,dword ptr [edi+1Ch]
xor ecx,eax
and ebx,dword ptr [edi+4h]
xor eax,dword ptr [edi+10h]
xor ebx,ecx
and eax,dword ptr [edi+0Ch]
xor eax,ebx
ror eax,7h
ror dword ptr [edi],0Bh
add eax,dword ptr [esi+3Ch]
add dword ptr [edi],eax
mov ecx,dword ptr [edi+14h]
mov eax,dword ptr [edi+10h]
and ecx,dword ptr [edi+4h]
mov ebx,dword ptr [edi+18h]
xor ecx,eax
and ebx,dword ptr [edi]
xor eax,dword ptr [edi+0Ch]
xor ebx,ecx
and eax,dword ptr [edi+8h]
xor eax,ebx
ror eax,7h
ror dword ptr [edi+1Ch],0Bh
add eax,dword ptr [esi+40h]
add dword ptr [edi+1Ch],eax
mov ecx,dword ptr [edi+10h]
mov eax,dword ptr [edi+0Ch]
and ecx,dword ptr [edi]
mov ebx,dword ptr [edi+14h]
xor ecx,eax
and ebx,dword ptr [edi+1Ch]
xor eax,dword ptr [edi+8h]
xor ebx,ecx
and eax,dword ptr [edi+4h]
xor eax,ebx
ror eax,7h
ror dword ptr [edi+18h],0Bh
add eax,dword ptr [esi+44h]
add dword ptr [edi+18h],eax
mov ecx,dword ptr [edi+0Ch]
mov eax,dword ptr [edi+8h]
and ecx,dword ptr [edi+1Ch]
mov ebx,dword ptr [edi+10h]
xor ecx,eax
and ebx,dword ptr [edi+18h]
xor eax,dword ptr [edi+4h]
xor ebx,ecx
and eax,dword ptr [edi]
xor eax,ebx
ror eax,7h
ror dword ptr [edi+14h],0Bh
add eax,dword ptr [esi+48h]
add dword ptr [edi+14h],eax
mov ecx,dword ptr [edi+8h]
mov eax,dword ptr [edi+4h]
and ecx,dword ptr [edi+18h]
mov ebx,dword ptr [edi+0Ch]
xor ecx,eax
and ebx,dword ptr [edi+14h]
xor eax,dword ptr [edi]
xor ebx,ecx
and eax,dword ptr [edi+1Ch]
xor eax,ebx
ror eax,7h
ror dword ptr [edi+10h],0Bh
add eax,dword ptr [esi+4Ch]
add dword ptr [edi+10h],eax
mov ecx,dword ptr [edi+4h]
mov eax,dword ptr [edi]
and ecx,dword ptr [edi+14h]
mov ebx,dword ptr [edi+8h]
xor ecx,eax
and ebx,dword ptr [edi+10h]
xor eax,dword ptr [edi+1Ch]
xor ebx,ecx
and eax,dword ptr [edi+18h]
xor eax,ebx
ror eax,7h
ror dword ptr [edi+0Ch],0Bh
add eax,dword ptr [esi+50h]
add dword ptr [edi+0Ch],eax
mov ecx,dword ptr [edi]
mov eax,dword ptr [edi+1Ch]
and ecx,dword ptr [edi+10h]
mov ebx,dword ptr [edi+4h]
xor ecx,eax
and ebx,dword ptr [edi+0Ch]
xor eax,dword ptr [edi+18h]
xor ebx,ecx
and eax,dword ptr [edi+14h]
xor eax,ebx
ror eax,7h
ror dword ptr [edi+8h],0Bh
add eax,dword ptr [esi+54h]
add dword ptr [edi+8h],eax
mov ecx,dword ptr [edi+1Ch]
mov eax,dword ptr [edi+18h]
and ecx,dword ptr [edi+0Ch]
mov ebx,dword ptr [edi]
xor ecx,eax
and ebx,dword ptr [edi+8h]
xor eax,dword ptr [edi+14h]
xor ebx,ecx
and eax,dword ptr [edi+10h]
xor eax,ebx
ror eax,7h
ror dword ptr [edi+4h],0Bh
add eax,dword ptr [esi+58h]
add dword ptr [edi+4h],eax
mov ecx,dword ptr [edi+18h]
mov eax,dword ptr [edi+14h]
and ecx,dword ptr [edi+8h]
mov ebx,dword ptr [edi+1Ch]
xor ecx,eax
and ebx,dword ptr [edi+4h]
xor eax,dword ptr [edi+10h]
xor ebx,ecx
and eax,dword ptr [edi+0Ch]
xor eax,ebx
ror eax,7h
ror dword ptr [edi],0Bh
add eax,dword ptr [esi+5Ch]
add dword ptr [edi],eax
mov ecx,dword ptr [edi+14h]
mov eax,dword ptr [edi+10h]
and ecx,dword ptr [edi+4h]
mov ebx,dword ptr [edi+18h]
xor ecx,eax
and ebx,dword ptr [edi]
xor eax,dword ptr [edi+0Ch]
xor ebx,ecx
and eax,dword ptr [edi+8h]
xor eax,ebx
ror eax,7h
ror dword ptr [edi+1Ch],0Bh
add eax,dword ptr [esi+60h]
add dword ptr [edi+1Ch],eax
mov ecx,dword ptr [edi+10h]
mov eax,dword ptr [edi+0Ch]
and ecx,dword ptr [edi]
mov ebx,dword ptr [edi+14h]
xor ecx,eax
and ebx,dword ptr [edi+1Ch]
xor eax,dword ptr [edi+8h]
xor ebx,ecx
and eax,dword ptr [edi+4h]
xor eax,ebx
ror eax,7h
ror dword ptr [edi+18h],0Bh
add eax,dword ptr [esi+64h]
add dword ptr [edi+18h],eax
mov ecx,dword ptr [edi+0Ch]
mov eax,dword ptr [edi+8h]
and ecx,dword ptr [edi+1Ch]
mov ebx,dword ptr [edi+10h]
xor ecx,eax
and ebx,dword ptr [edi+18h]
xor eax,dword ptr [edi+4h]
xor ebx,ecx
and eax,dword ptr [edi]
xor eax,ebx
ror eax,7h
ror dword ptr [edi+14h],0Bh
add eax,dword ptr [esi+68h]
add dword ptr [edi+14h],eax
mov ecx,dword ptr [edi+8h]
mov eax,dword ptr [edi+4h]
and ecx,dword ptr [edi+18h]
mov ebx,dword ptr [edi+0Ch]
xor ecx,eax
and ebx,dword ptr [edi+14h]
xor eax,dword ptr [edi]
xor ebx,ecx
and eax,dword ptr [edi+1Ch]
xor eax,ebx
ror eax,7h
ror dword ptr [edi+10h],0Bh
add eax,dword ptr [esi+6Ch]
add dword ptr [edi+10h],eax
mov ecx,dword ptr [edi+4h]
mov eax,dword ptr [edi]
and ecx,dword ptr [edi+14h]
mov ebx,dword ptr [edi+8h]
xor ecx,eax
and ebx,dword ptr [edi+10h]
xor eax,dword ptr [edi+1Ch]
xor ebx,ecx
and eax,dword ptr [edi+18h]
xor eax,ebx
ror eax,7h
ror dword ptr [edi+0Ch],0Bh
add eax,dword ptr [esi+70h]
add dword ptr [edi+0Ch],eax
mov ecx,dword ptr [edi]
mov eax,dword ptr [edi+1Ch]
and ecx,dword ptr [edi+10h]
mov ebx,dword ptr [edi+4h]
xor ecx,eax
and ebx,dword ptr [edi+0Ch]
xor eax,dword ptr [edi+18h]
xor ebx,ecx
and eax,dword ptr [edi+14h]
xor eax,ebx
ror eax,7h
ror dword ptr [edi+8h],0Bh
add eax,dword ptr [esi+74h]
add dword ptr [edi+8h],eax
mov ecx,dword ptr [edi+1Ch]
mov eax,dword ptr [edi+18h]
and ecx,dword ptr [edi+0Ch]
mov ebx,dword ptr [edi]
xor ecx,eax
and ebx,dword ptr [edi+8h]
xor eax,dword ptr [edi+14h]
xor ebx,ecx
and eax,dword ptr [edi+10h]
xor eax,ebx
ror eax,7h
ror dword ptr [edi+4h],0Bh
add eax,dword ptr [esi+78h]
add dword ptr [edi+4h],eax
mov ecx,dword ptr [edi+18h]
mov eax,dword ptr [edi+14h]
and ecx,dword ptr [edi+8h]
mov ebx,dword ptr [edi+1Ch]
xor ecx,eax
and ebx,dword ptr [edi+4h]
xor eax,dword ptr [edi+10h]
xor ebx,ecx
and eax,dword ptr [edi+0Ch]
xor eax,ebx
ror eax,7h
ror dword ptr [edi],0Bh
add eax,dword ptr [esi+7Ch]
add dword ptr [edi],eax
mov ebx,dword ptr [edi+4h]
mov eax,dword ptr [edi]
mov edx,dword ptr [edi+8h]
not eax
and ebx,edx
and eax,dword ptr [edi+0Ch]
xor ebx,dword ptr [edi+18h]
xor eax,dword ptr [edi+10h]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi+0Ch]
and eax,dword ptr [edi+14h]
and ecx,dword ptr [edi+4h]
and edx,dword ptr [edi]
xor eax,ecx
xor edx,dword ptr [edi+18h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+1Ch],0Bh
add eax,dword ptr [esi+14h]
add eax,452821E6h
add dword ptr [edi+1Ch],eax
mov ebx,dword ptr [edi]
mov eax,dword ptr [edi+1Ch]
mov edx,dword ptr [edi+4h]
not eax
and ebx,edx
and eax,dword ptr [edi+8h]
xor ebx,dword ptr [edi+14h]
xor eax,dword ptr [edi+0Ch]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi+8h]
and eax,dword ptr [edi+10h]
and ecx,dword ptr [edi]
and edx,dword ptr [edi+1Ch]
xor eax,ecx
xor edx,dword ptr [edi+14h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+18h],0Bh
add eax,dword ptr [esi+38h]
add eax,38D01377h
add dword ptr [edi+18h],eax
mov ebx,dword ptr [edi+1Ch]
mov eax,dword ptr [edi+18h]
mov edx,dword ptr [edi]
not eax
and ebx,edx
and eax,dword ptr [edi+4h]
xor ebx,dword ptr [edi+10h]
xor eax,dword ptr [edi+8h]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi+4h]
and eax,dword ptr [edi+0Ch]
and ecx,dword ptr [edi+1Ch]
and edx,dword ptr [edi+18h]
xor eax,ecx
xor edx,dword ptr [edi+10h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+14h],0Bh
add eax,dword ptr [esi+68h]
add eax,0BE5466CFh
add dword ptr [edi+14h],eax
mov ebx,dword ptr [edi+18h]
mov eax,dword ptr [edi+14h]
mov edx,dword ptr [edi+1Ch]
not eax
and ebx,edx
and eax,dword ptr [edi]
xor ebx,dword ptr [edi+0Ch]
xor eax,dword ptr [edi+4h]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi]
and eax,dword ptr [edi+8h]
and ecx,dword ptr [edi+18h]
and edx,dword ptr [edi+14h]
xor eax,ecx
xor edx,dword ptr [edi+0Ch]
xor eax,edx
ror eax,7h
ror dword ptr [edi+10h],0Bh
add eax,dword ptr [esi+48h]
add eax,34E90C6Ch
add dword ptr [edi+10h],eax
mov ebx,dword ptr [edi+14h]
mov eax,dword ptr [edi+10h]
mov edx,dword ptr [edi+18h]
not eax
and ebx,edx
and eax,dword ptr [edi+1Ch]
xor ebx,dword ptr [edi+8h]
xor eax,dword ptr [edi]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi+1Ch]
and eax,dword ptr [edi+4h]
and ecx,dword ptr [edi+14h]
and edx,dword ptr [edi+10h]
xor eax,ecx
xor edx,dword ptr [edi+8h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+0Ch],0Bh
add eax,dword ptr [esi+2Ch]
add eax,0C0AC29B7h
add dword ptr [edi+0Ch],eax
mov ebx,dword ptr [edi+10h]
mov eax,dword ptr [edi+0Ch]
mov edx,dword ptr [edi+14h]
not eax
and ebx,edx
and eax,dword ptr [edi+18h]
xor ebx,dword ptr [edi+4h]
xor eax,dword ptr [edi+1Ch]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi+18h]
and eax,dword ptr [edi]
and ecx,dword ptr [edi+10h]
and edx,dword ptr [edi+0Ch]
xor eax,ecx
xor edx,dword ptr [edi+4h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+8h],0Bh
add eax,dword ptr [esi+70h]
add eax,0C97C50DDh
add dword ptr [edi+8h],eax
mov ebx,dword ptr [edi+0Ch]
mov eax,dword ptr [edi+8h]
mov edx,dword ptr [edi+10h]
not eax
and ebx,edx
and eax,dword ptr [edi+14h]
xor ebx,dword ptr [edi]
xor eax,dword ptr [edi+18h]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi+14h]
and eax,dword ptr [edi+1Ch]
and ecx,dword ptr [edi+0Ch]
and edx,dword ptr [edi+8h]
xor eax,ecx
xor edx,dword ptr [edi]
xor eax,edx
ror eax,7h
ror dword ptr [edi+4h],0Bh
add eax,dword ptr [esi+1Ch]
add eax,3F84D5B5h
add dword ptr [edi+4h],eax
mov ebx,dword ptr [edi+8h]
mov eax,dword ptr [edi+4h]
mov edx,dword ptr [edi+0Ch]
not eax
and ebx,edx
and eax,dword ptr [edi+10h]
xor ebx,dword ptr [edi+1Ch]
xor eax,dword ptr [edi+14h]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi+10h]
and eax,dword ptr [edi+18h]
and ecx,dword ptr [edi+8h]
and edx,dword ptr [edi+4h]
xor eax,ecx
xor edx,dword ptr [edi+1Ch]
xor eax,edx
ror eax,7h
ror dword ptr [edi],0Bh
add eax,dword ptr [esi+40h]
add eax,0B5470917h
add dword ptr [edi],eax
mov ebx,dword ptr [edi+4h]
mov eax,dword ptr [edi]
mov edx,dword ptr [edi+8h]
not eax
and ebx,edx
and eax,dword ptr [edi+0Ch]
xor ebx,dword ptr [edi+18h]
xor eax,dword ptr [edi+10h]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi+0Ch]
and eax,dword ptr [edi+14h]
and ecx,dword ptr [edi+4h]
and edx,dword ptr [edi]
xor eax,ecx
xor edx,dword ptr [edi+18h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+1Ch],0Bh
add eax,dword ptr [esi]
add eax,9216D5D9h
add dword ptr [edi+1Ch],eax
mov ebx,dword ptr [edi]
mov eax,dword ptr [edi+1Ch]
mov edx,dword ptr [edi+4h]
not eax
and ebx,edx
and eax,dword ptr [edi+8h]
xor ebx,dword ptr [edi+14h]
xor eax,dword ptr [edi+0Ch]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi+8h]
and eax,dword ptr [edi+10h]
and ecx,dword ptr [edi]
and edx,dword ptr [edi+1Ch]
xor eax,ecx
xor edx,dword ptr [edi+14h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+18h],0Bh
add eax,dword ptr [esi+5Ch]
add eax,8979FB1Bh
add dword ptr [edi+18h],eax
mov ebx,dword ptr [edi+1Ch]
mov eax,dword ptr [edi+18h]
mov edx,dword ptr [edi]
not eax
and ebx,edx
and eax,dword ptr [edi+4h]
xor ebx,dword ptr [edi+10h]
xor eax,dword ptr [edi+8h]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi+4h]
and eax,dword ptr [edi+0Ch]
and ecx,dword ptr [edi+1Ch]
and edx,dword ptr [edi+18h]
xor eax,ecx
xor edx,dword ptr [edi+10h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+14h],0Bh
add eax,dword ptr [esi+50h]
add eax,0D1310BA6h
add dword ptr [edi+14h],eax
mov ebx,dword ptr [edi+18h]
mov eax,dword ptr [edi+14h]
mov edx,dword ptr [edi+1Ch]
not eax
and ebx,edx
and eax,dword ptr [edi]
xor ebx,dword ptr [edi+0Ch]
xor eax,dword ptr [edi+4h]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi]
and eax,dword ptr [edi+8h]
and ecx,dword ptr [edi+18h]
and edx,dword ptr [edi+14h]
xor eax,ecx
xor edx,dword ptr [edi+0Ch]
xor eax,edx
ror eax,7h
ror dword ptr [edi+10h],0Bh
add eax,dword ptr [esi+58h]
add eax,98DFB5ACh
add dword ptr [edi+10h],eax
mov ebx,dword ptr [edi+14h]
mov eax,dword ptr [edi+10h]
mov edx,dword ptr [edi+18h]
not eax
and ebx,edx
and eax,dword ptr [edi+1Ch]
xor ebx,dword ptr [edi+8h]
xor eax,dword ptr [edi]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi+1Ch]
and eax,dword ptr [edi+4h]
and ecx,dword ptr [edi+14h]
and edx,dword ptr [edi+10h]
xor eax,ecx
xor edx,dword ptr [edi+8h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+0Ch],0Bh
add eax,dword ptr [esi+4h]
add eax,2FFD72DBh
add dword ptr [edi+0Ch],eax
mov ebx,dword ptr [edi+10h]
mov eax,dword ptr [edi+0Ch]
mov edx,dword ptr [edi+14h]
not eax
and ebx,edx
and eax,dword ptr [edi+18h]
xor ebx,dword ptr [edi+4h]
xor eax,dword ptr [edi+1Ch]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi+18h]
and eax,dword ptr [edi]
and ecx,dword ptr [edi+10h]
and edx,dword ptr [edi+0Ch]
xor eax,ecx
xor edx,dword ptr [edi+4h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+8h],0Bh
add eax,dword ptr [esi+28h]
add eax,0D01ADFB7h
add dword ptr [edi+8h],eax
mov ebx,dword ptr [edi+0Ch]
mov eax,dword ptr [edi+8h]
mov edx,dword ptr [edi+10h]
not eax
and ebx,edx
and eax,dword ptr [edi+14h]
xor ebx,dword ptr [edi]
xor eax,dword ptr [edi+18h]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi+14h]
and eax,dword ptr [edi+1Ch]
and ecx,dword ptr [edi+0Ch]
and edx,dword ptr [edi+8h]
xor eax,ecx
xor edx,dword ptr [edi]
xor eax,edx
ror eax,7h
ror dword ptr [edi+4h],0Bh
add eax,dword ptr [esi+10h]
add eax,0B8E1AFEDh
add dword ptr [edi+4h],eax
mov ebx,dword ptr [edi+8h]
mov eax,dword ptr [edi+4h]
mov edx,dword ptr [edi+0Ch]
not eax
and ebx,edx
and eax,dword ptr [edi+10h]
xor ebx,dword ptr [edi+1Ch]
xor eax,dword ptr [edi+14h]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi+10h]
and eax,dword ptr [edi+18h]
and ecx,dword ptr [edi+8h]
and edx,dword ptr [edi+4h]
xor eax,ecx
xor edx,dword ptr [edi+1Ch]
xor eax,edx
ror eax,7h
ror dword ptr [edi],0Bh
add eax,dword ptr [esi+20h]
add eax,6A267E96h
add dword ptr [edi],eax
mov ebx,dword ptr [edi+4h]
mov eax,dword ptr [edi]
mov edx,dword ptr [edi+8h]
not eax
and ebx,edx
and eax,dword ptr [edi+0Ch]
xor ebx,dword ptr [edi+18h]
xor eax,dword ptr [edi+10h]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi+0Ch]
and eax,dword ptr [edi+14h]
and ecx,dword ptr [edi+4h]
and edx,dword ptr [edi]
xor eax,ecx
xor edx,dword ptr [edi+18h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+1Ch],0Bh
add eax,dword ptr [esi+78h]
add eax,0BA7C9045h
add dword ptr [edi+1Ch],eax
mov ebx,dword ptr [edi]
mov eax,dword ptr [edi+1Ch]
mov edx,dword ptr [edi+4h]
not eax
and ebx,edx
and eax,dword ptr [edi+8h]
xor ebx,dword ptr [edi+14h]
xor eax,dword ptr [edi+0Ch]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi+8h]
and eax,dword ptr [edi+10h]
and ecx,dword ptr [edi]
and edx,dword ptr [edi+1Ch]
xor eax,ecx
xor edx,dword ptr [edi+14h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+18h],0Bh
add eax,dword ptr [esi+0Ch]
add eax,0F12C7F99h
add dword ptr [edi+18h],eax
mov ebx,dword ptr [edi+1Ch]
mov eax,dword ptr [edi+18h]
mov edx,dword ptr [edi]
not eax
and ebx,edx
and eax,dword ptr [edi+4h]
xor ebx,dword ptr [edi+10h]
xor eax,dword ptr [edi+8h]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi+4h]
and eax,dword ptr [edi+0Ch]
and ecx,dword ptr [edi+1Ch]
and edx,dword ptr [edi+18h]
xor eax,ecx
xor edx,dword ptr [edi+10h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+14h],0Bh
add eax,dword ptr [esi+54h]
add eax,24A19947h
add dword ptr [edi+14h],eax
mov ebx,dword ptr [edi+18h]
mov eax,dword ptr [edi+14h]
mov edx,dword ptr [edi+1Ch]
not eax
and ebx,edx
and eax,dword ptr [edi]
xor ebx,dword ptr [edi+0Ch]
xor eax,dword ptr [edi+4h]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi]
and eax,dword ptr [edi+8h]
and ecx,dword ptr [edi+18h]
and edx,dword ptr [edi+14h]
xor eax,ecx
xor edx,dword ptr [edi+0Ch]
xor eax,edx
ror eax,7h
ror dword ptr [edi+10h],0Bh
add eax,dword ptr [esi+24h]
add eax,0B3916CF7h
add dword ptr [edi+10h],eax
mov ebx,dword ptr [edi+14h]
mov eax,dword ptr [edi+10h]
mov edx,dword ptr [edi+18h]
not eax
and ebx,edx
and eax,dword ptr [edi+1Ch]
xor ebx,dword ptr [edi+8h]
xor eax,dword ptr [edi]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi+1Ch]
and eax,dword ptr [edi+4h]
and ecx,dword ptr [edi+14h]
and edx,dword ptr [edi+10h]
xor eax,ecx
xor edx,dword ptr [edi+8h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+0Ch],0Bh
add eax,dword ptr [esi+44h]
add eax,801F2E2h
add dword ptr [edi+0Ch],eax
mov ebx,dword ptr [edi+10h]
mov eax,dword ptr [edi+0Ch]
mov edx,dword ptr [edi+14h]
not eax
and ebx,edx
and eax,dword ptr [edi+18h]
xor ebx,dword ptr [edi+4h]
xor eax,dword ptr [edi+1Ch]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi+18h]
and eax,dword ptr [edi]
and ecx,dword ptr [edi+10h]
and edx,dword ptr [edi+0Ch]
xor eax,ecx
xor edx,dword ptr [edi+4h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+8h],0Bh
add eax,dword ptr [esi+60h]
add eax,858EFC16h
add dword ptr [edi+8h],eax
mov ebx,dword ptr [edi+0Ch]
mov eax,dword ptr [edi+8h]
mov edx,dword ptr [edi+10h]
not eax
and ebx,edx
and eax,dword ptr [edi+14h]
xor ebx,dword ptr [edi]
xor eax,dword ptr [edi+18h]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi+14h]
and eax,dword ptr [edi+1Ch]
and ecx,dword ptr [edi+0Ch]
and edx,dword ptr [edi+8h]
xor eax,ecx
xor edx,dword ptr [edi]
xor eax,edx
ror eax,7h
ror dword ptr [edi+4h],0Bh
add eax,dword ptr [esi+74h]
add eax,636920D8h
add dword ptr [edi+4h],eax
mov ebx,dword ptr [edi+8h]
mov eax,dword ptr [edi+4h]
mov edx,dword ptr [edi+0Ch]
not eax
and ebx,edx
and eax,dword ptr [edi+10h]
xor ebx,dword ptr [edi+1Ch]
xor eax,dword ptr [edi+14h]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi+10h]
and eax,dword ptr [edi+18h]
and ecx,dword ptr [edi+8h]
and edx,dword ptr [edi+4h]
xor eax,ecx
xor edx,dword ptr [edi+1Ch]
xor eax,edx
ror eax,7h
ror dword ptr [edi],0Bh
add eax,dword ptr [esi+18h]
add eax,71574E69h
add dword ptr [edi],eax
mov ebx,dword ptr [edi+4h]
mov eax,dword ptr [edi]
mov edx,dword ptr [edi+8h]
not eax
and ebx,edx
and eax,dword ptr [edi+0Ch]
xor ebx,dword ptr [edi+18h]
xor eax,dword ptr [edi+10h]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi+0Ch]
and eax,dword ptr [edi+14h]
and ecx,dword ptr [edi+4h]
and edx,dword ptr [edi]
xor eax,ecx
xor edx,dword ptr [edi+18h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+1Ch],0Bh
add eax,dword ptr [esi+4Ch]
add eax,0A458FEA3h
add dword ptr [edi+1Ch],eax
mov ebx,dword ptr [edi]
mov eax,dword ptr [edi+1Ch]
mov edx,dword ptr [edi+4h]
not eax
and ebx,edx
and eax,dword ptr [edi+8h]
xor ebx,dword ptr [edi+14h]
xor eax,dword ptr [edi+0Ch]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi+8h]
and eax,dword ptr [edi+10h]
and ecx,dword ptr [edi]
and edx,dword ptr [edi+1Ch]
xor eax,ecx
xor edx,dword ptr [edi+14h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+18h],0Bh
add eax,dword ptr [esi+30h]
add eax,0F4933D7Eh
add dword ptr [edi+18h],eax
mov ebx,dword ptr [edi+1Ch]
mov eax,dword ptr [edi+18h]
mov edx,dword ptr [edi]
not eax
and ebx,edx
and eax,dword ptr [edi+4h]
xor ebx,dword ptr [edi+10h]
xor eax,dword ptr [edi+8h]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi+4h]
and eax,dword ptr [edi+0Ch]
and ecx,dword ptr [edi+1Ch]
and edx,dword ptr [edi+18h]
xor eax,ecx
xor edx,dword ptr [edi+10h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+14h],0Bh
add eax,dword ptr [esi+3Ch]
add eax,0D95748Fh
add dword ptr [edi+14h],eax
mov ebx,dword ptr [edi+18h]
mov eax,dword ptr [edi+14h]
mov edx,dword ptr [edi+1Ch]
not eax
and ebx,edx
and eax,dword ptr [edi]
xor ebx,dword ptr [edi+0Ch]
xor eax,dword ptr [edi+4h]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi]
and eax,dword ptr [edi+8h]
and ecx,dword ptr [edi+18h]
and edx,dword ptr [edi+14h]
xor eax,ecx
xor edx,dword ptr [edi+0Ch]
xor eax,edx
ror eax,7h
ror dword ptr [edi+10h],0Bh
add eax,dword ptr [esi+34h]
add eax,728EB658h
add dword ptr [edi+10h],eax
mov ebx,dword ptr [edi+14h]
mov eax,dword ptr [edi+10h]
mov edx,dword ptr [edi+18h]
not eax
and ebx,edx
and eax,dword ptr [edi+1Ch]
xor ebx,dword ptr [edi+8h]
xor eax,dword ptr [edi]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi+1Ch]
and eax,dword ptr [edi+4h]
and ecx,dword ptr [edi+14h]
and edx,dword ptr [edi+10h]
xor eax,ecx
xor edx,dword ptr [edi+8h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+0Ch],0Bh
add eax,dword ptr [esi+8h]
add eax,718BCD58h
add dword ptr [edi+0Ch],eax
mov ebx,dword ptr [edi+10h]
mov eax,dword ptr [edi+0Ch]
mov edx,dword ptr [edi+14h]
not eax
and ebx,edx
and eax,dword ptr [edi+18h]
xor ebx,dword ptr [edi+4h]
xor eax,dword ptr [edi+1Ch]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi+18h]
and eax,dword ptr [edi]
and ecx,dword ptr [edi+10h]
and edx,dword ptr [edi+0Ch]
xor eax,ecx
xor edx,dword ptr [edi+4h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+8h],0Bh
add eax,dword ptr [esi+64h]
add eax,82154AEEh
add dword ptr [edi+8h],eax
mov ebx,dword ptr [edi+0Ch]
mov eax,dword ptr [edi+8h]
mov edx,dword ptr [edi+10h]
not eax
and ebx,edx
and eax,dword ptr [edi+14h]
xor ebx,dword ptr [edi]
xor eax,dword ptr [edi+18h]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi+14h]
and eax,dword ptr [edi+1Ch]
and ecx,dword ptr [edi+0Ch]
and edx,dword ptr [edi+8h]
xor eax,ecx
xor edx,dword ptr [edi]
xor eax,edx
ror eax,7h
ror dword ptr [edi+4h],0Bh
add eax,dword ptr [esi+7Ch]
add eax,7B54A41Dh
add dword ptr [edi+4h],eax
mov ebx,dword ptr [edi+8h]
mov eax,dword ptr [edi+4h]
mov edx,dword ptr [edi+0Ch]
not eax
and ebx,edx
and eax,dword ptr [edi+10h]
xor ebx,dword ptr [edi+1Ch]
xor eax,dword ptr [edi+14h]
mov ecx,edx
xor eax,ebx
xor ecx,dword ptr [edi+10h]
and eax,dword ptr [edi+18h]
and ecx,dword ptr [edi+8h]
and edx,dword ptr [edi+4h]
xor eax,ecx
xor edx,dword ptr [edi+1Ch]
xor eax,edx
ror eax,7h
ror dword ptr [edi],0Bh
add eax,dword ptr [esi+6Ch]
add eax,0C25A59B5h
add dword ptr [edi],eax
mov edx,dword ptr [edi+14h]
mov eax,dword ptr [edi+4h]
and edx,dword ptr [edi+10h]
mov ebx,dword ptr [edi+14h]
and eax,dword ptr [edi+10h]
xor edx,dword ptr [edi+18h]
xor eax,dword ptr [edi]
and ebx,dword ptr [edi+8h]
xor edx,dword ptr [edi]
xor eax,ebx
and edx,dword ptr [edi+0Ch]
xor eax,edx
ror eax,7h
ror dword ptr [edi+1Ch],0Bh
add eax,dword ptr [esi+4Ch]
add eax,9C30D539h
add dword ptr [edi+1Ch],eax
mov edx,dword ptr [edi+10h]
mov eax,dword ptr [edi]
and edx,dword ptr [edi+0Ch]
mov ebx,dword ptr [edi+10h]
and eax,dword ptr [edi+0Ch]
xor edx,dword ptr [edi+14h]
xor eax,dword ptr [edi+1Ch]
and ebx,dword ptr [edi+4h]
xor edx,dword ptr [edi+1Ch]
xor eax,ebx
and edx,dword ptr [edi+8h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+18h],0Bh
add eax,dword ptr [esi+24h]
add eax,2AF26013h
add dword ptr [edi+18h],eax
mov edx,dword ptr [edi+0Ch]
mov eax,dword ptr [edi+1Ch]
and edx,dword ptr [edi+8h]
mov ebx,dword ptr [edi+0Ch]
and eax,dword ptr [edi+8h]
xor edx,dword ptr [edi+10h]
xor eax,dword ptr [edi+18h]
and ebx,dword ptr [edi]
xor edx,dword ptr [edi+18h]
xor eax,ebx
and edx,dword ptr [edi+4h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+14h],0Bh
add eax,dword ptr [esi+10h]
add eax,0C5D1B023h
add dword ptr [edi+14h],eax
mov edx,dword ptr [edi+8h]
mov eax,dword ptr [edi+18h]
and edx,dword ptr [edi+4h]
mov ebx,dword ptr [edi+8h]
and eax,dword ptr [edi+4h]
xor edx,dword ptr [edi+0Ch]
xor eax,dword ptr [edi+14h]
and ebx,dword ptr [edi+1Ch]
xor edx,dword ptr [edi+14h]
xor eax,ebx
and edx,dword ptr [edi]
xor eax,edx
ror eax,7h
ror dword ptr [edi+10h],0Bh
add eax,dword ptr [esi+50h]
add eax,286085F0h
add dword ptr [edi+10h],eax
mov edx,dword ptr [edi+4h]
mov eax,dword ptr [edi+14h]
and edx,dword ptr [edi]
mov ebx,dword ptr [edi+4h]
and eax,dword ptr [edi]
xor edx,dword ptr [edi+8h]
xor eax,dword ptr [edi+10h]
and ebx,dword ptr [edi+18h]
xor edx,dword ptr [edi+10h]
xor eax,ebx
and edx,dword ptr [edi+1Ch]
xor eax,edx
ror eax,7h
ror dword ptr [edi+0Ch],0Bh
add eax,dword ptr [esi+70h]
add eax,0CA417918h
add dword ptr [edi+0Ch],eax
mov edx,dword ptr [edi]
mov eax,dword ptr [edi+10h]
and edx,dword ptr [edi+1Ch]
mov ebx,dword ptr [edi]
and eax,dword ptr [edi+1Ch]
xor edx,dword ptr [edi+4h]
xor eax,dword ptr [edi+0Ch]
and ebx,dword ptr [edi+14h]
xor edx,dword ptr [edi+0Ch]
xor eax,ebx
and edx,dword ptr [edi+18h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+8h],0Bh
add eax,dword ptr [esi+44h]
add eax,0B8DB38EFh
add dword ptr [edi+8h],eax
mov edx,dword ptr [edi+1Ch]
mov eax,dword ptr [edi+0Ch]
and edx,dword ptr [edi+18h]
mov ebx,dword ptr [edi+1Ch]
and eax,dword ptr [edi+18h]
xor edx,dword ptr [edi]
xor eax,dword ptr [edi+8h]
and ebx,dword ptr [edi+10h]
xor edx,dword ptr [edi+8h]
xor eax,ebx
and edx,dword ptr [edi+14h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+4h],0Bh
add eax,dword ptr [esi+20h]
add eax,8E79DCB0h
add dword ptr [edi+4h],eax
mov edx,dword ptr [edi+18h]
mov eax,dword ptr [edi+8h]
and edx,dword ptr [edi+14h]
mov ebx,dword ptr [edi+18h]
and eax,dword ptr [edi+14h]
xor edx,dword ptr [edi+1Ch]
xor eax,dword ptr [edi+4h]
and ebx,dword ptr [edi+0Ch]
xor edx,dword ptr [edi+4h]
xor eax,ebx
and edx,dword ptr [edi+10h]
xor eax,edx
ror eax,7h
ror dword ptr [edi],0Bh
add eax,dword ptr [esi+58h]
add eax,603A180Eh
add dword ptr [edi],eax
mov edx,dword ptr [edi+14h]
mov eax,dword ptr [edi+4h]
and edx,dword ptr [edi+10h]
mov ebx,dword ptr [edi+14h]
and eax,dword ptr [edi+10h]
xor edx,dword ptr [edi+18h]
xor eax,dword ptr [edi]
and ebx,dword ptr [edi+8h]
xor edx,dword ptr [edi]
xor eax,ebx
and edx,dword ptr [edi+0Ch]
xor eax,edx
ror eax,7h
ror dword ptr [edi+1Ch],0Bh
add eax,dword ptr [esi+74h]
add eax,6C9E0E8Bh
add dword ptr [edi+1Ch],eax
mov edx,dword ptr [edi+10h]
mov eax,dword ptr [edi]
and edx,dword ptr [edi+0Ch]
mov ebx,dword ptr [edi+10h]
and eax,dword ptr [edi+0Ch]
xor edx,dword ptr [edi+14h]
xor eax,dword ptr [edi+1Ch]
and ebx,dword ptr [edi+4h]
xor edx,dword ptr [edi+1Ch]
xor eax,ebx
and edx,dword ptr [edi+8h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+18h],0Bh
add eax,dword ptr [esi+38h]
add eax,0B01E8A3Eh
add dword ptr [edi+18h],eax
mov edx,dword ptr [edi+0Ch]
mov eax,dword ptr [edi+1Ch]
and edx,dword ptr [edi+8h]
mov ebx,dword ptr [edi+0Ch]
and eax,dword ptr [edi+8h]
xor edx,dword ptr [edi+10h]
xor eax,dword ptr [edi+18h]
and ebx,dword ptr [edi]
xor edx,dword ptr [edi+18h]
xor eax,ebx
and edx,dword ptr [edi+4h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+14h],0Bh
add eax,dword ptr [esi+64h]
add eax,0D71577C1h
add dword ptr [edi+14h],eax
mov edx,dword ptr [edi+8h]
mov eax,dword ptr [edi+18h]
and edx,dword ptr [edi+4h]
mov ebx,dword ptr [edi+8h]
and eax,dword ptr [edi+4h]
xor edx,dword ptr [edi+0Ch]
xor eax,dword ptr [edi+14h]
and ebx,dword ptr [edi+1Ch]
xor edx,dword ptr [edi+14h]
xor eax,ebx
and edx,dword ptr [edi]
xor eax,edx
ror eax,7h
ror dword ptr [edi+10h],0Bh
add eax,dword ptr [esi+30h]
add eax,0BD314B27h
add dword ptr [edi+10h],eax
mov edx,dword ptr [edi+4h]
mov eax,dword ptr [edi+14h]
and edx,dword ptr [edi]
mov ebx,dword ptr [edi+4h]
and eax,dword ptr [edi]
xor edx,dword ptr [edi+8h]
xor eax,dword ptr [edi+10h]
and ebx,dword ptr [edi+18h]
xor edx,dword ptr [edi+10h]
xor eax,ebx
and edx,dword ptr [edi+1Ch]
xor eax,edx
ror eax,7h
ror dword ptr [edi+0Ch],0Bh
add eax,dword ptr [esi+60h]
add eax,78AF2FDAh
add dword ptr [edi+0Ch],eax
mov edx,dword ptr [edi]
mov eax,dword ptr [edi+10h]
and edx,dword ptr [edi+1Ch]
mov ebx,dword ptr [edi]
and eax,dword ptr [edi+1Ch]
xor edx,dword ptr [edi+4h]
xor eax,dword ptr [edi+0Ch]
and ebx,dword ptr [edi+14h]
xor edx,dword ptr [edi+0Ch]
xor eax,ebx
and edx,dword ptr [edi+18h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+8h],0Bh
add eax,dword ptr [esi+78h]
add eax,55605C60h
add dword ptr [edi+8h],eax
mov edx,dword ptr [edi+1Ch]
mov eax,dword ptr [edi+0Ch]
and edx,dword ptr [edi+18h]
mov ebx,dword ptr [edi+1Ch]
and eax,dword ptr [edi+18h]
xor edx,dword ptr [edi]
xor eax,dword ptr [edi+8h]
and ebx,dword ptr [edi+10h]
xor edx,dword ptr [edi+8h]
xor eax,ebx
and edx,dword ptr [edi+14h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+4h],0Bh
add eax,dword ptr [esi+40h]
add eax,0E65525F3h
add dword ptr [edi+4h],eax
mov edx,dword ptr [edi+18h]
mov eax,dword ptr [edi+8h]
and edx,dword ptr [edi+14h]
mov ebx,dword ptr [edi+18h]
and eax,dword ptr [edi+14h]
xor edx,dword ptr [edi+1Ch]
xor eax,dword ptr [edi+4h]
and ebx,dword ptr [edi+0Ch]
xor edx,dword ptr [edi+4h]
xor eax,ebx
and edx,dword ptr [edi+10h]
xor eax,edx
ror eax,7h
ror dword ptr [edi],0Bh
add eax,dword ptr [esi+68h]
add eax,0AA55AB94h
add dword ptr [edi],eax
mov edx,dword ptr [edi+14h]
mov eax,dword ptr [edi+4h]
and edx,dword ptr [edi+10h]
mov ebx,dword ptr [edi+14h]
and eax,dword ptr [edi+10h]
xor edx,dword ptr [edi+18h]
xor eax,dword ptr [edi]
and ebx,dword ptr [edi+8h]
xor edx,dword ptr [edi]
xor eax,ebx
and edx,dword ptr [edi+0Ch]
xor eax,edx
ror eax,7h
ror dword ptr [edi+1Ch],0Bh
add eax,dword ptr [esi+7Ch]
add eax,57489862h
add dword ptr [edi+1Ch],eax
mov edx,dword ptr [edi+10h]
mov eax,dword ptr [edi]
and edx,dword ptr [edi+0Ch]
mov ebx,dword ptr [edi+10h]
and eax,dword ptr [edi+0Ch]
xor edx,dword ptr [edi+14h]
xor eax,dword ptr [edi+1Ch]
and ebx,dword ptr [edi+4h]
xor edx,dword ptr [edi+1Ch]
xor eax,ebx
and edx,dword ptr [edi+8h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+18h],0Bh
add eax,dword ptr [esi+3Ch]
add eax,63E81440h
add dword ptr [edi+18h],eax
mov edx,dword ptr [edi+0Ch]
mov eax,dword ptr [edi+1Ch]
and edx,dword ptr [edi+8h]
mov ebx,dword ptr [edi+0Ch]
and eax,dword ptr [edi+8h]
xor edx,dword ptr [edi+10h]
xor eax,dword ptr [edi+18h]
and ebx,dword ptr [edi]
xor edx,dword ptr [edi+18h]
xor eax,ebx
and edx,dword ptr [edi+4h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+14h],0Bh
add eax,dword ptr [esi+1Ch]
add eax,55CA396Ah
add dword ptr [edi+14h],eax
mov edx,dword ptr [edi+8h]
mov eax,dword ptr [edi+18h]
and edx,dword ptr [edi+4h]
mov ebx,dword ptr [edi+8h]
and eax,dword ptr [edi+4h]
xor edx,dword ptr [edi+0Ch]
xor eax,dword ptr [edi+14h]
and ebx,dword ptr [edi+1Ch]
xor edx,dword ptr [edi+14h]
xor eax,ebx
and edx,dword ptr [edi]
xor eax,edx
ror eax,7h
ror dword ptr [edi+10h],0Bh
add eax,dword ptr [esi+0Ch]
add eax,2AAB10B6h
add dword ptr [edi+10h],eax
mov edx,dword ptr [edi+4h]
mov eax,dword ptr [edi+14h]
and edx,dword ptr [edi]
mov ebx,dword ptr [edi+4h]
and eax,dword ptr [edi]
xor edx,dword ptr [edi+8h]
xor eax,dword ptr [edi+10h]
and ebx,dword ptr [edi+18h]
xor edx,dword ptr [edi+10h]
xor eax,ebx
and edx,dword ptr [edi+1Ch]
xor eax,edx
ror eax,7h
ror dword ptr [edi+0Ch],0Bh
add eax,dword ptr [esi+4h]
add eax,0B4CC5C34h
add dword ptr [edi+0Ch],eax
mov edx,dword ptr [edi]
mov eax,dword ptr [edi+10h]
and edx,dword ptr [edi+1Ch]
mov ebx,dword ptr [edi]
and eax,dword ptr [edi+1Ch]
xor edx,dword ptr [edi+4h]
xor eax,dword ptr [edi+0Ch]
and ebx,dword ptr [edi+14h]
xor edx,dword ptr [edi+0Ch]
xor eax,ebx
and edx,dword ptr [edi+18h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+8h],0Bh
add eax,dword ptr [esi]
add eax,1141E8CEh
add dword ptr [edi+8h],eax
mov edx,dword ptr [edi+1Ch]
mov eax,dword ptr [edi+0Ch]
and edx,dword ptr [edi+18h]
mov ebx,dword ptr [edi+1Ch]
and eax,dword ptr [edi+18h]
xor edx,dword ptr [edi]
xor eax,dword ptr [edi+8h]
and ebx,dword ptr [edi+10h]
xor edx,dword ptr [edi+8h]
xor eax,ebx
and edx,dword ptr [edi+14h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+4h],0Bh
add eax,dword ptr [esi+48h]
add eax,0A15486AFh
add dword ptr [edi+4h],eax
mov edx,dword ptr [edi+18h]
mov eax,dword ptr [edi+8h]
and edx,dword ptr [edi+14h]
mov ebx,dword ptr [edi+18h]
and eax,dword ptr [edi+14h]
xor edx,dword ptr [edi+1Ch]
xor eax,dword ptr [edi+4h]
and ebx,dword ptr [edi+0Ch]
xor edx,dword ptr [edi+4h]
xor eax,ebx
and edx,dword ptr [edi+10h]
xor eax,edx
ror eax,7h
ror dword ptr [edi],0Bh
add eax,dword ptr [esi+6Ch]
add eax,7C72E993h
add dword ptr [edi],eax
mov edx,dword ptr [edi+14h]
mov eax,dword ptr [edi+4h]
and edx,dword ptr [edi+10h]
mov ebx,dword ptr [edi+14h]
and eax,dword ptr [edi+10h]
xor edx,dword ptr [edi+18h]
xor eax,dword ptr [edi]
and ebx,dword ptr [edi+8h]
xor edx,dword ptr [edi]
xor eax,ebx
and edx,dword ptr [edi+0Ch]
xor eax,edx
ror eax,7h
ror dword ptr [edi+1Ch],0Bh
add eax,dword ptr [esi+34h]
add eax,0B3EE1411h
add dword ptr [edi+1Ch],eax
mov edx,dword ptr [edi+10h]
mov eax,dword ptr [edi]
and edx,dword ptr [edi+0Ch]
mov ebx,dword ptr [edi+10h]
and eax,dword ptr [edi+0Ch]
xor edx,dword ptr [edi+14h]
xor eax,dword ptr [edi+1Ch]
and ebx,dword ptr [edi+4h]
xor edx,dword ptr [edi+1Ch]
xor eax,ebx
and edx,dword ptr [edi+8h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+18h],0Bh
add eax,dword ptr [esi+18h]
add eax,636FBC2Ah
add dword ptr [edi+18h],eax
mov edx,dword ptr [edi+0Ch]
mov eax,dword ptr [edi+1Ch]
and edx,dword ptr [edi+8h]
mov ebx,dword ptr [edi+0Ch]
and eax,dword ptr [edi+8h]
xor edx,dword ptr [edi+10h]
xor eax,dword ptr [edi+18h]
and ebx,dword ptr [edi]
xor edx,dword ptr [edi+18h]
xor eax,ebx
and edx,dword ptr [edi+4h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+14h],0Bh
add eax,dword ptr [esi+54h]
add eax,2BA9C55Dh
add dword ptr [edi+14h],eax
mov edx,dword ptr [edi+8h]
mov eax,dword ptr [edi+18h]
and edx,dword ptr [edi+4h]
mov ebx,dword ptr [edi+8h]
and eax,dword ptr [edi+4h]
xor edx,dword ptr [edi+0Ch]
xor eax,dword ptr [edi+14h]
and ebx,dword ptr [edi+1Ch]
xor edx,dword ptr [edi+14h]
xor eax,ebx
and edx,dword ptr [edi]
xor eax,edx
ror eax,7h
ror dword ptr [edi+10h],0Bh
add eax,dword ptr [esi+28h]
add eax,741831F6h
add dword ptr [edi+10h],eax
mov edx,dword ptr [edi+4h]
mov eax,dword ptr [edi+14h]
and edx,dword ptr [edi]
mov ebx,dword ptr [edi+4h]
and eax,dword ptr [edi]
xor edx,dword ptr [edi+8h]
xor eax,dword ptr [edi+10h]
and ebx,dword ptr [edi+18h]
xor edx,dword ptr [edi+10h]
xor eax,ebx
and edx,dword ptr [edi+1Ch]
xor eax,edx
ror eax,7h
ror dword ptr [edi+0Ch],0Bh
add eax,dword ptr [esi+5Ch]
add eax,0CE5C3E16h
add dword ptr [edi+0Ch],eax
mov edx,dword ptr [edi]
mov eax,dword ptr [edi+10h]
and edx,dword ptr [edi+1Ch]
mov ebx,dword ptr [edi]
and eax,dword ptr [edi+1Ch]
xor edx,dword ptr [edi+4h]
xor eax,dword ptr [edi+0Ch]
and ebx,dword ptr [edi+14h]
xor edx,dword ptr [edi+0Ch]
xor eax,ebx
and edx,dword ptr [edi+18h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+8h],0Bh
add eax,dword ptr [esi+2Ch]
add eax,9B87931Eh
add dword ptr [edi+8h],eax
mov edx,dword ptr [edi+1Ch]
mov eax,dword ptr [edi+0Ch]
and edx,dword ptr [edi+18h]
mov ebx,dword ptr [edi+1Ch]
and eax,dword ptr [edi+18h]
xor edx,dword ptr [edi]
xor eax,dword ptr [edi+8h]
and ebx,dword ptr [edi+10h]
xor edx,dword ptr [edi+8h]
xor eax,ebx
and edx,dword ptr [edi+14h]
xor eax,edx
ror eax,7h
ror dword ptr [edi+4h],0Bh
add eax,dword ptr [esi+14h]
add eax,0AFD6BA33h
add dword ptr [edi+4h],eax
mov edx,dword ptr [edi+18h]
mov eax,dword ptr [edi+8h]
and edx,dword ptr [edi+14h]
mov ebx,dword ptr [edi+18h]
and eax,dword ptr [edi+14h]
xor edx,dword ptr [edi+1Ch]
xor eax,dword ptr [edi+4h]
and ebx,dword ptr [edi+0Ch]
xor edx,dword ptr [edi+4h]
xor eax,ebx
and edx,dword ptr [edi+10h]
xor eax,edx
ror eax,7h
ror dword ptr [edi],0Bh
add eax,dword ptr [esi+8h]
add eax,6C24CF5Ch
add dword ptr [edi],eax
mov edx,dword ptr [edi]
mov ecx,dword ptr [edi+4h]
mov ebx,dword ptr [edi+8h]
mov eax,dword ptr [edi+0Ch]
add edx,dword ptr [haval_buf]
add ecx,dword ptr [haval_buf+4]
add ebx,dword ptr [haval_buf+8]
add eax,dword ptr [haval_buf+12]
mov dword ptr [haval_buf],edx
mov dword ptr [haval_buf+4h],ecx
mov dword ptr [haval_buf+8h],ebx
mov dword ptr [haval_buf+12],eax
mov dword ptr [edi],edx
mov dword ptr [edi+4h],ecx
mov dword ptr [edi+8h],ebx
mov dword ptr [edi+0Ch],eax
mov edx,dword ptr [edi+10h]
mov ecx,dword ptr [edi+14h]
mov ebx,dword ptr [edi+18h]
mov eax,dword ptr [edi+1Ch]
add edx,dword ptr [haval_buf+16]
add ecx,dword ptr [haval_buf+20]
add ebx,dword ptr [haval_buf+24]
add eax,dword ptr [haval_buf+28]
mov dword ptr [haval_buf+16],edx
mov dword ptr [haval_buf+20],ecx
mov dword ptr [haval_buf+24],ebx
mov dword ptr [haval_buf+28],eax
mov dword ptr [edi+10h],edx
mov dword ptr [edi+14h],ecx
mov dword ptr [edi+18h],ebx
mov dword ptr [edi+1Ch],eax
sub dword ptr [haval2],80h
add esi,80h
jmp LABEL_0x004010A9 ; => 0x004010A9

LABEL_0x0040242F:
cmp byte ptr [havalb1],0h
je LABEL_0x004024B5 ; => 0x004024B5
mov ecx,dword ptr [haval2]
mov byte ptr [havalb1],0h
mov dword ptr [haval2],80h
mov eax,ecx
mov edi,offset haval_buf3
test eax,eax
je LABEL_0x00402467 ; => 0x00402467

LABEL_0x0040245A:
mov bl,byte ptr [ecx+esi-1h]
mov byte ptr [ecx+edi-1h],bl
dec ecx
jne LABEL_0x0040245A ; => 0x0040245A
add edi,eax

LABEL_0x00402467:
mov byte ptr [edi],80h
mov ecx,eax
inc edi
sub ecx,75h
neg ecx
je LABEL_0x0040248B ; => 0x0040248B
jns LABEL_0x00402486 ; => 0x00402486
add dword ptr [haval2],80h
add ecx,80h

LABEL_0x00402486:
xor al,al
cld 
rep stosb 

LABEL_0x0040248B:
mov word ptr [edi],2019h
add edi,2h
xor edx,edx
mov eax,dword ptr [haval1]
mov ebx,8h
mul ebx
mov dword ptr [edi],eax
mov dword ptr [edi+4h],edx
mov esi,offset haval_buf3
mov edi,offset haval_buf2
jmp LABEL_0x004010A9 ; => 0x004010A9

LABEL_0x004024B5:
mov esi,dword ptr [ebp+8h]
mov cx,word ptr [edi+1Bh]
mov dl,byte ptr [edi+1Dh]
shl ecx,10h
shl edx,10h
mov ch,byte ptr [edi+16h]
mov dx,word ptr [edi+17h]
mov cl,byte ptr [edi+11h]
shl edx,8h
add ecx,dword ptr [edi]
mov dl,byte ptr [edi+12h]
mov dword ptr [esi],ecx
mov al,byte ptr [edi+19h]
mov bl,byte ptr [edi+1Ah]
add edx,dword ptr [edi+4h]
mov bh,byte ptr [edi+1Fh]
mov ah,byte ptr [edi+1Eh]
mov dword ptr [esi+4h],edx
shl ebx,10h
shl eax,10h
mov bl,byte ptr [edi+10h]
mov ax,word ptr [edi+13h]
mov bh,byte ptr [edi+15h]
add eax,dword ptr [edi+8h]
add ebx,dword ptr [edi+0Ch]
mov dword ptr [esi+8h],eax
mov dword ptr [esi+0Ch],ebx
mov edi,offset haval_buf
mov ecx,53h
xor eax,eax
cld 
rep stosd 
popad 
leave 
retn 0Ch
; Finish of selected range: 0x0040251A

d2k2_crackme08_hash endp
END