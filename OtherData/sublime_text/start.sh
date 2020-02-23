#!/bin/bash
gnome-terminal -e "/bin/bash -c 'gcc $1 -o $2; $2; echo; read -p 'Pulse_intro_pasa_salir...'; exit; exec /bin/bash'; &"