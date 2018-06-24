# Build en Windows:
1. ```git clone https://github.com/saques/stegobmp.git``` clonar el repositorio
2. ```git submodule init``` inicializar los submódulos
3. ```git submodule update```
4. Instalar [OpenSSL](https://slproweb.com/products/Win32OpenSSL.html), [Visual Studio 2017 Community](https://www.visualstudio.com/downloads/) y [CMake](https://cmake.org/download/).
5. Abrir el proyecto. File->Open->CMake...
7. Editar los includePaths del archivo CppProperties.json (Project->Edit Settings) para x64-Debug y x64-Release con:
```
"includePath": [
	"${env.INCLUDE}",
	"C:\\msys64\\mingw64\\include",
	"${projectRoot}\\cxxopts\\include",
	"${projectRoot}\\easyloggingpp\\src",
	"${projectRoot}"
],
```
6. Compilar y ejecutar.


# Build en Linux
### Probado en Pampero, usando GCC 7.3.0, OpenSSL 1.1.0g y CMake 3.10.2
1. ```git clone https://github.com/saques/stegobmp.git``` 
2. ```cd stegobmp``` 
3. ```git submodule init```
4. ```git submodule update```
5. ```mkdir build```
6. ```cd build```
7. ```cmake ..```
8. ```make```
9. Los ejecutables ``stegobmp`` y ``tests_run`` deberian encontrarse en la carpeta ``build``
### Nota: 
Los *warnings* que informa GCC al compilar se refieren al uso de *strings* constantes en 
el archivo `test.cpp` para probar el funcionamiento correcto del *parseo* de lineas de comando.
Como sólo afecta a los *tests*, decidimos ignorarlos para mantener el código limpio y simple (la solución
hubiera requerido crear un *buffer* con `malloc` y copiar con `strcpy` cada *string* constante que querramos
usar dentro del *mock* de `argv`, y luego liberarlos con `free`).