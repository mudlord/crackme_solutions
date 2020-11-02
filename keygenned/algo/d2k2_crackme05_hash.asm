.686p
.model flat, stdcall
option casemap:none

.code

option prologue:none
option epilogue:none

; Start of selected range: 0x00401000


d2k2_crackme05_hash PROC x:DWORD,y:DWORD,z:DWORD
push ebp
mov ebp,esp
add esp,0FFFFFFF0h
push eax
push ebx
push ecx
push edx
push edi
push esi
mov edi,dword ptr [ebp+8h]
mov eax,dword ptr [ebp+0Ch]
inc eax
add edi,eax
mov byte ptr [edi-1h],80h
xor edx,edx
mov ebx,40h
div ebx
neg edx
add edx,40h
cmp edx,8h
jae LABEL_0x0040102F ; => 0x0040102F
add edx,40h

LABEL_0x0040102F:
mov ecx,edx
xor al,al
rep stosb 
mov eax,dword ptr [ebp+0Ch]
inc edx
add dword ptr [ebp+0Ch],edx
xor edx,edx
mov ebx,8h
mul ebx
mov dword ptr [edi-8h],eax
mov dword ptr [edi-4h],edx
mov edx,dword ptr [ebp+0Ch]
mov edi,dword ptr [ebp+8h]
mov esi,dword ptr [ebp+10h]
mov dword ptr [esi],0FDABCDEFh
mov dword ptr [esi+4h],0D9E0FA1Bh
mov dword ptr [esi+8h],0F5A6B7C8h
mov dword ptr [esi+0Ch],0A1B2D3E4h

LABEL_0x0040106F:
mov eax,dword ptr [esi]
mov dword ptr [ebp-4h],eax
mov eax,dword ptr [esi+4h]
mov dword ptr [ebp-8h],eax
mov eax,dword ptr [esi+8h]
mov dword ptr [ebp-0Ch],eax
mov eax,dword ptr [esi+0Ch]
mov dword ptr [ebp-10h],eax
mov eax,dword ptr [ebp-8h]
mov ebx,dword ptr [ebp-0Ch]
mov ecx,dword ptr [ebp-10h]
and ebx,eax
not eax
and eax,ecx
or eax,ebx
add eax,dword ptr [ebp-4h]
add eax,dword ptr [edi]
add eax,0D76AA478h
mov cl,7h
rol eax,cl
add eax,dword ptr [ebp-8h]
mov dword ptr [ebp-4h],eax
mov eax,dword ptr [ebp-4h]
mov ebx,dword ptr [ebp-8h]
mov ecx,dword ptr [ebp-0Ch]
and ebx,eax
not eax
and eax,ecx
or eax,ebx
add eax,dword ptr [ebp-10h]
add eax,dword ptr [edi+4h]
add eax,0E8C7B756h
mov cl,0Ch
rol eax,cl
add eax,dword ptr [ebp-4h]
mov dword ptr [ebp-10h],eax
mov eax,dword ptr [ebp-10h]
mov ebx,dword ptr [ebp-4h]
mov ecx,dword ptr [ebp-8h]
and ebx,eax
not eax
and eax,ecx
or eax,ebx
add eax,dword ptr [ebp-0Ch]
add eax,dword ptr [edi+8h]
add eax,242070DBh
mov cl,11h
rol eax,cl
add eax,dword ptr [ebp-10h]
mov dword ptr [ebp-0Ch],eax
mov eax,dword ptr [ebp-0Ch]
mov ebx,dword ptr [ebp-10h]
mov ecx,dword ptr [ebp-4h]
and ebx,eax
not eax
and eax,ecx
or eax,ebx
add eax,dword ptr [ebp-8h]
add eax,dword ptr [edi+0Ch]
add eax,0C1BDCEEEh
mov cl,16h
rol eax,cl
add eax,dword ptr [ebp-0Ch]
mov dword ptr [ebp-8h],eax
mov eax,dword ptr [ebp-8h]
mov ebx,dword ptr [ebp-0Ch]
mov ecx,dword ptr [ebp-10h]
and ebx,eax
not eax
and eax,ecx
or eax,ebx
add eax,dword ptr [ebp-4h]
add eax,dword ptr [edi+10h]
add eax,0F57C0FAFh
mov cl,7h
rol eax,cl
add eax,dword ptr [ebp-8h]
mov dword ptr [ebp-4h],eax
mov eax,dword ptr [ebp-4h]
mov ebx,dword ptr [ebp-8h]
mov ecx,dword ptr [ebp-0Ch]
and ebx,eax
not eax
and eax,ecx
or eax,ebx
add eax,dword ptr [ebp-10h]
add eax,dword ptr [edi+14h]
add eax,4787C62Ah
mov cl,0Ch
rol eax,cl
add eax,dword ptr [ebp-4h]
mov dword ptr [ebp-10h],eax
mov eax,dword ptr [ebp-10h]
mov ebx,dword ptr [ebp-4h]
mov ecx,dword ptr [ebp-8h]
and ebx,eax
not eax
and eax,ecx
or eax,ebx
add eax,dword ptr [ebp-0Ch]
add eax,dword ptr [edi+18h]
add eax,0A8304613h
mov cl,11h
rol eax,cl
add eax,dword ptr [ebp-10h]
mov dword ptr [ebp-0Ch],eax
mov eax,dword ptr [ebp-0Ch]
mov ebx,dword ptr [ebp-10h]
mov ecx,dword ptr [ebp-4h]
and ebx,eax
not eax
and eax,ecx
or eax,ebx
add eax,dword ptr [ebp-8h]
add eax,dword ptr [edi+1Ch]
add eax,0FD469501h
mov cl,16h
rol eax,cl
add eax,dword ptr [ebp-0Ch]
mov dword ptr [ebp-8h],eax
mov eax,dword ptr [ebp-8h]
mov ebx,dword ptr [ebp-0Ch]
mov ecx,dword ptr [ebp-10h]
and ebx,eax
not eax
and eax,ecx
or eax,ebx
add eax,dword ptr [ebp-4h]
add eax,dword ptr [edi+20h]
add eax,698098D8h
mov cl,7h
rol eax,cl
add eax,dword ptr [ebp-8h]
mov dword ptr [ebp-4h],eax
mov eax,dword ptr [ebp-4h]
mov ebx,dword ptr [ebp-8h]
mov ecx,dword ptr [ebp-0Ch]
and ebx,eax
not eax
and eax,ecx
or eax,ebx
add eax,dword ptr [ebp-10h]
add eax,dword ptr [edi+24h]
add eax,8B44F7AFh
mov cl,0Ch
rol eax,cl
add eax,dword ptr [ebp-4h]
mov dword ptr [ebp-10h],eax
mov eax,dword ptr [ebp-10h]
mov ebx,dword ptr [ebp-4h]
mov ecx,dword ptr [ebp-8h]
and ebx,eax
not eax
and eax,ecx
or eax,ebx
add eax,dword ptr [ebp-0Ch]
add eax,dword ptr [edi+28h]
add eax,0FFFF5BB1h
mov cl,11h
rol eax,cl
add eax,dword ptr [ebp-10h]
mov dword ptr [ebp-0Ch],eax
mov eax,dword ptr [ebp-0Ch]
mov ebx,dword ptr [ebp-10h]
mov ecx,dword ptr [ebp-4h]
and ebx,eax
not eax
and eax,ecx
or eax,ebx
add eax,dword ptr [ebp-8h]
add eax,dword ptr [edi+2Ch]
add eax,895CD7BEh
mov cl,16h
rol eax,cl
add eax,dword ptr [ebp-0Ch]
mov dword ptr [ebp-8h],eax
mov eax,dword ptr [ebp-8h]
mov ebx,dword ptr [ebp-0Ch]
mov ecx,dword ptr [ebp-10h]
and ebx,eax
not eax
and eax,ecx
or eax,ebx
add eax,dword ptr [ebp-4h]
add eax,dword ptr [edi+30h]
add eax,6B901122h
mov cl,7h
rol eax,cl
add eax,dword ptr [ebp-8h]
mov dword ptr [ebp-4h],eax
mov eax,dword ptr [ebp-4h]
mov ebx,dword ptr [ebp-8h]
mov ecx,dword ptr [ebp-0Ch]
and ebx,eax
not eax
and eax,ecx
or eax,ebx
add eax,dword ptr [ebp-10h]
add eax,dword ptr [edi+34h]
add eax,0FD987193h
mov cl,0Ch
rol eax,cl
add eax,dword ptr [ebp-4h]
mov dword ptr [ebp-10h],eax
mov eax,dword ptr [ebp-10h]
mov ebx,dword ptr [ebp-4h]
mov ecx,dword ptr [ebp-8h]
and ebx,eax
not eax
and eax,ecx
or eax,ebx
add eax,dword ptr [ebp-0Ch]
add eax,dword ptr [edi+38h]
add eax,0A679438Eh
mov cl,11h
rol eax,cl
add eax,dword ptr [ebp-10h]
mov dword ptr [ebp-0Ch],eax
mov eax,dword ptr [ebp-0Ch]
mov ebx,dword ptr [ebp-10h]
mov ecx,dword ptr [ebp-4h]
and ebx,eax
not eax
and eax,ecx
or eax,ebx
add eax,dword ptr [ebp-8h]
add eax,dword ptr [edi+3Ch]
add eax,49B40821h
mov cl,16h
rol eax,cl
add eax,dword ptr [ebp-0Ch]
mov dword ptr [ebp-8h],eax
mov eax,dword ptr [ebp-8h]
mov ebx,dword ptr [ebp-0Ch]
mov ecx,dword ptr [ebp-10h]
and eax,ecx
not ecx
and ecx,ebx
or eax,ecx
add eax,dword ptr [ebp-4h]
add eax,dword ptr [edi+4h]
add eax,0F61E2562h
mov cl,5h
rol eax,cl
add eax,dword ptr [ebp-8h]
mov dword ptr [ebp-4h],eax
mov eax,dword ptr [ebp-4h]
mov ebx,dword ptr [ebp-8h]
mov ecx,dword ptr [ebp-0Ch]
and eax,ecx
not ecx
and ecx,ebx
or eax,ecx
add eax,dword ptr [ebp-10h]
add eax,dword ptr [edi+18h]
add eax,0C040B340h
mov cl,9h
rol eax,cl
add eax,dword ptr [ebp-4h]
mov dword ptr [ebp-10h],eax
mov eax,dword ptr [ebp-10h]
mov ebx,dword ptr [ebp-4h]
mov ecx,dword ptr [ebp-8h]
and eax,ecx
not ecx
and ecx,ebx
or eax,ecx
add eax,dword ptr [ebp-0Ch]
add eax,dword ptr [edi+2Ch]
add eax,265E5A51h
mov cl,0Eh
rol eax,cl
add eax,dword ptr [ebp-10h]
mov dword ptr [ebp-0Ch],eax
mov eax,dword ptr [ebp-0Ch]
mov ebx,dword ptr [ebp-10h]
mov ecx,dword ptr [ebp-4h]
and eax,ecx
not ecx
and ecx,ebx
or eax,ecx
add eax,dword ptr [ebp-8h]
add eax,dword ptr [edi]
add eax,0E9B6C7AAh
mov cl,14h
rol eax,cl
add eax,dword ptr [ebp-0Ch]
mov dword ptr [ebp-8h],eax
mov eax,dword ptr [ebp-8h]
mov ebx,dword ptr [ebp-0Ch]
mov ecx,dword ptr [ebp-10h]
and eax,ecx
not ecx
and ecx,ebx
or eax,ecx
add eax,dword ptr [ebp-4h]
add eax,dword ptr [edi+14h]
add eax,0D62F105Dh
mov cl,5h
rol eax,cl
add eax,dword ptr [ebp-8h]
mov dword ptr [ebp-4h],eax
mov eax,dword ptr [ebp-4h]
mov ebx,dword ptr [ebp-8h]
mov ecx,dword ptr [ebp-0Ch]
and eax,ecx
not ecx
and ecx,ebx
or eax,ecx
add eax,dword ptr [ebp-10h]
add eax,dword ptr [edi+28h]
add eax,2441453h
mov cl,9h
rol eax,cl
add eax,dword ptr [ebp-4h]
mov dword ptr [ebp-10h],eax
mov eax,dword ptr [ebp-10h]
mov ebx,dword ptr [ebp-4h]
mov ecx,dword ptr [ebp-8h]
and eax,ecx
not ecx
and ecx,ebx
or eax,ecx
add eax,dword ptr [ebp-0Ch]
add eax,dword ptr [edi+3Ch]
add eax,0D8A1E681h
mov cl,0Eh
rol eax,cl
add eax,dword ptr [ebp-10h]
mov dword ptr [ebp-0Ch],eax
mov eax,dword ptr [ebp-0Ch]
mov ebx,dword ptr [ebp-10h]
mov ecx,dword ptr [ebp-4h]
and eax,ecx
not ecx
and ecx,ebx
or eax,ecx
add eax,dword ptr [ebp-8h]
add eax,dword ptr [edi+10h]
add eax,0E7D3FBC8h
mov cl,14h
rol eax,cl
add eax,dword ptr [ebp-0Ch]
mov dword ptr [ebp-8h],eax
mov eax,dword ptr [ebp-8h]
mov ebx,dword ptr [ebp-0Ch]
mov ecx,dword ptr [ebp-10h]
and eax,ecx
not ecx
and ecx,ebx
or eax,ecx
add eax,dword ptr [ebp-4h]
add eax,dword ptr [edi+24h]
add eax,21E1CDE6h
mov cl,5h
rol eax,cl
add eax,dword ptr [ebp-8h]
mov dword ptr [ebp-4h],eax
mov eax,dword ptr [ebp-4h]
mov ebx,dword ptr [ebp-8h]
mov ecx,dword ptr [ebp-0Ch]
and eax,ecx
not ecx
and ecx,ebx
or eax,ecx
add eax,dword ptr [ebp-10h]
add eax,dword ptr [edi+38h]
add eax,0C33707D6h
mov cl,9h
rol eax,cl
add eax,dword ptr [ebp-4h]
mov dword ptr [ebp-10h],eax
mov eax,dword ptr [ebp-10h]
mov ebx,dword ptr [ebp-4h]
mov ecx,dword ptr [ebp-8h]
and eax,ecx
not ecx
and ecx,ebx
or eax,ecx
add eax,dword ptr [ebp-0Ch]
add eax,dword ptr [edi+0Ch]
add eax,0F4D50D87h
mov cl,0Eh
rol eax,cl
add eax,dword ptr [ebp-10h]
mov dword ptr [ebp-0Ch],eax
mov eax,dword ptr [ebp-0Ch]
mov ebx,dword ptr [ebp-10h]
mov ecx,dword ptr [ebp-4h]
and eax,ecx
not ecx
and ecx,ebx
or eax,ecx
add eax,dword ptr [ebp-8h]
add eax,dword ptr [edi+20h]
add eax,455A14EDh
mov cl,14h
rol eax,cl
add eax,dword ptr [ebp-0Ch]
mov dword ptr [ebp-8h],eax
mov eax,dword ptr [ebp-8h]
mov ebx,dword ptr [ebp-0Ch]
mov ecx,dword ptr [ebp-10h]
and eax,ecx
not ecx
and ecx,ebx
or eax,ecx
add eax,dword ptr [ebp-4h]
add eax,dword ptr [edi+34h]
add eax,0A9E3E905h
mov cl,5h
rol eax,cl
add eax,dword ptr [ebp-8h]
mov dword ptr [ebp-4h],eax
mov eax,dword ptr [ebp-4h]
mov ebx,dword ptr [ebp-8h]
mov ecx,dword ptr [ebp-0Ch]
and eax,ecx
not ecx
and ecx,ebx
or eax,ecx
add eax,dword ptr [ebp-10h]
add eax,dword ptr [edi+8h]
add eax,0FCEFA3F8h
mov cl,9h
rol eax,cl
add eax,dword ptr [ebp-4h]
mov dword ptr [ebp-10h],eax
mov eax,dword ptr [ebp-10h]
mov ebx,dword ptr [ebp-4h]
mov ecx,dword ptr [ebp-8h]
and eax,ecx
not ecx
and ecx,ebx
or eax,ecx
add eax,dword ptr [ebp-0Ch]
add eax,dword ptr [edi+1Ch]
add eax,676F02D9h
mov cl,0Eh
rol eax,cl
add eax,dword ptr [ebp-10h]
mov dword ptr [ebp-0Ch],eax
mov eax,dword ptr [ebp-0Ch]
mov ebx,dword ptr [ebp-10h]
mov ecx,dword ptr [ebp-4h]
and eax,ecx
not ecx
and ecx,ebx
or eax,ecx
add eax,dword ptr [ebp-8h]
add eax,dword ptr [edi+30h]
add eax,8D2A4C8Ah
mov cl,14h
rol eax,cl
add eax,dword ptr [ebp-0Ch]
mov dword ptr [ebp-8h],eax
mov eax,dword ptr [ebp-8h]
mov ebx,dword ptr [ebp-0Ch]
mov ecx,dword ptr [ebp-10h]
xor eax,ebx
xor eax,ecx
add eax,dword ptr [ebp-4h]
add eax,dword ptr [edi+14h]
add eax,0FFFA3942h
mov cl,4h
rol eax,cl
add eax,dword ptr [ebp-8h]
mov dword ptr [ebp-4h],eax
mov eax,dword ptr [ebp-4h]
mov ebx,dword ptr [ebp-8h]
mov ecx,dword ptr [ebp-0Ch]
xor eax,ebx
xor eax,ecx
add eax,dword ptr [ebp-10h]
add eax,dword ptr [edi+20h]
add eax,8771F681h
mov cl,0Bh
rol eax,cl
add eax,dword ptr [ebp-4h]
mov dword ptr [ebp-10h],eax
mov eax,dword ptr [ebp-10h]
mov ebx,dword ptr [ebp-4h]
mov ecx,dword ptr [ebp-8h]
xor eax,ebx
xor eax,ecx
add eax,dword ptr [ebp-0Ch]
add eax,dword ptr [edi+2Ch]
add eax,6D9D6122h
mov cl,10h
rol eax,cl
add eax,dword ptr [ebp-10h]
mov dword ptr [ebp-0Ch],eax
mov eax,dword ptr [ebp-0Ch]
mov ebx,dword ptr [ebp-10h]
mov ecx,dword ptr [ebp-4h]
xor eax,ebx
xor eax,ecx
add eax,dword ptr [ebp-8h]
add eax,dword ptr [edi+38h]
add eax,0FDE5380Ch
mov cl,17h
rol eax,cl
add eax,dword ptr [ebp-0Ch]
mov dword ptr [ebp-8h],eax
mov eax,dword ptr [ebp-8h]
mov ebx,dword ptr [ebp-0Ch]
mov ecx,dword ptr [ebp-10h]
xor eax,ebx
xor eax,ecx
add eax,dword ptr [ebp-4h]
add eax,dword ptr [edi+4h]
add eax,0A4BEEA44h
mov cl,4h
rol eax,cl
add eax,dword ptr [ebp-8h]
mov dword ptr [ebp-4h],eax
mov eax,dword ptr [ebp-4h]
mov ebx,dword ptr [ebp-8h]
mov ecx,dword ptr [ebp-0Ch]
xor eax,ebx
xor eax,ecx
add eax,dword ptr [ebp-10h]
add eax,dword ptr [edi+10h]
add eax,4BDECFA9h
mov cl,0Bh
rol eax,cl
add eax,dword ptr [ebp-4h]
mov dword ptr [ebp-10h],eax
mov eax,dword ptr [ebp-10h]
mov ebx,dword ptr [ebp-4h]
mov ecx,dword ptr [ebp-8h]
xor eax,ebx
xor eax,ecx
add eax,dword ptr [ebp-0Ch]
add eax,dword ptr [edi+1Ch]
add eax,0F6BB4B60h
mov cl,10h
rol eax,cl
add eax,dword ptr [ebp-10h]
mov dword ptr [ebp-0Ch],eax
mov eax,dword ptr [ebp-0Ch]
mov ebx,dword ptr [ebp-10h]
mov ecx,dword ptr [ebp-4h]
xor eax,ebx
xor eax,ecx
add eax,dword ptr [ebp-8h]
add eax,dword ptr [edi+28h]
add eax,0BEBFBC70h
mov cl,17h
rol eax,cl
add eax,dword ptr [ebp-0Ch]
mov dword ptr [ebp-8h],eax
mov eax,dword ptr [ebp-8h]
mov ebx,dword ptr [ebp-0Ch]
mov ecx,dword ptr [ebp-10h]
xor eax,ebx
xor eax,ecx
add eax,dword ptr [ebp-4h]
add eax,dword ptr [edi+34h]
add eax,289B7EC6h
mov cl,4h
rol eax,cl
add eax,dword ptr [ebp-8h]
mov dword ptr [ebp-4h],eax
mov eax,dword ptr [ebp-4h]
mov ebx,dword ptr [ebp-8h]
mov ecx,dword ptr [ebp-0Ch]
xor eax,ebx
xor eax,ecx
add eax,dword ptr [ebp-10h]
add eax,dword ptr [edi]
add eax,0EAA127FAh
mov cl,0Bh
rol eax,cl
add eax,dword ptr [ebp-4h]
mov dword ptr [ebp-10h],eax
mov eax,dword ptr [ebp-10h]
mov ebx,dword ptr [ebp-4h]
mov ecx,dword ptr [ebp-8h]
xor eax,ebx
xor eax,ecx
add eax,dword ptr [ebp-0Ch]
add eax,dword ptr [edi+0Ch]
add eax,0D4EF3085h
mov cl,10h
rol eax,cl
add eax,dword ptr [ebp-10h]
mov dword ptr [ebp-0Ch],eax
mov eax,dword ptr [ebp-0Ch]
mov ebx,dword ptr [ebp-10h]
mov ecx,dword ptr [ebp-4h]
xor eax,ebx
xor eax,ecx
add eax,dword ptr [ebp-8h]
add eax,dword ptr [edi+18h]
add eax,4881D05h
mov cl,17h
rol eax,cl
add eax,dword ptr [ebp-0Ch]
mov dword ptr [ebp-8h],eax
mov eax,dword ptr [ebp-8h]
mov ebx,dword ptr [ebp-0Ch]
mov ecx,dword ptr [ebp-10h]
xor eax,ebx
xor eax,ecx
add eax,dword ptr [ebp-4h]
add eax,dword ptr [edi+24h]
add eax,0D9D4D039h
mov cl,4h
rol eax,cl
add eax,dword ptr [ebp-8h]
mov dword ptr [ebp-4h],eax
mov eax,dword ptr [ebp-4h]
mov ebx,dword ptr [ebp-8h]
mov ecx,dword ptr [ebp-0Ch]
xor eax,ebx
xor eax,ecx
add eax,dword ptr [ebp-10h]
add eax,dword ptr [edi+30h]
add eax,0E6DB99E5h
mov cl,0Bh
rol eax,cl
add eax,dword ptr [ebp-4h]
mov dword ptr [ebp-10h],eax
mov eax,dword ptr [ebp-10h]
mov ebx,dword ptr [ebp-4h]
mov ecx,dword ptr [ebp-8h]
xor eax,ebx
xor eax,ecx
add eax,dword ptr [ebp-0Ch]
add eax,dword ptr [edi+3Ch]
add eax,1FA27CF8h
mov cl,10h
rol eax,cl
add eax,dword ptr [ebp-10h]
mov dword ptr [ebp-0Ch],eax
mov eax,dword ptr [ebp-0Ch]
mov ebx,dword ptr [ebp-10h]
mov ecx,dword ptr [ebp-4h]
xor eax,ebx
xor eax,ecx
add eax,dword ptr [ebp-8h]
add eax,dword ptr [edi+8h]
add eax,0C4AC5665h
mov cl,17h
rol eax,cl
add eax,dword ptr [ebp-0Ch]
mov dword ptr [ebp-8h],eax
mov eax,dword ptr [ebp-8h]
mov ebx,dword ptr [ebp-0Ch]
mov ecx,dword ptr [ebp-10h]
not ecx
or eax,ecx
xor eax,ebx
add eax,dword ptr [ebp-4h]
add eax,dword ptr [edi]
add eax,0F4292244h
mov cl,6h
rol eax,cl
add eax,dword ptr [ebp-8h]
mov dword ptr [ebp-4h],eax
mov eax,dword ptr [ebp-4h]
mov ebx,dword ptr [ebp-8h]
mov ecx,dword ptr [ebp-0Ch]
not ecx
or eax,ecx
xor eax,ebx
add eax,dword ptr [ebp-10h]
add eax,dword ptr [edi+1Ch]
add eax,432AFF97h
mov cl,0Ah
rol eax,cl
add eax,dword ptr [ebp-4h]
mov dword ptr [ebp-10h],eax
mov eax,dword ptr [ebp-10h]
mov ebx,dword ptr [ebp-4h]
mov ecx,dword ptr [ebp-8h]
not ecx
or eax,ecx
xor eax,ebx
add eax,dword ptr [ebp-0Ch]
add eax,dword ptr [edi+38h]
add eax,0AB9423A7h
mov cl,0Fh
rol eax,cl
add eax,dword ptr [ebp-10h]
mov dword ptr [ebp-0Ch],eax
mov eax,dword ptr [ebp-0Ch]
mov ebx,dword ptr [ebp-10h]
mov ecx,dword ptr [ebp-4h]
not ecx
or eax,ecx
xor eax,ebx
add eax,dword ptr [ebp-8h]
add eax,dword ptr [edi+14h]
add eax,0FC93A039h
mov cl,15h
rol eax,cl
add eax,dword ptr [ebp-0Ch]
mov dword ptr [ebp-8h],eax
mov eax,dword ptr [ebp-8h]
mov ebx,dword ptr [ebp-0Ch]
mov ecx,dword ptr [ebp-10h]
not ecx
or eax,ecx
xor eax,ebx
add eax,dword ptr [ebp-4h]
add eax,dword ptr [edi+30h]
add eax,655B59C3h
mov cl,6h
rol eax,cl
add eax,dword ptr [ebp-8h]
mov dword ptr [ebp-4h],eax
mov eax,dword ptr [ebp-4h]
mov ebx,dword ptr [ebp-8h]
mov ecx,dword ptr [ebp-0Ch]
not ecx
or eax,ecx
xor eax,ebx
add eax,dword ptr [ebp-10h]
add eax,dword ptr [edi+0Ch]
add eax,8F0CCC92h
mov cl,0Ah
rol eax,cl
add eax,dword ptr [ebp-4h]
mov dword ptr [ebp-10h],eax
mov eax,dword ptr [ebp-10h]
mov ebx,dword ptr [ebp-4h]
mov ecx,dword ptr [ebp-8h]
not ecx
or eax,ecx
xor eax,ebx
add eax,dword ptr [ebp-0Ch]
add eax,dword ptr [edi+28h]
add eax,0FFEFF47Dh
mov cl,0Fh
rol eax,cl
add eax,dword ptr [ebp-10h]
mov dword ptr [ebp-0Ch],eax
mov eax,dword ptr [ebp-0Ch]
mov ebx,dword ptr [ebp-10h]
mov ecx,dword ptr [ebp-4h]
not ecx
or eax,ecx
xor eax,ebx
add eax,dword ptr [ebp-8h]
add eax,dword ptr [edi+4h]
add eax,85845DD1h
mov cl,15h
rol eax,cl
add eax,dword ptr [ebp-0Ch]
mov dword ptr [ebp-8h],eax
mov eax,dword ptr [ebp-8h]
mov ebx,dword ptr [ebp-0Ch]
mov ecx,dword ptr [ebp-10h]
not ecx
or eax,ecx
xor eax,ebx
add eax,dword ptr [ebp-4h]
add eax,dword ptr [edi+20h]
add eax,6FA87E4Fh
mov cl,6h
rol eax,cl
add eax,dword ptr [ebp-8h]
mov dword ptr [ebp-4h],eax
mov eax,dword ptr [ebp-4h]
mov ebx,dword ptr [ebp-8h]
mov ecx,dword ptr [ebp-0Ch]
not ecx
or eax,ecx
xor eax,ebx
add eax,dword ptr [ebp-10h]
add eax,dword ptr [edi+3Ch]
add eax,0FE2CE6E0h
mov cl,0Ah
rol eax,cl
add eax,dword ptr [ebp-4h]
mov dword ptr [ebp-10h],eax
mov eax,dword ptr [ebp-10h]
mov ebx,dword ptr [ebp-4h]
mov ecx,dword ptr [ebp-8h]
not ecx
or eax,ecx
xor eax,ebx
add eax,dword ptr [ebp-0Ch]
add eax,dword ptr [edi+18h]
add eax,0A3014314h
mov cl,0Fh
rol eax,cl
add eax,dword ptr [ebp-10h]
mov dword ptr [ebp-0Ch],eax
mov eax,dword ptr [ebp-0Ch]
mov ebx,dword ptr [ebp-10h]
mov ecx,dword ptr [ebp-4h]
not ecx
or eax,ecx
xor eax,ebx
add eax,dword ptr [ebp-8h]
add eax,dword ptr [edi+34h]
add eax,4E0811A1h
mov cl,15h
rol eax,cl
add eax,dword ptr [ebp-0Ch]
mov dword ptr [ebp-8h],eax
mov eax,dword ptr [ebp-8h]
mov ebx,dword ptr [ebp-0Ch]
mov ecx,dword ptr [ebp-10h]
not ecx
or eax,ecx
xor eax,ebx
add eax,dword ptr [ebp-4h]
add eax,dword ptr [edi+10h]
add eax,0F7537E82h
mov cl,6h
rol eax,cl
add eax,dword ptr [ebp-8h]
mov dword ptr [ebp-4h],eax
mov eax,dword ptr [ebp-4h]
mov ebx,dword ptr [ebp-8h]
mov ecx,dword ptr [ebp-0Ch]
not ecx
or eax,ecx
xor eax,ebx
add eax,dword ptr [ebp-10h]
add eax,dword ptr [edi+2Ch]
add eax,0BD3AF235h
mov cl,0Ah
rol eax,cl
add eax,dword ptr [ebp-4h]
mov dword ptr [ebp-10h],eax
mov eax,dword ptr [ebp-10h]
mov ebx,dword ptr [ebp-4h]
mov ecx,dword ptr [ebp-8h]
not ecx
or eax,ecx
xor eax,ebx
add eax,dword ptr [ebp-0Ch]
add eax,dword ptr [edi+8h]
add eax,2AD7D2BBh
mov cl,0Fh
rol eax,cl
add eax,dword ptr [ebp-10h]
mov dword ptr [ebp-0Ch],eax
mov eax,dword ptr [ebp-0Ch]
mov ebx,dword ptr [ebp-10h]
mov ecx,dword ptr [ebp-4h]
not ecx
or eax,ecx
xor eax,ebx
add eax,dword ptr [ebp-8h]
add eax,dword ptr [edi+24h]
add eax,0EB86D391h
mov cl,15h
rol eax,cl
add eax,dword ptr [ebp-0Ch]
mov dword ptr [ebp-8h],eax
mov eax,dword ptr [ebp-4h]
add dword ptr [esi],eax
mov eax,dword ptr [ebp-8h]
add dword ptr [esi+4h],eax
mov eax,dword ptr [ebp-0Ch]
add dword ptr [esi+8h],eax
mov eax,dword ptr [ebp-10h]
add dword ptr [esi+0Ch],eax
add edi,40h
sub edx,40h
jne LABEL_0x0040106F ; => 0x0040106F
mov ecx,4h

LABEL_0x004019CA:
mov eax,dword ptr [esi]
xchg ah,al
rol eax,10h
xchg ah,al
mov dword ptr [esi],eax
add esi,4h
loop LABEL_0x004019CA ; => 0x004019CA
pop esi
pop edi
pop edx
pop ecx
pop ebx
pop eax
leave 
ret 0Ch
; Finish of selected range: 0x004019E3
d2k2_crackme05_hash ENDP

END
; Finish of selected range: 0x004019E3
