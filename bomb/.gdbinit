add-auto-load-safe-path /home/pyd/csapp/bomblab/bomb/.gdbinit
set args ans
b explode_bomb
b phase_6
b *phase_6 + 0xeb