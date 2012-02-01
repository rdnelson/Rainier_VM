ds Sortarray "beca"
mov ecx [@Sortarray]
mov ebx @SortArray
add ebx 4 //bubble requires ebx to be address of array, ecx to be length
:bubble
sub ecx 1
push ecx
push ebx
xor ecx ecx
movb eax [ebx]
movb edx [ebx + 1]
test eax edx //eax is first element
jle :noswap
:swap
xor eax edx
xor edx eax
xor eax edx //swap them
movb [ebx] eax
movb [ebx + 1] edx
add ecx 1
:noswap
mov eax ebx
add eax 1 //eax is current position
pop edx
pop ebx
push ebx //ebx retains value (peek?)
add ebx edx //ebx is end - 1 of buffer
test eax ebx //check if at end - 1 of buffer
mov ebx eax //ebx is current position
mov eax ecx //eax is number of flips
mov ecx edx //ecx is len - 1
jne :bubble //jmp if not at end
mov edx 0
test eax edx //test if number of flips is zero
je :done
pop ebx //reset to buffer address
jmp :bubble //restart sort
:done
mov edx @Sortarray
mov eax 1
sys
mov eax 0
mov edx 0
sys
