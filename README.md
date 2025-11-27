# Space Invaders
Um fan remake do famoso arcade Space Invaders feito por alunos de Ciência da Computação da CESAR School

- Jogo programado em C utilizando da biblioteca raylib
- Estruturas implementadas: structs, matrizes, alocação dinâmica, listas encadeadas, manipulação de arquivo e ponteiros


# Como Jogar

O jogo se trata de um piloto espacial que deve enfrentar sozinho dez ondas de naves inimigas para defender a sua terra natal de uma invasão extraterrestre

- Use as telcas WASD para mover-se e E para atirar

- Você deve evitar ser atingido pelos tiros inimigos. Sua nave consegue aguentar até quatro acertos

- A cada onda inimiga superada, a quantidade de naves inimigas aumenta

# Como compilar (exemplo Linux)

Comece entrando no terminal de comandos do Linux e rode esses comandos em ordem

```bash
       
sudo apt install build-essential git
sudo apt update
sudo apt install libasound2-dev mesa-common-dev libx11-dev libxrandr-dev libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev
```

       
Para instalar a raylib e configurá-la:
```bash
       
git clone raylib
cd raylib/src
make PLATFORM=PLATFORM_DESKTOP 
sudo make install
```

Em seguida, clone o projeto

```bash
git clone https://github.com/telmo-wq/Space_Invaders_PIF.git
cd Space_Invaders_PIF
code .

```

Abra um terminal e rode a linha de comando para compilar o código

Comando para Windows:
C:\raylib\w64devkit\bin\mingw32-make.exe RAYLIB_PATH=C:/raylib/raylib PROJECT_NAME=jogo OBJS="main.o tiro.o inimigo.o player.o teste_titulo.o" BUILD_MODE=DEBUG

Comando para Linux:
gcc *.c -o main -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 2>&1

Em seguida, rode ./main




