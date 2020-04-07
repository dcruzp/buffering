# Haciendo Buffering 

Esto es un progama escrito en c para hacerle buffering a todo lo que se escribe por consola , para interactuar con la entada del teclado y tener mayor control sobre las teclas que se presionan en el teclado , que van a la entrada estandar  

## Que es lo que tengo 

Hay dos ficheros que tiene el codigos que van en funcion del lo que se queire hacer en un final que es 
  * --buffering.c--  que es donde se controla toda la parte de buffering del programa 
  * --test.c-- que es donde tengo todo el codigo para hacer un historial , (algo asi como simular lo que se hace ene el history de bash) 


### Corrindo los programas 

Para correr lo que tengo en buffering puedo hacer lo siguinete: 

```
./run_buffering.sh 
```

Y para correr lo que tengo en el fichero test.c puedo hacer lo siguiente: 

```
./run_test.sh
```

ademas se com esta todo en codigo por en un solo fichero se puede correr de la siguinte manera tambien: 

```
gcc <nombredelfichero> -o <nombredesalidadelfichero> && ./<nombredesalidadelfichero> 
```

## NOTA 

Todo el desarrollo de este programa es con el fin de vincularlo con el shell que estoy haciendo , por lo que aqui solo estoy viendo la parte de interactuar con la consola y hacer el historial lo mas practico posible y eficiente que se pueda. 

### Vinculacion del proyecto 

* [Shell] (https://github.com/dcruzp/shell) - La direccione del repositorio dende se encuentra el proyecto del shell

## Autor 

* **Daniel de la Cruz Prieto **  - [dcruzp] (http://github.com/dcruzp)

### Agradecimientos 

* Cualquiera que quiera contribuir con este proyecto 