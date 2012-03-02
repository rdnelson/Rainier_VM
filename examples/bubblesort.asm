ds Before "Before: "
ds After "After: "
#ds Sortarray "The Quick Brown fox jumped over the lazy dogs\n"
ds Sortarray "TABD\n"
mov eax 1
mov edx @Before
sys
mov edx @Sortarray
sys
mov ecx [@Sortarray]
mov ebx @Sortarray
sub ecx 1 #leave new line at end
add ebx 4 #bubble requires ebx to be address of array, ecx to be length

:start
push eax #push its initial value
test ecx 1 #if there's 1 or fewer characters
jle :done

xor eax eax # clear eax so that flip count is 0
sub ecx 1
add ecx ebx
push ebx #push array base
:bubble
call :printstr
push ecx #ecx is the final position to check
mov ecx eax #ecx is flip count
movb eax [ebx]
movb edx [ebx + 1]
test eax edx #eax is first element, edx is second
jle :noswap
:swap
xor eax edx
xor edx eax
xor eax edx #swap them
movb [ebx] eax
movb [ebx + 1] edx
add ecx 1 #a swap has been made
:noswap

add ebx 1 #ebx is new current position
pop edx #edx is final position
	#pop ebx #ebx is final position
	#push ebx ##ebx retains value (should I add a peek function?)
	#add ebx edx #ebx is end - 1 of buffer
test ebx edx #check if at end of buffer

mov eax ecx #//eax is number of flips
mov ecx edx #//ecx is final location of buffer
jne :bubble #//jmp if not at end

pop ebx #//reset to original buffer address
push ebx #//repush the base

sub ecx 1 #move the final point one step closer
test ebx ecx
je :done #no more string to sort
mov edx 0
test eax edx #//test if number of flips is zero (done early)
xor eax eax #reset number of flips
je :done
jmp :bubble #//restart sort
:done
pop eax #reset it's initial value

mov eax 1
mov edx @After
sys

mov edx @Sortarray
sys
mov eax 0
mov edx 0
sys

:printstr
push eax
push edx

mov eax 1
mov edx @Sortarray
sys

pop edx
pop eax

ret
