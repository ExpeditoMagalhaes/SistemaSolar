# SistemaSolar

Este projeto é uma simulação simples do Sistema Solar em C++ e a biblioteca de código aberto Glut para gráficos 3D que tem por objetivo a obtenção da nota da AP 1 da disciplina de Computação Gráfica, do curso de ENgenharia da Computação 2022.1.

Foi adicionado funcionalidades básicas para o programa como manipulação de camara (apertando as teclas 1, 2 e 3), zoom +(botão 1 do mouse) e - (botão 2 do mouse) e também variar entre as velocidades em que os planetas se movem, se deve ser mais rápido (tecla f)  ou mais lento (tecla s). A simulação do Sistema Solar consiste nos 8 planetas e o Sol.
Vale ressaltar que o presente trabalho se limita a simular o movimento básico do planeta do Sistema Solar, e que este exercício não usa uma simulação elíptica precisa ou o movimento real de cada um dos planetas (baseado em equações estelares ou outras).

O link para o vídeo com a simulação do trabalho no Youtube se encontra abaixo:

Link para o Youtube: https://youtu.be/J1A77RbASiE

## Inicialização

Baixe o projeto clicando no botão de download:

https://github.com/ExpeditoMagalhaes/SistemaSolar.git

## Pré-Requisitos
 
Será preciso os seguintes conjuntos de bibliotecas e programas em seu computador local:

-[Visual Studio](https://visualstudio.microsoft.com/pt-br/vs/)<br/>
-[Glut](https://www.transmissionzero.co.uk/software/freeglut-devel/)

Após baixar a biblioteca GLUT e descompactar o arquivo, deve-se adicioná-la no Visual Studio. Em caso de maiores dificuldades para configurar a biblioteca GLUT com o Visual Studio usar como material de apoio o passo a passo abaixo:
#### PASSO 1
Primeiro vamos começar com o arquivo dll freeglut que se encontra no seguinte caminho \freeglut-MSVC-3.0.0-2.mp\freeglut\bin, esse arquivo deve ser copiado e colado no diretorio “Windows” no drive (:C).
#### PASSO 2
Posteriormente, deve-se copiar a pasta GL que se encontra no seguinte caminho \freeglut-MSVC-3.0.0-2.mp\freeglut\include e colar dentro da pasta de inclusão (include)  no Visual Studio que se encontra no seguinte caminho \Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.32.31326\include
#### PASSO 3
Por fim, deve-se copiar o arquivo freeglut.lib que se encontra no seguinte caminho \freeglut-MSVC-3.0.0-2.mp\freeglut\lib e colar no Visual Studio na pasta lib  que se encontra no seguinte caminho \Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.32.31326\lib\x86

## Executando
 Para inicializar o programa, alguns diretórios devem ser modificados, conforme o passo a passo a baixo:

## Exemplo

Visão das Câmeras: <br />

Câmera 1:
![SolarCam1](https://user-images.githubusercontent.com/71902902/174695733-0c6af737-984a-4f07-8d4c-28670e9848fa.gif)


Câmera 2:
![SolarCam2](https://user-images.githubusercontent.com/71902902/174695765-8dfbbf92-3d71-4fa4-8006-d71a98c66970.gif)


Câmera 3:
![Camera3Solar](https://user-images.githubusercontent.com/71902902/174696861-2b86dd02-040a-4287-a1b9-9c705a25b22a.gif)


<br />

## Autores 
Equipe 2:
  <br />Expedito Rodrigues Magalhães Filho - 378989
  <br />Francisco Anderson Mendonça Freire - 385107
  <br />Francisco Evandro Ribeiro Martins Filho - 385143
  <br />Fiama Carla Martins de Sousa - 472959
  <br />Francilândio Lima - 472644
