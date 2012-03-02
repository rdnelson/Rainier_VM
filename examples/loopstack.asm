ds Text "Hello World\n"
mov eax, [0]
mov edx, 4
push edx
pop ecx
:start
mov edx @Text
mov eax 1
sys
loop :start
jmp :end
mov ebx 255
:end
mov eax 0
mov edx 1
sys
