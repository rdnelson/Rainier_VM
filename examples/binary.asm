ds Error "There has been an error somewhere!\n"
ds Success "No errors occured during execution\n"
mov eax 1
and 5
mov edx 2
or edx
mov edx 3
test eax edx
jne :Error
mov ecx 9
xor ecx
mov edx 10
test eax edx
jne :Error
not
and 7
mov edx 5
test eax edx
jne :Error
mov eax 1
mov edx @Success
sys
xor eax
mov edx 0
sys
:Error
mov eax 1
mov edx @Error
sys
xor eax
mov edx 1
sys

