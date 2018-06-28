Instrucciones:

* hugo.bmp contiene texto plano embebido en modo LSBE.
	* Inspeccionando a partir del byte N°54, se ve que, de ser LSB4, el tamaño del contenido sería de ‭72A6F85E‬ bytes, es decir, casi 2 GB, lo cual es imposible. De ser LSB1, al ser el primer bit 1, el tamaño sería aún más imposible. Por ende, debe ser LSBE.
* secreto1.bmp contiene texto plano embebido en modo LSB1.
	* Procediendo de igual forma, asumiendo que el tipo de inserción es LSB4, el tamaño del contenido sería de ‭A46C68EA‬ bytes, es decir, casi 3 GB. Por ende, debe ser LSB1. 
* titanic.bmp contiene texto plano embebido en modo LSB4.
	* Por descarte, se probó LSB4 y anduvo. 
	
Para descomprimir:

* No funciono unicamente cambiar la extension del archivo. Consultando este link: http://wiki.linuxquestions.org/wiki/Embed_a_zip_file_into_an_image , y observando el final del archivo, se detectó el inicio del header del ZIP 0x04034b50 (en little endian). Luego, con el hex editor, se copió esta parte y se creó un nuevo archivo ZIP, así si pudo leerse.
	
sol1.txt:

cada mina es un 1.
cada fila forma una letra.
Los ascii de las letras empiezan todos en 01.
Asi encontraras el algoritmo y el modo
La password esta en otro archivo
Con algoritmo, modo y password hay un .wmv encriptado y oculto.
	
Analizando el buscaminas:

(01)000100 - D
(01)100101 - e
(01)110011 - s
(01)000101 - E
(01)100011 - c
(01)100010 - b

Luego:
	
	* Algoritmo: DES
	* Modo: ECB
	* Password: camaleon ("la password es camaleon", data al final de lima.bmp)
	
Así, se puede descomprimir el wav.